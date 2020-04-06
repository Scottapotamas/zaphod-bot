/**
 * @file      simple_state_machine.h
 *
 * @brief     A very basic macro level state machine architecure that
 *            can be used with a simple switch statements. To use it there
 *            needs to be a couple of enums and a structure with the state
 *            machine private data. Like in:
 *
 *                typedef enum
 *                {
 *                   STATE_1,
 *                   STATE_2
 *                } MyStates_t;
 *
 *                typedef struct
 *                {
 *                    MyStates_t previousState;
 *                    MyStates_t currentState;
 *                    MyStates_t nextState;
 *                    uint32_t   some_var;
 *                } MyStateVars_t;
 *
 *                MyStateVars_t my_state_machine;
 *
 *            Then in the processing loop that handles the state machine:
 *
 *               MyStateVars_t *me = &my_state_machine;
 *
 *               switch( me->currentState )
 *               {
 *                   case STATE_1:
 *                       STATE_ENTRY_ACTION
 *                           some_var = 0;
 *                       STATE_TRANSITION_TEST
 *                           if( condition )
 *                           {
 *                               STATE_NEXT( STATE_1 );
 *                           }
 *                        STATE_EXIT_ACTION
 *                           some_var = 1;
 *                        STATE_END
 *                        break;
 *
 *                   case STATE_2:
 *                       STATE_ENTRY_ACTION
 *                            some_var = 2;
 *                        STATE_TRANSITION_TEST
 *                            if( condition )
 *                            {
 *                                STATE_NEXT( STATE_1 );
 *                            }
 *                        STATE_EXIT_ACTION
 *                            some_var = 3;
 *                        STATE_END
 *                        break;
 *            }
 *
 *
 * @author    Marco Hess <marcoh@applidyne.com.au>
 *
 * @copyright (c) 2017 Applidyne Australia Pty. Ltd. - All rights reserved.
 */

#ifndef SIMPLE_STATE_MACHINE_H
#define SIMPLE_STATE_MACHINE_H

/* -------------------------------------------------------------------------- */

/* ~~~ State Machine Handling Macros ~~~ */

#define STATE_INIT_INITIAL(s)  me->previousState = -1;                     \
                               me->currentState  = (s);                    \
                               me->nextState     = (s)

#define STATE_ENTRY_ACTION     if( me->currentState != me->previousState ) \
                               {

#define STATE_TRANSITION_TEST      me->previousState = me->currentState;   \
                               }                                           \
                               if( me->nextState == me->currentState )     \
                               {

#define STATE_EXIT_ACTION      }                                           \
                               if( me->nextState != me->currentState )     \
                               {

#define STATE_END                  me->currentState = me->nextState;       \
                               }

#define STATE_NEXT(s)          me->nextState = (s)

#define STATE_IS_TRANSITIONING ( ( me->previousState != me->currentState ) \
                               || ( me->currentState != me->nextState ) )


/* ----- End ---------------------------------------------------------------- */

#endif
