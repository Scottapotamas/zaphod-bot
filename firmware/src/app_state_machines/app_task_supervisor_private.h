#ifndef APP_TASK_SUPERVISOR_PRIVATE_H
#define APP_TASK_SUPERVISOR_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- Private State Functions Declarations ------------------------------- */

PRIVATE void AppTaskSupervisorConstructor( AppTaskSupervisor *me );

PRIVATE void AppTaskSupervisor_initial( AppTaskSupervisor *me, const StateEvent *e );

PRIVATE STATE AppTaskSupervisor_main( AppTaskSupervisor *me, const StateEvent *e );

PRIVATE STATE AppTaskSupervisor_disarmed( AppTaskSupervisor *me, const StateEvent *e );

PRIVATE STATE AppTaskSupervisor_arm_start( AppTaskSupervisor *me, const StateEvent *e );

PRIVATE STATE AppTaskSupervisor_arm_error( AppTaskSupervisor *me, const StateEvent *e );

PRIVATE STATE AppTaskSupervisor_arm_success( AppTaskSupervisor *me, const StateEvent *e );

PRIVATE STATE AppTaskSupervisor_armed_event( AppTaskSupervisor *me, const StateEvent *e );

PRIVATE STATE AppTaskSupervisor_armed_manual( AppTaskSupervisor *me, const StateEvent *e );

PRIVATE STATE AppTaskSupervisor_armed_track( AppTaskSupervisor *me, const StateEvent *e );

PRIVATE STATE AppTaskSupervisor_armed_demo( AppTaskSupervisor *me, const StateEvent *e );

PRIVATE STATE AppTaskSupervisor_armed_change_mode( AppTaskSupervisor *me, const StateEvent *e );

PRIVATE STATE AppTaskSupervisor_disarm_graceful( AppTaskSupervisor *me, const StateEvent *e );

PRIVATE void AppTaskSupervisorProcessModeRequest( AppTaskSupervisor *me );
PRIVATE void AppTaskSupervisorPublishRehomeEvent( void );
PRIVATE void AppTaskSupervisorButtonEvent( ButtonId_t button, ButtonPressType_t press_type );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* APP_TASK_SUPERVISOR_PRIVATE_H */
