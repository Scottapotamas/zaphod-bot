/* ----- System Includes ---------------------------------------------------- */

#include <float.h>
#include <math.h>
#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "led_interpolator.h"

#include "app_signals.h"
#include "app_times.h"
#include "event_subscribe.h"
#include "simple_state_machine.h"
#include "qassert.h"
#include "app_events.h"
#include "global.h"

#include "led.h"
#include "led_types.h"
#include "timer_ms.h"

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ----- Defines ------------------------------------------------------------ */

typedef enum
{
    ANIMATION_OFF,
    ANIMATION_WAIT_AND_EXECUTE,
} RGBState_t;

typedef struct
{
    RGBState_t previousState;
    RGBState_t currentState;
    RGBState_t nextState;

    Fade_t  fade_a;          // Slot A fade storage
    Fade_t  fade_b;          // Slot B fade storage
    Fade_t *current_fade;    // Points to fade_a or fade_b

    bool       enable;             // if the planner is enabled
    timer_ms_t epoch_timestamp;    // Reference system time for fade offset sequencing

    timer_ms_t animation_started;         // timestamp the start
    timer_ms_t animation_est_complete;    // timestamp when the animation will end
    float      progress_percent;          // calculated progress

    RGBColour_t led_colour;    // current channel outputs
} LEDPlanner_t;

/* ----- Private Variables -------------------------------------------------- */

PRIVATE LEDPlanner_t planner;

PRIVATE void
led_interpolator_calculate_percentage( uint16_t fade_duration );

PRIVATE void
led_interpolator_execute_fade( Fade_t *fade, float percentage );

PRIVATE FadeSolution_t
hsi_lerp_linear( HSIColour_t p[], size_t points, float pos_weight, HSIColour_t *output );

PRIVATE void
led_interpolator_notify_animation_started( uint32_t fade_id );

PRIVATE void
led_interpolator_notify_animation_complete( uint32_t fade_id );

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
led_interpolator_init( void )
{
    memset( &planner, 0, sizeof( planner ) );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
led_interpolator_set_epoch_reference( timer_ms_t timestamp_ms )
{
    LEDPlanner_t *me = &planner;

    if( timestamp_ms )
    {
        me->epoch_timestamp = timestamp_ms;
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC void
led_interpolator_set_objective( Fade_t *fade_to_process )
{
    REQUIRE( fade_to_process );

    LEDPlanner_t *me               = &planner;
    Fade_t       *fade_insert_slot = { 0 };    // Allows us to put the new fade into whichever slot is available

    if( me->fade_a.duration == 0 )
    {
        fade_insert_slot = &me->fade_a;
    }
    else if( me->fade_b.duration == 0 )
    {
        fade_insert_slot = &me->fade_b;
    }

    if( fade_insert_slot != NULL )
    {
        memcpy( fade_insert_slot, fade_to_process, sizeof( Fade_t ) );
    }
}

PUBLIC bool
led_interpolator_is_ready_for_next( void )
{
    bool slot_a_ready = ( planner.fade_a.duration == 0 );
    bool slot_b_ready = ( planner.fade_b.duration == 0 );

    return ( slot_a_ready || slot_b_ready );
}

PUBLIC bool
led_interpolator_is_empty( void )
{
    bool slot_a_empty = ( planner.fade_a.duration == 0 );
    bool slot_b_empty = ( planner.fade_b.duration == 0 );

    return ( slot_a_empty && slot_b_empty );
}

PUBLIC void
led_interpolator_start( void )
{
    planner.enable = true;

    // Process the loop now
    led_interpolator_process();
}

PUBLIC void
led_interpolator_stop( void )
{
    planner.enable = false;
    memset( &planner.fade_a, 0, sizeof( Fade_t ) );
    memset( &planner.fade_b, 0, sizeof( Fade_t ) );

    // Immediately do cleanup etc
    // TODO: feels like 'expects external behaviour'
    //      re: working around the hooking/unhooking process loop not providing enough time for cleanup
    led_interpolator_process();
}

/* -------------------------------------------------------------------------- */

PUBLIC float
led_interpolator_get_progress( void )
{
    return planner.progress_percent;
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
led_interpolator_get_fade_done( void )
{
    return ( planner.progress_percent >= 1.0f - FLT_EPSILON );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
led_interpolator_set_dark( void )
{
    planner.led_colour.red   = 0U;
    planner.led_colour.green = 0U;
    planner.led_colour.blue  = 0U;

    led_set( planner.led_colour.red, planner.led_colour.green, planner.led_colour.blue );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
led_interpolator_process( void )
{
    LEDPlanner_t *me = &planner;

    if( !me->enable )
    {
        STATE_NEXT( ANIMATION_OFF );
    }

    switch( me->currentState )
    {
        case ANIMATION_OFF:
            STATE_ENTRY_ACTION
            led_interpolator_set_dark();
            STATE_TRANSITION_TEST
            if( me->enable )
            {
                if( me->fade_a.duration || me->fade_b.duration )
                {
                    STATE_NEXT( ANIMATION_WAIT_AND_EXECUTE );
                }
            }

            STATE_EXIT_ACTION
            timer_ms_stop( &me->animation_started );
            STATE_END
            break;

        case ANIMATION_WAIT_AND_EXECUTE:
            STATE_ENTRY_ACTION
            // Pick the first slot with a valid move
            if( me->fade_a.duration )
            {
                me->current_fade = &me->fade_a;
            }
            else if( me->fade_b.duration )
            {
                me->current_fade = &me->fade_b;
            }

            STATE_TRANSITION_TEST
            // Calculate the time since the 'epoch' event
            uint32_t time_since_epoch_ms = timer_ms_stopwatch_lap( &me->epoch_timestamp );

            // Start the fade once sync offset time matches the epoch + elapsed time
            if( time_since_epoch_ms >= me->current_fade->sync_offset && !timer_ms_is_running( &me->animation_started ) )
            {
                timer_ms_stopwatch_start( &me->animation_started );
                timer_ms_start( &me->animation_est_complete, me->current_fade->duration );
                me->progress_percent = 0;

                led_interpolator_notify_animation_started( me->current_fade->sync_offset );
            }

            if( time_since_epoch_ms > me->current_fade->sync_offset )
            {
                // Set the LED as required for this tick's progress percentage
                led_interpolator_calculate_percentage( me->current_fade->duration );
                led_interpolator_execute_fade( me->current_fade, me->progress_percent );

                // Check if the move is done
                if( led_interpolator_get_fade_done() )
                {
                    led_interpolator_notify_animation_complete( me->current_fade->sync_offset );

                    // TODO: update UI with status?

                    // Clear out the current state
                    memset( me->current_fade, 0, sizeof( Fade_t ) );

                    // Update the pointer to the other slot
                    if( me->current_fade == &me->fade_a )
                    {
                        me->current_fade = &me->fade_b;
                    }
                    else
                    {
                        me->current_fade = &me->fade_a;
                    }

                    timer_ms_stop( &me->animation_started );

                    // Fall back into the handler's off state until slots are loaded
                    if( !me->current_fade->duration )
                    {
                        STATE_NEXT( ANIMATION_OFF );
                    }
                }
            }
            STATE_EXIT_ACTION
            me->current_fade     = 0;
            me->progress_percent = 0;
            timer_ms_stop( &me->animation_started );
            timer_ms_stop( &me->animation_est_complete );
            STATE_END
            break;
    }
}

/* -------------------------------------------------------------------------- */

PRIVATE void
led_interpolator_calculate_percentage( uint16_t fade_duration )
{
    LEDPlanner_t *me = &planner;

    // calculate current target completion based on time elapsed
    // time remaining is the allotted duration - time used (start to now), divide by the duration to get 0.0->1.0 progress
    uint32_t time_used = timer_ms_stopwatch_lap( &me->animation_started );

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

    HSIColour_t     fade_target   = { 0.0f, 0.0f, 0.0f };
    GenericColour_t output_values = { 0.0f, 0.0f, 0.0f };

    switch( fade->type )
    {
        case _INSTANT_CHANGE:
            fade_target.hue        = fade->input_colours[0].hue;
            fade_target.saturation = fade->input_colours[0].saturation;
            fade_target.intensity  = fade->input_colours[0].intensity;
            break;

        case _LINEAR_RAMP:
            hsi_lerp_linear( fade->input_colours, fade->num_pts, percentage, &fade_target );
            break;
    }

    // Perform colour compensation adjustments
    hsi_to_rgb( fade_target.hue, fade_target.saturation, fade_target.intensity, &output_values.x, &output_values.y, &output_values.z );

    // Set the LED channel values in RGB percentages [0.0f -> 1.0f]
    led_set( output_values.x, output_values.y, output_values.z );
}

/* -------------------------------------------------------------------------- */

FadeSolution_t
hsi_lerp_linear( HSIColour_t p[], size_t points, float pos_weight, HSIColour_t *output )
{
    REQUIRE( p );
    REQUIRE( points == 2);  // need 2 points for a line

    if( pos_weight <= 0.0f + FLT_EPSILON )
    {
        memcpy( output, &p[0], sizeof( HSIColour_t ) );
        return FADE_VALID;
    }

    if( pos_weight >= 1.0f - FLT_EPSILON )
    {
        memcpy( output, &p[1], sizeof( HSIColour_t ) );
        return FADE_VALID;
    }

    // Linear interpolation between two points (lerp)

    // for hue, remember it covers a circular range: we need to take the shortest path across the wrapping point
    float distance_ccw = ( p[0].hue >= p[1].hue ) ? p[0].hue - p[1].hue : 1 + p[0].hue - p[1].hue;
    float distance_cw  = ( p[0].hue >= p[1].hue ) ? 1 + p[1].hue - p[0].hue : p[1].hue - p[0].hue;

    output->hue = ( distance_cw <= distance_ccw ) ? p[1].hue + ( distance_cw * pos_weight ) : p[1].hue - ( distance_ccw * pos_weight );

    // handle wrapping around
    if( output->hue < 0.0f )
    {
        output->hue += 1.0f;
    }

    if( output->hue > 1.0f )
    {
        output->hue -= 1.0f;
    }

    output->saturation = p[0].saturation + pos_weight * ( p[1].saturation - p[0].saturation );
    output->intensity  = p[0].intensity + pos_weight * ( p[1].intensity - p[0].intensity );

    return FADE_VALID;
}

/* -------------------------------------------------------------------------- */

PRIVATE void
led_interpolator_notify_animation_started( uint32_t fade_id )
{
    SyncTimestampEvent *barrier_ev = EVENT_NEW( SyncTimestampEvent, ANIMATION_STARTED );
    uint32_t            publish_id = fade_id;

    memcpy( &barrier_ev->epoch, &publish_id, sizeof( fade_id ) );
    eventPublish( (StateEvent *)barrier_ev );
}

PRIVATE void
led_interpolator_notify_animation_complete( uint32_t fade_id )
{
    SyncTimestampEvent *barrier_ev = EVENT_NEW( SyncTimestampEvent, ANIMATION_COMPLETE );
    uint32_t            publish_id = fade_id;

    memcpy( &barrier_ev->epoch, &publish_id, sizeof( fade_id ) );
    eventPublish( (StateEvent *)barrier_ev );
}

/* ----- End ---------------------------------------------------------------- */
