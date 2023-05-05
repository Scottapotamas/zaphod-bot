/* -------------------------------------------------------------------------- */

#include <float.h>
#include <string.h>

#include "path_interpolator.h"

#include "cartesian_interpolator.h"
#include "cartesian_helpers.h"

#include "global.h"

#include "movement_types.h"
#include "stopwatch.h"
#include "qassert.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* -------------------------------------------------------------------------- */

typedef struct
{
    Movement_t  move_a;          // Slot A movement storage
    Movement_t  move_b;          // Slot B movement storage
    Movement_t *current_move;    // Points to move_a or move_b

    bool     enable;             // The planner is enabled
    uint32_t epoch_timestamp;    // Reference system time for move offset sequencing

    uint32_t movement_started;         // timestamp the start point
    uint32_t movement_est_complete;    // timestamp the predicted end point
    float    progress_percent;         // calculated progress
    PositionRequestFn output_cb;     // target positions are emitted as arguments against this callback
} MotionPlanner_t;

/* -------------------------------------------------------------------------- */

PRIVATE MotionPlanner_t planner[NUMBER_PATH_INTERPOLATORS] = { 0 };

PRIVATE void path_interpolator_set_next( PathInterpolatorInstance_t interpolator,
                                         Movement_t *movement_to_process );

PRIVATE void path_interpolator_calculate_percentage( PathInterpolatorInstance_t interpolator, uint16_t move_duration );

PRIVATE void path_interpolator_premove_transforms( Movement_t *move );
PRIVATE void path_interpolator_apply_rotation_offset( Movement_t *move );
PRIVATE void path_interpolator_execute_move( Movement_t *move, float percentage );

PRIVATE void path_interpolator_notify_pathing_started( uint32_t move_id );
PRIVATE void path_interpolator_notify_pathing_complete( uint32_t move_id );

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_init( PathInterpolatorInstance_t interpolator )
{
    REQUIRE( interpolator < NUMBER_PATH_INTERPOLATORS );
    MotionPlanner_t *me = &planner[interpolator];

    memset( me, 0, sizeof( MotionPlanner_t ) );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_update_output_callback( PositionRequestFn callback )
{
    MotionPlanner_t *me = &planner[PATH_INTERPOLATOR_DELTA];
    me->output_cb = callback;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_set_epoch_reference( PathInterpolatorInstance_t interpolator, uint32_t sync_timer )
{
    REQUIRE( interpolator < NUMBER_PATH_INTERPOLATORS );
    MotionPlanner_t *me = &planner[interpolator];

    if( sync_timer )
    {
        me->epoch_timestamp = sync_timer;
    }
}

/* -------------------------------------------------------------------------- */

// TODO: there's an implication that the path interpolator maintains it's own queue
//          if so, we need to be able to report occupancy, clear it etc from a higher level?

PUBLIC void path_interpolator_add_request( Movement_t *movement_to_process )
{
    path_interpolator_set_next( PATH_INTERPOLATOR_DELTA, movement_to_process );
}

// TODO: refactor/rethink how expansion motors work
//PUBLIC void path_interpolator_request_expansion( Movement_t *movement_to_process )
//{
//    path_interpolator_set_next( PATH_INTERPOLATOR_EXPANSION, movement_to_process );
//}

PRIVATE void path_interpolator_set_next( PathInterpolatorInstance_t interpolator,
                                         Movement_t *movement_to_process )
{
    REQUIRE( interpolator < NUMBER_PATH_INTERPOLATORS );
    REQUIRE( movement_to_process->metadata.num_pts );
    REQUIRE( movement_to_process->duration );

    MotionPlanner_t *me = &planner[interpolator];
    Movement_t      *movement_insert_slot = { 0 };    // Allows us to put the new move into whichever slot is available

    if( me->move_a.metadata.num_pts == 0 )
    {
        movement_insert_slot = &me->move_a;
    }
    else if( me->move_b.metadata.num_pts == 0 )
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

    return ( me->progress_percent >= 1.0f - FLT_EPSILON );
}

/* -------------------------------------------------------------------------- */

PRIVATE void
path_interpolator_calculate_percentage( PathInterpolatorInstance_t interpolator, uint16_t move_duration )
{
    REQUIRE( interpolator < NUMBER_PATH_INTERPOLATORS );
    MotionPlanner_t *me = &planner[interpolator];

    // calculate current target completion based on time elapsed
    // time remaining is the allotted duration - time used (start to now), divide by the duration to get 0.0->1.0 progress
    uint32_t time_used = stopwatch_lap( &me->movement_started );

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

    // Process the loop now
    path_interpolator_process( interpolator );

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

    // Immediately do cleanup etc
    // TODO: feels like 'expects external behaviour'
    //      re: working around the hooking/unhooking process loop not providing enough time for cleanup
    path_interpolator_process( interpolator );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_process_delta( void )
{
    path_interpolator_process( PATH_INTERPOLATOR_DELTA );
}


/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_process( PathInterpolatorInstance_t interpolator )
{
    REQUIRE( interpolator < NUMBER_PATH_INTERPOLATORS );
    MotionPlanner_t *me = &planner[interpolator];


        // TODO: First run behaviour
            // Pick the first slot with a valid move
            if( me->move_a.duration )
            {
                me->current_move = &me->move_a;
            }
            else if( me->move_b.duration )
            {
                me->current_move = &me->move_b;
            }

        // TODO: Loop behaviour
            // Calculate the time since the 'epoch' event
            uint32_t time_since_epoch_ms = stopwatch_lap( &me->epoch_timestamp );

            // Start the move once the move sync offset time matches the epoch + elapsed time
            // TODO deadline_pending syntax does the right thing, but is misleading as we're not running a deadline.
            //      refactor for clarity
            if( time_since_epoch_ms >= me->current_move->sync_offset && !stopwatch_deadline_pending( &me->movement_started ) )
            {
                // Start the move
                stopwatch_start( &me->movement_started );

                // TODO: this isn't used and can probably be removed?
                // stopwatch_deadline_start( &me->movement_est_complete, me->current_move->duration );

                me->progress_percent = 0;

                path_interpolator_notify_pathing_started( me->current_move->sync_offset );
                path_interpolator_apply_rotation_offset( me->current_move );
                path_interpolator_premove_transforms( me->current_move );

                mm_per_second_t speed = cartesian_move_speed( me->current_move );

                // TODO how will this be handled now?
                if( speed > 300U ) //configuration_get_effector_speed_limit() )
                {
//                    EmergencyStopEvent *estop_evt = EVENT_NEW( EmergencyStopEvent, MOTION_EMERGENCY );
//                    if( estop_evt )
//                    {
//                        estop_evt->cause = EMERGENCY_REQUEST_DENIED;
//                        eventPublish( (StateEvent *)estop_evt );
//                    }

//                    user_interface_report_error( "Requested illegal speed move" );
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

                    // TODO: exit/cleanup behaviour
//                    me->current_move     = 0;
//                    timer_ms_stop( &me->movement_started );

                    // Other move slot ready?

                    // Fall back into the handler's off state until new moves are loaded
                    if( !me->current_move->duration )
                    {
                        me->enable = false;
                    }
                }
            }

//    user_interface_set_pathing_status( me->currentState );
}

/* -------------------------------------------------------------------------- */

PRIVATE void
path_interpolator_premove_transforms( Movement_t *move )
{
    // TODO: refactor requried once kinematics module can provide position info
    CartesianPoint_t effector_position = { .x = 0, .y = 0, .z = 0 };//effector_get_position();

    // apply current position to a relative movement
    if( (MotionReference_t)move->metadata.ref == _POS_RELATIVE )
    {
        for( uint8_t i = 0; i < move->metadata.num_pts; i++ )
        {
            move->points[i].x += effector_position.x;
            move->points[i].y += effector_position.y;
            move->points[i].z += effector_position.z;
        }
    }

    // A transit move is from current position to point 1, so overwrite 0 with current position,
    // and then reuse a normal line movement
    if( (MotionAdjective_t)move->metadata.type == _POINT_TRANSIT )
    {
        if( move->metadata.num_pts == 1 )
        {
            move->points[_LINE_END].x = move->points[_LINE_START].x;
            move->points[_LINE_END].y = move->points[_LINE_START].y;
            move->points[_LINE_END].z = move->points[_LINE_START].z;
            move->metadata.num_pts = 2;
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
    REQUIRE( move->metadata.num_pts );

    // TODO: refactor required
    float offset_deg = 0.0f; //configuration_get_z_rotation();

    for( uint8_t i = 0; i < move->metadata.num_pts; i++ )
    {
        cartesian_point_rotate_around_z( &move->points[i], offset_deg );
    }
}

/* -------------------------------------------------------------------------- */

PRIVATE void
path_interpolator_execute_move( Movement_t *move, float percentage )
{
    REQUIRE( move->metadata.num_pts );

    CartesianPoint_t target   = { 0, 0, 0 };    // target position in cartesian space
//    MotionSolution_t solve_ok = SOLUTION_ERROR;

    switch( move->metadata.type )
    {
        case _POINT_TRANSIT:
            cartesian_point_on_line( move->points, move->metadata.num_pts, percentage, &target );
            break;

        case _LINE:
            cartesian_point_on_line( move->points, move->metadata.num_pts, percentage, &target );
            break;

        case _CATMULL_SPLINE:
            cartesian_point_on_catmull_spline( move->points, move->metadata.num_pts, percentage, &target );
            break;

        case _CATMULL_SPLINE_LINEARISED:
            percentage = cartesian_distance_linearisation_from_lut( move->sync_offset, percentage );
            cartesian_point_on_catmull_spline( move->points, move->metadata.num_pts, percentage, &target );
            break;

        case _BEZIER_QUADRATIC:
            cartesian_point_on_quadratic_bezier( move->points, move->metadata.num_pts, percentage, &target );
            break;

        case _BEZIER_QUADRATIC_LINEARISED:
            percentage = cartesian_distance_linearisation_from_lut( move->sync_offset, percentage );
            cartesian_point_on_quadratic_bezier( move->points, move->metadata.num_pts, percentage, &target );
            break;

        case _BEZIER_CUBIC:
            cartesian_point_on_cubic_bezier( move->points, move->metadata.num_pts, percentage, &target );
            break;

        case _BEZIER_CUBIC_LINEARISED:
            percentage = cartesian_distance_linearisation_from_lut( move->sync_offset, percentage );
            cartesian_point_on_cubic_bezier( move->points, move->metadata.num_pts, percentage, &target );
            break;

        default:
            ASSERT_PRINTF( false, "Invalid move type" );
            break;
    }

    // Provide our solved position to the user's callback
    if( planner[PATH_INTERPOLATOR_DELTA].output_cb )
    {
        planner[PATH_INTERPOLATOR_DELTA].output_cb( &target );
    }

    // Update the config/UI data based on this move
//    user_interface_set_movement_data( move->sync_offset, move->metadata.type, (uint8_t)( percentage * 100 ) );
}

/* -------------------------------------------------------------------------- */

// TODO: needs to be unique/tagged with which interpolator has started?
PRIVATE void
path_interpolator_notify_pathing_started( uint32_t move_id )
{
    // TODO: obsolete code removal required
//    SyncTimestampEvent *barrier_ev = EVENT_NEW( SyncTimestampEvent, PATHING_STARTED );
//    uint32_t            publish_id = move_id;
//
//    memcpy( &barrier_ev->epoch, &publish_id, sizeof( move_id ) );
//    eventPublish( (StateEvent *)barrier_ev );
}

/* -------------------------------------------------------------------------- */

// TODO: needs to be unique/tagged with which interpolator is done?
PRIVATE void
path_interpolator_notify_pathing_complete( uint32_t move_id )
{
//    SyncTimestampEvent *barrier_ev = EVENT_NEW( SyncTimestampEvent, PATHING_COMPLETE );
//    uint32_t            publish_id = move_id;
//
//    memcpy( &barrier_ev->epoch, &publish_id, sizeof( move_id ) );
//    eventPublish( (StateEvent *)barrier_ev );
}

/* ----- End ---------------------------------------------------------------- */
