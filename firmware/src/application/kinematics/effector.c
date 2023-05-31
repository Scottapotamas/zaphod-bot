/* -------------------------------------------------------------------------- */

#include <math.h>
#include <string.h>

#include "global.h"
#include "qassert.h"
#include "signals.h"

#include "effector.h"
#include "kinematics.h"
#include "cartesian_helpers.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* -------------------------------------------------------------------------- */

// TODO move this into a centralised defines file
#define EFFECTOR_SPEED_LIMIT (750U)    // mm/second

/* -------------------------------------------------------------------------- */

PRIVATE CartesianPoint_t effector_position;    // position of the end effector
PRIVATE CartesianPoint_t requested_position;   // position we should try to reach in this tick
PRIVATE uint32_t position_update_timestamp;

PRIVATE SemaphoreHandle_t xNewEffectorTargetSemaphore;
PRIVATE SemaphoreHandle_t xEffectorMutex;

PRIVATE TimerHandle_t xEffectorStatsTimer;

PRIVATE Subject effector_subject;

/* -------------------------------------------------------------------------- */

PRIVATE bool effector_is_near_home( CartesianPoint_t *position );

PRIVATE void effector_publish_velocity( uint32_t distance_since_last );

PRIVATE void effector_statistics_stale( TimerHandle_t xTimer );

/* -------------------------------------------------------------------------- */

PUBLIC void
effector_init( void )
{
    kinematics_init();

    memset( &effector_position, 0, sizeof(CartesianPoint_t) );
    memset( &requested_position, 0, sizeof(CartesianPoint_t) );
    position_update_timestamp = 0;

    xNewEffectorTargetSemaphore = xSemaphoreCreateBinary();
    xEffectorMutex = xSemaphoreCreateMutex();

    ENSURE( xNewEffectorTargetSemaphore );
    ENSURE( xEffectorMutex );

    subject_init( &effector_subject );

    xEffectorStatsTimer = xTimerCreate("effectorStale",
                                        pdMS_TO_TICKS(15),  // TODO this timeout should be set correctly
                                        pdFALSE,
                                        0,
                                        effector_statistics_stale
    );
    REQUIRE( xEffectorStatsTimer );
    xTimerStart( xEffectorStatsTimer, pdMS_TO_TICKS(10) );

}

/* -------------------------------------------------------------------------- */

PUBLIC Subject * effector_get_subject( void )
{
    return &effector_subject;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
effector_request_target( CartesianPoint_t *position )
{
    REQUIRE( position );

    // Ask for access to the effector data
    if( xSemaphoreTake( xEffectorMutex, portMAX_DELAY ) )
    {
        // Is the request a new position
        if( memcmp( &effector_position, position, sizeof(CartesianPoint_t) ) != 0 )
        {
            memcpy( &requested_position, position, sizeof(CartesianPoint_t) );

            xSemaphoreGive( xEffectorMutex );

            // Notify the task of the new target position
            xSemaphoreGive( xNewEffectorTargetSemaphore );
        }
        else
        {
            xSemaphoreGive( xEffectorMutex );
        }
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC void
effector_reset( void )
{
    if( xSemaphoreTake( xEffectorMutex, portMAX_DELAY ) )
    {
        memset( &effector_position, 0, sizeof(CartesianPoint_t));
        memset( &requested_position, 0, sizeof(CartesianPoint_t));
        position_update_timestamp = 0;

        xSemaphoreGive( xEffectorMutex );
        xSemaphoreGive( xNewEffectorTargetSemaphore );
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC void effector_task( void* arg )
{
    for(;;)
    {
        // Wait for a new position event to be signalled
        if( xSemaphoreTake( xNewEffectorTargetSemaphore, portMAX_DELAY) )
        {
            // Gain access to effector data
            if( xSemaphoreTake(xEffectorMutex, portMAX_DELAY) )
            {
                // Calculate the effector's distance change
                uint32_t proposed_distance_um = cartesian_distance_between( &requested_position,
                                                                            &effector_position );

                // Check the instantaneous (1ms) distance request won't exceed effector limits
                // As microns/millisecond = millimeters/second, we can use take the mm/sec limit
                if( proposed_distance_um > EFFECTOR_SPEED_LIMIT )
                {
                    EventData alert = { 0 };
                    alert.stamped.timestamp = xTaskGetTickCount();
                    subject_notify( &effector_subject, FLAG_EFFECTOR_VIOLATION, alert );
                }
                else    // under the speed guard
                {
                    // target motor shaft angle in degrees
                    JointAngles_t angle_target = { 0.0f, 0.0f, 0.0f };

                    // Calculate a motor angle solution for the cartesian position
                    kinematics_point_to_angle( requested_position, &angle_target );

                    // Ask the motors to please move to the new target angles
                    EventData servo_update = { 0 };
                    servo_update.stamped.timestamp = xTaskGetTickCount();

                    servo_update.stamped.index = 0;
                    servo_update.stamped.data.f32 = angle_target.a1;
                    subject_notify( &effector_subject, SERVO_TARGET_DEGREES, servo_update );

                    servo_update.stamped.index = 1;
                    servo_update.stamped.data.f32 = angle_target.a2;
                    subject_notify( &effector_subject, SERVO_TARGET_DEGREES, servo_update );

                    servo_update.stamped.index = 2;
                    servo_update.stamped.data.f32 = angle_target.a3;
                    subject_notify( &effector_subject, SERVO_TARGET_DEGREES, servo_update );


                    // The request is now 'the current position'
                    memcpy( &effector_position, &requested_position, sizeof(CartesianPoint_t) );

                    // TODO calculate effector velocity?

                    // Notify the system of the effector position
                    EventData pos_update = { 0 };
                    pos_update.s_triple[EVT_X] = requested_position.x;
                    pos_update.s_triple[EVT_Y] = requested_position.y;
                    pos_update.s_triple[EVT_Z] = requested_position.z;
                    subject_notify( &effector_subject, EFFECTOR_POSITION, pos_update );

                    // and the velocity
                    effector_publish_velocity( proposed_distance_um );

                    // Overseer tasks want to be notified when the effector is at the home position
                    if( effector_is_near_home( &effector_position ) )
                    {
                        EventData home_update = { 0 };
                        home_update.stamped.timestamp = xTaskGetTickCount();
                        subject_notify( &effector_subject, EFFECTOR_NEAR_HOME, home_update );
                    }

                    // Clear the request
                    memset( &requested_position, 0, sizeof(CartesianPoint_t));
                }

                xSemaphoreGive(xEffectorMutex);
            }
        }   // end new target semaphore

    }   // end task loop
}

/* -------------------------------------------------------------------------- */

PRIVATE bool
effector_is_near_home( CartesianPoint_t *position )
{
    // only 1 micron error is tolerated
    bool x_homed = IS_IN_DEADBAND( position->x, 0, 1 );
    bool y_homed = IS_IN_DEADBAND( position->y, 0, 1 );
    bool z_homed = IS_IN_DEADBAND( position->z, 0, 1 );

    return ( x_homed && y_homed && z_homed );
}

/* -------------------------------------------------------------------------- */

PRIVATE void effector_publish_velocity( uint32_t distance_since_last )
{
    // Difference between positions over time
    uint32_t timestamp_now = xTaskGetTickCount();
    uint32_t delta_time = timestamp_now - position_update_timestamp;

    // TODO: how should sub millisecond moves be handled?
    if( delta_time == 0 )
    {
        delta_time += 1;
//        ENSURE( delta_time );
    }

    // Publish the velocity value
    EventData vel_update = { 0 };
    vel_update.stamped.timestamp = timestamp_now;
    vel_update.stamped.data.u32 = distance_since_last / delta_time;
    subject_notify( &effector_subject, EFFECTOR_SPEED, vel_update );

    // Refresh the stats stale timer
    if( distance_since_last )
    {
        xTimerReset( xEffectorStatsTimer, pdMS_TO_TICKS(2) );
    }

    position_update_timestamp = timestamp_now;
}

/* -------------------------------------------------------------------------- */

// Called when the effector task sits waiting for input events
// We assume no update events within the timeout period mean the velocity should be zero
PRIVATE void effector_statistics_stale( TimerHandle_t xTimer )
{
    if( xSemaphoreTake(xEffectorMutex, portMAX_DELAY) )
    {
        // Calculate and publish the velocity using a zero-distance update
        effector_publish_velocity( 0 );

        xSemaphoreGive(xEffectorMutex);
    }
}

/* -------------------------------------------------------------------------- */