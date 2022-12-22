/* ----- System Includes ---------------------------------------------------- */

#include <float.h>
#include <string.h>
#include <math.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "point_follower.h"

#include "app_events.h"
#include "app_signals.h"
#include "event_subscribe.h"
#include "global.h"
#include "simple_state_machine.h"
#include "average_short.h"

#include "clearpath.h"
#include "kinematics.h"
#include "motion_types.h"
#include "user_interface.h"
#include "app_times.h"
#include "timer_ms.h"

/* ----- Defines ------------------------------------------------------------ */

typedef enum
{
    FOLLOWER_OFF,
    FOLLOWER_RUN,
} FollowerState_t;

typedef struct
{
    FollowerState_t previousState;
    FollowerState_t currentState;
    FollowerState_t nextState;

    CartesianPoint_t requested;             // Store the most recent target position
    CartesianPoint_t effector_position;    // position of the end effector (used for relative moves)
    CartesianPoint_t previous_march;    // Velocity cache

    bool       enable;             // The follower is enabled

} PointFollower_t;

int32_t calculate_distance_this_tick( int32_t current, int32_t target, int32_t velocity );

/* ----- Private Variables -------------------------------------------------- */

PRIVATE PointFollower_t follower;

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
point_follower_init( void )
{
    memset( &follower, 0, sizeof( follower ) );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
point_follower_set_target( CartesianPoint_t *target )
{
    PointFollower_t *me = &follower;

    if( target )
    {
        memcpy( &me->requested, target, sizeof( CartesianPoint_t ) );
    }
    else
    {
        // TODO Assert on invalid input pointer?
    }

}

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
point_follower_get_effector_speed( void )
{
    // Stats provide 50 ticks (milliseconds) worth of distances travelled
    // therefore, the total microns travelled in 50ms * 20 = microns per second
//    uint32_t microns_per_second = average_short_get_sum( &follower.movement_statistics ) * 20;

    return 0;
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
point_follower_get_move_done( void )
{
    PointFollower_t *me = &follower;

    // TODO: Check if distance is close enough to target

    return ( me->currentState == FOLLOWER_OFF );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
point_follower_start( void )
{
    PointFollower_t *me = &follower;

    // Request that the state-machine transitions to "ON"
    me->enable = true;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
point_follower_stop( void )
{
    PointFollower_t *me = &follower;

    // Request that the state-machine return to "OFF"
    me->enable = false;

}

/* -------------------------------------------------------------------------- */

// If there's a difference between the current and target positions
//      (simple) calculate the position difference for this timestep given a velocity
//      This position is passed through IK and then to motors
//      (advanced) position difference has acceleration shaping in each x/y/z axis and predictive slowing prior to hitting target
//      This should help reduce jerk when changing direction too quickly

// If any specific move is planned, fall out of this mode and back into queue execution
// If easing is supported, just stop accepting targets and wait for it to stop.
// Then reset any velocity estimates, targets etc


PUBLIC void
point_follower_process( void )
{
    PointFollower_t *me = &follower;

    if( !me->enable )
    {
        STATE_NEXT( FOLLOWER_OFF );
    }

    switch( me->currentState )
    {
        case FOLLOWER_OFF:
            STATE_ENTRY_ACTION

            STATE_TRANSITION_TEST
            if( me->enable )
            {
                STATE_NEXT( FOLLOWER_RUN );
            }

            STATE_EXIT_ACTION
            STATE_END
            break;

        case FOLLOWER_RUN:
            STATE_ENTRY_ACTION

            STATE_TRANSITION_TEST

            CartesianPoint_t target       = { 0, 0, 0 };    // target position in cartesian space
            CartesianPoint_t march        = { 0, 0, 0 };    // quantity of positional error
            JointAngles_t    angle_target = { 0, 0, 0 }; // target motor shaft angle in degrees

            // How far do we move for this tick (includes linear accelerations with velocity limiter)
            march.x = calculate_distance_this_tick( me->effector_position.x, me->requested.x, me->previous_march.x );
            march.y = calculate_distance_this_tick( me->effector_position.y, me->requested.y, me->previous_march.y );
            march.z = calculate_distance_this_tick( me->effector_position.z, me->requested.z, me->previous_march.z );

            // Cache it for next step, used as part of acceleration calculations
            memcpy( &me->previous_march, &march, sizeof(CartesianPoint_t) );

            // Apply it as our new target for this step
            target.x = me->effector_position.x + march.x;
            target.y = me->effector_position.y + march.y;
            target.z = me->effector_position.z + march.z;

            // Calculate a motor angle solution for the cartesian position
            kinematics_point_to_angle( target, &angle_target );

            // Update the current effector position (naively assumes that any requested move is achieved before the next tick)
            memcpy( &me->effector_position, &target, sizeof( CartesianPoint_t ) );

            // Ask the motors to please move there
            servo_set_target_angle_limited( _CLEARPATH_1, angle_target.a1 );
            servo_set_target_angle_limited( _CLEARPATH_2, angle_target.a2 );
            servo_set_target_angle_limited( _CLEARPATH_3, angle_target.a3 );

            STATE_EXIT_ACTION

            STATE_END
            break;
    }

    user_interface_set_position( me->effector_position.x, me->effector_position.y, me->effector_position.z );
//    user_interface_set_pathing_status( me->currentState );
//    user_interface_set_effector_speed( point_follower_get_effector_speed() );
}

int8_t acceleration = 1;
int32_t velocity_max = 200;

int32_t calculate_distance_this_tick( int32_t current, int32_t target, int32_t velocity )
{
    int32_t error = target - current;

    if( !error )
    {
        return 0;
    }

    // Determine direction of error
    int8_t direction = ( error < 0 )? -1 : 1;

    // Calculate how many acceleration steps are in our velocity
    uint32_t ticks = abs(velocity) / acceleration;
    uint32_t deceleration_distance = (abs(velocity) * ticks) + (0.5 * acceleration * ticks*ticks);

    if( abs(error) <= deceleration_distance )
    {
        // Start decelerating as we're approaching the target
        velocity -= direction * acceleration;

        // Handle remainder caused by positions finer than the acceleration step, etc
        if( !velocity )
        {
            velocity = error;
        }
    }
    else
    {
        // Ramp the velocity in the direction of our target
        velocity += direction * acceleration;
    }

    velocity = CLAMP(velocity, - 1 * velocity_max, velocity_max);
    return velocity;
}

/* ----- End ---------------------------------------------------------------- */
