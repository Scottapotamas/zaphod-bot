#include "request_handler.h"
#include "qassert.h"
#include <string.h>

DEFINE_THIS_FILE;

#define MAX_POOL_SIZE 10

typedef struct {
    Movement_t movement;
    bool is_used;
} MovementPoolSlot_t;

typedef struct
{
    MovementPoolSlot_t slots[MAX_POOL_SIZE];
} MovementPool_t;

PRIVATE MovementPool_t pool = { 0 };


PRIVATE void request_handler_insert_movement(RequestHandler_t *rh, const Movement_t *movement);
PRIVATE int32_t request_handler_find_free_pool_slot( void );
PRIVATE bool request_handler_emit_next_movement( RequestHandler_t *rh );

PUBLIC void request_handler_init(RequestHandler_t *rh)
{
    // Create and initialize input and output FreeRTOS queues
    rh->input_queue = xQueueCreate( MAX_QUEUE_SIZE, sizeof(Movement_t) );
    rh->output_queue = xQueueCreate( MAX_QUEUE_SIZE, sizeof(Movement_t) );

    // Debug view annotations
    vQueueAddToRegistry( rh->input_queue, "rqHin");
    vQueueAddToRegistry( rh->output_queue, "rqHout");

}

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

PUBLIC void request_handler_add_movement(RequestHandler_t *rh, const Movement_t *movement)
{
    REQUIRE( rh );
    REQUIRE( movement );

    BaseType_t result = xQueueSendToBack(rh->input_queue, (void *)movement, (TickType_t)0);
    REQUIRE( result == pdPASS );
}

PRIVATE void request_handler_insert_movement(RequestHandler_t *rh, const Movement_t *movement)
{
    REQUIRE( rh );
    REQUIRE( movement );

    uint32_t slot_index = request_handler_find_free_pool_slot();
    REQUIRE( slot_index >= 0 );

    memcpy( &pool.slots[slot_index].movement, movement, sizeof(Movement_t) );
    pool.slots[slot_index].is_used = true;
}

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

PRIVATE bool request_handler_emit_next_movement( RequestHandler_t *rh )
{
    REQUIRE( rh );

    uint32_t min_sync_offset = UINT32_MAX;
    int32_t min_sync_offset_slot_index = -1;

    // Find the smallest sync_offset entry in the pool
    for( uint32_t i = 0; i < MAX_POOL_SIZE; i++ )
    {
        if(    pool.slots[i].is_used
            && pool.slots[i].movement.sync_offset < min_sync_offset)
        {
            min_sync_offset = pool.slots[i].movement.sync_offset;
            min_sync_offset_slot_index = (int32_t)i;
        }
    }

    // Put the movement into the output queue
    if( min_sync_offset_slot_index != -1 )
    {
        Movement_t *movement = &pool.slots[min_sync_offset_slot_index].movement;

        // Allow short block here for the output queue to drain, should have room though...
        BaseType_t result = xQueueSendToBack( rh->output_queue,
                                              (void *)movement,
                                              2     //portMAX_DELAY
        );

        // As emit ran with the assumption that the queue had room,
        // and we allow 2ms queue timeout, failure now means the queue is full/stuck?
        // Realistically shouldn't occur someone else is adding to that queue?
        ENSURE( result );

        // Mark the pool entry
        pool.slots[min_sync_offset_slot_index].is_used = false;

        return result;
    }

    return false;
}