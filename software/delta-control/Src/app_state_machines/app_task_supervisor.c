/* ----- System Includes ---------------------------------------------------- */

#include <string.h>
#include <event_subscribe.h>

/* ----- Local Includes ----------------------------------------------------- */
#include "global.h"
#include "qassert.h"
#include "app_config.h"
#include "app_times.h"
#include "app_signals.h"
#include "app_events.h"
#include "app_version.h"

#include "app_task_supervisor.h"
#include "app_task_supervisor_private.h"

#include "status.h"
#include "buzzer.h"
#include "sensors.h"
#include "path_interpolator.h"
#include "configuration.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ----- Public Functions --------------------------------------------------- */

PUBLIC StateTask *
appTaskSupervisorCreate( AppTaskSupervisor *me,
                         StateEvent        *eventQueueData[ ],
                         const uint8_t     eventQueueSize )
{
    // Clear all task data
    memset( me, 0, sizeof(AppTaskSupervisor) );

    // Initialise State Machine State
    AppTaskSupervisorConstructor( me );

    /* Initialise State Machine Task */
    return stateTaskCreate( (StateTask*)me,
                            eventQueueData,
                            eventQueueSize,
                            0,
                            0 );
}

/* ----- Private Functions -------------------------------------------------- */


/* ----- Private Functions -------------------------------------------------- */

// State Machine Construction
PRIVATE void AppTaskSupervisorConstructor( AppTaskSupervisor *me )
{
    stateTaskCtor( &me->super, (State)&AppTaskSupervisor_initial );
}

/* -------------------------------------------------------------------------- */

// State Machine Initial State
PRIVATE void AppTaskSupervisor_initial( AppTaskSupervisor *me,
                                        const StateEvent *e __attribute__((__unused__)) )
{
	button_init( BUTTON_0, AppTaskSupervisorButtonEvent );
	button_init( BUTTON_1, AppTaskSupervisorButtonEvent );

    // Detect user activities
    eventSubscribe( (StateTask*)me, BUTTON_NORMAL_SIGNAL );
    eventSubscribe( (StateTask*)me, MECHANISM_START );
    eventSubscribe( (StateTask*)me, MECHANISM_STOP );
    eventSubscribe( (StateTask*)me, MECHANISM_REHOME );

    eventSubscribe( (StateTask*)me, MOVEMENT_REQUEST );

    // motion handler events
    eventSubscribe( (StateTask*)me, MOTION_ERROR );
    eventSubscribe( (StateTask*)me, MOTION_HOMED );
    eventSubscribe( (StateTask*)me, MOTION_DISABLED );

    // operation modes
    eventSubscribe( (StateTask*)me, MODE_TRACK );
    eventSubscribe( (StateTask*)me, MODE_DEMO );
    eventSubscribe( (StateTask*)me, MODE_EVENT );
    eventSubscribe( (StateTask*)me, MODE_MANUAL );

    eventSubscribe( (StateTask*)me, START_QUEUE_SYNC );

    eventSubscribe( (StateTask*)me, QUEUE_SYNC_MOTION_NEXT );
    eventSubscribe( (StateTask*)me, QUEUE_SYNC_LED_NEXT );

    STATE_INIT( &AppTaskSupervisor_main );
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskSupervisor_main( AppTaskSupervisor *me,
                                      const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
        {
        	config_set_main_state(1);

        	//start the board hardware sensors
        	sensors_enable();

            status_green(false);
            status_yellow(false);

        	return 0;
        }

        case BUTTON_NORMAL_SIGNAL:
		   switch( ((ButtonPressedEvent*)e)->id )
		   {
			   case BUTTON_0:
				   me->selected_control_mode = CONTROL_DEMO;
				   eventPublish( EVENT_NEW( StateEvent, MECHANISM_START ) );
				   return 0;

			   case BUTTON_1:
				   eventPublish( EVENT_NEW( StateEvent, MECHANISM_STOP ) );
				   return 0;
			   default:
				   break;
		   }
		   break;

        case STATE_INIT_SIGNAL:
            STATE_INIT( &AppTaskSupervisor_disarmed );
            return 0;

    }
    return (STATE)hsmTop;
}

/* -------------------------------------------------------------------------- */

// Start a mechanism startup

PRIVATE STATE AppTaskSupervisor_disarmed( AppTaskSupervisor *me,
                                         	 const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
        	config_set_main_state(2);
            config_set_control_mode( me->selected_control_mode );

            status_green(false);
            status_yellow(false);

        	return 0;

        case MECHANISM_START:
        	STATE_TRAN( AppTaskSupervisor_arm_start );
        	return 0;

        case MODE_TRACK:
        	me->selected_control_mode = CONTROL_TRACK;
            config_set_control_mode( me->selected_control_mode );
            return 0;

        case MODE_EVENT:
        	me->selected_control_mode = CONTROL_EVENT;
            config_set_control_mode( me->selected_control_mode );
            return 0;

        case MODE_MANUAL:
            me->selected_control_mode = CONTROL_MANUAL;
            config_set_control_mode( me->selected_control_mode );
            return 0;

        case MODE_DEMO:
        	me->selected_control_mode = CONTROL_DEMO;
            config_set_control_mode( me->selected_control_mode );
            return 0;

		case STATE_EXIT_SIGNAL:
            config_set_control_mode( me->selected_control_mode );
			return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}

/* -------------------------------------------------------------------------- */

// Start a mechanism startup

PRIVATE STATE AppTaskSupervisor_arm_start( AppTaskSupervisor *me,
                                         	 const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
        	config_set_main_state(3);

            status_yellow(true);

        	//request a motion handler homing process
            eventPublish( EVENT_NEW( StateEvent, MOTION_PREPARE ) );

        	// timeout incase the motion handler doesn't signal back
        	eventTimerStartEvery( &me->timer1,
                                 (StateTask* )me,
                                 (StateEvent* )&stateEventReserved[ STATE_TIMEOUT1_SIGNAL ],
                                 MS_TO_TICKS( 8000 ) );
        	//todo use declared servo failure time here
        	return 0;

        case STATE_TIMEOUT1_SIGNAL:
        	STATE_TRAN( AppTaskSupervisor_arm_error );
        	return 0;

        case MOTION_ERROR:
        	STATE_TRAN( AppTaskSupervisor_arm_error );
        	return 0;

        case MOTION_HOMED:
        	STATE_TRAN( AppTaskSupervisor_arm_success );
        	return 0;

        case MODE_TRACK:
        	me->selected_control_mode = CONTROL_TRACK;
            config_set_control_mode( me->selected_control_mode );
            return 0;

        case MODE_EVENT:
        	me->selected_control_mode = CONTROL_EVENT;
            config_set_control_mode( me->selected_control_mode );
            return 0;

        case MODE_MANUAL:
            me->selected_control_mode = CONTROL_MANUAL;
            config_set_control_mode( me->selected_control_mode );
            return 0;

        case MODE_DEMO:
        	me->selected_control_mode = CONTROL_DEMO;
            config_set_control_mode( me->selected_control_mode );
            return 0;

		case STATE_EXIT_SIGNAL:
            config_set_control_mode( me->selected_control_mode );
			eventTimerStopIfActive(&me->timer1);
			return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}

/* -------------------------------------------------------------------------- */

// Handle startup errors

PRIVATE STATE AppTaskSupervisor_arm_error( AppTaskSupervisor *me,
                                         	 const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
        	config_set_main_state(4);
            config_set_control_mode( me->selected_control_mode );
        	//cleanup and prepare for recovery

        	//send message to UI

        	//blink some lights?

            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );

        	return 0;

        case STATE_STEP1_SIGNAL:
        	STATE_TRAN( AppTaskSupervisor_disarmed );
            return 0;

        case MODE_TRACK:
            me->selected_control_mode = CONTROL_TRACK;
            return 0;

        case MODE_EVENT:
            me->selected_control_mode = CONTROL_EVENT;
            return 0;

        case MODE_MANUAL:
            me->selected_control_mode = CONTROL_MANUAL;
            return 0;

        case MODE_DEMO:
            me->selected_control_mode = CONTROL_DEMO;
            return 0;

		case STATE_EXIT_SIGNAL:

			return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}

/* -------------------------------------------------------------------------- */

// Motor arming succeeded

PRIVATE STATE AppTaskSupervisor_arm_success( AppTaskSupervisor *me,
                                         	 const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
        	// update state for UI
        	config_set_main_state(5);
            status_yellow(false);
            status_green(true);

        	//start additional subsystems here as required

            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );

        	return 0;

        case STATE_STEP1_SIGNAL:

			switch( me->selected_control_mode )
			{
				case CONTROL_DEMO:
					STATE_TRAN( AppTaskSupervisor_armed_demo );
					break;
				case CONTROL_TRACK:
					STATE_TRAN( AppTaskSupervisor_armed_track );
					break;

				case CONTROL_EVENT:
					STATE_TRAN( AppTaskSupervisor_armed_event );
					break;

			    case CONTROL_MANUAL:
			        STATE_TRAN( AppTaskSupervisor_armed_manual );
			        break;

				default:
					STATE_TRAN( AppTaskSupervisor_disarm_graceful );
					break;
			}

            return 0;

        case MOTION_ERROR:
        	STATE_TRAN( AppTaskSupervisor_disarm_graceful );
        	return 0;

        case MECHANISM_STOP:
        	STATE_TRAN( AppTaskSupervisor_disarm_graceful );
        	return 0;

        case STATE_EXIT_SIGNAL:
            config_set_control_mode( me->selected_control_mode );
			return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}

/* -------------------------------------------------------------------------- */

// Running Supervisor State

PRIVATE STATE AppTaskSupervisor_armed_event( AppTaskSupervisor *me,
                                         	 const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
        	config_set_main_state(6);
        	config_set_control_mode( CONTROL_EVENT );

        	//set up any recurring monitoring processes

        	return 0;

        case MECHANISM_STOP:
        	STATE_TRAN( AppTaskSupervisor_disarm_graceful );
        	return 0;

        case MOTION_ERROR:
        	STATE_TRAN( AppTaskSupervisor_arm_error );
        	return 0;

        case QUEUE_SYNC_MOTION_NEXT:
        {
            // Received the ID of the 'next' motion in the motion queue
            BarrierSyncEvent *inbound_sync = (BarrierSyncEvent*)e;
            me->movement_id = inbound_sync->id;

            BarrierSyncEvent *next_sync = EVENT_NEW( BarrierSyncEvent, START_QUEUE_SYNC );
            next_sync->id = inbound_sync->id;

            eventPublish( (StateEvent*)next_sync );

//            eventPublish( EVENT_NEW( StateEvent, START_QUEUE_SYNC ) );
            return 0;
        }

        case QUEUE_SYNC_LED_NEXT:
        {

            return 0;
        }

        case START_QUEUE_SYNC:
        {
            // Passed in the identifier which we are blocking up against
            BarrierSyncEvent *inbound_sync = (BarrierSyncEvent*)e;

            //get the current head queue id of the motion and led handlers
            if( inbound_sync )
            {
                me->movement_id = inbound_sync->id;
            }

            // Create sync start events for the motion and led tasks
            BarrierSyncEvent *motor_sync = EVENT_NEW( BarrierSyncEvent, MOTION_QUEUE_START_SYNC );
            BarrierSyncEvent *led_sync = EVENT_NEW( BarrierSyncEvent, LED_QUEUE_START_SYNC );

            motor_sync->id = me->movement_id;
            led_sync->id = me->movement_id;

            eventPublish( (StateEvent*)motor_sync );
            eventPublish( (StateEvent*)led_sync );

            return 0;
        }

        case MOVEMENT_REQUEST:
        {
			//todo work out how to just rename the event target and re-emit the same pointer

        	//catch the inbound movement event
			MotionPlannerEvent *mpe = (MotionPlannerEvent*)e;

			// Create event to pass event for motion handler
     	    MotionPlannerEvent *motion_request = EVENT_NEW( MotionPlannerEvent, MOTION_QUEUE_ADD );

			if(motion_request)
			{
				// copy the movement into the new event for the motion handler.
				memcpy(&motion_request->move, &mpe->move, sizeof(Movement_t));
				eventPublish( (StateEvent*)motion_request );
			}

        	return 0;
        }

        case MODE_TRACK:
        	me->selected_control_mode = CONTROL_TRACK;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
        	return 0;

        case MODE_MANUAL:
            me->selected_control_mode = CONTROL_MANUAL;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
            return 0;

        case MODE_DEMO:
        	me->selected_control_mode = CONTROL_DEMO;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
        	return 0;

		case STATE_EXIT_SIGNAL:

			return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}


PRIVATE STATE AppTaskSupervisor_armed_manual( AppTaskSupervisor *me,
                                             const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
            config_set_main_state(6);
            config_set_control_mode( CONTROL_MANUAL );

            //set up any recurring monitoring processes

            return 0;

        case MECHANISM_STOP:
            STATE_TRAN( AppTaskSupervisor_disarm_graceful );
            return 0;

        case MOTION_ERROR:
            STATE_TRAN( AppTaskSupervisor_arm_error );
            return 0;

        case MECHANISM_REHOME:
        {
            //request a move to 0,0,0
            MotionPlannerEvent *motev = EVENT_NEW( MotionPlannerEvent, MOTION_QUEUE_ADD );
            motev->move.type = _POINT_TRANSIT;
            motev->move.ref = _POS_ABSOLUTE;
            motev->move.duration = 1500;
            motev->move.num_pts = 1;

            motev->move.points[0].x = 0;
            motev->move.points[0].y = 0;
            motev->move.points[0].z = 0;
            eventPublish( (StateEvent*)motev );
        }
            return 0;

        case MOVEMENT_REQUEST:
        {
            //todo work out how to just rename the event target and re-emit the same pointer

            //catch the inbound movement event
            MotionPlannerEvent *mpe = (MotionPlannerEvent*)e;

            // Create event to pass event for motion handler
            MotionPlannerEvent *motion_request = EVENT_NEW( MotionPlannerEvent, MOTION_QUEUE_ADD );

            if(motion_request)
            {
                // copy the movement into the new event for the motion handler.
                memcpy(&motion_request->move, &mpe->move, sizeof(Movement_t));
                eventPublish( (StateEvent*)motion_request );
            }

            return 0;
        }

        case MODE_TRACK:
            me->selected_control_mode = CONTROL_TRACK;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
            return 0;

        case MODE_EVENT:
            me->selected_control_mode = CONTROL_EVENT;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
            return 0;

        case MODE_DEMO:
            me->selected_control_mode = CONTROL_DEMO;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
            return 0;

        case STATE_EXIT_SIGNAL:

            return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}

PRIVATE STATE AppTaskSupervisor_armed_track( AppTaskSupervisor *me,
                                         	 const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
        	config_set_main_state(6);
        	config_set_control_mode( CONTROL_TRACK );

        	//set up any recurring monitoring processes
        	eventTimerStartEvery( &me->timer1,
                                 (StateTask* )me,
                                 (StateEvent* )&stateEventReserved[ STATE_TIMEOUT1_SIGNAL ],
                                 MS_TO_TICKS( 100 ) );

        	return 0;

        case STATE_TIMEOUT1_SIGNAL:
        {
        	status_yellow_toggle();
        	CartesianPoint_t current = path_interpolator_get_global_position();
        	CartesianPoint_t target  = config_get_tracking_target();

        	bool x_deadband = CHECK_RANGE(current.x, target.x, MM_TO_MICRONS(0.5));
        	bool y_deadband = CHECK_RANGE(current.y, target.y, MM_TO_MICRONS(0.5));
        	bool z_deadband = CHECK_RANGE(current.z, target.z, MM_TO_MICRONS(0.5));

        	if( !x_deadband || !y_deadband || !z_deadband )
        	{
				MotionPlannerEvent *motev = EVENT_NEW( MotionPlannerEvent, MOTION_QUEUE_ADD );
				motev->move.type = _POINT_TRANSIT;
				motev->move.ref = _POS_ABSOLUTE;
				motev->move.duration = 90;
				motev->move.num_pts = 1;

				motev->move.points[0].x = target.x;
				motev->move.points[0].y = target.y;
				motev->move.points[0].z = target.z;
				eventPublish( (StateEvent*)motev );
        	}
			return 0;
        }

        case MECHANISM_STOP:
        	STATE_TRAN( AppTaskSupervisor_disarm_graceful );
        	return 0;

        case MOTION_ERROR:
        	STATE_TRAN( AppTaskSupervisor_arm_error );
        	return 0;

        case MECHANISM_REHOME:
        	{
	        	//request a move to 0,0,0
				MotionPlannerEvent *motev = EVENT_NEW( MotionPlannerEvent, MOTION_QUEUE_ADD );
				motev->move.type = _POINT_TRANSIT;
				motev->move.ref = _POS_ABSOLUTE;
				motev->move.duration = 1500;
				motev->move.num_pts = 1;

				motev->move.points[0].x = 0;
				motev->move.points[0].y = 0;
				motev->move.points[0].z = 0;
				eventPublish( (StateEvent*)motev );
        	}
			return 0;

        case MODE_DEMO:
        	me->selected_control_mode = CONTROL_DEMO;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
        	return 0;

        case MODE_EVENT:
        	me->selected_control_mode = CONTROL_EVENT;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
        	return 0;

        case MODE_MANUAL:
            me->selected_control_mode = CONTROL_MANUAL;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
            return 0;

		case STATE_EXIT_SIGNAL:
            eventTimerStopIfActive( &me->timer1 );
            status_yellow(false);
			return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}

PRIVATE STATE AppTaskSupervisor_armed_demo( AppTaskSupervisor *me,
                                         	 const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
        	config_set_main_state(6);
        	config_set_control_mode( CONTROL_DEMO );

        	//todo write demonstration programs and a way to run through a sequence of them

        	return 0;

        case MECHANISM_STOP:
        	STATE_TRAN( AppTaskSupervisor_disarm_graceful );
        	return 0;

        case MOTION_ERROR:
        	STATE_TRAN( AppTaskSupervisor_arm_error );
        	return 0;

        case MODE_TRACK:
        	me->selected_control_mode = CONTROL_TRACK;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
        	return 0;

        case MODE_MANUAL:
            me->selected_control_mode = CONTROL_MANUAL;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
            return 0;

        case MODE_EVENT:
        	me->selected_control_mode = CONTROL_EVENT;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
        	return 0;

		case STATE_EXIT_SIGNAL:

			return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}

PRIVATE STATE AppTaskSupervisor_armed_change_mode( AppTaskSupervisor *me,
                                         	 const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
        	config_set_main_state(6);
        	config_set_control_mode( CONTROL_CHANGING );

        	//empty out the queues
            eventPublish( EVENT_NEW( StateEvent, MOTION_QUEUE_CLEAR ) );
            eventPublish( EVENT_NEW( StateEvent, LED_CLEAR_QUEUE ) );

            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );

        	eventTimerStartEvery( &me->timer1,
                                 (StateTask* )me,
                                 (StateEvent* )&stateEventReserved[ STATE_TIMEOUT1_SIGNAL ],
                                 MS_TO_TICKS( 50 ) );
        	return 0;

        case STATE_STEP1_SIGNAL:
        {
        	CartesianPoint_t position = path_interpolator_get_global_position();

        	// Check to make sure the mechanism is near the home position before changing mode
        	if( 	position.x < MM_TO_MICRONS(0.1)
				&& 	position.y < MM_TO_MICRONS(0.1)
				&& 	position.z < MM_TO_MICRONS(0.1)
				&& 	path_interpolator_get_move_done() )
        	{
        		switch( me->selected_control_mode )
        		{
					case CONTROL_DEMO:
						STATE_TRAN( AppTaskSupervisor_armed_demo );
						break;
					case CONTROL_TRACK:
						STATE_TRAN( AppTaskSupervisor_armed_track );
						break;

					case CONTROL_EVENT:
						STATE_TRAN( AppTaskSupervisor_armed_event );
						break;

        		    case CONTROL_MANUAL:
                        STATE_TRAN( AppTaskSupervisor_armed_manual );
                        break;

                    default:
						STATE_TRAN( AppTaskSupervisor_disarm_graceful );
						break;
        		}
        	}
        	else
        	{
            	//request a move to 0,0,0
    			MotionPlannerEvent *motev = EVENT_NEW( MotionPlannerEvent, MOTION_QUEUE_ADD );
    			motev->move.type = _POINT_TRANSIT;
    			motev->move.ref = _POS_ABSOLUTE;
    			motev->move.duration = 800;
    			motev->move.num_pts = 1;
    			motev->move.points[0].x = 0;
    			motev->move.points[0].y = 0;
    			motev->move.points[0].z = 0;

    			eventPublish( (StateEvent*)motev );
        	}

            return 0;
        }

        case STATE_TIMEOUT1_SIGNAL:
        {
        	CartesianPoint_t position = path_interpolator_get_global_position();

        	// Check to make sure the mechanism is near the home position before changing mode
        	if( 	position.x < MM_TO_MICRONS(0.1)
				&& 	position.y < MM_TO_MICRONS(0.1)
				&& 	position.z < MM_TO_MICRONS(0.1)
				&& 	path_interpolator_get_move_done() )
        	{
        		switch( me->selected_control_mode )
        		{
					case CONTROL_DEMO:
						STATE_TRAN( AppTaskSupervisor_armed_demo );
						break;
					case CONTROL_TRACK:
						STATE_TRAN( AppTaskSupervisor_armed_track );
						break;

					case CONTROL_EVENT:
						STATE_TRAN( AppTaskSupervisor_armed_event );
						break;

                    case CONTROL_MANUAL:
                        STATE_TRAN( AppTaskSupervisor_armed_manual );
                        break;

					default:
						STATE_TRAN( AppTaskSupervisor_disarm_graceful );
						break;
        		}
        	}
			return 0;
        }

        case MODE_TRACK:
            me->selected_control_mode = CONTROL_TRACK;
            return 0;

        case MODE_EVENT:
            me->selected_control_mode = CONTROL_EVENT;
            return 0;

        case MODE_MANUAL:
            me->selected_control_mode = CONTROL_MANUAL;
            return 0;

        case MODE_DEMO:
            me->selected_control_mode = CONTROL_DEMO;
            return 0;

        case MECHANISM_STOP:
        	STATE_TRAN( AppTaskSupervisor_disarm_graceful );
        	return 0;

        case MOTION_ERROR:
        	STATE_TRAN( AppTaskSupervisor_arm_error );
        	return 0;

		case STATE_EXIT_SIGNAL:
            config_set_control_mode( me->selected_control_mode );
            eventTimerStopIfActive( &me->timer1 );
			return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}

/* -------------------------------------------------------------------------- */

// Start a graceful shutdown process

PRIVATE STATE AppTaskSupervisor_disarm_graceful( AppTaskSupervisor *me,
                                         	 const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
        	config_set_main_state(7);

            config_set_control_mode( me->selected_control_mode );

            //empty out the queues
            eventPublish( EVENT_NEW( StateEvent, MOTION_QUEUE_CLEAR ) );
            eventPublish( EVENT_NEW( StateEvent, LED_CLEAR_QUEUE ) );

            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );

        	//come back and check the position until we are home
        	eventTimerStartEvery( &me->timer1,
                                 (StateTask* )me,
                                 (StateEvent* )&stateEventReserved[ STATE_TIMEOUT1_SIGNAL ],
                                 MS_TO_TICKS( 150 ) );

        	eventTimerStartOnce( &me->timer2,
                                 (StateTask* )me,
                                 (StateEvent* )&stateEventReserved[ STATE_TIMEOUT2_SIGNAL ],
                                 MS_TO_TICKS( 5000 ) );
        	return 0;

        case STATE_STEP1_SIGNAL:
        {
        	//request a move to 0,0,0
			MotionPlannerEvent *motev = EVENT_NEW( MotionPlannerEvent, MOTION_QUEUE_ADD );

			//transit to starting position
			motev->move.type = _POINT_TRANSIT;
			motev->move.ref = _POS_ABSOLUTE;
			motev->move.duration = 1500;
			motev->move.num_pts = 1;

			motev->move.points[0].x = 0;
			motev->move.points[0].y = 0;
			motev->move.points[0].z = 0;

			eventPublish( (StateEvent*)motev );

            return 0;
        }

        case STATE_TIMEOUT1_SIGNAL:
        {
        	// get global position
        	CartesianPoint_t position = path_interpolator_get_global_position();

        	// Check to make sure the mechanism is at the home position before disabling servo power
        	// Allow a few microns error on position in check
        	if( 	position.x < MM_TO_MICRONS(0.1)
				&& 	position.y < MM_TO_MICRONS(0.1)
				&& 	position.z < MM_TO_MICRONS(0.1)
				&& 	path_interpolator_get_move_done() )
        	{
            	eventPublish( EVENT_NEW( StateEvent, MOTION_EMERGENCY ) );

        	}

        	//todo consider adding a second timeout to e-stop if it doesn't get home in time?

			return 0;
        }

        case STATE_TIMEOUT2_SIGNAL:
        {
        	STATE_TRAN( AppTaskSupervisor_arm_error );
        	return 0;
        }

        case MOTION_DISABLED:
        	STATE_TRAN( AppTaskSupervisor_disarmed );
        	return 0;

		case STATE_EXIT_SIGNAL:
            config_set_control_mode( me->selected_control_mode );
            eventTimerStopIfActive(&me->timer1);
            eventTimerStopIfActive(&me->timer2);
			return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}

/* -------------------------------------------------------------------------- */

/*
 * Button handling publishes event
 */

PRIVATE void AppTaskSupervisorButtonEvent( ButtonId_t button,
                                          ButtonPressType_t press_type )
{
	if(press_type == BUTTON_PRESS_TYPE_NORMAL)
	{
	    ButtonEvent *be = EVENT_NEW( ButtonEvent, BUTTON_NORMAL_SIGNAL );
	    if( be )
	    {
	        be->id = button;
	        be->press_type = press_type;
	        eventPublish( (StateEvent*)be );
	    }
	}

}
/* ----- End ---------------------------------------------------------------- */
