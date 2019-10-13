/* ----- System Includes ---------------------------------------------------- */

#include <math.h>
#include <string.h>
#include <float.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "led_interpolator.h"
#include "led_types.h"
#include "configuration.h"
#include "led.h"
#include "global.h"

#include "simple_state_machine.h"
#include "event_subscribe.h"
#include <app_state_machines/app_signals.h>
#include "hal_systick.h"

/* ----- Defines ------------------------------------------------------------ */

typedef enum
{
    ANIMATION_OFF,
    ANIMATION_ON,
    ANIMATION_MANUAL,
} RGBState_t;

typedef struct
{
	RGBState_t  previousState;
	RGBState_t  currentState;
	RGBState_t  nextState;

    bool        manual_mode;        // user control
    bool		animation_mode;		//if the planner is enabled

    Fade_t	 	current_fade;		// pointer to the current movement
    uint32_t   	animation_started;	// timestamp the start
    float       progress_percent;	// calculated progress

    RGBColour_t	led_colour;	        // current channel outputs

} LEDPlanner_t;

/* ----- Private Variables -------------------------------------------------- */

PRIVATE LEDPlanner_t planner;

PRIVATE FadeSolution_t
hsi_lerp_linear( HSIColour_t p[], size_t points, float pos_weight, HSIColour_t* output );

PRIVATE void
hsi_to_rgb( float h, float s, float i, float* r, float* g, float* b );

PRIVATE float
hue_to_channel(float p, float q, float t);

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
led_interpolator_init( void )
{
    memset( &planner, 0, sizeof( planner ) );

    led_init();
}

/* -------------------------------------------------------------------------- */

PUBLIC void
led_interpolator_set_objective( Fade_t* fade_to_process )
{
    LEDPlanner_t *me = &planner;

    memcpy( &me->current_fade, fade_to_process, sizeof(Fade_t) );
    planner.manual_mode = false;
    planner.animation_mode = true;
}

/* -------------------------------------------------------------------------- */

PUBLIC float
led_interpolator_get_progress( void )
{
	return planner.progress_percent;
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
led_interpolator_get_fade_done(void )
{
	return ( planner.progress_percent >= 1.0f - FLT_EPSILON && !planner.animation_mode);
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

/* -------------------------------------------------------------------------- */

PUBLIC void
led_interpolator_off( void )
{
	planner.led_colour.red 		= 0;
	planner.led_colour.green 	= 0;
	planner.led_colour.blue 	= 0;
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
        		config_set_led_status(me->currentState);
//                led_set(0,0,0);

            STATE_TRANSITION_TEST

                led_set( planner.led_colour.red, planner.led_colour.red, planner.led_colour.red);

                if(planner.animation_mode)
                {
                    STATE_NEXT( ANIMATION_ON );
                }

                if(planner.manual_mode)
                {
                    STATE_NEXT( ANIMATION_MANUAL );
                }

            STATE_EXIT_ACTION
            STATE_END
            break;

        case ANIMATION_ON:
            STATE_ENTRY_ACTION
				me->animation_started = hal_systick_get_ms();
            	me->progress_percent = 0;
                led_enable(true);
                config_set_led_status(me->currentState);

            STATE_TRANSITION_TEST

				Fade_t *animation = &me->current_fade;

            	HSIColour_t 	fade_target 	= { 0.0f, 0.0f, 0.0f };
            	GenericColour_t output_values 	= { 0.0f, 0.0f, 0.0f };

            	//calculate current target completion based on time
            	// time remaining is the allotted duration - time used (start to now), divide by the duration to get 0.0->1.0 progress
            	uint32_t time_used = hal_systick_get_ms() - me->animation_started;
            	me->progress_percent = (float)( time_used ) / animation->duration;

            	if( me->progress_percent >= 1.0f - FLT_EPSILON )
            	{
            		//fade is complete, the planner can stop now
                    eventPublish( EVENT_NEW( StateEvent, ANIMATION_COMPLETE ) );
            		STATE_NEXT( ANIMATION_OFF );
            	}
            	else
            	{
                	switch( animation->type )
                	{
    					case _INSTANT_CHANGE:
    						fade_target.hue 		= animation->input_colours[0].hue;
    						fade_target.saturation 	= animation->input_colours[0].saturation;
    						fade_target.intensity 	= animation->input_colours[0].intensity;

    						break;

    					case _LINEAR_RAMP:
    						hsi_lerp_linear(animation->input_colours, animation->num_pts, me->progress_percent, &fade_target);
    						break;
                	}

    				// Perform colour compensation adjustments
                	hsi_to_rgb(fade_target.hue, fade_target.saturation, fade_target.intensity, &output_values.x, &output_values.y, &output_values.z);

                	// Set the LED channel values in RGB percentages [0.0f -> 1.0f]
                    led_set( output_values.x, output_values.y, output_values.z);

            	}

            STATE_EXIT_ACTION
				planner.animation_mode = false;

            STATE_END
            break;

        case ANIMATION_MANUAL:
            STATE_ENTRY_ACTION
                config_set_led_status(me->currentState);

        STATE_TRANSITION_TEST

                HSIColour_t manual_setpoint = { 0.0f, 0.0f, 0.0f };
                GenericColour_t output_values 	= { 0.0f, 0.0f, 0.0f };

                bool enabled = false;

                // Get the user's intended values
                config_get_led_manual( &manual_setpoint.hue, &manual_setpoint.saturation, &manual_setpoint.intensity, &enabled);

                led_enable(enabled);    // Turn off the LED if the user isn't wanting it on

                // Perform colour translation
                hsi_to_rgb( manual_setpoint.hue, manual_setpoint.saturation, manual_setpoint.intensity,
                            &output_values.x, &output_values.y, &output_values.z );

                // Set the LED channel values in RGB percentages [0.0f -> 1.0f]
                led_set( output_values.x, output_values.y, output_values.z );

                if( !planner.manual_mode )
                {
                    STATE_NEXT( ANIMATION_OFF );
                }

            STATE_EXIT_ACTION
                led_enable(false);
            STATE_END

            break;

    }
}

/* -------------------------------------------------------------------------- */

FadeSolution_t
hsi_lerp_linear( HSIColour_t p[], size_t points, float pos_weight, HSIColour_t *output )
{
	if(points < 2)
	{
		// need 2 points for a line
		return FADE_ERROR;
	}

	if(pos_weight <= 0.0f + FLT_EPSILON)
	{
        memcpy( output, &p[0], sizeof(HSIColour_t) );
		return FADE_VALID;
	}

	if(pos_weight >= 1.0f - FLT_EPSILON)
	{
        memcpy( output, &p[1], sizeof(HSIColour_t) );
        return FADE_VALID;
	}

    // Linear interpolation between two points (lerp)

	//for hue, remember that its a circular range so we need to take the shortest path across the wrapping
	float distance_ccw = (p[0].hue >= p[1].hue) ?     p[0].hue - p[1].hue  : 1 + p[0].hue - p[1].hue;
	float distance_cw  = (p[0].hue >= p[1].hue) ? 1 + p[1].hue - p[0].hue  :     p[1].hue - p[0].hue;

	output->hue = (distance_cw <= distance_ccw) ? p[1].hue + (distance_cw * pos_weight) : p[1].hue - (distance_ccw * pos_weight);

	//handle wrapping around
    if (output->hue < 0)
    {
    	output->hue += 1;
    }

    if (output->hue > 1)
    {
    	output->hue -= 1;
    }

	output->saturation 	= p[0].saturation + pos_weight*( p[1].saturation - p[0].saturation );
	output->intensity	= p[0].intensity  + pos_weight*( p[1].intensity  - p[0].intensity  );

    return FADE_VALID;
}

/* -------------------------------------------------------------------------- */

/* Converts a Hue/Saturation/Intensity colour to a Red/Green/Blue one
 * Input HSI are [0, 1]
 * Output RGB are [0, 1]
 */
void
hsi_to_rgb( float h, float s, float i, float* r, float* g, float* b )
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
            q = i * (1 + s);
        }
        else
        {
            q = i + s - i * s;
        }

        p = 2 * i - q;

        *r = hue_to_channel( p, q, h + 1/3.0 );
        *g = hue_to_channel( p, q, h );
        *b = hue_to_channel( p, q, h - 1/3.0 );
    }

}

// Helper for HSI/RGB conversions
float hue_to_channel(float p, float q, float t)
{
    if( t < 0 )
    {
        t += 1;
    }

    if( t > 1 )
    {
        t -= 1;
    }

    if( t < 1/6.0 )
    {
        return p + (q - p) * 6 * t;
    }

    if( t < 1/2.0 )
    {
        return q;
    }

    if( t < 2/3.0 )
    {
        return p + (q - p) * (2 / 3.0 - t) * 6;
    }

    return p;
}

/* ----- End ---------------------------------------------------------------- */
