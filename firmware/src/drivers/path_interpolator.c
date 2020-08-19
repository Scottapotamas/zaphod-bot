/* ----- System Includes ---------------------------------------------------- */

#include <float.h>
#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "path_interpolator.h"

#include "app_events.h"
#include "app_signals.h"
#include "event_subscribe.h"
#include "global.h"
#include "simple_state_machine.h"

#include "hal_systick.h"

#include "clearpath.h"
#include "configuration.h"
#include "kinematics.h"
#include "motion_types.h"
#include "status.h"

/* ----- Defines ------------------------------------------------------------ */

typedef enum
{
    PLANNER_OFF,
    PLANNER_EXECUTE_A,
    PLANNER_EXECUTE_B
} PlanningState_t;

typedef struct
{
    PlanningState_t previousState;
    PlanningState_t currentState;
    PlanningState_t nextState;

    Movement_t move_a;    // copy of the current movement
    Movement_t move_b;    // copy of the current movement

    bool     enable;                   //if the planner is enabled
    uint32_t movement_started;         // timestamp the start point
    uint32_t movement_est_complete;    // timestamp the predicted end point
    float    progress_percent;         // calculated progress

    CartesianPoint_t effector_position;    //position of the end effector (used for relative moves)

} MotionPlanner_t;

/* ----- Private Variables -------------------------------------------------- */

PRIVATE MotionPlanner_t planner;

PRIVATE void path_interpolator_premove_transforms( Movement_t *move );
PRIVATE void path_interpolator_execute_move( Movement_t *move, float percentage );
PRIVATE void path_interpolator_calculate_percentage( uint16_t move_duration );

PRIVATE void path_interpolator_notify_pathing_started( uint16_t move_id );
PRIVATE void path_interpolator_notify_pathing_complete( uint16_t move_id );

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
path_interpolator_init( void )
{
    memset( &planner, 0, sizeof( planner ) );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_set_next( Movement_t *movement_to_process )
{
    MotionPlanner_t *me                   = &planner;
    Movement_t *     movement_insert_slot = { 0 };    // allows us to put the new move into whichever slot is available

    if( me->move_a.duration == 0 )
    {
        movement_insert_slot = &me->move_a;
    }
    else if( me->move_b.duration == 0 )
    {
        movement_insert_slot = &me->move_b;
    }

    memcpy( movement_insert_slot, movement_to_process, sizeof( Movement_t ) );
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
path_interpolator_is_ready_for_next( void )
{
    bool slot_a_ready = ( planner.move_a.duration == 0 );
    bool slot_b_ready = ( planner.move_b.duration == 0 );
    return ( slot_a_ready || slot_b_ready );
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
    return ( planner.progress_percent >= 1.0f - FLT_EPSILON ) || planner.enable == false;
}

/* -------------------------------------------------------------------------- */

PRIVATE void
path_interpolator_calculate_percentage( uint16_t move_duration )
{
    MotionPlanner_t *me = &planner;

    // calculate current target completion based on time elapsed
    // time remaining is the allotted duration - time used (start to now), divide by the duration to get 0.0->1.0 progress
    uint32_t time_used = hal_systick_get_ms() - me->movement_started;

    if( move_duration )
    {
        me->progress_percent = (float)( time_used ) / move_duration;
    }
    else
    {
        me->progress_percent = 0;
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC CartesianPoint_t
path_interpolator_get_global_position( void )
{
    return planner.effector_position;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_start( void )
{
    // Request that the statemachine transitions to "ON"
    planner.enable = true;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_stop( void )
{
    MotionPlanner_t *me = &planner;

    // Request that the statemachine return to "OFF"
    me->enable = false;

    // Wipe out the moves currently loaded into the queue
    memset( &me->move_a, 0, sizeof( Movement_t ) );
    memset( &me->move_b, 0, sizeof( Movement_t ) );
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
            config_set_pathing_status( me->currentState );
            STATE_TRANSITION_TEST
            if( planner.enable )
            {
                if( me->move_a.duration )
                {
                    STATE_NEXT( PLANNER_EXECUTE_A );
                }
                else if( me->move_b.duration )
                {
                    STATE_NEXT( PLANNER_EXECUTE_B );
                }
            }
            STATE_EXIT_ACTION
            STATE_END
            break;

        case PLANNER_EXECUTE_A:
            STATE_ENTRY_ACTION
            config_set_pathing_status( me->currentState );
            path_interpolator_notify_pathing_started( me->move_a.identifier );

            path_interpolator_premove_transforms( &me->move_a );
            me->movement_started      = hal_systick_get_ms();
            me->movement_est_complete = me->movement_started + me->move_a.duration;
            me->progress_percent      = 0;
            STATE_TRANSITION_TEST
            path_interpolator_calculate_percentage( me->move_a.duration );

            if( !planner.enable || !me->move_a.duration )
            {
                STATE_NEXT( PLANNER_OFF );
            }
            else if( path_interpolator_get_move_done() )
            {
                if( me->move_b.duration )
                {
                    STATE_NEXT( PLANNER_EXECUTE_B );
                }
                else
                {
                    STATE_NEXT( PLANNER_OFF );
                }

                path_interpolator_notify_pathing_complete( me->move_a.identifier );
            }
            else
            {
                path_interpolator_execute_move( &me->move_a, me->progress_percent );
            }

            STATE_EXIT_ACTION
            memset( &me->move_a, 0, sizeof( Movement_t ) );
            STATE_END
            break;

        case PLANNER_EXECUTE_B:
            STATE_ENTRY_ACTION
            config_set_pathing_status( me->currentState );
            path_interpolator_notify_pathing_started( me->move_b.identifier );

            path_interpolator_premove_transforms( &me->move_b );
            me->movement_started      = hal_systick_get_ms();
            me->movement_est_complete = me->movement_started + me->move_b.duration;
            me->progress_percent      = 0;
            STATE_TRANSITION_TEST
            path_interpolator_calculate_percentage( me->move_b.duration );

            if( !planner.enable || !me->move_b.duration )
            {
                STATE_NEXT( PLANNER_OFF );
            }
            else if( path_interpolator_get_move_done() )
            {
                if( me->move_a.duration )
                {
                    STATE_NEXT( PLANNER_EXECUTE_A );
                }
                else
                {
                    STATE_NEXT( PLANNER_OFF );
                }

                path_interpolator_notify_pathing_complete( me->move_b.identifier );
            }
            else
            {
                path_interpolator_execute_move( &me->move_b, me->progress_percent );
            }

            STATE_EXIT_ACTION
            memset( &me->move_b, 0, sizeof( Movement_t ) );
            STATE_END
            break;
    }
}

PRIVATE void
path_interpolator_premove_transforms( Movement_t *move )
{
    //apply current position to a relative movement
    if( move->ref == _POS_RELATIVE )
    {
        for( uint8_t i = 0; i < move->num_pts; i++ )
        {
            move->points[i].x += planner.effector_position.x;
            move->points[i].y += planner.effector_position.y;
            move->points[i].z += planner.effector_position.z;
        }
    }

    // A transit move is from current position to point 1, so overwrite 0 with current position,
    // and then reuse a normal line movement
    if( move->type == _POINT_TRANSIT )
    {
        if( move->num_pts == 1 )
        {
            move->points[1].x = move->points[0].x;
            move->points[1].y = move->points[0].y;
            move->points[1].z = move->points[0].z;
            move->num_pts     = 2;
        }

        move->points[0].x = planner.effector_position.x;
        move->points[0].y = planner.effector_position.y;
        move->points[0].z = planner.effector_position.z;
    }
}

PRIVATE void
path_interpolator_execute_move( Movement_t *move, float percentage )
{
    CartesianPoint_t target       = { 0, 0, 0 };    //target position in cartesian space
    JointAngles_t    angle_target = { 0, 0, 0 };    //target motor shaft angle in degrees

    switch( move->type )
    {
        case _POINT_TRANSIT:
            cartesian_point_on_line( move->points, move->num_pts, percentage, &target );
            break;

        case _LINE:
            cartesian_point_on_line( move->points, move->num_pts, percentage, &target );
            break;

        case _CATMULL_SPLINE:
            cartesian_point_on_catmull_spline( move->points, move->num_pts, percentage, &target );
            break;

        case _BEZIER_QUADRATIC:
            cartesian_point_on_quadratic_bezier( move->points, move->num_pts, percentage, &target );
            break;

        case _BEZIER_CUBIC:
            cartesian_point_on_cubic_bezier( move->points, move->num_pts, percentage, &target );
            break;
        default:
            //TODO this should be considered a motion error

            break;
    }

    // Calculate a motor angle solution for the cartesian position
    kinematics_point_to_angle( target, &angle_target );

    // Ask the motors to please move there
    servo_set_target_angle_limited( _CLEARPATH_1, angle_target.a1 );
    servo_set_target_angle_limited( _CLEARPATH_2, angle_target.a2 );
    servo_set_target_angle_limited( _CLEARPATH_3, angle_target.a3 );

    // Update the config/UI data based on these actions
    config_set_position( target.x, target.y, target.z );
    memcpy( &planner.effector_position, &target, sizeof( CartesianPoint_t ) );
    config_set_movement_data( move->identifier, move->type, ( uint8_t )( percentage * 100 ) );
}

PRIVATE void
path_interpolator_notify_pathing_started( uint16_t move_id )
{
    BarrierSyncEvent *barrier_ev = EVENT_NEW( BarrierSyncEvent, PATHING_STARTED );
    uint16_t          publish_id = move_id;

    memcpy( &barrier_ev->id, &publish_id, sizeof( move_id ) );
    eventPublish( (StateEvent *)barrier_ev );
}

PRIVATE void
path_interpolator_notify_pathing_complete( uint16_t move_id )
{
    BarrierSyncEvent *barrier_ev = EVENT_NEW( BarrierSyncEvent, PATHING_COMPLETE );
    uint16_t          publish_id = move_id;

    memcpy( &barrier_ev->id, &publish_id, sizeof( move_id ) );
    eventPublish( (StateEvent *)barrier_ev );
}

/* ----- End ---------------------------------------------------------------- */
