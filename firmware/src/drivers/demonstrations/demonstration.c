/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "demonstration.h"
#include "internal_sequence.h"
#include "attractor_sequence.h"
#include "random_points.h"

#include "app_events.h"
#include "app_signals.h"
#include "event_subscribe.h"

/* -------------------------------------------------------------------------- */

PRIVATE bool
demonstration_program_is_attractor( uint8_t demo_index );

/* -------------------------------------------------------------------------- */

PRIVATE uint8_t move_index_progress = 0;
PRIVATE uint8_t demo_to_run = 0;

/* -------------------------------------------------------------------------- */

PUBLIC void
demonstration_init( uint8_t selected_demo )
{
    demo_to_run = selected_demo;
    move_index_progress = 0;

    // TODO cleanup logic?
    if( demonstration_program_is_attractor( demo_to_run ) )
    {
        attractor_sequence_init( demo_to_run - 10 );
    }
    else
    {
        switch( demo_to_run )
        {
            case 0:
                internal_sequence_init();
                break;
            case 1:
                random_points_init();
                break;

        }
    }
}

/* -------------------------------------------------------------------------- */

PRIVATE bool
demonstration_program_is_attractor( uint8_t demo_index )
{
    return ( demo_index >= 10 && demo_index <= 20 );
}

/* -------------------------------------------------------------------------- */

PUBLIC uint8_t
demonstration_enqueue_moves( uint8_t max_moves_emitted )
{
    uint8_t moves_emitted = 0;
    Movement_t next_move = { 0 };

    while( moves_emitted < max_moves_emitted )
    {
        // Strange Effector
        if( demonstration_program_is_attractor( demo_to_run ) )
        {
            attractor_sequence_get_move( move_index_progress, &next_move );

            if( move_index_progress == 0 )
            {
                MotionPlannerEvent *transit = EVENT_NEW( MotionPlannerEvent, MOTION_QUEUE_ADD );

                if( transit )
                {
                    transit->move.type          = _POINT_TRANSIT;
                    transit->move.ref           = _POS_ABSOLUTE;
                    transit->move.sync_offset   = 0;
                    transit->move.duration      = 1000;
                    transit->move.num_pts       = 1;
                    transit->move.points[0].x   = next_move.points[_CATMULL_START].x;
                    transit->move.points[0].y   = next_move.points[_CATMULL_START].y;
                    transit->move.points[0].z   = next_move.points[_CATMULL_START].z;

                    eventPublish( (StateEvent *)transit );
                    moves_emitted += 1;
                }

            }

            move_index_progress += 1;
        }

        switch( demo_to_run )
        {
            case 0:
                // Ask the sequencer for the next move
                internal_sequence_get_move( move_index_progress, &next_move );
                move_index_progress += 1;
            break;

            case 1:
                random_points_get_move( move_index_progress, &next_move );
                move_index_progress += 1;
                break;

            default:
                // TODO handle an invalid demo being requested?
                break;
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
