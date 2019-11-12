/**
 * @file hsm.c
 *
 * @brief Hierarchical State Machine Core Functions
 *
 * @author  Marco Hess <marcoh@applidyne.com.au>
 *
 * @copyright (c) 2013-2015 Applidyne Australia Pty. Ltd. - All rights reserved.
 */

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "state_hsm.h"
#include "qassert.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ----------------------- Private Functions Declarations ------------------ */

PRIVATE void
hsmStaticTransitionSetup( Hsm *me, Tran_ *t, State target );

/* ----------------------- Private Macros ---------------------------------- */

// Helps to execute a state with one of the reserved events
#define TRIGGER(state_, signal_) \
  (State)(*(state_))(me, &stateEventReserved[signal_])

/* ----- Public Functions --------------------------------------------------- */

PUBLIC Hsm *
hsmCtor( Hsm *me, PseudoState initial )
{
  me->activeState = hsmTop;
  me->sourceState = (State)initial;
  return me;
}

/* -------------------------------------------------------------------------- */

PUBLIC STATE
hsmTop( Hsm *me        __attribute__((unused)),
        const StateEvent *e __attribute__((unused)) )
{
  return 0;  // At the top of the chain we return 0 for every event
             // so with the STATE_EMPTY_SIGNAL we can see if we are at the top
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

PUBLIC void
hsmInit( Hsm *me, const StateEvent *e )
{
  register State tmpState;

  REQUIRE( me->activeState == (State)hsmTop && // HSM not yet executed
           me->sourceState != 0);              // Ensure we have a valid source

  /// Save me->activeState in a temporary
  tmpState = me->activeState;

  /// Run the initial transition
  (*(PseudoState)me->sourceState)( me, e );

  /// The initial transition must go to a state that still
  /// references back to HsmTop. If not we got a bad state machine.
  ASSERT( tmpState == TRIGGER( me->activeState, STATE_EMPTY_SIGNAL ) );

  /// update the temporary
  tmpState = me->activeState;

  /// enter the state
  (*tmpState)( me, &stateEventReserved[STATE_ENTRY_SIGNAL] );

  /// Keep going with the INIT until the state tells us the
  /// event was handled.
  while( (*tmpState)(me, &stateEventReserved[STATE_INIT_SIGNAL]) == 0 )
  {
    /// Initial transition must go one level deep
    /// If you hit this assert, you may have a handler for the
    /// STATE_INIT_SIGNAL that returns 0 but does not actually
    /// transfer into a new state ?
    ASSERT( tmpState == TRIGGER( me->activeState, STATE_EMPTY_SIGNAL ) );

    tmpState = me->activeState;

    // enter the substate
    (*tmpState)( me, &stateEventReserved[STATE_ENTRY_SIGNAL] );
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

//! Return true when we are within the ancestor hierarchy of the
/// in the indicated state
PUBLIC bool
hsmIsIn( Hsm *me, State state )
{
  register State tmpState;

  /// Traverse the state hierarchy from the current
  /// state to the top most ancestor
  for( tmpState = me->activeState;
       tmpState != 0;
       tmpState = TRIGGER( tmpState, STATE_EMPTY_SIGNAL ) )
  {
    /// do the states match?
    if( tmpState == state )
    {
      return true;
    }
  }

  /// No match found, return false
  return false;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

//! Execute a static transition. Where possible using the statically
/// allocated memory of transition actions.
PUBLIC void
hsmStaticTransition( Hsm *me, Tran_ *tran, State target )
{
  register State s;

  /// Validate that we don't transition to the top state
  REQUIRE( target != (State)hsmTop );

  /// A state change can be triggered from a different (i.e. parent)
  /// state than the current active state (i.e. when the event responsible
  /// for the state change was not handled in the active state. We call
  /// this the sourceState and we first exit the activeState all the way
  /// up to the sourceState.
  for( s = me->activeState; s != me->sourceState; )
  {
    State t;

     /// Make sure that tmp state is valid
    ASSERT( s != 0 );

    /// Trigger the EXIT signal and try to find the super state.
    t = TRIGGER( s, STATE_EXIT_SIGNAL );

    /// When the TRIGGER returns 0, it means that the EXIT event was
    /// triggered and we need to use a new trigger with the EMPTY signal
    /// to find the ancestor of this state. If the EXIT was not triggered
    /// we already have the ancestor of this state.
    if( t != 0 )
    {
      s = t;
    }
    else
    {
      s = TRIGGER( s, STATE_EMPTY_SIGNAL );
    }
  }

  /// Check if have a transaction record for the static state change.
  /// If not go and record it.

  if( tran->actions == 0 ) // tran object initialized?
  {
    // No, setup the transition
    hsmStaticTransitionSetup( me, tran, target );
  }
  else
  {  // Yes, execute transition chain
    register State *c = &tran->chain[0];
    register uint16_t action;
    for( action = tran->actions >> 1;
         action != 0;
         action >>= 2, ++c)
    {
      (*(*c))(me, &stateEventReserved[ action & 3 ]);
    }
    me->activeState = *c;
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

PUBLIC void
hsmDispatch( Hsm *me, const StateEvent *e )
{
  /// Execute the event in the current state and keep
  /// passing the event up the ancestor hierachy chain
  /// until the event is handled (i.e. returns 0) or
  /// me->sourceState becomes 0 because we reached the
  /// top state HsmTop (which returns 0 for everything)
  me->sourceState = me->activeState;
  do
  {
    me->sourceState = (State)(*me->sourceState)( me, e );
  } while( me->sourceState != 0 );
}

/* ----------------------- Private Functions ------------------------------- */

PRIVATE void
hsmStaticTransitionSetup( Hsm *me, Tran_ *tran, State target )
{
  State    statesToEnter[ DIM(tran->chain) ];
  State    targetParent;
  State    sourceParent;
  State    s;
  State    *c;
  State    *e;
  State    *lca;
  uint16_t a = 0;

  #define RECORD(state_, sig_)                   \
    if( TRIGGER(state_, sig_) == 0 )             \
    {                                            \
      a = (uint16_t)((a >> 2) | ((sig_) << 14)); \
      *c++ = (state_);                           \
    } else ((void)0)

  c = &tran->chain[0];
  *(e = &statesToEnter[0]) = 0;

  /// assume entry to target
  *(++e) = target;

  /// ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
  /// Check 1: Are we transitioning to the same state?
  ///          i.e. source == target

  if( me->sourceState == target )
  {
    // exit source
    RECORD( me->sourceState, STATE_EXIT_SIGNAL );
    goto StaticInLCA;
  }

  /// ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
  /// Check 2: Are we transitioning to a child of this state?
  ///          i.e. source == target->super

  targetParent = TRIGGER( target, STATE_EMPTY_SIGNAL );

  if( me->sourceState == targetParent )
  {
    goto StaticInLCA;
  }

  /// ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
  /// Check 3: Are we transitioning to another child of same parent?
  ///          i.e. source->super == target->super

  sourceParent = TRIGGER( me->sourceState, STATE_EMPTY_SIGNAL );

  if( sourceParent == targetParent )
  {
    // exit source
    RECORD( me->sourceState, STATE_EXIT_SIGNAL );
    goto StaticInLCA;
  }

  /// ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
  /// Check 4: Are we transitionining up to the parent of this state?
  ///          i.e. source->super == target

  if( sourceParent == target )
  {
    /// Exit the source state as we are moving to the parent
    RECORD( me->sourceState, STATE_EXIT_SIGNAL );

    /// The parent state remains active, so it does not need
    /// to be entered again i.e. we are reverting the assumption
    /// to enter the target.
    --e;
    goto StaticInLCA;
  }

  /// ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
  /// Check 5: Are we transitioning down to some substate of this same branch?
  ///          i.e. source == target->super->super... hierarchy
  ///          check this by traversing the tree from the target parent upwards
  ///          until we find the source.

  *(++e) = targetParent;
  for( s = TRIGGER( targetParent, STATE_EMPTY_SIGNAL );
       s != 0;
       s = TRIGGER( s, STATE_EMPTY_SIGNAL ))
  {
    if( me->sourceState == s )
    {
      goto StaticInLCA;
    }
    *(++e) = s;
  }

  /// exit source
  RECORD( me->sourceState, STATE_EXIT_SIGNAL );

  /// ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
  /// Check 6: Check rest of source->super == target->super->super...

  for( lca = e; *lca != 0; --lca )
  {
    if( sourceParent == *lca )
    {
      // do not enter the LCA
      e = lca - 1;
      goto StaticInLCA;
    }
  }

  /// ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
  /// Check 7: Check each me->sourceState->super->super..for each target...

  for( s = sourceParent; s; s = TRIGGER( s, STATE_EMPTY_SIGNAL ) )
  {
    for( lca = e; *lca != 0; --lca )
    {
      if( s == *lca )
      {
        // do not enter the LCA
        e = lca - 1;
        goto StaticInLCA;
      }
    }
    // exit source
    RECORD( s, STATE_EXIT_SIGNAL );
  }

  // ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-

  /// if we reach this we have a bad state machine
  ASSERT(0);

  // ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-

StaticInLCA: // now we are in the LCA of me->sourceState and target
  ASSERT( e < &statesToEnter[ sizeof(statesToEnter)/sizeof(*statesToEnter) ] );

  while( ( s = *e-- ) != 0 )
  {                        /* retrace entry path in reverse order */
    RECORD(s, STATE_ENTRY_SIGNAL);                       /* enter s */
  }

  /// Update current state
  me->activeState = target;

  while( TRIGGER(target, STATE_INIT_SIGNAL) == 0 )
  {
    /// initial transition must go one level deep
    ASSERT( target == TRIGGER( me->activeState, STATE_EMPTY_SIGNAL ) );
    a = (uint16_t)((a >> 2) | (STATE_INIT_SIGNAL << 14));
    *c++ = target;
    target = me->activeState;
    RECORD( target, STATE_ENTRY_SIGNAL ); // enter target
  }

  #undef RECORD

  *c = target;
  tran->actions = (uint16_t)((a >> (15U - (c - &tran->chain[0])*2)) | 0x1);

  ENSURE( c < &tran->chain[DIM(tran->chain)] );  // chain fits in
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

//! Handle a dynamic state transition
PUBLIC void
hsmDynamicTransition( Hsm *me, State target )
{
  State statesToEnter[7];
  State *currentStateToEnter;
  State targetParent;
  State sourceParent;
  State s;
  State *lca;

  /// Validate that we don't transition to the top state
  REQUIRE( target != (State)hsmTop );

  /// A state change can be triggered from a different (i.e. parent)
  /// state than the current active state (i.e. when the event responsible
  /// for the state change was not handled in the active state. We call
  /// this the sourceState and we first exit the activeState all the way
  /// up to the sourceState.
  for( s = me->activeState; s != me->sourceState; )
  {
    State t;

    /// Make sure that tmp state is valid
    ASSERT( s != 0 );

    /// Trigger the EXIT signal and try to find the super state.
    t = TRIGGER( s, STATE_EXIT_SIGNAL );

    /// When the TRIGGER returns 0, it means that the EXIT event was
    /// triggered and we need to use a new trigger with the EMPTY signal
    /// to find the ancestor of this state. If the EXIT was not triggered
    /// we already have the ancestor of this state.
    if( t != 0 )
    {
      s = t;
    }
    else
    {
      s = TRIGGER( s, STATE_EMPTY_SIGNAL );
    }
  }
  //currentStateToEnter = &statesToEnter[0];
  //*currentStateToEnter = 0; // Setup last in state to enter.
  *(currentStateToEnter = &statesToEnter[0]) = 0;

  /// Final state that (we assume) needs to be entered.
  /// Some of the checks below may reverse this assumption
  ///*(++currentStateToEnter) = target;
  *(++currentStateToEnter) = target;

  /// ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
  /// Check 1: Are we transitioning to the same state?
  ///          i.e. source == target

  if( me->sourceState == target )
  {
    // For transitions to the same state, we
    // still execute the exit-entry signals
    (void)TRIGGER( me->sourceState, STATE_EXIT_SIGNAL );
    goto DynInLCA;
  }

  /// ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
  /// Check 2: Are we transitioning to a child of this state?
  ///          i.e. source == target->super

  targetParent = TRIGGER( target, STATE_EMPTY_SIGNAL );

  if( me->sourceState == targetParent )
  {
    /// No need to exit the source state as we are decending to a child
    goto DynInLCA;
  }

  /// ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
  /// Check 3: Are we transitioning to another child of same parent?
  ///          i.e. source->super == target->super

  sourceParent = TRIGGER( me->sourceState, STATE_EMPTY_SIGNAL );

  if( sourceParent == targetParent )
  {
    /// Exit the source state
    (void)TRIGGER( me->sourceState, STATE_EXIT_SIGNAL );
    goto DynInLCA;
  }

  /// ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
  /// Check 4: Are we transitioning to the parent of this state?
  ///          i.e. source->super == target

  if( sourceParent == target )
  {
    /// Exit the source state as we are moving to the parent
    (void)TRIGGER( me->sourceState, STATE_EXIT_SIGNAL );

    /// The parent state remains active, so it is not to be entered again
    //--currentStateToEnter;
    --currentStateToEnter;
    goto DynInLCA;
  }

  // ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
  /// Check 5: Are we transitioning down to some substate of this same branch?
  ///          i.e. source == target->super->super... hierarchy
  ///          check this by traversing the tree from the target parent upwards
  ///          until we find the source.

  *(++currentStateToEnter) = targetParent;
  for( s = TRIGGER( targetParent, STATE_EMPTY_SIGNAL );
       s != 0;
       s = TRIGGER( s, STATE_EMPTY_SIGNAL ) )
  {
    if( me->sourceState == s )
    {
      goto DynInLCA;
    }

    //*(++currentStateToEnter) = s;
    *(++currentStateToEnter) = s;
  }

  // ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-

  /// We are definitly not in the same banch of states, so we are
  /// now sure that the sourceState needs to be exited
  (void)TRIGGER( me->sourceState, STATE_EXIT_SIGNAL );

  /// ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
  /// Check 6: Are we transitioning down to some substate of an adjacent branch?...
  ///          i.e. source->super == target->super->super...

  for( lca = currentStateToEnter;
       *lca != 0;
       --lca )
  {
    if( sourceParent == *lca )
    {
      currentStateToEnter = lca - 1;
      goto DynInLCA;
    }
  }

  /// ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
  /// Check 7: Are we transition over multiple branches and levels...
  ///          i.e. each source->super->super..for each target...

  for( s = sourceParent;
       s;
       s = TRIGGER( s, STATE_EMPTY_SIGNAL ) )
  {
    for( lca = currentStateToEnter ;
         *lca != 0;
         --lca )
    {
      if( s == *lca )
      {
        currentStateToEnter = lca - 1;
        goto DynInLCA;
      }
    }
    (void)TRIGGER( s, STATE_EXIT_SIGNAL );                            /* exit s */
  }

  /// ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
  /// When reach this, we could not find a path between the source and the target,
  /// something is very wrong with the state machine (i.e. misconfigured)
  ASSERT(0);

  /// ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
  /// We are now in the lowest common ancestor of the source and target states
DynInLCA:

  /// Ensure that the number of transitions has not overflowed the
  /// room we reserved for this.
  /// ASSERT( currentStateToEnter < &statesToEnter[sizeof(statesToEnter)/sizeof(*statesToEnter)] );
  ASSERT( currentStateToEnter < &statesToEnter[sizeof(statesToEnter)/sizeof(*statesToEnter)] );

  /// currentStateToEnter is now pointing to the most top state that needs entering
  /// so we go in reverse and trigger the ENTRY signal of these states until
  /// we hit the 0 that we put in the first entry of statesToEnter.

  while( ( s = *currentStateToEnter-- ) != 0 )
  {
    (void)TRIGGER( s, STATE_ENTRY_SIGNAL );
  }

  /// Update the current state to our new target state
  me->activeState = target;

  /// In this new state, check if it triggers the initialisation
  /// of another state. If so, follow that path until it is no longer
  /// handled (i.e. we reached the final target of the STATE_INIT).

  while( TRIGGER( target, STATE_INIT_SIGNAL ) == 0 )
  {
    /// Ensure that the transition was only one level deep
    ASSERT( target == TRIGGER( me->activeState, STATE_EMPTY_SIGNAL ) );

    /// Update target to the new active state
    /// (activeState is updated in the STATE_INIT macro call).
    target = me->activeState;

    /// Ensure we also trigger the ENTRY event for this new state.
    (void)TRIGGER( target, STATE_ENTRY_SIGNAL );
  }
}

/* ----- End ---------------------------------------------------------------- */
