/**
 * @file hsm.h
 *
 * @brief Hierarchical State Machine Core Functions
 *
 * @author  Marco Hess <marcoh@applidyne.com.au>
 *
 * @copyright (c) 2013-2015 Applidyne Australia Pty. Ltd. - All rights reserved.
 */

#ifndef   STATE_HSM_H
#define   STATE_HSM_H

#ifdef    __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */
#include <stdint.h>
#include <stdbool.h>

/* ----- Local Includes ----------------------------------------------------- */
#include "global.h"
#include "state_event.h"

/* ----- Types -------------------------------------------------------------- */

struct  Hsm; // Forward declaration
typedef void (*PseudoState)( struct Hsm *me, const StateEvent *e );
typedef PseudoState (*State)( struct Hsm *me, const StateEvent *e );
typedef PseudoState STATE;  // return value from a state-handler

// Hierarchical State Machine base class
typedef struct Hsm Hsm;
struct Hsm
{
  State  activeState;   //<! Currently active state
  State  sourceState;   //<! Temporary source state during a transition
};

/* ----- Public Functions --------------------------------------------------- */

//! Enter the initial state of the state machine
PUBLIC void
hsmInit( Hsm *me, const StateEvent *e );

//! Run a single Run-To-Completion step for the state machine
PUBLIC void
hsmDispatch( Hsm *me, const StateEvent *e );

//! Return true when we are in the indicated state
PUBLIC bool
hsmIsIn( Hsm *me, State state );

/* ----------------------- Protected Functions ----------------------------- */

typedef struct Tran_ Tran_;
struct Tran_
{
  State    chain[8];    //<! Allow up to 8 levels deep nesting of states
  uint16_t actions;     //<! action mask (2-bits for action, allows for 8)
};

//! State Machine Constructor
PUBLIC Hsm *
hsmCtor( Hsm *me, PseudoState initial );

//! Top State where unhandled events gets send
PUBLIC STATE
hsmTop( Hsm *me, const StateEvent * );

//! Inline macro to get current state
#define HsmGetState_(me_) ( ((Hsm*)(me_))->activeState )

//! Handle a static state transition (one that can be pre-calculated
PUBLIC void
hsmStaticTransition( Hsm *me, Tran_ *t, State target );

//! Handle a dynamic state transition (one that can always runtime evaluated)
PUBLIC void
hsmDynamicTransition( Hsm *me, State target );

/* ----------------------- Macros ------------------------------------------ */

//! Method to transition the initial state
#define STATE_INIT(target_) (((Hsm*)me)->activeState = (State)(target_))

//! Method to transition to another state. This one uses a statically
/// allocated structure to 'remember' the route to take.
#define STATE_TRAN_STATIC(target_) do { \
      static Tran_ t_;           \
      hsmStaticTransition( (Hsm *)me, &t_, (State)(target_) );\
   } while(0)

//! Method to do a dynamic state transition. This one always evaluates
/// the actions to be performed.
#define STATE_TRAN_DYN(__target__) \
      hsmDynamicTransition( (Hsm *)me, (State)(__target__) )

//! Default operate in DYN mode to conserve RAM space.
#define STATE_TRAN(target_)     STATE_TRAN_DYN(target_)

/* ----------------------- End --------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* STATE_HSM_H */
