/* -------------------------------------------------------------------------- */

#include <string.h>

#include "request_handler.h"
#include "qassert.h"
#include "broker.h"

#include "FreeRTOS.h"
#include "queue.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE;

#define MAX_POOL_SIZE 100

#define POOL_STORAGE_BYTES_MOVEMENT ( MAX_POOL_SIZE * sizeof(Movement_t) )
#define POOL_STORAGE_BYTES_FADES ( MAX_POOL_SIZE * sizeof(Fade_t) )

/* -------------------------------------------------------------------------- */

PRIVATE void request_handler_insert_entry( RequestHandlerInstance_t instance, void *entry_ptr );

PRIVATE uint32_t request_handler_add( RequestHandlerInstance_t handler, void *entry_ptr, uint32_t entry_size );

PRIVATE bool request_handler_find_free_pool_slot( RequestHandlerInstance_t instance, uint32_t *slot );

PRIVATE bool request_handler_clear_pool_slot( RequestHandlerInstance_t instance, uint32_t slot_index );

PRIVATE bool request_handler_find_candidate( RequestHandlerInstance_t instance, uint32_t *slot );

PRIVATE void request_handler_emit_ordered_entries( RequestHandlerInstance_t instance );

PRIVATE uint32_t request_handler_poll_output_queue_usage( RequestHandlerInstance_t instance );

PRIVATE void request_handler_notify_usage_metrics( RequestHandlerInstance_t instance );

PRIVATE void request_handler_notify_missing_entry( RequestHandlerInstance_t instance, uint32_t key );

/* -------------------------------------------------------------------------- */

/*  TODO Request Handler Refactor Notes
 *      - State storage structure cleanup
 *      - Remove the task from the sorting/pool implementations
 *      - Separation of responsibilities
 *      - Look into a more efficient storage data structure,
 *      - Look into search performance optimisations
 * */

typedef struct {
    uint32_t key;   // UUID or sortable index value
    bool is_used;   // marks slot usage in the pool
    // TODO: there are 3 wasted bytes here
} EntryMetadata_t;

typedef struct
{
    RequestHandlerInstance_t instance;
    QueueHandle_t input_queue;          // input FreeRTOS queue
    RequestableCallbackFn output_cb;    // external function which handles the event

    uint32_t expected_sync_offset;      // timestamp for the next expected output event

    uint8_t entry_size;                     // size of a single entry
    uint8_t num_slots;                      // number of entries that fit in this pool
    uint8_t num_slots_used;
    uint8_t *storage_ptr;                   // refer to the storage buffer backing the pool
    uint32_t storage_size;                  // size of the storage pool in bytes
    EntryMetadata_t slots[MAX_POOL_SIZE];   // sidecar information for each pool entries
} RequestPool_t;

PRIVATE uint8_t movement_storage[POOL_STORAGE_BYTES_MOVEMENT] = { 0 };
PRIVATE uint8_t fade_storage[POOL_STORAGE_BYTES_FADES] = { 0 };

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

    return ( me->num_slots_used * 100 / (me->num_slots) );
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
    me->num_slots_used = 0;
    me->expected_sync_offset = 0;

    request_handler_notify_usage_metrics( instance );
}

/* -------------------------------------------------------------------------- */

PRIVATE void request_handler_insert_entry( RequestHandlerInstance_t instance, void *entry_ptr )
{
    REQUIRE( instance < NUM_REQUEST_HANDLERS );
    REQUIRE( entry_ptr );
    RequestPool_t *me = &pools[instance];

    // Find a slot in the pool for the entry
    uint32_t slot_index = 0;
    bool slot_available = request_handler_find_free_pool_slot( instance, &slot_index );
    ENSURE( slot_available );

    // Calculate the memory offset corresponding to this slot in the mempool
    uint32_t entry_offset = slot_index * me->entry_size;
    ENSURE( entry_offset + me->entry_size <= me->storage_size );

    // Copy it into the pool
    memcpy( &me->storage_ptr[entry_offset], entry_ptr, me->entry_size );

    // Update the metadata for the entry
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
            ASSERT_PRINTF(false, "Invalid pool instance");
        break;
    }

    me->slots[slot_index].is_used = true;
    me->num_slots_used += 1;
    request_handler_notify_usage_metrics( instance );
}

/* -------------------------------------------------------------------------- */

PRIVATE bool request_handler_find_free_pool_slot( RequestHandlerInstance_t instance, uint32_t *slot )
{
    REQUIRE( slot );
    REQUIRE( instance < NUM_REQUEST_HANDLERS );
    RequestPool_t *me = &pools[instance];

    for( uint32_t i = 0; i < me->num_slots; i++ )
    {
        if( !me->slots[i].is_used )
        {
            *slot = i;
            return true;
        }
    }

    return false;
}

/* -------------------------------------------------------------------------- */

PRIVATE bool request_handler_clear_pool_slot( RequestHandlerInstance_t instance, uint32_t slot_index )
{
    REQUIRE( instance < NUM_REQUEST_HANDLERS );
    RequestPool_t *me = &pools[instance];

    REQUIRE( slot_index <= me->num_slots );

    // TODO: consider wiping the underlying memory in the pool?

    // Mark the slot as free for use
    me->slots[slot_index].is_used = false;
    me->slots[slot_index].key     = 0;

    me->num_slots_used -= 1;

    return true;
}

/* -------------------------------------------------------------------------- */

// Find the smallest sync_offset entry in the pool
// Returns true if an entry was found
PRIVATE bool request_handler_find_candidate( RequestHandlerInstance_t instance, uint32_t *slot )
{
    REQUIRE( slot );
    REQUIRE( instance < NUM_REQUEST_HANDLERS );
    RequestPool_t *me = &pools[instance];

    bool found_a_candidate = false;
    uint32_t min_sync_offset = UINT32_MAX;

    for( uint32_t i = 0; i < me->num_slots; i++ )
    {
        if(    me->slots[i].is_used
            && me->slots[i].key < min_sync_offset)
        {
            min_sync_offset = me->slots[i].key;
            *slot = i;
            found_a_candidate = true;
        }
    }

    return found_a_candidate;
}

/* -------------------------------------------------------------------------- */

PRIVATE void request_handler_emit_ordered_entries( RequestHandlerInstance_t instance )
{
    REQUIRE( instance < NUM_REQUEST_HANDLERS );
    RequestPool_t *me = &pools[instance];

    // Is there room in the destination queue?
    uint32_t queue_pressure = request_handler_poll_output_queue_usage( instance );

    // Fill the queue to a reasonable backpressure
    while( queue_pressure < 90 && me->num_slots_used )
    {
        uint32_t candidate_slot_index = 0;
        bool candidate_found = request_handler_find_candidate( instance, &candidate_slot_index );

        if( !candidate_found )
        {
            return;
        }

        // Determine if the entry is suitable to emit
        // If the entry has a key (aka sync_offset) of 0, it's the first one to emit
        EntryMetadata_t *entry = &me->slots[candidate_slot_index];
        bool candidate_valid = ( entry->key == me->expected_sync_offset || entry->key == 0 );

        if( !candidate_valid )
        {
            // Stale entries should be culled
            if( entry->key < me->expected_sync_offset)
            {
                request_handler_clear_pool_slot( instance, candidate_slot_index );
            }
            else
            {
                // TODO: see if anything can be done if it's missing?
                request_handler_notify_missing_entry( instance, me->expected_sync_offset );
            }

            return;
        }

        // Dispatch the entry via the output callback
        uint32_t entry_offset = candidate_slot_index * me->entry_size;
        switch( me->output_cb.type )
        {
            case CALLBACK_MOVEMENT:
            {
                // Get the entry, pass it to the output handler function
                Movement_t *move = (Movement_t *)&me->storage_ptr[entry_offset];
                queue_pressure   = me->output_cb.fn.move( move );

                // Update internal state with the next expected key value
                me->expected_sync_offset = move->sync_offset + move->duration;
            }
            break;

            case CALLBACK_FADE:
            {
                Fade_t *fade   = (Fade_t *)&me->storage_ptr[entry_offset];
                queue_pressure = me->output_cb.fn.fade( fade );

                me->expected_sync_offset = fade->sync_offset + fade->duration;
            }
            break;

            default:
                ASSERT_PRINTF(false, "Pool attempted emit to invalid callback");
                break;
        }

        request_handler_clear_pool_slot( instance, candidate_slot_index );
        request_handler_notify_usage_metrics( instance );
    }
}

/* -------------------------------------------------------------------------- */

// Call the output handler function with a nullptr, it returns queue pressure
PRIVATE uint32_t request_handler_poll_output_queue_usage( RequestHandlerInstance_t instance )
{
    REQUIRE( instance < NUM_REQUEST_HANDLERS );
    RequestPool_t *me = &pools[instance];

    // Handle the callback union options
    switch( me->output_cb.type )
    {
        case CALLBACK_MOVEMENT:
            return me->output_cb.fn.move( NULL );

        case CALLBACK_FADE:
            return me->output_cb.fn.fade( NULL );

        case CALLBACK_INVALID:
        default:
            // no callback configured, can't poll it
            return 100;   // TODO: is emitting 'full' a sane choice here?
    }
}

/* -------------------------------------------------------------------------- */

PRIVATE void request_handler_notify_usage_metrics( RequestHandlerInstance_t instance )
{
    REQUIRE( instance < NUM_REQUEST_HANDLERS );
    RequestPool_t *me = &pools[instance];

    SYSTEM_EVENT_FLAG event = SYSTEM_NUM_FIELDS;
    switch( instance )
    {
        case REQUEST_HANDLER_MOVES:
            event = QUEUE_UTILISATION_MOVEMENT;
            break;
        case REQUEST_HANDLER_FADES:
            event = QUEUE_UTILISATION_LIGHTING;
            break;
        default:
            ASSERT(false);
            break;
    }

    PublishedEvent queue_pressure = { 0 };
    queue_pressure.topic = event;
    queue_pressure.data.stamped.timestamp = xTaskGetTickCount();
    queue_pressure.data.stamped.value.u32 = ( me->num_slots_used * 100 / (me->num_slots) );
    broker_publish( &queue_pressure );
}

PRIVATE void request_handler_notify_missing_entry( RequestHandlerInstance_t instance, uint32_t key )
{
    REQUIRE( instance < NUM_REQUEST_HANDLERS );
    RequestPool_t *me = &pools[instance];

    SYSTEM_EVENT_FLAG event = SYSTEM_NUM_FIELDS;
    switch( instance )
    {
        case REQUEST_HANDLER_MOVES:
            event = FLAG_MOVE_MISSING;
            break;
        case REQUEST_HANDLER_FADES:
            event = FLAG_FADE_MISSING;
            break;
        default:
            ASSERT(false);
            break;
    }

    PublishedEvent alert = { 0 };
    alert.topic = event;
    alert.data.stamped.timestamp = xTaskGetTickCount();
    alert.data.stamped.value.u32 = key;
    broker_publish( &alert );
}

/* -------------------------------------------------------------------------- */