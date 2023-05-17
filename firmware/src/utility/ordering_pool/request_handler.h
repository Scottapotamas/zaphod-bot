#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

/* -------------------------------------------------------------------------- */

#include <stdint.h>

#include "movement_types.h"
#include "lighting_types.h"
#include "subject.h"

/* -------------------------------------------------------------------------- */

#define REQUEST_HANDLER_INPUT_QUEUE_SIZE 5

/* -------------------------------------------------------------------------- */

typedef enum
{
    REQUEST_HANDLER_MOVES = 0,
    REQUEST_HANDLER_FADES = 1,
    NUM_REQUEST_HANDLERS    // must be the last enum entry
} RequestHandlerInstance_t;

/* -------------------------------------------------------------------------- */

// Allow for a consistent API which allows for arbitrary callback function shapes being used
// for output from the pool

typedef enum
{
    CALLBACK_INVALID = 0,
    CALLBACK_MOVEMENT,
    CALLBACK_FADE,
} CallbackTypes;

typedef uint32_t (*MovementRequestFn)(Movement_t *move);
typedef uint32_t (*LightingRequestFn)(Fade_t *fade);

typedef struct {
    CallbackTypes type;
    union {
        MovementRequestFn move;
        LightingRequestFn fade;
    } fn;
} RequestableCallbackFn;

/* -------------------------------------------------------------------------- */

PUBLIC void request_handler_init( RequestHandlerInstance_t instance );

/* -------------------------------------------------------------------------- */

PUBLIC void* request_handler_get_context_for( RequestHandlerInstance_t instance );

/* -------------------------------------------------------------------------- */

PUBLIC Subject* request_handler_get_subject_for( RequestHandlerInstance_t instance );

/* -------------------------------------------------------------------------- */

PUBLIC void request_handler_attach_output_callback( RequestHandlerInstance_t instance, RequestableCallbackFn callback );

/* -------------------------------------------------------------------------- */

// Arg points to a variable RequestHandlerInstance_t with the enum value for this task
PUBLIC void request_handler_task( void *arg );

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t request_handler_add_movement( Movement_t *movement );

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t request_handler_add_fade( Fade_t *fade );

/* -------------------------------------------------------------------------- */

PUBLIC void request_handler_clear( RequestHandlerInstance_t handler );

/* -------------------------------------------------------------------------- */

#endif // REQUEST_HANDLER_H