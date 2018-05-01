#ifndef APP_TASK_SUPERVISOR_PRIVATE_H
#define APP_TASK_SUPERVISOR_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- Private State Functions Declarations ------------------------------- */

PRIVATE void AppTaskSupervisorConstructor( AppTaskSupervisor *me );
PRIVATE void AppTaskSupervisor_initial( AppTaskSupervisor *me,
                                        const StateEvent *e );

PRIVATE STATE AppTaskSupervisor_main( AppTaskSupervisor *me,
                                      const StateEvent *e );

PRIVATE STATE AppTaskSupervisor_load_config( AppTaskSupervisor *me,
                                      const StateEvent *e );

PRIVATE void AppTaskSupervisorButtonEvent( ButtonId_t button,
                                          ButtonPressType_t press_type );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* APP_TASK_SUPERVISOR_PRIVATE_H */
