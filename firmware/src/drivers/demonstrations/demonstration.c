/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "demonstration.h"
#include "internal_sequence.h"
#include "attractor_sequence.h"

#include "user_interface.h"

#include "app_events.h"
#include "app_signals.h"
#include "event_subscribe.h"

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */

uint8_t move_index_progress = 0;


    /* -------------------------------------------------------------------------- */

PUBLIC void
demonstration_init( void )
{
    move_index_progress = 0;
}

/* -------------------------------------------------------------------------- */

PUBLIC uint8_t
demonstration_enqueue_moves( uint8_t max_moves_emitted )
{
    uint8_t moves_emitted = 0;
    Movement_t next_move = { 0 };

    uint8_t demo_to_run = user_interface_get_attractor_species();

    while( moves_emitted < max_moves_emitted )
    {
        // Is it a hard-coded demo sequence
        if( demo_to_run == 0 )
        {
            // Ask the sequencer for the next move
            internal_sequence_get_move( move_index_progress, &next_move );
            move_index_progress += 1;
        }

        // Strange Effector
        if( demo_to_run > 1 )
        {
            attractor_sequence_get_move( move_index_progress, &next_move );
            move_index_progress += 1;
        }

        // Check if a new move was provided
        if( !next_move.num_pts )
        {
            break;
        }

        // Generate the event
        MotionPlannerEvent *motion_request = EVENT_NEW( MotionPlannerEvent, MOTION_QUEUE_ADD );

        if( motion_request )
        {
            memcpy( &motion_request->move, &next_move, sizeof( Movement_t ) );
            eventPublish( (StateEvent *)motion_request );
            moves_emitted += 1;
        }

    }

    return moves_emitted;
}

/* -------------------------------------------------------------------------- */


/* ----- End ---------------------------------------------------------------- */
