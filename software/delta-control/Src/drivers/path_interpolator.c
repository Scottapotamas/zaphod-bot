/* ----- System Includes ---------------------------------------------------- */

#include <string.h>
#include <float.h>
#include <math.h>
#include <app_state_machines/app_signals.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "path_interpolator.h"
#include "event_subscribe.h"
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

	Movement_t		current_move;		// copy of the current movement
	bool			enable;				//if the planner is enabled
    uint32_t        movement_started;	// timestamp the start point
    float        	progress_percent;	// calculated progress

    CartesianPoint_t	effector_position;	//position of the end effector (used for relative moves)

} MotionPlanner_t;

/* ----- Private Variables -------------------------------------------------- */

PRIVATE MotionPlanner_t planner;


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

	memcpy( &me->current_move, movement_to_process, sizeof(Movement_t) );

	//apply current position to a relative movement
	if( me->current_move.ref == _POS_RELATIVE )
	{
		for( uint8_t i = 0; i < me->current_move.num_pts; i++ )
		{
            me->current_move.points[i].x += me->effector_position.x;
            me->current_move.points[i].y += me->effector_position.y;
            me->current_move.points[i].z += me->effector_position.z;
		}
	}

	// Clamp all inbound points to the motion boundaries
	for( uint8_t i = 0; i < planner.current_move.num_pts; i++ )
	{
		//todo move point clamp ranges to somewhere more sensible
		me->current_move.points[i].x = CLAMP( me->current_move.points[i].x, MM_TO_MICRONS(-200), MM_TO_MICRONS(200) );
		me->current_move.points[i].y = CLAMP( me->current_move.points[i].y, MM_TO_MICRONS(-200), MM_TO_MICRONS(200) );
		me->current_move.points[i].z = CLAMP( me->current_move.points[i].z, MM_TO_MICRONS(0), MM_TO_MICRONS(300) );
	}

	// A transit move is from current position to point 1, so overwrite 0 with current position, and then reuse a normal line movement
	if( me->current_move.type == _POINT_TRANSIT)
	{
		if(me->current_move.num_pts == 1)
		{
			me->current_move.points[1].x = me->current_move.points[0].x;
			me->current_move.points[1].y = me->current_move.points[0].y;
			me->current_move.points[1].z = me->current_move.points[0].z;
			me->current_move.num_pts = 2;
		}

		me->current_move.points[0].x = me->effector_position.x;
		me->current_move.points[0].y = me->effector_position.y;
		me->current_move.points[0].z = me->effector_position.z;
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
path_interpolator_reset( void )
{
    MotionPlanner_t *me = &planner;

    // Request that the statemachine return to "OFF"
    me->enable = false;
    memset( &me->current_move, 0, sizeof(CartesianPoint_t));

}

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_set_home( void )
{
	planner.effector_position.x = 0;
	planner.effector_position.y = 0;
	planner.effector_position.z = 0;
	config_set_position( planner.effector_position.x, planner.effector_position.y, planner.effector_position.z );
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

				Movement_t *move = &me->current_move;

            	CartesianPoint_t target 	= { 0, 0, 0 };	//target position in cartesian space
            	JointAngles_t angle_target 	= { 0, 0, 0 };	//target motor shaft angle in degrees

            	//calculate current target completion based on time
            	// time remaining is the allotted duration - time used (start to now), divide by the duration to get 0.0->1.0 progress
            	uint32_t time_used = hal_systick_get_ms() - me->movement_started;
            	me->progress_percent = (float)( time_used ) / move->duration;

            	if( me->progress_percent >= 1.0f - FLT_EPSILON || planner.enable == false )
            	{
            		//movement is complete, the planner can stop now
            		STATE_NEXT( PLANNER_OFF );
                    eventPublish( EVENT_NEW( StateEvent, PATHING_COMPLETE ) );
                }
            	else
            	{
                	switch( move->type )
                	{
    					case _POINT_TRANSIT:
                            cartesian_point_on_line(move->points, move->num_pts, me->progress_percent, &target);
    			    		break;

    					case _LINE:
                            cartesian_point_on_line(move->points, move->num_pts, me->progress_percent, &target);
    						break;

    					case _CATMULL_SPLINE:
                            cartesian_point_on_catmull_spline(move->points, move->num_pts, me->progress_percent,
                                                              &target);
    						break;

    					case _BEZIER_QUADRATIC:
                            cartesian_point_on_quadratic_bezier(move->points, move->num_pts, me->progress_percent,
                                                                &target);
    						break;

    					case _BEZIER_CUBIC:
                            cartesian_point_on_cubic_bezier(move->points, move->num_pts, me->progress_percent, &target);
    						break;
    					default:
    						//TODO this should be considered a motion error

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

/* ----- End ---------------------------------------------------------------- */
