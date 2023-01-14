/* ----- System Includes ---------------------------------------------------- */

#include <math.h>
#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include "average_short.h"
#include "app_times.h"
#include "app_signals.h"
#include "event_subscribe.h"
#include "qassert.h"

#include "clearpath.h"

#include "configuration.h"
#include "motion_types.h"
#include "user_interface.h"

/* ----- Defines ------------------------------------------------------------ */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ----- Private Variables -------------------------------------------------- */

PRIVATE int32_t effector_position;    // position of the expansion effector
PRIVATE int32_t requested_position;   // position we should try to reach in this tick
PRIVATE bool new_target = false;

PRIVATE AverageShort_t movement_statistics;

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
expansion_init( void )
{
    effector_position = 0;
    requested_position = 0;
    average_short_init( &movement_statistics, SPEED_ESTIMATOR_SPAN );
}

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
expansion_get_speed( void )
{
    // Stats provide 50 ticks (milliseconds) worth of distances travelled
    // therefore, the total microns travelled in 50ms * 20 = microns per second
    uint32_t microns_per_second = average_short_get_sum( &movement_statistics ) * 20;

    return microns_per_second;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
expansion_request_target( int32_t position )
{
    requested_position = position;
    new_target = true;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
expansion_set_home( void )
{
    effector_position = 0;
}

/* -------------------------------------------------------------------------- */

PUBLIC int32_t
expansion_get_position( void )
{
    return effector_position;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
expansion_process( void )
{
    if( new_target )
    {
        float angle_target = 0.0f;  // target motor shaft angle in degrees

        // Calculate the distance change for this tick
        uint32_t proposed_distance_um = 0;  // cartesian_distance_between( &requested_position, &effector_position );

        // Check the instantaneous (1ms) distance request won't exceed effector limits
        // As microns/millisecond = millimeters/second, we can use take the mm/sec limit
        if( proposed_distance_um > configuration_get_expansion_speed_limit() )
        {
            // Violations shouldn't reach the effectors, so E-STOP immediately
            eventPublish( EVENT_NEW( StateEvent, MOTION_EMERGENCY ) );
        }
        else    // under the speed limit
        {
            // Keep track of the distance change over time for speed stats
            average_short_update( &movement_statistics, (uint16_t)proposed_distance_um );

            // Calculate a motor solution, done using the 'point follower' style


            // Ask the motor to please move there
            servo_set_target_angle_limited( _CLEARPATH_4, angle_target );

            effector_position = requested_position;
            requested_position = 0;
            new_target = false;
        }
    }
    else
    {
        average_short_update( &movement_statistics, 0 );
    }

    // TODO: Update user interface with stats?

}

/* ----- End ---------------------------------------------------------------- */
