/* -------------------------------------------------------------------------- */

#include <string.h>

#include "request_handler.h"
#include "qassert.h"

#include "FreeRTOS.h"
#include "queue.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE;

#define MAX_POOL_SIZE 40

#define POOL_STORAGE_BYTES_MOVEMENT ( MAX_POOL_SIZE * sizeof(Movement_t) )
#define POOL_STORAGE_BYTES_FADES ( MAX_POOL_SIZE * sizeof(Fade_t) )

/* -------------------------------------------------------------------------- */

PRIVATE void request_handler_insert_entry( RequestHandlerInstance_t instance, void *entry_ptr );

PRIVATE uint32_t request_handler_add( RequestHandlerInstance_t handler, void *entry_ptr, uint32_t entry_size );

PRIVATE int32_t request_handler_find_free_pool_slot( RequestHandlerInstance_t instance );

PRIVATE int32_t request_handler_find_sorted_candidate( RequestHandlerInstance_t instance );

PRIVATE void request_handler_emit_ordered_entries( RequestHandlerInstance_t instance );

/* -------------------------------------------------------------------------- */

/*  TODO Request Handler Rework
 *      - Queue usage percentage information?
 *      - Look into a more efficient storage data structure,
 *      - Look into search performance optimisations
 * */

typedef struct {
    uint32_t key;   // UUID or sortable index value
    bool is_used;   // marks slot usage in the pool
} EntryMetadata_t;

typedef struct
{
    RequestHandlerInstance_t instance;
    QueueHandle_t input_queue;          // input FreeRTOS queue
    RequestableCallbackFn output_cb;    // external function which handles the event
    uint32_t expected_sync_offset;      // timestamp for the next expected output event

    uint8_t entry_size;                     // size of a single entry
    uint8_t num_slots;                      // number of entries that fit in this pool
    uint8_t *storage_ptr;                   // refer to the storage buffer backing the pool
    uint32_t storage_size;                  // size of the storage pool in bytes
    EntryMetadata_t slots[MAX_POOL_SIZE];   // sidecar information for each pool entries
} RequestPool_t;

uint8_t movement_storage[POOL_STORAGE_BYTES_MOVEMENT] = { 0 };
uint8_t fade_storage[POOL_STORAGE_BYTES_FADES] = { 0 };

PRIVATE RequestPool_t pools[NUM_REQUEST_HANDLERS] = { 0 };

/* -------------------------------------------------------------------------- */

PUBLIC void request_handler_init( RequestHandlerInstance_t instance )
{
    REQUIRE( instance < NUM_REQUEST_HANDLERS );
    RequestPool_t *me = &pools[instance];
    memset( me, 0, sizeof( RequestPool_t ) );

    me->instance = instance;    // TODO: resolve this hack to get around the task pvParameters arg being a pointer to the pool instead of the instance enum

    // Create and initialize FreeRTOS queues for this instance
    switch( instance )
    {
        case REQUEST_HANDLER_MOVES:
            me->entry_size = sizeof(Movement_t);
            me->num_slots = MAX_POOL_SIZE;    // TOOD: temp variable until per-instance slot sizing variations are supported
            me->storage_size = me->entry_size * me->num_slots;

            ENSURE( sizeof(movement_storage) >= me->storage_size );
            me->storage_ptr = &movement_storage[0];

            me->input_queue = xQueueCreate( REQUEST_HANDLER_INPUT_QUEUE_SIZE, me->entry_size  );
            vQueueAddToRegistry( me->input_queue, "rqhMove"); // Debug view annotations
            break;
        case REQUEST_HANDLER_FADES:
            me->entry_size = sizeof(Fade_t);
            me->num_slots = MAX_POOL_SIZE;    // TOOD: temp variable until per-instance slot sizing variations are supported
            me->storage_size = me->entry_size * me->num_slots;

            ENSURE( sizeof(fade_storage) >= me->storage_size );
            me->storage_ptr = &fade_storage[0];

            me->input_queue = xQueueCreate( REQUEST_HANDLER_INPUT_QUEUE_SIZE, me->entry_size );
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
    return (void*)&pools[instance];
}

/* -------------------------------------------------------------------------- */

PUBLIC void request_handler_attach_output_callback( RequestHandlerInstance_t instance, RequestableCallbackFn callback )
{
    REQUIRE( instance < NUM_REQUEST_HANDLERS );

    switch( callback.type )
    {
        case CALLBACK_INVALID:
            // Reset it to an invalid/empty callback
            memset( &pools[instance].output_cb, 0, sizeof(RequestableCallbackFn) );
            break;

        case CALLBACK_MOVEMENT:
            // ensure that the callback has the right type for the pool
            REQUIRE( instance == REQUEST_HANDLER_MOVES );
            pools[instance].output_cb = callback;
            break;

        case CALLBACK_FADE:
            REQUIRE( instance == REQUEST_HANDLER_FADES );
            pools[instance].output_cb = callback;
            break;

        default:
            ASSERT(false);
            break;
    }
}

/* -------------------------------------------------------------------------- */

// TODO: Currently needs to be large enough for the biggest entry type
//       - consider dynamically allocating it per-task based on each task's known entry size?
uint8_t incoming_entry_buffer[sizeof(Movement_t)] = { 0 };

PUBLIC void request_handler_task( void *arg )
{
    REQUIRE( arg );
    RequestPool_t *pool = arg;
    ENSURE( pool );

    for(;;)
    {
        BaseType_t req_needs_slot = xQueueReceive(pool->input_queue,
                                           (void *)&incoming_entry_buffer,
                                           5 // portMAX_DELAY
                                           );

        if( req_needs_slot )
        {
            request_handler_insert_entry( pool->instance, &incoming_entry_buffer);
        }

        // Keep the output queue populated with ordered entries
        request_handler_emit_ordered_entries( pool->instance );
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t request_handler_add_movement( Movement_t *movement )
{
    return request_handler_add( REQUEST_HANDLER_MOVES,
                                movement,
                                sizeof( Movement_t ) );
}

PUBLIC uint32_t request_handler_add_fade( Fade_t *fade )
{
    return request_handler_add( REQUEST_HANDLER_FADES,
                                fade,
                                sizeof( Fade_t ) );
}

PRIVATE uint32_t request_handler_add( RequestHandlerInstance_t instance, void *entry_ptr, uint32_t entry_size )
{
    REQUIRE( instance < NUM_REQUEST_HANDLERS );
    REQUIRE( entry_ptr );
    REQUIRE( entry_size );

    RequestPool_t *me = &pools[instance];

    ENSURE( entry_size == me->entry_size );

    BaseType_t result = xQueueSendToBack( me->input_queue, entry_ptr, (TickType_t)0 );
    ENSURE( result == pdPASS );

    return uxQueueSpacesAvailable( me->input_queue );
}

/* -------------------------------------------------------------------------- */

PUBLIC void request_handler_clear( RequestHandlerInstance_t instance )
{
    REQUIRE( instance < NUM_REQUEST_HANDLERS );
    RequestPool_t *me = &pools[instance];

    // Empty the input/output queues
    xQueueReset( me->input_queue );

    // TODO: Does this need to be behind a mutex for safety?

    // Wipe the pool storage
    memset( &me->slots, 0, sizeof(me->slots));
    memset( me->storage_ptr, 0, me->storage_size);

    me->expected_sync_offset = 0;
}

/* -------------------------------------------------------------------------- */

PRIVATE void request_handler_insert_entry( RequestHandlerInstance_t instance, void *entry_ptr )
{
    REQUIRE( instance < NUM_REQUEST_HANDLERS );
    REQUIRE( entry_ptr );
    RequestPool_t *me = &pools[instance];

    // Find a slot in the pool for the entry
    uint32_t slot_index = request_handler_find_free_pool_slot( instance );
    ENSURE( slot_index >= 0 );

    // Calculate the memory offset corresponding to this slot in the mempool then copy it over
    uint32_t entry_offset = slot_index * me->entry_size;
    ENSURE( entry_offset + me->entry_size <= me->storage_size );

    memcpy( &me->storage_ptr[entry_offset], entry_ptr, me->entry_size );

    // Update the metadata for the entry
    me->slots[slot_index].is_used = true;
    switch( instance )
    {
        case REQUEST_HANDLER_MOVES:
        {
            Movement_t *move = entry_ptr;
            me->slots[slot_index].key = move->sync_offset;
        }
        break;

        case REQUEST_HANDLER_FADES:
        {
            Fade_t *fade = entry_ptr;
            me->slots[slot_index].key = fade->sync_offset;
        }
        break;

        default:
            ASSERT(false);
        break;
    }
}

/* -------------------------------------------------------------------------- */

PRIVATE int32_t request_handler_find_free_pool_slot( RequestHandlerInstance_t instance )
{
    REQUIRE( instance < NUM_REQUEST_HANDLERS );
    RequestPool_t *me = &pools[instance];

    for( uint32_t i = 0; i < me->num_slots; i++ )
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
    RequestPool_t *me = &pools[instance];

    uint32_t min_sync_offset = UINT32_MAX;
    int32_t min_sync_offset_slot_index = -1;

    for( uint32_t i = 0; i < me->num_slots; i++ )
    {
        if(    me->slots[i].is_used
            && me->slots[i].key < min_sync_offset)
        {
            min_sync_offset = me->slots[i].key;
            min_sync_offset_slot_index = (int32_t)i;
        }
    }

    return min_sync_offset_slot_index;
}

/* -------------------------------------------------------------------------- */

PRIVATE void request_handler_emit_ordered_entries( RequestHandlerInstance_t instance )
{
    REQUIRE( instance < NUM_REQUEST_HANDLERS );
    RequestPool_t *me = &pools[instance];

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

        EntryMetadata_t *entry = &me->slots[candidate_slot_index];

        // Is it in-order?
        if(    entry->key == me->expected_sync_offset
            || entry->key == 0 )   // first move will have an offset of zero
        {
            uint32_t entry_offset = candidate_slot_index * me->entry_size;

            // Dispatch the move via the output callback
            switch( me->output_cb.type )
            {
                //TODO: check if a simple 'not invalid' check is sufficient
                //      if it is, don't handle each type, just call the union with a void pointer?
                case CALLBACK_MOVEMENT:
                {
                    // Get the entry
                    Movement_t *move = (Movement_t *)&me->storage_ptr[entry_offset];
                    me->expected_sync_offset = move->sync_offset + move->duration;

                    queue_spaces = me->output_cb.fn.move( move );
                }
                    break;

                case CALLBACK_FADE:
                {
                    Fade_t *fade = (Fade_t *)&me->storage_ptr[entry_offset];
                    me->expected_sync_offset = fade->sync_offset + fade->duration;

                    queue_spaces = me->output_cb.fn.fade( fade );
                }
                    break;

                default:
                    ASSERT(false);
                    break;
            }

            // Mark the slot as free for use
            me->slots[candidate_slot_index].is_used = false;
        }
    }
}

/* -------------------------------------------------------------------------- */