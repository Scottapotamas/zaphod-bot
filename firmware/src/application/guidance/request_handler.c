/* -------------------------------------------------------------------------- */

#include "request_handler.h"
#include "qassert.h"
#include <string.h>

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE;

#define MAX_POOL_SIZE 10

/* -------------------------------------------------------------------------- */

PRIVATE void request_handler_insert_movement( RequestHandler_t *rh, const Movement_t *movement );
PRIVATE int32_t request_handler_find_free_pool_slot( void );
PRIVATE int32_t request_handler_find_sorted_candidate( void );
PRIVATE bool request_handler_emit_next_movement( RequestHandler_t *rh );

/* -------------------------------------------------------------------------- */

typedef struct {
    Movement_t movement;
    bool is_used;
} MovementPoolSlot_t;

typedef struct
{
    MovementPoolSlot_t slots[MAX_POOL_SIZE];
    uint32_t expected_sync_offset;
} MovementPool_t;

PRIVATE MovementPool_t pool = { 0 };

/* -------------------------------------------------------------------------- */

PUBLIC void request_handler_init( RequestHandler_t *rh )
{
    // Create and initialize input and output FreeRTOS queues
    rh->input_queue = xQueueCreate( MAX_QUEUE_SIZE, sizeof(Movement_t) );
    rh->output_queue = xQueueCreate( MAX_QUEUE_SIZE, sizeof(Movement_t) );

    // Debug view annotations
    vQueueAddToRegistry( rh->input_queue, "rqHin");
    vQueueAddToRegistry( rh->output_queue, "rqHout");

}

/* -------------------------------------------------------------------------- */

PUBLIC void request_handler_task( void *arg  )
{
    // arg is a pointer to the request handler object from task creation
    //  i.e. equiv to RequestHandler_t *rh
    RequestHandler_t *rh = arg;
    REQUIRE( rh->input_queue );
    REQUIRE( rh->output_queue );

    for(;;)
    {
        Movement_t incoming_movement;
        BaseType_t result = xQueueReceive(rh->input_queue,
                                           (void *)&incoming_movement,
                                           5 // portMAX_DELAY
                                           );

        if( result )
        {
            request_handler_insert_movement(rh, &incoming_movement);
        }

        // Keep the output queue populated with ordered movements
        while( uxQueueSpacesAvailable(rh->output_queue) > 0 )
        {
            // Emit the next movement if there's room in the output queue
            if (!request_handler_emit_next_movement( rh ))
            {
                // No moves output
                break;
            }
        }

    }
}

/* -------------------------------------------------------------------------- */

PUBLIC void request_handler_add_movement( RequestHandler_t *rh, const Movement_t *movement )
{
    REQUIRE( rh );
    REQUIRE( movement );

    BaseType_t result = xQueueSendToBack(rh->input_queue, (void *)movement, (TickType_t)0);
    REQUIRE( result == pdPASS );
}

/* -------------------------------------------------------------------------- */

PUBLIC void request_handler_clear( RequestHandler_t *rh )
{
    REQUIRE( rh );

    // TODO: Does this need to be behind a mutex for safety?

    // Empty the input/output queues
    xQueueReset( rh->input_queue );
    xQueueReset( rh->input_queue );

    // Wipe the pool
    memset( &pool, 0, sizeof(MovementPool_t));

}

/* -------------------------------------------------------------------------- */

PRIVATE void request_handler_insert_movement( RequestHandler_t *rh, const Movement_t *movement )
{
    REQUIRE( rh );
    REQUIRE( movement );

    uint32_t slot_index = request_handler_find_free_pool_slot();
    REQUIRE( slot_index >= 0 );

    memcpy( &pool.slots[slot_index].movement, movement, sizeof(Movement_t) );
    pool.slots[slot_index].is_used = true;
}

/* -------------------------------------------------------------------------- */

PRIVATE int32_t request_handler_find_free_pool_slot( void )
{
    for( uint32_t i = 0; i < MAX_POOL_SIZE; i++ )
    {
        if( !pool.slots[i].is_used)
        {
            return (int32_t)i;
        }
    }

    return -1;
}

/* -------------------------------------------------------------------------- */

// Find the smallest sync_offset entry in the pool

PRIVATE int32_t request_handler_find_sorted_candidate( void )
{
    uint32_t min_sync_offset = UINT32_MAX;
    int32_t min_sync_offset_slot_index = -1;

    for( uint32_t i = 0; i < MAX_POOL_SIZE; i++ )
    {
        if(    pool.slots[i].is_used
            && pool.slots[i].movement.sync_offset < min_sync_offset)
        {
            min_sync_offset = pool.slots[i].movement.sync_offset;
            min_sync_offset_slot_index = (int32_t)i;
        }
    }

    return min_sync_offset_slot_index;
}

/* -------------------------------------------------------------------------- */

PRIVATE bool request_handler_emit_next_movement( RequestHandler_t *rh )
{
    REQUIRE( rh );

    int32_t candidate_slot_index = request_handler_find_sorted_candidate();

    // Put the movement into the output queue
    if( candidate_slot_index != -1 )
    {
        Movement_t *movement = &pool.slots[candidate_slot_index].movement;

        // Is the move in-order?
        if(    movement->sync_offset == pool.expected_sync_offset
            || movement->sync_offset == 0 )   // first move will have an offset of zero
        {
            // Allowed to briefly block here for the output queue to drain, should have room though...
            BaseType_t result = xQueueSendToBack( rh->output_queue,
                                                  (void *)movement,
                                                  2     //portMAX_DELAY
            );

            // As emit ran with the assumption that the queue had room,
            // and we allow 2ms queue timeout, failure now means the queue is full/stuck?
            // Realistically shouldn't occur someone else is adding to that queue?
            ENSURE( result );

            // Update the pool and slot metadata
            pool.slots[candidate_slot_index].is_used = false;
            pool.expected_sync_offset = movement->sync_offset + movement->duration;

            return result;
        }

    }

    return false;
}

/* -------------------------------------------------------------------------- */