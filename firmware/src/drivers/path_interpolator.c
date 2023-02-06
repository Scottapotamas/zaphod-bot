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

#include "effector.h"
#include "motion_types.h"
#include "user_interface.h"
#include "configuration.h"
#include "app_times.h"
#include "timer_ms.h"
#include "qassert.h"

/* ----- Defines ------------------------------------------------------------ */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

typedef enum
{
    PLANNER_OFF,
    PLANNER_WAIT_AND_EXECUTE,
} PlanningState_t;

typedef struct
{
    PlanningState_t previousState;
    PlanningState_t currentState;
    PlanningState_t nextState;

    Movement_t  move_a;          // Slot A movement storage
    Movement_t  move_b;          // Slot B movement storage
    Movement_t *current_move;    // Points to move_a or move_b

    bool       enable;             // The planner is enabled
    timer_ms_t epoch_timestamp;    // Reference system time for move offset sequencing

    timer_ms_t movement_started;         // timestamp the start point
    timer_ms_t movement_est_complete;    // timestamp the predicted end point
    float      progress_percent;         // calculated progress

} MotionPlanner_t;

/* ----- Private Variables -------------------------------------------------- */

PRIVATE MotionPlanner_t planner[NUMBER_PATH_INTERPOLATORS] = { 0 };

PRIVATE void path_interpolator_calculate_percentage( PathInterpolatorInstance_t interpolator, uint16_t move_duration );

PRIVATE void path_interpolator_premove_transforms( Movement_t *move );
PRIVATE void path_interpolator_apply_rotation_offset( Movement_t *move );
PRIVATE void path_interpolator_execute_move( Movement_t *move, float percentage );

PRIVATE void path_interpolator_notify_pathing_started( uint32_t move_id );
PRIVATE void path_interpolator_notify_pathing_complete( uint32_t move_id );

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
path_interpolator_init( PathInterpolatorInstance_t interpolator )
{
    REQUIRE( interpolator < NUMBER_PATH_INTERPOLATORS );
    MotionPlanner_t *me = &planner[interpolator];

    memset( me, 0, sizeof( MotionPlanner_t ) );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_set_epoch_reference( PathInterpolatorInstance_t interpolator, timer_ms_t sync_timer )
{
    REQUIRE( interpolator < NUMBER_PATH_INTERPOLATORS );
    MotionPlanner_t *me = &planner[interpolator];

    if( sync_timer )
    {
        me->epoch_timestamp = sync_timer;
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_set_next( PathInterpolatorInstance_t interpolator, Movement_t *movement_to_process )
{
    REQUIRE( interpolator < NUMBER_PATH_INTERPOLATORS );
    REQUIRE( movement_to_process->duration );

    MotionPlanner_t *me = &planner[interpolator];
    Movement_t      *movement_insert_slot = { 0 };    // Allows us to put the new move into whichever slot is available

    if( me->move_a.duration == 0 )
    {
        movement_insert_slot = &me->move_a;
    }
    else if( me->move_b.duration == 0 )
    {
        movement_insert_slot = &me->move_b;
    }

    if( movement_insert_slot != NULL )
    {
        memcpy( movement_insert_slot, movement_to_process, sizeof( Movement_t ) );
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
path_interpolator_is_ready_for_next( PathInterpolatorInstance_t interpolator )
{
    REQUIRE( interpolator < NUMBER_PATH_INTERPOLATORS );
    MotionPlanner_t *me = &planner[interpolator];

    bool slot_a_ready = ( me->move_a.duration == 0 );
    bool slot_b_ready = ( me->move_b.duration == 0 );
    return ( slot_a_ready || slot_b_ready );
}

/* -------------------------------------------------------------------------- */

PUBLIC float
path_interpolator_get_progress( PathInterpolatorInstance_t interpolator )
{
    REQUIRE( interpolator < NUMBER_PATH_INTERPOLATORS );
    MotionPlanner_t *me = &planner[interpolator];

    return me->progress_percent;
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
path_interpolator_get_move_done( PathInterpolatorInstance_t interpolator )
{
    REQUIRE( interpolator < NUMBER_PATH_INTERPOLATORS );
    MotionPlanner_t *me = &planner[interpolator];

    return ( me->progress_percent >= 1.0f - FLT_EPSILON ) || me->currentState == PLANNER_OFF;
}

/* -------------------------------------------------------------------------- */

PRIVATE void
path_interpolator_calculate_percentage( PathInterpolatorInstance_t interpolator, uint16_t move_duration )
{
    REQUIRE( interpolator < NUMBER_PATH_INTERPOLATORS );
    MotionPlanner_t *me = &planner[interpolator];

    // calculate current target completion based on time elapsed
    // time remaining is the allotted duration - time used (start to now), divide by the duration to get 0.0->1.0 progress
    uint32_t time_used = timer_ms_stopwatch_lap( &me->movement_started );

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

PUBLIC void
path_interpolator_start( PathInterpolatorInstance_t interpolator )
{
    REQUIRE( interpolator < NUMBER_PATH_INTERPOLATORS );
    MotionPlanner_t *me = &planner[interpolator];

    // Request that the state-machine transitions to "ON"
    me->enable = true;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_stop( PathInterpolatorInstance_t interpolator )
{
    REQUIRE( interpolator < NUMBER_PATH_INTERPOLATORS );
    MotionPlanner_t *me = &planner[interpolator];

    // Request that the state-machine return to "OFF"
    me->enable = false;

    // Wipe out the moves currently loaded into the queue
    memset( &me->move_a, 0, sizeof( Movement_t ) );
    memset( &me->move_b, 0, sizeof( Movement_t ) );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_process_delta( void )
{
    path_interpolator_process( PATH_INTERPOLATOR_DELTA );

}

PUBLIC void
path_interpolator_process_expansion( void )
{
    path_interpolator_process( PATH_INTERPOLATOR_EXPANSION );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_process( PathInterpolatorInstance_t interpolator )
{
    REQUIRE( interpolator < NUMBER_PATH_INTERPOLATORS );
    MotionPlanner_t *me = &planner[interpolator];

    if( !me->enable )
    {
        STATE_NEXT( PLANNER_OFF );
    }

    switch( me->currentState )
    {
        case PLANNER_OFF:
            STATE_ENTRY_ACTION

            STATE_TRANSITION_TEST
            if( me->enable )
            {
                // At least one slot has a loaded move?
                if( me->move_a.duration || me->move_b.duration )
                {
                    STATE_NEXT( PLANNER_WAIT_AND_EXECUTE );
                }
            }
            STATE_EXIT_ACTION
            STATE_END
            break;

        case PLANNER_WAIT_AND_EXECUTE:
            STATE_ENTRY_ACTION
            // Pick the first slot with a valid move
            if( me->move_a.duration )
            {
                me->current_move = &me->move_a;
            }
            else if( me->move_b.duration )
            {
                me->current_move = &me->move_b;
            }

            STATE_TRANSITION_TEST
            // Calculate the time since the 'epoch' event
            uint32_t time_since_epoch_ms = timer_ms_stopwatch_lap( &me->epoch_timestamp );

            // Start the move once the move sync offset time matches the epoch + elapsed time
            if( time_since_epoch_ms >= me->current_move->sync_offset && !timer_ms_is_running( &me->movement_started ) )
            {
                // Start the move
                timer_ms_stopwatch_start( &me->movement_started );
                timer_ms_start( &me->movement_est_complete, me->current_move->duration );
                me->progress_percent = 0;

                path_interpolator_notify_pathing_started( me->current_move->sync_offset );
                path_interpolator_apply_rotation_offset( me->current_move );
                path_interpolator_premove_transforms( me->current_move );

                mm_per_second_t speed = cartesian_move_speed( me->current_move );

                if( speed > configuration_get_effector_speed_limit() )
                {
                    // TODO do something other than just E-STOP on the overspeed move
                    eventPublish( EVENT_NEW( StateEvent, MOTION_EMERGENCY ) );

                    user_interface_report_error( "Requested illegal speed move" );
                }

            }

            if( time_since_epoch_ms > me->current_move->sync_offset )
            {
                // Continue the move
                path_interpolator_calculate_percentage( interpolator, me->current_move->duration );
                path_interpolator_execute_move( me->current_move, me->progress_percent );

                // Check if the move is done
                if( path_interpolator_get_move_done( interpolator ) )
                {
                    path_interpolator_notify_pathing_complete( me->current_move->sync_offset );

                    // Clear out the current state
                    memset( me->current_move, 0, sizeof( Movement_t ) );

                    // Update the pointer to the other slot
                    if( me->current_move == &me->move_a )
                    {
                        me->current_move = &me->move_b;
                    }
                    else
                    {
                        me->current_move = &me->move_a;
                    }

                    timer_ms_stop( &me->movement_started );

                    // Other move slot ready?

                    // Fall back into the handler's off state until new moves are loaded
                    if( !me->current_move->duration )
                    {
                        me->enable = false;
                        STATE_NEXT( PLANNER_OFF );
                    }
                }
            }
            STATE_EXIT_ACTION
            me->current_move     = 0;
            me->progress_percent = 0;
            timer_ms_stop( &me->movement_started );
            timer_ms_stop( &me->movement_est_complete );
            STATE_END
            break;
    }

    user_interface_set_pathing_status( me->currentState );
}

/* -------------------------------------------------------------------------- */

PRIVATE void
path_interpolator_premove_transforms( Movement_t *move )
{
    CartesianPoint_t effector_position = effector_get_position();

    // apply current position to a relative movement
    if( move->ref == _POS_RELATIVE )
    {
        for( uint8_t i = 0; i < move->num_pts; i++ )
        {
            move->points[i].x += effector_position.x;
            move->points[i].y += effector_position.y;
            move->points[i].z += effector_position.z;
        }
    }

    // A transit move is from current position to point 1, so overwrite 0 with current position,
    // and then reuse a normal line movement
    if( move->type == _POINT_TRANSIT )
    {
        if( move->num_pts == 1 )
        {
            move->points[_LINE_END].x = move->points[_LINE_START].x;
            move->points[_LINE_END].y = move->points[_LINE_START].y;
            move->points[_LINE_END].z = move->points[_LINE_START].z;
            move->num_pts = 2;
        }

        move->points[_LINE_START].x = effector_position.x;
        move->points[_LINE_START].y = effector_position.y;
        move->points[_LINE_START].z = effector_position.z;
    }
}

/* -------------------------------------------------------------------------- */

PRIVATE void
path_interpolator_apply_rotation_offset( Movement_t *move )
{
    float offset_deg = configuration_get_z_rotation();

    for( uint8_t i = 0; i < move->num_pts; i++ )
    {
        cartesian_point_rotate_around_z( &move->points[i], offset_deg );
    }
}

/* -------------------------------------------------------------------------- */

PRIVATE void
path_interpolator_execute_move( Movement_t *move, float percentage )
{
    CartesianPoint_t target   = { 0, 0, 0 };    // target position in cartesian space
    MotionSolution_t solve_ok = false;

    switch( move->type )
    {
        case _POINT_TRANSIT:
            solve_ok = cartesian_point_on_line( move->points, move->num_pts, percentage, &target );
            break;

        case _LINE:
            solve_ok = cartesian_point_on_line( move->points, move->num_pts, percentage, &target );
            break;

        case _CATMULL_SPLINE:
            solve_ok = cartesian_point_on_catmull_spline( move->points, move->num_pts, percentage, &target );
            break;

        case _CATMULL_SPLINE_LINEARISED:
            percentage = cartesian_distance_linearisation_from_lut( move->sync_offset, percentage );
            solve_ok = cartesian_point_on_catmull_spline( move->points, move->num_pts, percentage, &target );
            break;

        case _BEZIER_QUADRATIC:
            solve_ok = cartesian_point_on_quadratic_bezier( move->points, move->num_pts, percentage, &target );
            break;

        case _BEZIER_QUADRATIC_LINEARISED:
            percentage = cartesian_distance_linearisation_from_lut( move->sync_offset, percentage );
            solve_ok = cartesian_point_on_quadratic_bezier( move->points, move->num_pts, percentage, &target );
            break;

        case _BEZIER_CUBIC:
            solve_ok = cartesian_point_on_cubic_bezier( move->points, move->num_pts, percentage, &target );
            break;

        case _BEZIER_CUBIC_LINEARISED:
            percentage = cartesian_distance_linearisation_from_lut( move->sync_offset, percentage );
            solve_ok = cartesian_point_on_cubic_bezier( move->points, move->num_pts, percentage, &target );
            break;

        default:
            // TODO this should be considered a motion error
            break;
    }

    // TODO: work out how to extract the effector solution from expansion solution
    effector_request_target( &target );

    // Update the config/UI data based on this move
    user_interface_set_movement_data( move->sync_offset, move->type, (uint8_t)( percentage * 100 ) );
}

/* -------------------------------------------------------------------------- */

// TODO: needs to be unique/tagged with which interpolator has started?
PRIVATE void
path_interpolator_notify_pathing_started( uint32_t move_id )
{
    SyncTimestampEvent *barrier_ev = EVENT_NEW( SyncTimestampEvent, PATHING_STARTED );
    uint32_t            publish_id = move_id;

    memcpy( &barrier_ev->epoch, &publish_id, sizeof( move_id ) );
    eventPublish( (StateEvent *)barrier_ev );
}

/* -------------------------------------------------------------------------- */

// TODO: needs to be unique/tagged with which interpolator is done?
PRIVATE void
path_interpolator_notify_pathing_complete( uint32_t move_id )
{
    SyncTimestampEvent *barrier_ev = EVENT_NEW( SyncTimestampEvent, PATHING_COMPLETE );
    uint32_t            publish_id = move_id;

    memcpy( &barrier_ev->epoch, &publish_id, sizeof( move_id ) );
    eventPublish( (StateEvent *)barrier_ev );
}

/* ----- End ---------------------------------------------------------------- */
