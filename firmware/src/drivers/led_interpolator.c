/* ----- System Includes ---------------------------------------------------- */

#include <float.h>
#include <math.h>
#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "led_interpolator.h"

#include "app_signals.h"
#include "app_times.h"
#include "event_subscribe.h"
#include "hal_systick.h"
#include "simple_state_machine.h"

#include "user_interface.h"
#include "global.h"
#include "led.h"
#include "led_types.h"

/* ----- Defines ------------------------------------------------------------ */

typedef enum
{
    ANIMATION_OFF,
    ANIMATION_EXECUTE_A,
    ANIMATION_EXECUTE_B,
    ANIMATION_MANUAL,
} RGBState_t;

typedef struct
{
    RGBState_t previousState;
    RGBState_t currentState;
    RGBState_t nextState;

    bool     manual_mode;      // user control
    bool     animation_run;    // if the planner is enabled
    uint16_t execute_id;

    Fade_t   fade_a;                    // pointer to a movement
    Fade_t   fade_b;                    // pointer to b movement
    uint32_t animation_started;         // timestamp the start
    uint32_t animation_est_complete;    // timestamp when the animation will end
    float    progress_percent;          // calculated progress

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
hsi_to_rgb( float h, float s, float i, float *r, float *g, float *b );

PRIVATE float
hue_to_channel( float p, float q, float t );

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
led_interpolator_init( void )
{
    memset( &planner, 0, sizeof( planner ) );

    led_init();
}

/* -------------------------------------------------------------------------- */

PUBLIC void
led_interpolator_set_objective( Fade_t *fade_to_process )
{
    LEDPlanner_t *me               = &planner;
    Fade_t *      fade_insert_slot = { 0 };    // allows us to put the new fade into whichever slot is available

    if( me->fade_a.duration == 0 )
    {
        fade_insert_slot = &me->fade_a;
    }
    else if( me->fade_b.duration == 0 )
    {
        fade_insert_slot = &me->fade_b;
    }

    memcpy( fade_insert_slot, fade_to_process, sizeof( Fade_t ) );
    me->manual_mode = false;
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
led_interpolator_start_id( uint16_t id )
{
    planner.execute_id = id;
    led_interpolator_start();
}

PUBLIC void
led_interpolator_start( void )
{
    planner.animation_run = true;
}

PUBLIC void
led_interpolator_stop( void )
{
    planner.animation_run = false;
    memset( &planner.fade_a, 0, sizeof( Fade_t ) );
    memset( &planner.fade_b, 0, sizeof( Fade_t ) );
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
led_interpolator_manual_override_on( void )
{
    planner.manual_mode = true;
}

PUBLIC void
led_interpolator_manual_override_release( void )
{
    planner.manual_mode = false;
}

PUBLIC void
led_interpolator_manual_control_set( float hue, float saturation, float intensity, bool enabled )
{
    led_enable( enabled );    // Turn off the LED if the user isn't wanting it on

    GenericColour_t output_values = { 0.0f, 0.0f, 0.0f };

    if( enabled )
    {
        // Perform colour translation
        hsi_to_rgb( hue, saturation, intensity,
                    &output_values.x, &output_values.y, &output_values.z );

        // Set the LED channel values in RGB percentages [0.0f -> 1.0f]
        led_set( output_values.x, output_values.y, output_values.z );
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC void
led_interpolator_set_dark( void )
{
    planner.led_colour.red   = 0;
    planner.led_colour.green = 0;
    planner.led_colour.blue  = 0;

    led_set( planner.led_colour.red, planner.led_colour.red, planner.led_colour.red );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
led_interpolator_process( void )
{
    LEDPlanner_t *me = &planner;

    switch( me->currentState )
    {
        case ANIMATION_OFF:
            STATE_ENTRY_ACTION
            user_interface_set_led_status( me->currentState );
            led_interpolator_set_dark();

            // Track how long we've been off for
            me->animation_started = hal_systick_get_ms();
            STATE_TRANSITION_TEST
            if( me->animation_run )
            {
                if( me->fade_a.duration && me->fade_a.identifier == me->execute_id )
                {
                    STATE_NEXT( ANIMATION_EXECUTE_A );
                }
                else if( me->fade_b.duration && me->fade_b.identifier == me->execute_id )
                {
                    STATE_NEXT( ANIMATION_EXECUTE_B );
                }
            }

            if( me->manual_mode )
            {
                STATE_NEXT( ANIMATION_MANUAL );
            }

            // If off for extended period of time, turn the LED driver off
            if( hal_systick_get_ms() - me->animation_started >= LED_SLEEP_TIMER )
            {
                led_enable( false );
            }

            STATE_EXIT_ACTION
            led_enable( true );
            STATE_END
            break;

        case ANIMATION_EXECUTE_A:
            STATE_ENTRY_ACTION
            user_interface_set_led_status( me->currentState );
            me->animation_started      = hal_systick_get_ms();
            me->animation_est_complete = me->animation_started + me->fade_a.duration;
            me->progress_percent       = 0;
            STATE_TRANSITION_TEST
            led_interpolator_calculate_percentage( me->fade_a.duration );

            if( !me->animation_run || !me->fade_a.duration )
            {
                STATE_NEXT( ANIMATION_OFF );
            }
            else if( led_interpolator_get_fade_done() )
            {
                if( me->fade_b.duration && me->fade_b.identifier == me->execute_id )
                {
                    STATE_NEXT( ANIMATION_EXECUTE_B );
                }
                else
                {
                    STATE_NEXT( ANIMATION_OFF );
                }

                eventPublish( EVENT_NEW( StateEvent, ANIMATION_COMPLETE ) );
            }
            else
            {
                led_interpolator_execute_fade( &me->fade_a, me->progress_percent );
            }

            STATE_EXIT_ACTION
            memset( &me->fade_a, 0, sizeof( Fade_t ) );
            STATE_END
            break;

        case ANIMATION_EXECUTE_B:
            STATE_ENTRY_ACTION
            user_interface_set_led_status( me->currentState );
            me->animation_started      = hal_systick_get_ms();
            me->animation_est_complete = me->animation_started + me->fade_b.duration;
            me->progress_percent       = 0;
            STATE_TRANSITION_TEST
            led_interpolator_calculate_percentage( me->fade_b.duration );

            if( !me->animation_run || !me->fade_b.duration )
            {
                STATE_NEXT( ANIMATION_OFF );
            }
            else if( led_interpolator_get_fade_done() )
            {
                if( me->fade_a.duration && me->fade_a.identifier == me->execute_id )
                {
                    STATE_NEXT( ANIMATION_EXECUTE_A );
                }
                else
                {
                    STATE_NEXT( ANIMATION_OFF );
                }

                eventPublish( EVENT_NEW( StateEvent, ANIMATION_COMPLETE ) );
            }
            else
            {
                led_interpolator_execute_fade( &me->fade_b, me->progress_percent );
            }

            STATE_EXIT_ACTION
            memset( &me->fade_b, 0, sizeof( Fade_t ) );
            STATE_END
            break;

        case ANIMATION_MANUAL:
            STATE_ENTRY_ACTION
            user_interface_set_led_status( me->currentState );

            STATE_TRANSITION_TEST
            // All the fun for this state is done one-shot when the setting event comes in
            // See led_interpolator_manual_control_set() earlier in this file
            if( !planner.manual_mode )
            {
                STATE_NEXT( ANIMATION_OFF );
            }
            STATE_EXIT_ACTION

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
    uint32_t time_used = hal_systick_get_ms() - me->animation_started;

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
    if( points < 2 )
    {
        // need 2 points for a line
        return FADE_ERROR;
    }

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

    //for hue, remember that its a circular range so we need to take the shortest path across the wrapping
    float distance_ccw = ( p[0].hue >= p[1].hue ) ? p[0].hue - p[1].hue : 1 + p[0].hue - p[1].hue;
    float distance_cw  = ( p[0].hue >= p[1].hue ) ? 1 + p[1].hue - p[0].hue : p[1].hue - p[0].hue;

    output->hue = ( distance_cw <= distance_ccw ) ? p[1].hue + ( distance_cw * pos_weight ) : p[1].hue - ( distance_ccw * pos_weight );

    //handle wrapping around
    if( output->hue < 0 )
    {
        output->hue += 1;
    }

    if( output->hue > 1 )
    {
        output->hue -= 1;
    }

    output->saturation = p[0].saturation + pos_weight * ( p[1].saturation - p[0].saturation );
    output->intensity  = p[0].intensity + pos_weight * ( p[1].intensity - p[0].intensity );

    return FADE_VALID;
}

/* -------------------------------------------------------------------------- */

/* Converts a Hue/Saturation/Intensity colour to a Red/Green/Blue one
 * Input HSI are [0, 1]
 * Output RGB are [0, 1]
 */
void hsi_to_rgb( float h, float s, float i, float *r, float *g, float *b )
{
    float q;
    float p;

    // no saturation --> white/achromatic at the given intensity level
    if( s <= 0 )
    {
        *r = i;
        *g = i;
        *b = i;
    }
    else
    {
        if( i < 0.5 )
        {
            q = i * ( 1 + s );
        }
        else
        {
            q = i + s - i * s;
        }

        p = 2 * i - q;

        *r = hue_to_channel( p, q, h + 1 / 3.0 );
        *g = hue_to_channel( p, q, h );
        *b = hue_to_channel( p, q, h - 1 / 3.0 );
    }
}

// Helper for HSI/RGB conversions
float hue_to_channel( float p, float q, float t )
{
    if( t < 0 )
    {
        t += 1;
    }

    if( t > 1 )
    {
        t -= 1;
    }

    if( t < 1 / 6.0 )
    {
        return p + ( q - p ) * 6 * t;
    }

    if( t < 1 / 2.0 )
    {
        return q;
    }

    if( t < 2 / 3.0 )
    {
        return p + ( q - p ) * ( 2 / 3.0 - t ) * 6;
    }

    return p;
}

/* ----- End ---------------------------------------------------------------- */
