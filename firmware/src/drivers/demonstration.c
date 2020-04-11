/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "demonstration.h"
#include "motion_types.h"

#include "event_subscribe.h"
#include "app_events.h"
#include "app_signals.h"

/* -------------------------------------------------------------------------- */

#define POINT_MM( X, Y, Z ) { .x=MM_TO_MICRONS(X), .y=MM_TO_MICRONS(Y), .z=MM_TO_MICRONS(Z) }

// Create an acceleration shaped line movement powered by a catmull spline with control points near the start and end points
#define SHAPING_WEIGHT 0.01 // how aggressively to acceleration shape catmull "lines"

#define POINT_PAIR_CATMULL_MM( X1, Y1, Z1, X2, Y2, Z2 ) { \
    POINT_MM(X1, Y1, Z1), \
    POINT_MM(X1 + ((X2 - X1) * SHAPING_WEIGHT), Y1 + ((Y2 - Y1) * SHAPING_WEIGHT), Z1 + ((Z2 - Z1) * SHAPING_WEIGHT)), \
    POINT_MM(X2 + ((X1 - X2) * SHAPING_WEIGHT), Y2 + ((Y1 - Y2) * SHAPING_WEIGHT), Z2 + ((Z1 - Z2) * SHAPING_WEIGHT)), \
    POINT_MM(X2, Y2, Z2) \
}



#define MOVE_TO( DURATION, POINT ) {.type =_POINT_TRANSIT, .ref =_POS_ABSOLUTE, .identifier=0, .duration=DURATION, .num_pts=1, .points={ POINT }}
#define MOVE_BETWEEN( DURATION, FROM, TO ) {.type =_LINE, .ref =_POS_ABSOLUTE, .identifier=0, .duration=DURATION, .num_pts=2, .points={ FROM, TO}}
#define MOVE_BETWEEN_SMOOTH( DURATION, X1, Y1, Z1, X2, Y2, Z2 ) {.type =_CATMULL_SPLINE, .ref =_POS_ABSOLUTE, .identifier=0, .duration=DURATION, .num_pts=4, .points=POINT_PAIR_CATMULL_MM(X1, Y1, Z1, X2, Y2, Z2) }


/* -------------------------------------------------------------------------- */

static const Movement_t demo_one[] = {
        MOVE_BETWEEN_SMOOTH(2000, 0,0,0, 0,0,50),
//        MOVE_TO( 2000, POINT_MM(0,0,50)),

        MOVE_TO( 500,  POINT_MM(20,0,50)),
        MOVE_TO( 1000, POINT_MM(-20,0,50)),
        MOVE_TO( 500,  POINT_MM(0,0,50)),

        MOVE_TO( 500,  POINT_MM(0,20,50)),
        MOVE_TO( 1000, POINT_MM(0,-20,50)),
        MOVE_TO( 500,  POINT_MM(0,0,50)),


//        {.type =_POINT_TRANSIT, .ref =_POS_ABSOLUTE, .identifier=1, .duration=500, .num_pts=2, .points={ {.x = 23, .y=53, .z=50}, {.x = 5, .y=3, .z=8} }},
        };

/* -------------------------------------------------------------------------- */

PRIVATE void demonstration_emit_event( uint8_t sequence );


/* -------------------------------------------------------------------------- */

PUBLIC void
demonstration_init( void )
{


}

/* -------------------------------------------------------------------------- */

PUBLIC void
demonstration_prepare_demo_move( uint8_t demo_index )
{

}

/* -------------------------------------------------------------------------- */

PUBLIC void
demonstration_prepare_sequence( void )
{
    for( uint8_t i = 0; i < DIM(demo_one); i++ )
    {
        demonstration_emit_event( i );
    }
}

/* -------------------------------------------------------------------------- */


PRIVATE void
demonstration_emit_event( uint8_t sequence )
{
    // TODO Ensure the requested index isn't out of sequence (no gaps)
    // TODO Validate that the effector is at the starting point for the event

    if( sequence <= DIM(demo_one) )
    {
        // Generate the event
        MotionPlannerEvent *motion_request = EVENT_NEW( MotionPlannerEvent, MOTION_QUEUE_ADD );

        if(motion_request)
        {
            memcpy(&motion_request->move, &demo_one[sequence], sizeof(Movement_t));
            motion_request->move.identifier = sequence;   // set the ID of the move, as the demo array doesn't have any
            eventPublish( (StateEvent*)motion_request );
        }

    }
}

/* ----- End ---------------------------------------------------------------- */
