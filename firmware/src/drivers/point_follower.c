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

#include "effector.h"
#include "expansion.h"
#include "motion_types.h"
#include "app_times.h"
#include "timer_ms.h"
#include "qassert.h"

/* ----- Defines ------------------------------------------------------------ */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

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

    CartesianPoint_t requested;         // Store the most recent target position
    CartesianPoint_t previous_march;    // Velocity cache

    bool enable; // The follower is enabled
} PointFollower_t;

PRIVATE int32_t calculate_distance_this_tick( int32_t current, int32_t target, int32_t velocity );

/* ----- Private Variables -------------------------------------------------- */

PRIVATE PointFollower_t follow_state[NUMBER_POINT_FOLLOWERS] = { 0 };

PRIVATE int8_t acceleration = 1;
PRIVATE int32_t velocity_max = 200;

/* ----- Public Functions --------------------------------------------------- */

// TODO: ability to set velocity and acceleration as init arguments
PUBLIC void
point_follower_init( PointFollowInstance_t follower )
{
    REQUIRE( follower < NUMBER_POINT_FOLLOWERS );
    PointFollower_t *me = &follow_state[follower];

    memset( me, 0, sizeof( PointFollower_t ) );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
point_follower_set_target( PointFollowInstance_t follower, CartesianPoint_t *target )
{
    REQUIRE( follower < NUMBER_POINT_FOLLOWERS );
    REQUIRE( target );

    PointFollower_t *me = &follow_state[follower];
    memcpy( &me->requested, target, sizeof( CartesianPoint_t ) );
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
point_follower_get_move_done( PointFollowInstance_t follower )
{
    REQUIRE( follower < NUMBER_POINT_FOLLOWERS );
    PointFollower_t *me = &follow_state[follower];

    // TODO: Check if distance is close enough to target

    return ( me->currentState == FOLLOWER_OFF );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
point_follower_start( PointFollowInstance_t follower )
{
    REQUIRE( follower < NUMBER_POINT_FOLLOWERS );
    PointFollower_t *me = &follow_state[follower];

    memset( &me->requested, 0, sizeof(CartesianPoint_t) );
    memset( &me->previous_march, 0, sizeof(CartesianPoint_t) );

    // Request that the state-machine transitions to "ON"
    me->enable = true;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
point_follower_stop( PointFollowInstance_t follower )
{
    REQUIRE( follower < NUMBER_POINT_FOLLOWERS );
    PointFollower_t *me = &follow_state[follower];

    // Request that the state-machine return to "OFF"
    me->enable = false;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
point_follower_process_delta( void )
{
    point_follower_process( POINT_FOLLOWER_DELTA );
}

PUBLIC void
point_follower_process_expansion( void )
{
    point_follower_process( POINT_FOLLOWER_EXPANSION );
}

/* -------------------------------------------------------------------------- */

// If there's a difference between the current and target positions
//      Calculate the change for this tick, taking acceleration shaping in each x/y/z axis
//      and predicatively start slowing prior to hitting target
//      This should help reduce jerk when changing direction too quickly

PUBLIC void
point_follower_process( PointFollowInstance_t follower )
{
    REQUIRE( follower < NUMBER_POINT_FOLLOWERS );
    PointFollower_t *me = &follow_state[follower];

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
            CartesianPoint_t effector = { 0 };     // End-effector position

            switch( follower )
            {
                case POINT_FOLLOWER_DELTA:
                    effector = effector_get_position();
                    break;
                case POINT_FOLLOWER_EXPANSION:
                    effector.x = expansion_get_position();
                    break;
                default:
                    ASSERT( false );
            }

            CartesianPoint_t march        = { 0, 0, 0 };    // Quantity of positional cartesian error
            CartesianPoint_t target       = { 0, 0, 0 };    // Follower's target position for this tick

            // How far do we move for this tick (includes linear accelerations with velocity limiter)
            march.x = calculate_distance_this_tick( effector.x, me->requested.x, me->previous_march.x );
            march.y = calculate_distance_this_tick( effector.y, me->requested.y, me->previous_march.y );
            march.z = calculate_distance_this_tick( effector.z, me->requested.z, me->previous_march.z );

            // Cache it for next step, used as part of acceleration calculations
            memcpy( &me->previous_march, &march, sizeof(CartesianPoint_t) );

            // Apply it as our new target for this step
            target.x = effector.x + march.x;
            target.y = effector.y + march.y;
            target.z = effector.z + march.z;

            switch( follower )
            {
                case POINT_FOLLOWER_DELTA:
                    effector_request_target( &target );
                    break;

                case POINT_FOLLOWER_EXPANSION:
                    expansion_request_target( target.x );
                    break;

                default:
                    ASSERT_PRINTF( false, "Invalid point follow inst" );
            }

            STATE_EXIT_ACTION

            STATE_END
            break;
    }
}

/* -------------------------------------------------------------------------- */

PRIVATE int32_t
calculate_distance_this_tick( int32_t current, int32_t target, int32_t velocity )
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
