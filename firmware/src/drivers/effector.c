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
#include "app_events.h"

#include "clearpath.h"
#include "kinematics.h"
#include "motion_types.h"

/* ----- Defines ------------------------------------------------------------ */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ----- Private Variables -------------------------------------------------- */

PRIVATE CartesianPoint_t effector_position;    // position of the end effector
PRIVATE CartesianPoint_t requested_position;   // position we should try to reach in this tick
PRIVATE bool new_target = false;

PRIVATE AverageShort_t movement_statistics;

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
effector_init( void )
{
    kinematics_init();

    memset( &effector_position, 0, sizeof(CartesianPoint_t) );
    average_short_init( &movement_statistics, SPEED_ESTIMATOR_SPAN );
}

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
effector_get_speed( void )
{
    // Stats provide 50 ticks (milliseconds) worth of distances travelled
    // therefore, the total microns travelled in 50ms * 20 = microns per second
    uint32_t microns_per_second = average_short_get_sum( &movement_statistics ) * 20;

    return microns_per_second;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
effector_request_target( CartesianPoint_t *position )
{
    REQUIRE( position );

    memcpy( &requested_position, position, sizeof( CartesianPoint_t ) );
    new_target = true;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
effector_set_home( void )
{
    effector_position.x = 0;
    effector_position.y = 0;
    effector_position.z = 0;
}

/* -------------------------------------------------------------------------- */

PUBLIC CartesianPoint_t
effector_get_position( void )
{
    return effector_position;
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
effector_is_near_home( void )
{
    CartesianPoint_t position = effector_get_position();

    bool x_homed = IS_IN_DEADBAND( position.x, 0, MM_TO_MICRONS( 0.1 ) );
    bool y_homed = IS_IN_DEADBAND( position.y, 0, MM_TO_MICRONS( 0.1 ) );
    bool z_homed = IS_IN_DEADBAND( position.z, 0, MM_TO_MICRONS( 0.1 ) );

    return ( x_homed && y_homed && z_homed );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
effector_process( void )
{
    if( new_target )
    {
        JointAngles_t angle_target = { 0.0f, 0.0f, 0.0f }; // target motor shaft angle in degrees

        // Calculate the effector's distance change for this tick
        uint32_t proposed_distance_um = cartesian_distance_between( &requested_position, &effector_position );

        // Check the instantaneous (1ms) distance request won't exceed effector limits
        // As microns/millisecond = millimeters/second, we can use take the mm/sec limit
        if( proposed_distance_um > EFFECTOR_SPEED_LIMIT )
        {
            // Violations shouldn't reach the effector kinematics, so E-STOP immediately
            //   motion requests from path planning shouldn't queue/plan over-speed moves!
            EmergencyStopEvent *estop_evt = EVENT_NEW( EmergencyStopEvent, MOTION_EMERGENCY );
            if( estop_evt )
            {
                estop_evt->cause = EMERGENCY_VIOLATION;
                eventPublish( (StateEvent *)estop_evt );
            }
        }
        else    // under the speed limit
        {
            // Keep track of the distance change over time for speed stats
            average_short_update( &movement_statistics, (uint16_t)proposed_distance_um );

            // Calculate a motor angle solution for the cartesian position
            kinematics_point_to_angle( requested_position, &angle_target );

            // Ask the motors to please move there
            servo_set_target_angle_limited( _CLEARPATH_1, angle_target.a1 );
            servo_set_target_angle_limited( _CLEARPATH_2, angle_target.a2 );
            servo_set_target_angle_limited( _CLEARPATH_3, angle_target.a3 );

            memcpy( &effector_position, &requested_position, sizeof(CartesianPoint_t) );
            memset( &requested_position, 0, sizeof(CartesianPoint_t));
            new_target = false;
        }
    }
    else
    {
        average_short_update( &movement_statistics, 0 );
    }
}

/* -------------------------------------------------------------------------- */

/* ----- End ---------------------------------------------------------------- */
