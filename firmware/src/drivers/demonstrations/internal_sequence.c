/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "internal_sequence.h"
#include "qassert.h"

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* -------------------------------------------------------------------------- */

#define POINT_MM( X, Y, Z ) { .x=MM_TO_MICRONS(X), .y=MM_TO_MICRONS(Y), .z=MM_TO_MICRONS(Z) }
#define POINT( X, Y, Z ) { .x=(X), .y=(Y), .z=(Z) }

// Create an acceleration shaped line movement powered by a Bézier curve with control points near the start and end points
#define INTERP_AB( A, B, WEIGHT ) ((A) + (((B) - (A)) * WEIGHT))
#define POINT_PAIR_CUBIC( X1, Y1, Z1, X2, Y2, Z2, SMOOTH ) { \
    POINT( X1, Y1, Z1 ), \
    POINT( INTERP_AB(X1, X2, SMOOTH), INTERP_AB(Y1, Y2, SMOOTH), INTERP_AB(Z1, Z2, SMOOTH) ), \
    POINT( INTERP_AB(X2, X1, SMOOTH), INTERP_AB(Y2, Y1, SMOOTH), INTERP_AB(Z2, Z1, SMOOTH) ), \
    POINT( X2, Y2, Z2 ) \
}

#define DELAY_MOVEMENT( DURATION ) { .metadata = { .type =_POINT_TRANSIT, .ref =_POS_RELATIVE, .num_pts=1, }, .sync_offset=0, .duration=(DURATION), .points={ {0,0,0} }}
#define MOVE_TO( DURATION, TO ) { .metadata = {.type =_POINT_TRANSIT, .ref =_POS_ABSOLUTE, .num_pts=1, }, .sync_offset=0, .duration=(DURATION), .points={ TO }}
#define MOVE_BETWEEN( DURATION, FROM, TO ) {.type =_LINE, .ref =_POS_ABSOLUTE, .sync_offset=0, .duration=(DURATION), .num_pts=2, .points={ FROM, TO}}
#define MOVE_BETWEEN_SMOOTH( DURATION, SMOOTH, X1, Y1, Z1, X2, Y2, Z2 ) { .metadata = { .type =_BEZIER_CUBIC, .ref =_POS_ABSOLUTE,  .num_pts=4, }, .sync_offset=0, .duration=(DURATION), .points=POINT_PAIR_CUBIC((X1)*1000, (Y1)*1000, (Z1)*1000, (X2)*1000, (Y2)*1000, (Z2)*1000, (SMOOTH)) }

#define CUBIC_BEZIER( DURATION, A, B, C, D ) { .metadata = { .type =_BEZIER_CUBIC, .ref =_POS_ABSOLUTE, .num_pts=4, }, .sync_offset=0, .duration=(DURATION), .points={ A, B, C, D } }

/* -------------------------------------------------------------------------- */

static const Movement_t demo_one[] = {
    // Move from the home point to our normal height
    MOVE_TO(1200, POINT_MM( 0, 0, 0 ) ),
    MOVE_BETWEEN_SMOOTH( 1000, 0.005f, 0, 0, 0, 0, 0, 50 ),

    //        MOVE_BETWEEN( 1000, POINT_MM(0,0,50), POINT_MM(-60,0,50)),
    //        MOVE_BETWEEN( 1000, POINT_MM(-60,0,50), POINT_MM(60,0,50)),
    //        MOVE_BETWEEN( 1000, POINT_MM(60,0,50), POINT_MM(0,0,50)),

    // linear moves on x-axis, then linear moves on y-axis
    DELAY_MOVEMENT( 100 ),
    MOVE_BETWEEN_SMOOTH( 1000, 0.05f, 0, 0, 50, -60, 0, 50 ),
    MOVE_BETWEEN_SMOOTH( 1000, 0.05f, -60, 0, 50, 60, 0, 50 ),
    MOVE_BETWEEN_SMOOTH( 1000, 0.05f, 60, 0, 50, 0, 0, 50 ),
    DELAY_MOVEMENT( 100 ),
    MOVE_BETWEEN_SMOOTH( 500, 0.05f, 0, 0, 50, 0, 60, 50 ),
    MOVE_BETWEEN_SMOOTH( 250, 0.01f, 0, 60, 50, 0, -60, 50 ),
    MOVE_BETWEEN_SMOOTH( 500, 0.05f, 0, -60, 50, 0, 0, 50 ),
    DELAY_MOVEMENT( 100 ),
    MOVE_BETWEEN_SMOOTH( 500, 0.05f, 0, 0, 50, 0, 0, 30 ),
    MOVE_BETWEEN_SMOOTH( 200, 0.01f, 0, 0, 30, 0, 0, 80 ),
    MOVE_BETWEEN_SMOOTH( 500, 0.05f, 0, 0, 80, 0, 0, 50 ),
    DELAY_MOVEMENT( 100 ),

    // rectangle on xy plane
    MOVE_BETWEEN_SMOOTH( 800, 0.001f, 0, 0, 50, 45, 45, 50 ),
    DELAY_MOVEMENT( 100 ),
    MOVE_BETWEEN_SMOOTH( 200, 0.01f, 45, 45, 50, -45, 45, 50 ),
    MOVE_BETWEEN_SMOOTH( 200, 0.01f, -45, 45, 50, -45, -45, 50 ),
    MOVE_BETWEEN_SMOOTH( 200, 0.01f, -45, -45, 50, 45, -45, 50 ),
    MOVE_BETWEEN_SMOOTH( 200, 0.01f, 45, -45, 50, 45, 45, 50 ),

    // Move straight to circle location
    DELAY_MOVEMENT( 100 ),
    MOVE_BETWEEN_SMOOTH( 600, 0.001f, 45, 45, 50, 0, 90, 50 ),
    DELAY_MOVEMENT( 100 ),

    // Slow Circle
    CUBIC_BEZIER( 1000,
                  POINT_MM( 0, 90, 50 ),
                  POINT_MM( 49, 90, 51 ),
                  POINT_MM( 90, 49, 52 ),
                  POINT_MM( 90, 0, 53 ) ),

    CUBIC_BEZIER( 950,
                  POINT_MM( 90, 0, 53 ),
                  POINT_MM( 90, -49, 54 ),
                  POINT_MM( 49, -90, 55 ),
                  POINT_MM( 0, -90, 56 ) ),

    CUBIC_BEZIER( 900,
                  POINT_MM( 0, -90, 56 ),
                  POINT_MM( -49, -90, 57 ),
                  POINT_MM( -90, -49, 58 ),
                  POINT_MM( -90, 0, 59 ) ),

    CUBIC_BEZIER( 850,
                  POINT_MM( -90, 0, 59 ),
                  POINT_MM( -90, 49, 60 ),
                  POINT_MM( -49, 90, 61 ),
                  POINT_MM( 0, 90, 62 ) ),

    // speed slowing
    CUBIC_BEZIER( 800,
                  POINT_MM( 0, 90, 62 ),
                  POINT_MM( 49, 90, 63 ),
                  POINT_MM( 90, 49, 64 ),
                  POINT_MM( 90, 0, 65 ) ),

    CUBIC_BEZIER( 750,
                  POINT_MM( 90, 0, 65 ),
                  POINT_MM( 90, -49, 66 ),
                  POINT_MM( 49, -90, 67 ),
                  POINT_MM( 0, -90, 68 ) ),

    CUBIC_BEZIER( 700,
                  POINT_MM( 0, -90, 68 ),
                  POINT_MM( -49, -90, 69 ),
                  POINT_MM( -90, -49, 70 ),
                  POINT_MM( -90, 0, 71 ) ),

    CUBIC_BEZIER( 650,
                  POINT_MM( -90, 0, 71 ),
                  POINT_MM( -90, 49, 72 ),
                  POINT_MM( -49, 90, 73 ),
                  POINT_MM( 0, 90, 74 ) ),

    // speed slowing
    CUBIC_BEZIER( 600,
                  POINT_MM( 0, 90, 74 ),
                  POINT_MM( 49, 90, 72 ),
                  POINT_MM( 90, 49, 69 ),
                  POINT_MM( 90, 0, 66 ) ),

    CUBIC_BEZIER( 550,
                  POINT_MM( 90, 0, 66 ),
                  POINT_MM( 90, -49, 63 ),
                  POINT_MM( 49, -90, 60 ),
                  POINT_MM( 0, -90, 58 ) ),

    CUBIC_BEZIER( 500,
                  POINT_MM( 0, -90, 58 ),
                  POINT_MM( -49, -90, 56 ),
                  POINT_MM( -90, -49, 55 ),
                  POINT_MM( -90, 0, 54 ) ),

    CUBIC_BEZIER( 450,
                  POINT_MM( -90, 0, 54 ),
                  POINT_MM( -90, 49, 53 ),
                  POINT_MM( -49, 90, 53 ),
                  POINT_MM( 0, 90, 52 ) ),

    // speed slowing
    CUBIC_BEZIER( 400,
                  POINT_MM( 0, 90, 52 ),
                  POINT_MM( 49, 90, 52 ),
                  POINT_MM( 90, 49, 52 ),
                  POINT_MM( 90, 0, 51 ) ),

    CUBIC_BEZIER( 350,
                  POINT_MM( 90, 0, 51 ),
                  POINT_MM( 90, -49, 51 ),
                  POINT_MM( 49, -90, 51 ),
                  POINT_MM( 0, -90, 50 ) ),

    CUBIC_BEZIER( 300,
                  POINT_MM( 0, -90, 50 ),
                  POINT_MM( -49, -90, 50 ),
                  POINT_MM( -90, -49, 50 ),
                  POINT_MM( -90, 0, 49 ) ),

    CUBIC_BEZIER( 250,
                  POINT_MM( -90, 0, 49 ),
                  POINT_MM( -90, 49, 49 ),
                  POINT_MM( -49, 90, 49 ),
                  POINT_MM( 0, 90, 48 ) ),

    // Fast circle
    CUBIC_BEZIER( 250,
                  POINT_MM( 0, 90, 48 ),
                  POINT_MM( 49, 90, 47 ),
                  POINT_MM( 90, 49, 47 ),
                  POINT_MM( 90, 0, 46 ) ),
    CUBIC_BEZIER( 250,
                  POINT_MM( 90, 0, 46 ),
                  POINT_MM( 90, -49, 45 ),
                  POINT_MM( 49, -90, 45 ),
                  POINT_MM( 0, -90, 44 ) ),
    CUBIC_BEZIER( 250,
                  POINT_MM( 0, -90, 44 ),
                  POINT_MM( -49, -90, 43 ),
                  POINT_MM( -90, -49, 43 ),
                  POINT_MM( -90, 0, 42 ) ),
    CUBIC_BEZIER( 250,
                  POINT_MM( -90, 0, 42 ),
                  POINT_MM( -90, 49, 41 ),
                  POINT_MM( -49, 90, 41 ),
                  POINT_MM( 0, 90, 40 ) ),

    // Shrinking circle
    CUBIC_BEZIER( 275,
                  POINT_MM( 0, 90, 40 ),
                  POINT_MM( 46, 84, 40 ),
                  POINT_MM( 78, 43, 40 ),
                  POINT_MM( 72, 0, 40 ) ),
    CUBIC_BEZIER( 325,
                  POINT_MM( 72, 0, 40 ),
                  POINT_MM( 66, -39, 40 ),
                  POINT_MM( 33, -60, 40 ),
                  POINT_MM( 0, -54, 40 ) ),
    CUBIC_BEZIER( 350,
                  POINT_MM( 0, -54, 40 ),
                  POINT_MM( -26, -48, 40 ),
                  POINT_MM( -42, -23, 40 ),
                  POINT_MM( -36, 0, 40 ) ),
    CUBIC_BEZIER( 350,
                  POINT_MM( -36, 0, 40 ),
                  POINT_MM( -30, 17, 40 ),
                  POINT_MM( -13, 24, 40 ),
                  POINT_MM( 0, 18, 40 ) ),
    CUBIC_BEZIER( 375,
                  POINT_MM( 0, 18, 40 ),
                  POINT_MM( 6, 12, 40 ),
                  POINT_MM( 6, 3, 40 ),
                  POINT_MM( 0, 0, 40 ) ),

    // Go home
    DELAY_MOVEMENT( 2000 ),
    //        MOVE_TO( 2000, POINT_MM(0,0,0)),
    MOVE_BETWEEN_SMOOTH( 2000, 0.0001f, 0, 0, 40, 0, 0, 0 ),

    //        {.type =_POINT_TRANSIT, .ref =_POS_ABSOLUTE, .identifier=1, .duration=500, .num_pts=2, .points={ {.x = 23, .y=53, .z=50}, {.x = 5, .y=3, .z=8} }},
};

/* -------------------------------------------------------------------------- */

PRIVATE uint32_t previous_sync_offset = 0;

/* -------------------------------------------------------------------------- */

PUBLIC void
internal_sequence_init( void )
{
    previous_sync_offset = 0;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
internal_sequence_get_move( uint8_t sequence_index, Movement_t *move )
{
    REQUIRE( move );

    if( sequence_index <= DIM( demo_one ) )
    {
        // Copy the move out of the sequence
        memcpy( move, &demo_one[sequence_index], sizeof( Movement_t ) );

        // Set the timestamp offset for the movement now
        if( sequence_index > 0 )
        {
            uint32_t previous_move_duration = demo_one[sequence_index - 1].duration;
            move->sync_offset = previous_sync_offset + previous_move_duration + 1;
        }

        previous_sync_offset = move->sync_offset;
    }
}

/* ----- End ---------------------------------------------------------------- */
