/* -------------------------------------------------------------------------- */

#include <float.h>
#include <string.h>
#include <math.h>

#include "point_follower.h"
#include "global.h"
#include "qassert.h"
#include "cartesian_helpers.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"


/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

typedef struct
{
    CartesianPoint_t requested;         // Store the most recent target position
    CartesianPoint_t previous_march;    // Velocity cache
    SemaphoreHandle_t xNewTargetSemaphore;

} PointFollower_t;

PRIVATE int32_t calculate_distance_this_tick( int32_t current, int32_t target, int32_t velocity );

/* -------------------------------------------------------------------------- */

PRIVATE PointFollower_t follow_state = { 0 };

PRIVATE int8_t acceleration = 1;
PRIVATE int32_t velocity_max = 200;

PositionRequestFn output_callback = { 0 };

/* -------------------------------------------------------------------------- */

// TODO: ability to set velocity and acceleration limits as init arguments
PUBLIC void
point_follower_init( void )
{
    PointFollower_t *me = &follow_state;
    memset( me, 0, sizeof( PointFollower_t ) );

    me->xNewTargetSemaphore = xSemaphoreCreateBinary();
    ENSURE( me->xNewTargetSemaphore );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
point_follower_update_output_callback( PositionRequestFn callback )
{
    output_callback = callback;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
point_follower_set_target( CartesianPoint_t *target )
{
    REQUIRE( target );

    PointFollower_t *me = &follow_state;
    memcpy( &me->requested, target, sizeof( CartesianPoint_t ) );

    // Notify the task of the new target position
    xSemaphoreGive( me->xNewTargetSemaphore );
}

/* -------------------------------------------------------------------------- */

// TODO a function which allows updating the idea of the external effector position?
//      it should also give the semaphore?

/* -------------------------------------------------------------------------- */

// If there's a difference between the current and target positions
//      Calculate the change for this tick, taking acceleration shaping in each x/y/z axis
//      and predicatively start slowing prior to hitting target
//      This should help reduce jerk when changing direction too quickly

PUBLIC void
point_follower_task( void* arg )
{
    PointFollower_t *me = &follow_state;

    for(;;)
    {
        CartesianPoint_t effector = { 0 };     // End-effector position
       // TODO: how do we get the end effector's position in this thread without excessive complication?
       //        effector = effector_get_position();

        // If we've achieved the target position, block the task until a new target arrives
        if( cartesian_distance_between( &me->requested, &effector ) < MM_TO_MICRONS(0.5) )
        {
            xSemaphoreTake( me->xNewTargetSemaphore, portMAX_DELAY);
        }

        CartesianPoint_t march = { 0, 0, 0 };    // Quantity of positional cartesian error
        CartesianPoint_t target = { 0, 0, 0 };    // Follower's target position for this tick

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

        if( output_callback )
        {
            output_callback( &target );
        }

        vTaskDelay( pdMS_TO_TICKS(1) );
    }   // end infinite task loop

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
