/* -------------------------------------------------------------------------- */

#include "overwatch.h"
#include "mode_mediator.h"

#include "simple_state_machine.h"
#include "signals.h"
#include "qassert.h"
#include "broker.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"

#include "request_handler.h"
#include "path_interpolator.h"
#include "point_follower.h"
#include "led_interpolator.h"
#include "effector.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE;

/* -------------------------------------------------------------------------- */

typedef enum
{
    OVERWATCH_DISARMED,
    OVERWATCH_ARMING,
    OVERWATCH_ARMED,
    OVERWATCH_DISARMING,
    OVERWATCH_EMERGENCY_STOP,
} OverwatchState_t;

typedef struct
{
    OverwatchState_t previousState;
    OverwatchState_t currentState;
    OverwatchState_t nextState;

    bool requested_arming;
    bool effector_home;

    bool servo_active[4];   // keep track of servo active/disarmed
    // TODO: this is actually disarmed/armed - there's no homing/error recovery consideration right now

} Overwatch_t;

PRIVATE TimerHandle_t xOverwatchStimulusTimer;

PRIVATE Overwatch_t supervisor_state = { 0 };

PRIVATE Subscriber *event_sub;

/* -------------------------------------------------------------------------- */

PRIVATE void overwatch_event_handler( void );

PRIVATE void overwatch_publish_main_state( void );

PRIVATE void overwatch_publish_mode_state( void );

PRIVATE void overwatch_timer_callback( TimerHandle_t xTimer );

PRIVATE void overwatch_estop_in( uint32_t timeout_ms );

PRIVATE void overwatch_estop_revoke_promise( void );

/* -------------------------------------------------------------------------- */

PUBLIC void overwatch_init( void )
{
//    memset( &supervisor_state, 0, sizeof( Overwatch_t ) );
    Overwatch_t *me = &supervisor_state;
    STATE_INIT_INITIAL(OVERWATCH_DISARMED);

    // Setup a stimulus timer handle
    xOverwatchStimulusTimer = xTimerCreate("overseer",
                                            pdMS_TO_TICKS(100),
                                            pdFALSE,
                                            0,
                                            overwatch_timer_callback
    );
    ENSURE( xOverwatchStimulusTimer );

    mode_mediator_init();

    // Setup subscriptions to events
    event_sub = broker_create_subscriber( "PSowatch", 10 );
    ENSURE( event_sub );

    // User requests
    broker_add_event_subscription( event_sub, FLAG_ARM );
    broker_add_event_subscription( event_sub, FLAG_DISARM );
    broker_add_event_subscription( event_sub, FLAG_ESTOP );
    broker_add_event_subscription( event_sub, FLAG_REHOME );
    broker_add_event_subscription( event_sub, FLAG_MODE_REQUEST );
    broker_add_event_subscription( event_sub, FLAG_SYNC_EPOCH );
    broker_add_event_subscription( event_sub, FLAG_REQUEST_QUEUE_CLEAR );

    // Subsystem state updates
    broker_add_event_subscription( event_sub, SERVO_STATE );
    broker_add_event_subscription( event_sub, EFFECTOR_NEAR_HOME );

    broker_add_event_subscription( event_sub, FLAG_EFFECTOR_VIOLATION );
    broker_add_event_subscription( event_sub, FLAG_PLANNER_VIOLATION );

    // TODO overwatch shouldn't be subscribing to position and pushing data downwards to pathing tasks
    broker_add_event_subscription( event_sub, EFFECTOR_POSITION );
}

/* -------------------------------------------------------------------------- */

PRIVATE void overwatch_event_handler( void )
{
    Overwatch_t *me = &supervisor_state;

    PublishedEvent event = { 0 };
    xQueueReceive( event_sub->queue, &event, portMAX_DELAY );

    switch( event.topic )
    {
        case FLAG_ARM:
            me->requested_arming = true;
            break;

        case FLAG_DISARM:
            me->requested_arming = false;
            break;

        // TODO: should effector/planner violations be handled differently from eSTOP?
        case FLAG_EFFECTOR_VIOLATION:
        case FLAG_PLANNER_VIOLATION:
        case FLAG_ESTOP:
            STATE_NEXT(OVERWATCH_EMERGENCY_STOP);
            break;

        case FLAG_REHOME:
            mode_mediator_request_rehome();
            break;

        case SERVO_STATE:   // A servo changed state...
            // TODO don't use hardcoded SERVO_STATE_ACTIVE value of 7 in state check
            me->servo_active[event.data.stamped.index] = ( event.data.stamped.value.u32 == 7);
            break;

        case FLAG_SYNC_EPOCH:
            // TODO: these tasks should catch the epoch themselves
            path_interpolator_set_epoch_reference( event.data.stamped.value.u32 );
            led_interpolator_set_epoch_reference( event.data.stamped.value.u32 );
            break;

        case FLAG_REQUEST_QUEUE_CLEAR:
            // TODO: these tasks should catch the clear themselves
            request_handler_clear( REQUEST_HANDLER_MOVES );
            request_handler_clear( REQUEST_HANDLER_FADES );
            path_interpolator_cleanup();
            led_interpolator_cleanup();
            break;

        case FLAG_MODE_REQUEST:     // UI requested a mode change
            mode_mediator_request_mode( event.data.stamped.value.u32 );
            break;

        case EFFECTOR_POSITION:
            // TODO: these tasks should catch position themselves?
            path_interpolator_update_effector_position( event.data.s_triple[EVT_X],
                                                        event.data.s_triple[EVT_Y],
                                                        event.data.s_triple[EVT_Z] );

            point_follower_update_effector_position( event.data.s_triple[EVT_X],
                                                     event.data.s_triple[EVT_Y],
                                                     event.data.s_triple[EVT_Z] );

            // Need to work out a 'not at home' event that's separate
            if( event.data.s_triple[EVT_X] != 0 || event.data.s_triple[EVT_Y] != 0 || event.data.s_triple[EVT_Z] != 0 )
            {
                me->effector_home = false;
                mode_mediator_set_is_homed(false );
            }
            break;

        case EFFECTOR_NEAR_HOME:
            // TODO: handle this behaviour in a cleaner manner?
            me->effector_home = true;
            mode_mediator_set_is_homed(true);
            break;

        default:
            ASSERT(false);
            break;
    }
}

PUBLIC void overwatch_task( void* arg )
{
    Overwatch_t *me = &supervisor_state;

    for(;;)
    {
        // Block while waiting for stimulus event
        overwatch_event_handler();

        // Run the state machine at least once per trigger, more if needed to handle transitions etc
        do {
            switch( me->currentState )
            {
                case OVERWATCH_DISARMED:
                    STATE_ENTRY_ACTION
                    mode_mediator_armed( false );

                    STATE_TRANSITION_TEST

                    if( me->requested_arming )
                    {
                        STATE_NEXT( OVERWATCH_ARMING );
                    }

                    STATE_EXIT_ACTION
                    mode_mediator_armed( true );

                    STATE_END
                    break;

                case OVERWATCH_ARMING:
                    STATE_ENTRY_ACTION
                    PublishedEvent temp = { .topic = OVERWATCH_SERVO_ENABLE,
                                            .data.stamped.timestamp = xTaskGetTickCount() };
                    broker_publish( &temp );

                    // A pending eSTOP event acts as a timeout
                    overwatch_estop_in( 9000 );
                    STATE_TRANSITION_TEST

                    // Are the motors/subsystems ready?
                    // TODO consider how to optionally handle the 4-th armed servo?
                    if( me->servo_active[0] && me->servo_active[1] && me->servo_active[2]  )
                    {
                        STATE_NEXT( OVERWATCH_ARMED );
                    }

                    if( !me->requested_arming )
                    {
                        STATE_NEXT( OVERWATCH_DISARMING );
                    }

                    STATE_EXIT_ACTION
                    overwatch_estop_revoke_promise();
                    STATE_END
                    break;

                case OVERWATCH_ARMED:
                    STATE_ENTRY_ACTION
                    effector_reset();

                    STATE_TRANSITION_TEST

                    if( !me->requested_arming )
                    {
                        STATE_NEXT( OVERWATCH_DISARMING );
                    }

                    if( !me->servo_active[0] || !me->servo_active[1] || !me->servo_active[2] )
                    {
                        STATE_NEXT( OVERWATCH_EMERGENCY_STOP );
                    }

                    STATE_EXIT_ACTION

                    STATE_END
                    break;

                case OVERWATCH_DISARMING:
                    STATE_ENTRY_ACTION
                    me->requested_arming = false;

                    // Ask the mechanism to go home
                    mode_mediator_request_rehome();

                    // A pending eSTOP event acts as a timeout
                    overwatch_estop_in( 2000 );
                    STATE_TRANSITION_TEST
                    // Are we home?
                    if( me->effector_home )
                    {
                        STATE_NEXT( OVERWATCH_EMERGENCY_STOP );
                    }

                    STATE_EXIT_ACTION
                    overwatch_estop_revoke_promise();

                    STATE_END
                    break;

                case OVERWATCH_EMERGENCY_STOP:
                    STATE_ENTRY_ACTION
                    me->requested_arming = false;

                    PublishedEvent temp = { .topic = OVERWATCH_SERVO_DISABLE, .data.stamped.timestamp = xTaskGetTickCount() };
                    broker_publish( &temp );

                    STATE_TRANSITION_TEST
                    // Are the motors/subsystems safe?
                    // TODO consider how to handle the optional 4-th servo?
                    if( !me->servo_active[0] && !me->servo_active[1] && !me->servo_active[2]  )
                    {
                        STATE_NEXT( OVERWATCH_DISARMED );
                    }
                    STATE_EXIT_ACTION
                    effector_reset();

                    STATE_END
                    break;

            }   // end state machine

        } while( STATE_IS_TRANSITIONING );

        // Run the sub-state machine which manages connections between tasks
        mode_mediator_task();

        // State machine handling is done, update the UI
        overwatch_publish_main_state();
        overwatch_publish_mode_state();


    }   // end task loop
}

/* -------------------------------------------------------------------------- */

PRIVATE void overwatch_publish_main_state( void )
{
    PublishedEvent sm_current = { 0 };
    sm_current.topic = OVERWATCH_STATE_UPDATE;
    sm_current.data.stamped.timestamp = xTaskGetTickCount();
    sm_current.data.stamped.value.u32 = supervisor_state.currentState;
    broker_publish( &sm_current );
}

/* -------------------------------------------------------------------------- */

PRIVATE void overwatch_publish_mode_state( void )
{
    // The next state is more accurate for notifications than the current state
    //   when disarmed, the mode state-machine isn't run and stays where it is, but the next
    //   state will be used as the transition when it activates, so show that to the user
    PublishedEvent mode_next = { 0 };
    mode_next.topic = OVERWATCH_MODE_UPDATE;
    mode_next.data.stamped.timestamp = xTaskGetTickCount();
    mode_next.data.stamped.value.u32 = mode_mediator_get_mode();
    broker_publish( &mode_next );
}

/* -------------------------------------------------------------------------- */

// Called by FreeRTOS timer task and used to stimulate the task.
// The task itself sets one-shot or repeating timers as needed.
PRIVATE void overwatch_timer_callback( TimerHandle_t xTimer )
{
    PublishedEvent temp = { .topic = FLAG_ESTOP };
    broker_publish( &temp );
}

PRIVATE void overwatch_estop_in( uint32_t timeout_ms )
{
    REQUIRE( xOverwatchStimulusTimer );
    xTimerChangePeriod( xOverwatchStimulusTimer, pdMS_TO_TICKS(timeout_ms), 1 );
    xTimerStart( xOverwatchStimulusTimer, 0 );
}

PRIVATE void overwatch_estop_revoke_promise( void )
{
    xTimerStop( xOverwatchStimulusTimer, 0 );
}

/* -------------------------------------------------------------------------- */