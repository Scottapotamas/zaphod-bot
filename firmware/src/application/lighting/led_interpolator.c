/* -------------------------------------------------------------------------- */

#include <float.h>
#include <math.h>
#include <string.h>

#include "led_interpolator.h"
#include "lighting_types.h"
#include "led_interpolators.h"
#include "stopwatch.h"

#include "qassert.h"
#include "global.h"

#include "FreeRTOS.h"
#include "queue.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* -------------------------------------------------------------------------- */

typedef struct
{
    QueueHandle_t xRequestQueue;

    Fade_t  current_fade;    // Store the currently executing fade
    bool    fade_ready;      // has the currently loaded fade completed checks/transforms?

    uint32_t epoch_timestamp;    // Reference system time for fade offset sequencing
    uint32_t animation_started;         // timestamp the start

    float      progress_percent;          // calculated progress

    HSIRequestFn output_cb;    // target colour is emitted as arguments against this callback

} LEDPlanner_t;

PRIVATE LEDPlanner_t planner;

/* -------------------------------------------------------------------------- */

PRIVATE void
led_interpolator_calculate_percentage( uint16_t fade_duration );

PRIVATE void
led_interpolator_execute_fade( Fade_t *fade, float percentage );

/* -------------------------------------------------------------------------- */

PUBLIC void led_interpolator_init( void )
{
    LEDPlanner_t *me = &planner;
    memset( me, 0, sizeof( LEDPlanner_t ) );

    me->xRequestQueue = xQueueCreate( 5, sizeof( Fade_t ) );
    REQUIRE( me->xRequestQueue );
    vQueueAddToRegistry( me->xRequestQueue, "fadeMoves" );    // Debug view annotation

}

/* -------------------------------------------------------------------------- */

PUBLIC void led_interpolator_update_output_callback( HSIRequestFn callback )
{
    LEDPlanner_t *me = &planner;
    me->output_cb = callback;
}

/* -------------------------------------------------------------------------- */

PUBLIC void led_interpolator_set_epoch_reference( uint32_t sync_timer )
{
    LEDPlanner_t *me = &planner;

    if( sync_timer )
    {
        me->epoch_timestamp = sync_timer;
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t led_interpolator_queue_request( Fade_t *fade_to_process )
{
    REQUIRE( fade_to_process );
    LEDPlanner_t *me = &planner;

    if( fade_to_process )
    {
        xQueueSendToBack( me->xRequestQueue, (void *)fade_to_process, (TickType_t)0 );
    }

    return uxQueueSpacesAvailable( me->xRequestQueue );
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
led_interpolator_get_fade_done( void )
{
    LEDPlanner_t *me = &planner;

    return ( me->progress_percent >= 1.0f - FLT_EPSILON );
}

/* -------------------------------------------------------------------------- */

PUBLIC void led_interpolator_task( void* arg )
{
    LEDPlanner_t *me = &planner;

    for( ;; )
    {
        // Fetch a pending animation off the queue if nothing is already loaded
        // Block until something arrives
        if( me->current_fade.duration == 0 )
        {
            xQueueReceive( me->xRequestQueue, &me->current_fade, portMAX_DELAY );
        }

        // Prepare the movement for execution
        if( !me->fade_ready )
        {
            // Run any pre-processing on the animation
            // NOTE: Colour transforms etc are post-processing steps done by the 'lower level' LED driver


            // Check the planned animation won't violate any rules

            me->fade_ready = true;
        }

        // Execute the animation
        if( me->fade_ready )
        {
            // Sync has been provided if the epoch isn't null
            bool     epoch_valid  = ( me->epoch_timestamp );
            uint32_t current_time = xTaskGetTickCount();

            if( epoch_valid )
            {
                // Animations time themselves relative to the sync/epoch timestamp
                // therefore the 'wallclock' the global timestamp is:
                uint32_t start_timestamp = me->epoch_timestamp + me->current_fade.sync_offset;

                // If we're early, the task should have a nap
                if( current_time < start_timestamp )
                {
                    uint32_t nap_duration = start_timestamp - current_time;
                    vTaskDelayUntil( &current_time, nap_duration );

                    current_time = xTaskGetTickCount(); // have to refresh the time after our nap.
                }

                if( !stopwatch_running( &me->animation_started ) )
                {
                    stopwatch_start( &me->animation_started );
                    // TODO: notify the system that a fade has started
                }

                // Set the LED as required for this tick's progress percentage
                led_interpolator_calculate_percentage( me->current_fade.duration );
                led_interpolator_execute_fade( &me->current_fade, me->progress_percent );

                // Animation has completed, run cleanup
                if( led_interpolator_get_fade_done() )
                {
                    // TODO: notify system that a fade has completed

                    // Clear out the current state
                    memset( &me->current_fade, 0, sizeof( Fade_t ) );
                    me->fade_ready = false;

                    stopwatch_stop( &me->animation_started );
                    me->progress_percent = 0;
                    // Allow it to go back and pick another move off the queue
                }

            }    // epoch valid

            vTaskDelay( pdMS_TO_TICKS( 1 ) );
        }    // fade execution

    }    // end infinite task loop

}

/* -------------------------------------------------------------------------- */

PRIVATE void
led_interpolator_calculate_percentage( uint16_t fade_duration )
{
    LEDPlanner_t *me = &planner;

    // calculate current target completion based on time elapsed
    // time remaining is the allotted duration - time used (start to now), divide by the duration to get 0.0->1.0 progress
    uint32_t time_used = stopwatch_lap( &me->animation_started );

    if( fade_duration )
    {
        me->progress_percent = (float)( time_used ) / fade_duration;
    }
    else
    {
        me->progress_percent = 0;
    }
}

/* -------------------------------------------------------------------------- */

PRIVATE void
led_interpolator_execute_fade( Fade_t *fade, float percentage )
{
    REQUIRE( fade );

    HSIColour_t fade_target = { 0.0f, 0.0f, 0.0f };

    // TODO allow discrete fades to specify positional noise and luma correction?
//    led_compensate_luma_for_speed( fade->settings.speed_compensated );
//    led_apply_positional_noise( fade->settings.positional_noise );

    switch( fade->settings.type )
    {
        case _INSTANT_CHANGE:
            fade_target.hue        = fade->input_colours[0].hue;
            fade_target.saturation = fade->input_colours[0].saturation;
            fade_target.intensity  = fade->input_colours[0].intensity;
            break;

        case _LINEAR_RAMP:
            hsi_lerp_linear( fade->input_colours, fade->settings.num_pts, percentage, &fade_target );
            break;
    }

    // Pass the computed colour to the next pipeline step via user callback
    if( planner.output_cb )
    {
        planner.output_cb( &fade_target );
    }
}

/* -------------------------------------------------------------------------- */