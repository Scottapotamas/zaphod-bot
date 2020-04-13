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
#define POINT( X, Y, Z ) { .x=X, .y=Y, .z=Z }

// Create an acceleration shaped line movement powered by a bezier curve with control points near the start and end points
#define INTERP_AB( A, B, WEIGHT ) (A + ((B - A) * WEIGHT))
#define POINT_PAIR_CUBIC( X1, Y1, Z1, X2, Y2, Z2, SMOOTH ) { \
    POINT( X1, Y1, Z1 ), \
    POINT( INTERP_AB(X1, X2, SMOOTH), INTERP_AB(Y1, Y2, SMOOTH), INTERP_AB(Z1, Z2, SMOOTH) ), \
    POINT( INTERP_AB(X2, X1, SMOOTH), INTERP_AB(Y2, Y1, SMOOTH), INTERP_AB(Z2, Z1, SMOOTH) ), \
    POINT( X2, Y2, Z2 ) \
}

#define DELAY_MOVEMENT( DURATION ) {.type =_POINT_TRANSIT, .ref =_POS_RELATIVE, .identifier=0, .duration=DURATION, .num_pts=1, .points={ {0,0,0} }}
#define MOVE_TO( DURATION, TO ) {.type =_POINT_TRANSIT, .ref =_POS_ABSOLUTE, .identifier=0, .duration=DURATION, .num_pts=1, .points={ TO }}
#define MOVE_BETWEEN( DURATION, FROM, TO ) {.type =_LINE, .ref =_POS_ABSOLUTE, .identifier=0, .duration=DURATION, .num_pts=2, .points={ FROM, TO}}
#define MOVE_BETWEEN_SMOOTH( DURATION, SMOOTH, X1, Y1, Z1, X2, Y2, Z2 ) {.type =_BEZIER_CUBIC, .ref =_POS_ABSOLUTE, .identifier=0, .duration=DURATION, .num_pts=4, .points=POINT_PAIR_CUBIC(X1*1000, Y1*1000, Z1*1000, X2*1000, Y2*1000, Z2*1000, SMOOTH) }

/* -------------------------------------------------------------------------- */

static const Movement_t demo_one[] = {
        // Move from the home point to our normal height
        MOVE_BETWEEN_SMOOTH(1000, 0.005f, 0,0,0, 0,0,50),

//        MOVE_BETWEEN( 1000, POINT_MM(0,0,50), POINT_MM(-60,0,50)),
//        MOVE_BETWEEN( 1000, POINT_MM(-60,0,50), POINT_MM(60,0,50)),
//        MOVE_BETWEEN( 1000, POINT_MM(60,0,50), POINT_MM(0,0,50)),

        // linear moves on x axis, then linear moves on y axis
        DELAY_MOVEMENT( 100 ),
        MOVE_BETWEEN_SMOOTH(1000, 0.05f, 0,0,50, -60,0,50),
        MOVE_BETWEEN_SMOOTH(1000, 0.05f, -60,0,50, 60,0,50),
        MOVE_BETWEEN_SMOOTH(1000, 0.05f, 60,0,50, 0,0,50),
        DELAY_MOVEMENT( 100 ),
        MOVE_BETWEEN_SMOOTH(500,  0.05f, 0,0,50, 0,60,50),
        MOVE_BETWEEN_SMOOTH(250, 0.01f, 0,60,50, 0,-60,50),
        MOVE_BETWEEN_SMOOTH(500,  0.05f, 0,-60,50, 0,0,50),
        DELAY_MOVEMENT( 100 ),

        // rectangle on xy plane
        MOVE_BETWEEN_SMOOTH(800,  0.001f, 0,0,50, 45,45,50),
        DELAY_MOVEMENT( 100 ),
        MOVE_BETWEEN_SMOOTH(200,  0.01f, 45,45,50, -45,45,50),
        MOVE_BETWEEN_SMOOTH(200,  0.01f, -45,45,50, -45,-45,50),
        MOVE_BETWEEN_SMOOTH(200,  0.01f, -45,-45,50, 45,-45,50),
        MOVE_BETWEEN_SMOOTH(200,  0.01f, 45,-45,50, 45,45,50),
        DELAY_MOVEMENT( 100 ),
        MOVE_BETWEEN_SMOOTH(600,  0.001f, 45,45,50, 0,0,50),

        // Go home
        DELAY_MOVEMENT( 2000 ),
        MOVE_TO( 2000, POINT_MM(0,0,0)),

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
