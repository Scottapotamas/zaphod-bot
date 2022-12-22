/* ----- System Includes ---------------------------------------------------- */

#include <math.h>
#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */
#include "global.h"
#include "average_short.h"
#include "app_times.h"

#include "kinematics.h"
#include "clearpath.h"

#include "configuration.h"
#include "motion_types.h"
#include "user_interface.h"

/* ----- Defines ------------------------------------------------------------ */

/* ----- Private Variables -------------------------------------------------- */

CartesianPoint_t effector_position;    // position of the end effector
CartesianPoint_t requested_position;   // position we should try to reach in this tick
bool new_target = false;

AverageShort_t movement_statistics;

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
    // Copy it in with some dirty flag?
    if( position )
    {
        memcpy( &requested_position, position, sizeof( CartesianPoint_t ) );
        new_target = true;
    }
    else
    {
        // TODO Assert on invalid input pointer?
    }

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

PUBLIC void
effector_process( void )
{
    if( new_target )
    {
        JointAngles_t angle_target = { 0.0f, 0.0f, 0.0f }; // target motor shaft angle in degrees

        // Calculate a motor angle solution for the cartesian position
        kinematics_point_to_angle( requested_position, &angle_target );

        // Calculate the effector's distance change for this tick
        uint32_t proposed_distance = cartesian_distance_between( &requested_position, &effector_position );
        average_short_update( &movement_statistics, (uint16_t)proposed_distance );

        // Ask the motors to please move there
        servo_set_target_angle_limited( _CLEARPATH_1, angle_target.a1 );
        servo_set_target_angle_limited( _CLEARPATH_2, angle_target.a2 );
        servo_set_target_angle_limited( _CLEARPATH_3, angle_target.a3 );

        memcpy( &effector_position, &requested_position, sizeof(CartesianPoint_t) );
        memset( &requested_position, 0, sizeof(CartesianPoint_t));
        new_target = false;
    }
    else
    {
        average_short_update( &movement_statistics, 0 );
    }

    user_interface_set_position( effector_position.x, effector_position.y, effector_position.z );
    user_interface_set_effector_speed( effector_get_speed() );
}

/* -------------------------------------------------------------------------- */

/* ----- End ---------------------------------------------------------------- */
