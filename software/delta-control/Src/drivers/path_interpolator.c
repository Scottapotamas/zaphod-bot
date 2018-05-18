/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "path_interpolator.h"
#include "motion_types.h"
#include "kinematics.h"
#include "clearpath.h"

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
    uint32_t        movement_started;	// timestamp the start point
    float        	progress_percent;	// calculated progress

} MotionPlanner_t;

/* ----- Private Variables -------------------------------------------------- */

PRIVATE MotionPlanner_t planner;

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
path_interpolator_init( )
{
    memset( &planner, 0, sizeof( planner ) );

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

            STATE_TRANSITION_TEST

            STATE_EXIT_ACTION
            STATE_END
            break;

        case PLANNER_ON:
            STATE_ENTRY_ACTION
				me->movement_started = hal_systick_get_ms();

            STATE_TRANSITION_TEST

				Movement_t *move = me->current_move;

            	CartesianPoint_t target 	= { 0, 0, 0 };	//target position in cartesian space
            	JointAngles_t angle_target 	= { 0, 0, 0 };	//target motor shaft angle in degrees

            	//calculate current target completion based on time
            	// time remaining is the allotted duration - time used (start to now), divide by the duration to get 0.0->1.0 progress
            	me->progress_percent = ( move->duration - ( me->movement_started - hal_systick_get_ms() ) ) / move->duration;

            	if( me->progress_percent >= 1.0 )
            	{
            		//movement is complete, the planner can stop now
            		STATE_NEXT( PLANNER_OFF );
            	}
            	else
            	{
                	switch( move->type )
                	{
    					case _POINT_TRANSIT:
    						target.x = move->points->x;
    						target.y = move->points->y;
    						target.z = move->points->z;
    						break;

    					case _LINE:
    						path_lerp_line( move->points, move->num_pts, me->progress_percent, &target );
    						break;

    					case _CATMULL_SPLINE:
    						path_catmull_spline( move->points, move->num_pts, me->progress_percent, &target );
    						break;
                	}

    				// Convert cartesian target to motor angles
                	kinematics_point_to_angle( target, &angle_target );

                	// Ask the motors to please move there
                	servo_set_target_angle( _CLEARPATH_1, angle_target.a1 );
                	servo_set_target_angle( _CLEARPATH_2, angle_target.a2 );
                	servo_set_target_angle( _CLEARPATH_3, angle_target.a3 );
            	}

            STATE_EXIT_ACTION

            STATE_END
            break;
    }
}


/* -------------------------------------------------------------------------- */

// p[0], p[1] are the two points in 3D space
// rel_weight is the 0.0-1.0 percentage position on the line
// the output pointer is the interpolated position on the line

PUBLIC KinematicsSolution_t
path_lerp_line( CartesianPoint_t p[], size_t points, float pos_weight, CartesianPoint_t *output )
{
	if(points < 2)
	{
		// need 2 points for a line
		return _SOLUTION_ERROR;
	}

	// exact start and end of splines don't need calculation as catmull curves _will_ pass through all points
	if(pos_weight == 0.0f)
	{
		output = &p[0];
		return _SOLUTION_VALID;
	}

	if(pos_weight == 1.0f)
	{
		output = &p[1];
		return _SOLUTION_VALID;
	}

    // Linear interpolation between two points (lerp)
	output->x = p[0].x + pos_weight*( p[1].x - p[0].x );
	output->y = p[0].y + pos_weight*( p[1].y - p[0].y );
	output->z = p[0].z + pos_weight*( p[1].z - p[0].z );

    return _SOLUTION_VALID;
}

/* -------------------------------------------------------------------------- */

// p[0], p[1], p[2], p[3] are the control points in 3D space
// rel_weight is the 0.0-1.0 percentage position on the curve between p1 and p2
// the output pointer is the interpolated position on the curve between p1 and p2

PUBLIC KinematicsSolution_t
path_catmull_spline( CartesianPoint_t p[], size_t points, float pos_weight, CartesianPoint_t *output )
{
	if(points < 4)
	{
		// need 4 points for solution
		return _SOLUTION_ERROR;
	}

	// exact start and end of splines don't need calculation as catmull curves _will_ pass through all points
	if(pos_weight == 0.0f)
	{
		output = &p[1];
		return _SOLUTION_VALID;	// todo add a 'end of range' flag?
	}

	if(pos_weight == 1.0f)
	{
		output = &p[2];
		return _SOLUTION_VALID;
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
	output->x = 0.5 * (
				( 2*p[1].x ) +
				(  -p[0].x   +   p[2].x ) * t +
				( 2*p[0].x   - 5*p[1].x   + 4*p[2].x - p[3].x) * t2 +
				(  -p[0].x   + 3*p[1].x   - 3*p[2].x + p[3].x) * t3 );

	output->y = 0.5 * (
				( 2*p[1].y ) +
				(  -p[0].y   +   p[2].y ) * t +
				( 2*p[0].y   - 5*p[1].y   + 4*p[2].y - p[3].y) * t2 +
				(  -p[0].y   + 3*p[1].y   - 3*p[2].y + p[3].y) * t3 );

	output->z = 0.5 * (
				( 2*p[1].z ) +
				(  -p[0].z   +   p[2].z ) * t +
				( 2*p[0].z   - 5*p[1].z   + 4*p[2].z - p[3].z) * t2 +
				(  -p[0].z   + 3*p[1].z   - 3*p[2].z + p[3].z) * t3 );

    return _SOLUTION_VALID;
}

/* ----- End ---------------------------------------------------------------- */
