/* -------------------------------------------------------------------------- */

#include <string.h>

#include "request_handler.h"
#include "qassert.h"

#include "FreeRTOS.h"
#include "queue.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE;

#define MAX_POOL_SIZE 40

/* -------------------------------------------------------------------------- */

PRIVATE void request_handler_insert_movement( RequestHandlerInstance_t instance, Movement_t *movement );

PRIVATE int32_t request_handler_find_free_pool_slot( RequestHandlerInstance_t instance );

PRIVATE int32_t request_handler_find_sorted_candidate( RequestHandlerInstance_t instance );

PRIVATE void request_handler_emit_ordered_entries( RequestHandlerInstance_t instance );

/* -------------------------------------------------------------------------- */

/*  TODO Request Handler Rework
 *      - Pool is x bytes in size
 *      - During init, work out how many slots fit in the pool
 *      - When adding/searching the pool, do so against the 'dynamic' slot count
 *      - Ability to add/emit entries with their generic size/type
 *      - Decouple the request handler from the types and callback types entirely?
 *      - Look into a more efficient storage data structure,
 *      - Look into search performance optimisations
 * */

typedef struct {
    bool is_used;
    Movement_t movement;
} MovementPoolSlot_t;

typedef struct
{
    MovementPoolSlot_t slots[MAX_POOL_SIZE];
    uint32_t expected_sync_offset;
    RequestHandlerInstance_t instance;
    QueueHandle_t input_queue;
    RequestableCallbackFn output_cb;
} RequestPool_t;

PRIVATE RequestPool_t pool[NUM_REQUEST_HANDLERS] = { 0 };

/* -------------------------------------------------------------------------- */

PRIVATE uint32_t request_handler_add( RequestHandlerInstance_t handler, Movement_t *movement );

/* -------------------------------------------------------------------------- */

PUBLIC void request_handler_init( RequestHandlerInstance_t instance )
{
    REQUIRE( instance < NUM_REQUEST_HANDLERS );
    RequestPool_t *me = &pool[instance];
    memset( me, 0, sizeof( RequestPool_t ) );

    me->instance = instance;    // TODO: resolve this hack to get around the task pvParameters arg being a pointer to the pool instead of the instance enum

    // Create and initialize FreeRTOS queues for this instance
    switch( instance )
    {
        case REQUEST_HANDLER_MOVES:
            me->input_queue = xQueueCreate( MAX_QUEUE_SIZE, sizeof(Movement_t) );
            vQueueAddToRegistry( me->input_queue, "rqhMove"); // Debug view annotations
            break;
        case REQUEST_HANDLER_FADES:
            me->input_queue = xQueueCreate( MAX_QUEUE_SIZE, sizeof(Fade_t) );
            vQueueAddToRegistry( me->input_queue, "rqhFade"); // Debug view annotations
            break;
        default:
            ASSERT(false);
            break;
    }
}

/* -------------------------------------------------------------------------- */


PUBLIC void* request_handler_get_context_for( RequestHandlerInstance_t instance )
{
    REQUIRE( instance < NUM_REQUEST_HANDLERS );
    return (void*)&pool[instance];
}

/* -------------------------------------------------------------------------- */

PUBLIC void request_handler_attach_output_callback( RequestHandlerInstance_t instance, RequestableCallbackFn callback )
{
    REQUIRE( instance < NUM_REQUEST_HANDLERS );

    switch( callback.type )
    {
        case CALLBACK_INVALID:
            // Reset it to an invalid/empty callback
            memset( &pool[instance].output_cb, 0, sizeof(RequestableCallbackFn) );
            break;

        case CALLBACK_MOVEMENT:
            // ensure that the callback has the right type for the pool
            REQUIRE( instance == REQUEST_HANDLER_MOVES );
            pool[instance].output_cb = callback;
            break;

        case CALLBACK_FADE:
            REQUIRE( instance == REQUEST_HANDLER_FADES );
            pool[instance].output_cb = callback;
            break;

        default:
            ASSERT(false);
            break;
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC void request_handler_task( void *arg )
{
    REQUIRE( arg );
    RequestPool_t *pool = arg;
    ENSURE( pool );

    for(;;)
    {
        Movement_t incoming_movement;
        BaseType_t req_needs_slot = xQueueReceive(pool->input_queue,
                                           (void *)&incoming_movement,
                                           5 // portMAX_DELAY
                                           );

        if( req_needs_slot )
        {
            request_handler_insert_movement( pool->instance, &incoming_movement);
        }

        // Keep the output queue populated with ordered movements
        request_handler_emit_ordered_entries( pool->instance );
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t request_handler_add_movement( Movement_t *movement )
{
    return request_handler_add( REQUEST_HANDLER_MOVES, movement );
}

// TODO: make input arg generic on size?
//       - should check the item being added matches the size accepted by the rh
PRIVATE uint32_t request_handler_add( RequestHandlerInstance_t instance, Movement_t *movement )
{
    REQUIRE( instance < NUM_REQUEST_HANDLERS );
    REQUIRE( movement );
    RequestPool_t *me = &pool[instance];

    BaseType_t result = xQueueSendToBack( me->input_queue, (void *)movement, (TickType_t)0 );
    ENSURE( result == pdPASS );

    return uxQueueSpacesAvailable( me->input_queue );
}

/* -------------------------------------------------------------------------- */

PUBLIC void request_handler_clear( RequestHandlerInstance_t instance )
{
    REQUIRE( instance < NUM_REQUEST_HANDLERS );
    RequestPool_t *me = &pool[instance];

    // TODO: Does this need to be behind a mutex for safety?

    // Empty the input/output queues
    xQueueReset( me->input_queue );

    // Wipe the pool storage
    memset( &me->slots, 0, sizeof(me->slots));
    me->expected_sync_offset = 0;
}

/* -------------------------------------------------------------------------- */

// TODO: rework arguments to support any possible type
//       - then check the type will fit in the queue
//       - then copy it into the slot correctly
PRIVATE void request_handler_insert_movement( RequestHandlerInstance_t instance, Movement_t *movement )
{
    REQUIRE( instance < NUM_REQUEST_HANDLERS );
    REQUIRE( movement );
    RequestPool_t *me = &pool[instance];

    uint32_t slot_index = request_handler_find_free_pool_slot( instance );
    ENSURE( slot_index >= 0 );


    memcpy( &me->slots[slot_index].movement, movement, sizeof(Movement_t) );
    me->slots[slot_index].is_used = true;
}

/* -------------------------------------------------------------------------- */

PRIVATE int32_t request_handler_find_free_pool_slot( RequestHandlerInstance_t instance )
{
    REQUIRE( instance < NUM_REQUEST_HANDLERS );
    RequestPool_t *me = &pool[instance];

    // TODO: when the pool has 'dynamic' slot counts, use that number instead of MAX_POOL_SIZE
    for( uint32_t i = 0; i < MAX_POOL_SIZE; i++ )
    {
        if( !me->slots[i].is_used)
        {
            return (int32_t)i;
        }
    }

    return -1;
}

/* -------------------------------------------------------------------------- */

// Find the smallest sync_offset entry in the pool

PRIVATE int32_t request_handler_find_sorted_candidate( RequestHandlerInstance_t instance )
{
    REQUIRE( instance < NUM_REQUEST_HANDLERS );
    RequestPool_t *me = &pool[instance];

    uint32_t min_sync_offset = UINT32_MAX;
    int32_t min_sync_offset_slot_index = -1;

    // TODO: when the pool has 'dynamic' slot counts, use that number instead of MAX_POOL_SIZE
    for( uint32_t i = 0; i < MAX_POOL_SIZE; i++ )
    {
        if(    me->slots[i].is_used
            && me->slots[i].movement.sync_offset < min_sync_offset)
        {
            min_sync_offset = me->slots[i].movement.sync_offset;
            min_sync_offset_slot_index = (int32_t)i;
        }
    }

    return min_sync_offset_slot_index;
}

/* -------------------------------------------------------------------------- */

PRIVATE void request_handler_emit_ordered_entries( RequestHandlerInstance_t instance )
{
    REQUIRE( instance < NUM_REQUEST_HANDLERS );
    RequestPool_t *me = &pool[instance];

    // Is there room in the destination queue?
    uint32_t queue_spaces = 0;
    switch( me->output_cb.type )
    {
        case CALLBACK_MOVEMENT:
            queue_spaces = me->output_cb.fn.move( NULL );
            break;

        case CALLBACK_FADE:
            queue_spaces = me->output_cb.fn.fade( NULL );
            break;

        case CALLBACK_INVALID:
            return; // no callback, can't emit events
    }

    // Start filling the queue
    while( queue_spaces > 1 )
    {
        int32_t candidate_slot_index = request_handler_find_sorted_candidate( instance );

        if( candidate_slot_index == -1 )
        {
            break;
        }

        // Get the entry
        Movement_t *movement = &me->slots[candidate_slot_index].movement;

        // Is it in-order?
        if(    movement->sync_offset == me->expected_sync_offset
            || movement->sync_offset == 0 )   // first move will have an offset of zero
        {
            // Dispatch the move via the output callback
            switch( me->output_cb.type )
            {
                //TODO: check if a simple 'not invalid' check is sufficient
                //      if it is, don't handle each type, just call the union with a void pointer?
                case CALLBACK_MOVEMENT:
                    queue_spaces = me->output_cb.fn.move( movement );
                    break;

                case CALLBACK_FADE:
                    // TODO: need fade support throughout the rest of the handler first...
                    queue_spaces = me->output_cb.fn.fade( NULL );
                    break;

                default:
                    ASSERT(false);
                    break;
            }

            // Update the pool and slot metadata
            me->slots[candidate_slot_index].is_used = false;
            me->expected_sync_offset = movement->sync_offset + movement->duration;
        }
    }
}

/* -------------------------------------------------------------------------- */