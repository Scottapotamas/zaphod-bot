/* ----- System Includes ---------------------------------------------------- */

#include <math.h>
#include <string.h>
#include <float.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "led_interpolator.h"
#include "led_types.h"
#include "configuration.h"

#include "global.h"
#include "simple_state_machine.h"
#include "hal_systick.h"

/* ----- Defines ------------------------------------------------------------ */

typedef enum
{
    PLANNER_OFF,
    PLANNER_ON,
} RGBState_t;

typedef struct
{
	RGBState_t  previousState;
	RGBState_t  currentState;
	RGBState_t  nextState;

	Fade_t	*	current_fade;		// pointer to the current movement
	bool		enable;				//if the planner is enabled
    uint32_t   	animation_started;	// timestamp the start
    float       progress_percent;	// calculated progress

    RGBColour_t	led_colour;	// current channel outputs

} LEDPlanner_t;

/* ----- Private Variables -------------------------------------------------- */

PRIVATE LEDPlanner_t planner;

PRIVATE FadeSolution_t
hsi_lerp_linear( HSIColour_t p[], size_t points, float pos_weight, HSIColour_t* output );

PRIVATE void
hsi_to_rgb( float h, float s, float i, float* r, float* g, float* b );

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
led_interpolator_init( void )
{
    memset( &planner, 0, sizeof( planner ) );

}

/* -------------------------------------------------------------------------- */

PUBLIC void
led_interpolator_set_objective( Fade_t* fade_to_process )
{
	planner.current_fade = fade_to_process;
	planner.enable = true;
}

/* -------------------------------------------------------------------------- */

PUBLIC float
led_interpolator_get_progress( void )
{
	return planner.progress_percent;
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
led_interpolator_get_move_done( void )
{
	return ( planner.progress_percent >= 1.0f - FLT_EPSILON && !planner.enable);
}

/* -------------------------------------------------------------------------- */

PUBLIC void
led_interpolator_off( void )
{
	planner.led_colour.red 		= 0;
	planner.led_colour.green 	= 0;
	planner.led_colour.blue 	= 0;

	return;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
led_interpolator_process( void )
{
	LEDPlanner_t *me = &planner;

    switch( me->currentState )
    {
        case PLANNER_OFF:
            STATE_ENTRY_ACTION
        		config_set_pathing_status(me->currentState);

            STATE_TRANSITION_TEST

			if(planner.enable)
			{
        		STATE_NEXT( PLANNER_ON );
			}

            STATE_EXIT_ACTION
            STATE_END
            break;

        case PLANNER_ON:
            STATE_ENTRY_ACTION
				me->animation_started = hal_systick_get_ms();
            	me->progress_percent = 0;

            	config_set_pathing_status(me->currentState);

            STATE_TRANSITION_TEST

				Fade_t *animation = me->current_fade;

            	HSIColour_t 	fade_target 	= { 0.0f, 0.0f, 0.0f };
            	GenericColour_t output_values 	= { 0.0f, 0.0f, 0.0f };

            	//calculate current target completion based on time
            	// time remaining is the allotted duration - time used (start to now), divide by the duration to get 0.0->1.0 progress
            	uint32_t time_used = hal_systick_get_ms() - me->animation_started;
            	me->progress_percent = (float)( time_used ) / animation->duration;

            	if( me->progress_percent >= 1.0f - FLT_EPSILON )
            	{
            		//fade is complete, the planner can stop now
            		STATE_NEXT( PLANNER_OFF );
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

                	// Set the LED channel values
                	//set_channel_red( output_values.x * 0xFFFF );

                	//update the config/UI data based on these actions

                	//todo implement calls to other layers here

            	}

            STATE_EXIT_ACTION
				planner.enable = false;
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
		output = &p[0];
		return FADE_VALID;
	}

	if(pos_weight >= 1.0f - FLT_EPSILON)
	{
		output = &p[1];
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

void
hsi_to_rgb( float h, float s, float i, float* r, float* g, float* b )
{
	float x = i * (1 - s);
	float y;
	float z;

	if( h < 2 * M_PI / 3 )
	{
		y = i * (1 + (s * cos(h)) / (cos(M_PI / 3 - h)));
		z = 3 * i - (x + y);

		*b = x;
		*r = y;
		*g = z;
	}
	else if( h < 4 * M_PI / 3 )
	{
		y = i * (1 + (s * cos(h - 2 * M_PI / 3)) / (cos(M_PI / 3 - (h  - 2 * M_PI / 3))));
		z = 3 * i - (x + y);

		*r = x;
		*g = y;
		*b = z;
	}
	else
	{
		y = i * (1 + (s * cos(h - 4 * M_PI / 3)) / (cos(M_PI / 3 - (h  - 4 * M_PI / 3))));
		z = 3 * i - (x + y);

		*g = x;
		*b = y;
		*r = z;
	}
}

/* ----- End ---------------------------------------------------------------- */
