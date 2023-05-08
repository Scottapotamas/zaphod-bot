#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

/* -------------------------------------------------------------------------- */

#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"

#include "movement_types.h"

/* -------------------------------------------------------------------------- */

#define MAX_QUEUE_SIZE 5

/* -------------------------------------------------------------------------- */

typedef struct MovementOrdering__
{
    QueueHandle_t input_queue;
    QueueHandle_t output_queue;
} RequestHandler_t;

/* -------------------------------------------------------------------------- */

PUBLIC void request_handler_init( RequestHandler_t *rh );

/* -------------------------------------------------------------------------- */

typedef void (*MovementRequestFn)(Movement_t *move);

PUBLIC void request_handler_attach_output_callback( MovementRequestFn callback );

/* -------------------------------------------------------------------------- */

PUBLIC void request_handler_task( void *arg );

/* -------------------------------------------------------------------------- */

PUBLIC void request_handler_add_movement( const Movement_t *movement );

/* -------------------------------------------------------------------------- */

PUBLIC void request_handler_clear( RequestHandler_t *rh );

/* -------------------------------------------------------------------------- */

#endif // REQUEST_HANDLER_H