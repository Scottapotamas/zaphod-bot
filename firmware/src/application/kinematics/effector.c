/* -------------------------------------------------------------------------- */

#include <math.h>
#include <string.h>

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "qassert.h"
#include "signals.h"

#include "effector.h"
#include "kinematics.h"
#include "cartesian_helpers.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

// TODO move this into a centralised defines file
#define EFFECTOR_SPEED_LIMIT (750U)    // mm/second

/* -------------------------------------------------------------------------- */

PRIVATE CartesianPoint_t effector_position;    // position of the end effector
PRIVATE CartesianPoint_t requested_position;   // position we should try to reach in this tick

PRIVATE SemaphoreHandle_t xNewEffectorTargetSemaphore;
PRIVATE SemaphoreHandle_t xEffectorMutex;

PRIVATE Subject effector_subject;
//PRIVATE AverageShort_t movement_statistics;

/* -------------------------------------------------------------------------- */

PUBLIC void
effector_init( void )
{
    kinematics_init();

    memset( &effector_position, 0, sizeof(CartesianPoint_t) );

    xNewEffectorTargetSemaphore = xSemaphoreCreateBinary();
    xEffectorMutex = xSemaphoreCreateMutex();

    ENSURE( xNewEffectorTargetSemaphore );
    ENSURE( xEffectorMutex );

    subject_init( &effector_subject );

//    average_short_init( &movement_statistics, SPEED_ESTIMATOR_SPAN );
}

PUBLIC Subject * effector_get_subject( void )
{
    return &effector_subject;
}

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
effector_get_speed( void )
{
    // Stats provide 50 ticks (milliseconds) worth of distances travelled
    // therefore, the total microns travelled in 50ms * 20 = microns per second
    uint32_t microns_per_second = 0;//average_short_get_sum( &movement_statistics ) * 20;

    return microns_per_second;
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
effector_set_home( void )
{
    // TODO: need to signal to the task that the position has been 'moved' without needing a kinematics shift?
    // TODO: consider how that impacts movement stats?
    if( xSemaphoreTake( xEffectorMutex, portMAX_DELAY ) )
    {
        effector_position.x = 0;
        effector_position.y = 0;
        effector_position.z = 0;

        xSemaphoreGive( xEffectorMutex );
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC CartesianPoint_t
effector_get_position( void )
{
    CartesianPoint_t pos = { 0 };

    if( xSemaphoreTake( xEffectorMutex, portMAX_DELAY ) )
    {
        pos = effector_position;
        xSemaphoreGive( xEffectorMutex );
    }

    return pos;
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
                    // Keep track of the distance change over time for speed stats
                    //            average_short_update( &movement_statistics, (uint16_t)proposed_distance_um );

                    // target motor shaft angle in degrees
                    JointAngles_t angle_target = { 0.0f, 0.0f, 0.0f };

                    // Calculate a motor angle solution for the cartesian position
                    kinematics_point_to_angle( requested_position, &angle_target );

                    // Ask the motors to please move there
                    //            servo_set_target_angle_limited( _CLEARPATH_1, angle_target.a1 );
                    //            servo_set_target_angle_limited( _CLEARPATH_2, angle_target.a2 );
                    //            servo_set_target_angle_limited( _CLEARPATH_3, angle_target.a3 );

                    // TODO calculate effector velocity?

                    // TODO update effector's current position and velocity information
                    EventData pos_update = { 0 };
                    pos_update.s_triple[EVT_X] = requested_position.x;
                    pos_update.s_triple[EVT_Y] = requested_position.y;
                    pos_update.s_triple[EVT_Z] = requested_position.z;

                    subject_notify( &effector_subject, EFFECTOR_POSITION, pos_update );

                    memcpy( &effector_position, &requested_position, sizeof(CartesianPoint_t) );
                    memset( &requested_position, 0, sizeof(CartesianPoint_t));
                }

                xSemaphoreGive(xEffectorMutex);
            }
        }   // end new target semaphore

    }   // end task loop
}

/* -------------------------------------------------------------------------- */