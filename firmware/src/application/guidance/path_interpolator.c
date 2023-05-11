/* -------------------------------------------------------------------------- */

#include <float.h>
#include <string.h>

#include "path_interpolator.h"

#include "cartesian_helpers.h"
#include "cartesian_interpolator.h"
#include "movement_types.h"
#include "stopwatch.h"

#include "global.h"
#include "qassert.h"
#include "signals.h"

#include "FreeRTOS.h"
#include "queue.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE;

/* -------------------------------------------------------------------------- */

typedef struct
{
    QueueHandle_t xRequestQueue;

    Movement_t current_move;    // Store the currently executing move
    bool       move_ready;      // has the currently loaded move completed checks/transforms?

    uint32_t epoch_timestamp;   // Reference system time for move offset sequencing
    uint32_t movement_started;  // timestamp the start point
    float    progress_percent;  // calculated progress

    PositionRequestFn output_cb;    // target positions are emitted as arguments against this callback
} MotionPlanner_t;

PRIVATE MotionPlanner_t planner = { 0 };

PRIVATE Subject pathing_subject;

/* -------------------------------------------------------------------------- */

PRIVATE void path_interpolator_premove_transforms( Movement_t *move );

PRIVATE void path_interpolator_apply_rotation_offset( Movement_t *move );

PRIVATE void path_interpolator_calculate_percentage( uint16_t move_duration );

PRIVATE void path_interpolator_execute_move( Movement_t *move, float percentage );

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_init( void )
{
    MotionPlanner_t *me = &planner;
    memset( me, 0, sizeof( MotionPlanner_t ) );

    me->xRequestQueue = xQueueCreate( 5, sizeof( Movement_t ) );
    REQUIRE( me->xRequestQueue );
    vQueueAddToRegistry( me->xRequestQueue, "pathMoves" );    // Debug view annotation

    subject_init( &pathing_subject );
}

/* -------------------------------------------------------------------------- */

PUBLIC Subject *path_interpolator_get_subject( void )
{
    return &pathing_subject;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_update_output_callback( PositionRequestFn callback )
{
    MotionPlanner_t *me = &planner;
    me->output_cb       = callback;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_set_epoch_reference( uint32_t sync_timer )
{
    MotionPlanner_t *me = &planner;

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
    MotionPlanner_t *me = &planner;
    xQueueSendToBack( me->xRequestQueue, (void *)movement_to_process, (TickType_t)0 );

    //    return uxQueueSpacesAvailable( me->xRequestQueue );
}

/* -------------------------------------------------------------------------- */
// TODO: fold this behaviour into the add_request callback?
PUBLIC uint32_t path_interpolator_queue_ready( void )
{
    MotionPlanner_t *me = &planner;

    return uxQueueSpacesAvailable( me->xRequestQueue );
}

/* -------------------------------------------------------------------------- */

PUBLIC float
path_interpolator_get_progress( void )
{
    MotionPlanner_t *me = &planner;

    return me->progress_percent;
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
path_interpolator_get_move_done( void )
{
    MotionPlanner_t *me = &planner;

    return ( me->progress_percent >= 1.0f - FLT_EPSILON );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_task( void *arg )
{
    MotionPlanner_t *me = &planner;

    for( ;; )
    {
        // Fetch a pending move off the queue if nothing is already loaded
        // Block until something arrives
        if( me->current_move.duration == 0 )
        {
            xQueueReceive( me->xRequestQueue, &me->current_move, portMAX_DELAY );
        }

        // Prepare the movement for execution
        if( !me->move_ready )
        {
            // TODO: move these to cartesian_utility_lib?

            path_interpolator_apply_rotation_offset( &me->current_move );
            path_interpolator_premove_transforms( &me->current_move );

            // Check the planned move won't violate any rules
            // TODO: update with the 'configured max speed' value i.e. configuration_get_effector_speed_limit()
            if( cartesian_move_speed( &me->current_move ) > 300U )
            {
                // Overspeed
                EventData alert = { 0 };
                alert.stamped.timestamp = xTaskGetTickCount();
                subject_notify( &pathing_subject, FLAG_PLANNER_VIOLATION, alert );
            }

            // TODO: add a check that the move doesn't exit the working volume?

            me->move_ready = true;
        }

        // Execute the move
        if( me->move_ready )
        {
            // Sync has been provided if the epoch isn't null
            bool     epoch_valid  = ( me->epoch_timestamp );
            uint32_t current_time = xTaskGetTickCount();

            if( epoch_valid )
            {
                // Movements time themselves relative to the sync/epoch timestamp
                // therefore the 'wallclock' the global timestamp is:
                uint32_t start_timestamp = me->epoch_timestamp + me->current_move.sync_offset;

                // If we're early, the task should have a nap
                if( current_time < start_timestamp )
                {
                    uint32_t nap_duration = start_timestamp - current_time;    // todo: do we need to wake one tick early?
                    vTaskDelayUntil( &current_time, nap_duration );

                    current_time = xTaskGetTickCount(); // have to refresh the time after our nap.
                    // TODO: consider refactoring the nap check with the intent of reducing nesting/refreshing time?
                }

                if( !stopwatch_running( &me->movement_started ) )
                {
                    stopwatch_start( &me->movement_started );
                    // TODO: notify the system that a move has started
                }

                // Execute the move
                path_interpolator_calculate_percentage( me->current_move.duration );
                path_interpolator_execute_move( &me->current_move, me->progress_percent );

                // Move has completed, run cleanup
                if( path_interpolator_get_move_done() )
                {
                    // TODO: notify system that a move has completed

                    // Clear out the current state
                    memset( &me->current_move, 0, sizeof( Movement_t ) );
                    me->move_ready = false;

                    stopwatch_stop( &me->movement_started );
                    me->progress_percent = 0;
                    // Allow it to go back and pick another move off the queue
                }

            }    // epoch valid

            vTaskDelay( pdMS_TO_TICKS( 1 ) );
        }    // move execution

    }    // end infinite task loop
}

/* -------------------------------------------------------------------------- */

PRIVATE void
path_interpolator_premove_transforms( Movement_t *move )
{
    // TODO: refactor requried once kinematics module can provide position info
    CartesianPoint_t effector_position = { .x = 0, .y = 0, .z = 0 };    // effector_get_position();

    // apply current position to a relative movement
    if( (MotionReference_t)move->metadata.ref == _POS_RELATIVE )
    {
        for( uint32_t i = 0; i < move->metadata.num_pts; i++ )
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
            move->metadata.num_pts    = 2;
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
    float offset_deg = 0.0f;    // configuration_get_z_rotation();

    for( uint32_t i = 0; i < move->metadata.num_pts; i++ )
    {
        cartesian_point_rotate_around_z( &move->points[i], offset_deg );
    }
}

/* -------------------------------------------------------------------------- */

PRIVATE void
path_interpolator_calculate_percentage( uint16_t move_duration )
{
    MotionPlanner_t *me = &planner;

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

PRIVATE void
path_interpolator_execute_move( Movement_t *move, float percentage )
{
    REQUIRE( move->metadata.num_pts );

    CartesianPoint_t target = { 0, 0, 0 };    // target position in cartesian space
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
    if( planner.output_cb )
    {
        planner.output_cb( &target );
    }
}

/* -------------------------------------------------------------------------- */