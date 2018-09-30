/* ----- System Includes ---------------------------------------------------- */

#include <string.h>
#include <float.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "path_interpolator.h"
#include "motion_types.h"
#include "kinematics.h"
#include "clearpath.h"
#include "configuration.h"

#include "global.h"
#include "simple_state_machine.h"
#include "hal_systick.h"

/* ----- Defines ------------------------------------------------------------ */

typedef enum
{
    PLANNER_OFF,
    PLANNER_ON,
} PlanningState_t;

typedef struct
{
	PlanningState_t   previousState;
	PlanningState_t   currentState;
	PlanningState_t   nextState;

	Movement_t	*	current_move;		// pointer to the current movement
	bool			enable;				//if the planner is enabled
    uint32_t        movement_started;	// timestamp the start point
    float        	progress_percent;	// calculated progress

    CartesianPoint_t	effector_position;	//position of the end effector (used for relative moves)

} MotionPlanner_t;

/* ----- Private Variables -------------------------------------------------- */

PRIVATE MotionPlanner_t planner;

PRIVATE KinematicsSolution_t
path_lerp_line( CartesianPoint_t p[], size_t points, float pos_weight, CartesianPoint_t *output );

PRIVATE KinematicsSolution_t
path_catmull_spline( CartesianPoint_t p[], size_t points, float pos_weight, CartesianPoint_t *output );

PRIVATE KinematicsSolution_t
path_quadratic_bezier_curve( CartesianPoint_t p[], size_t points, float pos_weight, CartesianPoint_t *output );

PRIVATE KinematicsSolution_t
path_cubic_bezier_curve( CartesianPoint_t p[], size_t points, float pos_weight, CartesianPoint_t *output );

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
path_interpolator_init( void )
{
    memset( &planner, 0, sizeof( planner ) );

}

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_set_objective( Movement_t	* movement_to_process )
{
	MotionPlanner_t *me = &planner;

	//todo accept a movement object and store inside the state for handling
	planner.current_move = movement_to_process;

	//apply current position to a relative movement
	if( movement_to_process->ref == _POS_RELATIVE )
	{
		for( uint8_t i = 0; i < planner.current_move->num_pts; i++ )
		{
			movement_to_process->points[i].x += me->effector_position.x;
			movement_to_process->points[i].y += me->effector_position.y;
			movement_to_process->points[i].z += me->effector_position.z;
		}
	}

	for( uint8_t i = 0; i < planner.current_move->num_pts; i++ )
	{
		//todo move point clamp ranges to somewhere more sensible
		movement_to_process->points[i].x = CLAMP( movement_to_process->points[i].x, MM_TO_MICRONS(-200), MM_TO_MICRONS(200) );
		movement_to_process->points[i].y = CLAMP( movement_to_process->points[i].y, MM_TO_MICRONS(-200), MM_TO_MICRONS(200) );
		movement_to_process->points[i].z = CLAMP( movement_to_process->points[i].z, MM_TO_MICRONS(0), MM_TO_MICRONS(300) );
	}

	//transit move is from current position to point 1, so overwrite 0 with current position, and then reuse a normal line movement
	if( movement_to_process->type == _POINT_TRANSIT)
	{
		if(movement_to_process->num_pts == 1)
		{
			movement_to_process->points[1].x = movement_to_process->points[0].x;
			movement_to_process->points[1].y = movement_to_process->points[0].y;
			movement_to_process->points[1].z = movement_to_process->points[0].z;
			movement_to_process->num_pts = 2;
		}

		movement_to_process->points[0].x = me->effector_position.x;
		movement_to_process->points[0].y = me->effector_position.y;
		movement_to_process->points[0].z = me->effector_position.z;
	}

	planner.enable = true;
}

/* -------------------------------------------------------------------------- */

PUBLIC float
path_interpolator_get_progress( void )
{
	return planner.progress_percent;
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
path_interpolator_get_move_done( void )
{
	return ( planner.progress_percent >= 1.0f - FLT_EPSILON && !planner.enable);
}

/* -------------------------------------------------------------------------- */

PUBLIC CartesianPoint_t
path_interpolator_get_global_position( void )
{
	return planner.effector_position;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_set_home( void )
{
	planner.effector_position.x = 0;
	planner.effector_position.y = 0;
	planner.effector_position.z = 0;
	config_set_position( planner.effector_position.x, planner.effector_position.y, planner.effector_position.z );

	return;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_process( void )
{
	MotionPlanner_t *me = &planner;

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
				me->movement_started = hal_systick_get_ms();
            	me->progress_percent = 0;

            	config_set_pathing_status(me->currentState);

            STATE_TRANSITION_TEST

				Movement_t *move = me->current_move;

            	CartesianPoint_t target 	= { 0, 0, 0 };	//target position in cartesian space
            	JointAngles_t angle_target 	= { 0, 0, 0 };	//target motor shaft angle in degrees

            	//calculate current target completion based on time
            	// time remaining is the allotted duration - time used (start to now), divide by the duration to get 0.0->1.0 progress
            	uint32_t time_used = hal_systick_get_ms() - me->movement_started;
            	me->progress_percent = (float)( time_used ) / move->duration;

            	if( me->progress_percent >= 1.0f - FLT_EPSILON )
            	{
            		//movement is complete, the planner can stop now
            		STATE_NEXT( PLANNER_OFF );
            	}
            	else
            	{
                	switch( move->type )
                	{
    					case _POINT_TRANSIT:
    						path_lerp_line( move->points, move->num_pts, me->progress_percent, &target );
    			    		break;

    					case _LINE:
    						path_lerp_line( move->points, move->num_pts, me->progress_percent, &target );
    						break;

    					case _CATMULL_SPLINE:
    						path_catmull_spline( move->points, move->num_pts, me->progress_percent, &target );
    						break;

    					case _BEZIER_QUADRATIC:
    						path_quadratic_bezier_curve( move->points, move->num_pts, me->progress_percent, &target );
    						break;

    					case _BEZIER_CUBIC:
    						path_cubic_bezier_curve( move->points, move->num_pts, me->progress_percent, &target );
    						break;
    					default:
    						//todo this should be considered a motion error

    						break;
                	}

                	kinematics_point_to_angle( target, &angle_target );

                	// Ask the motors to please move there
                	servo_set_target_angle( _CLEARPATH_1, angle_target.a1 );
                	servo_set_target_angle( _CLEARPATH_2, angle_target.a2 );
                	servo_set_target_angle( _CLEARPATH_3, angle_target.a3 );

                	//update the config/UI data based on these actions
                	config_set_position( target.x, target.y, target.z );
                	me->effector_position = target;

                	config_set_movement_data( move->identifier, move->type, (uint8_t)(me->progress_percent*100) );

            	}

            STATE_EXIT_ACTION
				planner.enable = false;
            STATE_END
            break;
    }

}


/* -------------------------------------------------------------------------- */

// p[0], p[1] are the two points in 3D space
// rel_weight is the 0.0-1.0 percentage position on the line
// the output pointer is the interpolated position on the line

PRIVATE KinematicsSolution_t
path_lerp_line( CartesianPoint_t p[], size_t points, float pos_weight, CartesianPoint_t *output )
{
	if(points < 2)
	{
		// need 2 points for a line
		return SOLUTION_ERROR;
	}

	// start and end of splines don't need calculation as catmull curves _will_ pass through all points
	if(pos_weight <= 0.0f + FLT_EPSILON)
	{
		output = &p[0];
		return SOLUTION_VALID;
	}

	if(pos_weight >= 1.0f - FLT_EPSILON)
	{
		output = &p[1];
		return SOLUTION_VALID;
	}

    // Linear interpolation between two points (lerp)
	output->x = p[0].x + pos_weight*( p[1].x - p[0].x );
	output->y = p[0].y + pos_weight*( p[1].y - p[0].y );
	output->z = p[0].z + pos_weight*( p[1].z - p[0].z );

    return SOLUTION_VALID;
}

/* -------------------------------------------------------------------------- */

// p[0], p[1], p[2], p[3] are the control points in 3D space
// rel_weight is the 0.0-1.0 percentage position on the curve between p1 and p2
// the output pointer is the interpolated position on the curve between p1 and p2

PRIVATE KinematicsSolution_t
path_catmull_spline( CartesianPoint_t p[], size_t points, float pos_weight, CartesianPoint_t *output )
{
	if(points < 4)
	{
		// need 4 points for solution
		return SOLUTION_ERROR;
	}

	// start and end of splines don't need calculation as catmull curves _will_ pass through all points
	if(pos_weight <= 0.0f + FLT_EPSILON)
	{
		output = &p[1];
		return SOLUTION_VALID;	// todo add a 'end of range' flag?
	}

	if(pos_weight >= 1.0f - FLT_EPSILON)
	{
		output = &p[2];
		return SOLUTION_VALID;
	}

    /* Derivation from http://www.mvps.org/directx/articles/catmull/
     *
								[  0  2  0  0 ]   [ p0 ]
	q(t) = 0.5( t, t^2, t^3 ) * [ -1  0  1  0 ] * [ p1 ]
								[  2 -5  4 -1 ]   [ p2 ]
								[ -1  3 -3  1 ]   [ p3 ]
     */

	// pre-calculate
    float t = pos_weight;
    float t2 = t * t;
    float t3 = t2 * t;

	// todo consider accelerating with matrix maths (neon) if perf improvements required
	output->x = 0.5f * (
				( 2*p[1].x ) +
				(  -p[0].x   +   p[2].x ) * t +
				( 2*p[0].x   - 5*p[1].x   + 4*p[2].x - p[3].x) * t2 +
				(  -p[0].x   + 3*p[1].x   - 3*p[2].x + p[3].x) * t3 );

	output->y = 0.5f * (
				( 2*p[1].y ) +
				(  -p[0].y   +   p[2].y ) * t +
				( 2*p[0].y   - 5*p[1].y   + 4*p[2].y - p[3].y) * t2 +
				(  -p[0].y   + 3*p[1].y   - 3*p[2].y + p[3].y) * t3 );

	output->z = 0.5f * (
				( 2*p[1].z ) +
				(  -p[0].z   +   p[2].z ) * t +
				( 2*p[0].z   - 5*p[1].z   + 4*p[2].z - p[3].z) * t2 +
				(  -p[0].z   + 3*p[1].z   - 3*p[2].z + p[3].z) * t3 );

    return SOLUTION_VALID;
}

/* -------------------------------------------------------------------------- */

// p[0], p[1], p[2] are the start, control and end points in 3D space
// rel_weight is the 0.0-1.0 percentage position on the curve between p0 and p2
// the output pointer is the interpolated position on the curve between p0 and p2

PRIVATE KinematicsSolution_t
path_quadratic_bezier_curve( CartesianPoint_t p[], size_t points, float pos_weight, CartesianPoint_t *output )
{
	if(points < 3)
	{
		// need 3 points for quadratic solution
		return SOLUTION_ERROR;
	}

	// start and end of bezier
	if(pos_weight <= 0.0f + FLT_EPSILON)
	{
		output = &p[0];
		return SOLUTION_VALID;
	}

	if(pos_weight >= 1.0f - FLT_EPSILON)
	{
		output = &p[2];
		return SOLUTION_VALID;
	}

	// General form for a quadratic bezier curve

	// B(t) = ((1-t)^2 * p0) + (2(1 - t) * t * p1) + (t^2 * p2) where 0 < t < 1

	//cache oft-used values to improve read-ability
	float t = pos_weight;
	float tsq = t * t;

	float omt = 1 - t;
	float omt2 = omt * omt;

	output->x = ( omt2 * p[0].x ) + ( 2 * omt * t * p[1].x ) + ( tsq * p[2].x );
	output->y = ( omt2 * p[0].y ) + ( 2 * omt * t * p[1].y ) + ( tsq * p[2].y );
	output->z = ( omt2 * p[0].z ) + ( 2 * omt * t * p[1].z ) + ( tsq * p[2].z );

    return SOLUTION_VALID;
}

/* -------------------------------------------------------------------------- */

// p[0], p[1], p[2], p[3] are the start, control, control and end points in 3D space
// rel_weight is the 0.0-1.0 percentage position on the curve between p0 and p2
// the output pointer is the interpolated position on the curve between p0 and p2

PRIVATE KinematicsSolution_t
path_cubic_bezier_curve( CartesianPoint_t p[], size_t points, float pos_weight, CartesianPoint_t *output )
{
	if(points < 4)
	{
		// need 3 points for quadratic solution
		return SOLUTION_ERROR;
	}

	// start and end of bezier
	if(pos_weight <= 0.0f + FLT_EPSILON)
	{
		output = &p[0];
		return SOLUTION_VALID;
	}

	if(pos_weight >= 1.0f - FLT_EPSILON)
	{
		output = &p[3];
		return SOLUTION_VALID;
	}

	// General form for a cubic bezier curve

	// B(t) = ((1-t)^3 * p0) + (3(1 - t)^2 * t * P1) + (3(1-t)t^2 * P2) + (t^3 * P3) where 0 < t < 1

	//cache oft-used values to improve read-ability
	float t = pos_weight;
	float tsq = t * t;
	float tcu = tsq * t;

	float omt = 1 - t;
	float omt2 = omt * omt;
	float omt3 = omt2 * omt;

	output->x = ( omt3 * p[0].x ) + ( 3 * omt2 * t * p[1].x ) + ( 3 * omt * tsq * p[2].x ) + ( tcu * p[2].x );
    output->y = ( omt3 * p[0].y ) + ( 3 * omt2 * t * p[1].y ) + ( 3 * omt * tsq * p[2].y ) + ( tcu * p[3].y );
    output->z = ( omt3 * p[0].z ) + ( 3 * omt2 * t * p[1].z ) + ( 3 * omt * tsq * p[2].z ) + ( tcu * p[3].z );

    return SOLUTION_VALID;
}

/* ----- End ---------------------------------------------------------------- */
