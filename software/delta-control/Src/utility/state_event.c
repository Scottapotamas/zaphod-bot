/**
 * @file event.c
 *
 * @brief This file specified the standard state machine event types
 *        in a static declaration so we don't have to allocate them
 *        dynamically every time we need to use them. None of these
 *        event types have any parameters.
 *
 * @author  Marco Hess <marcoh@applidyne.com.au>
 *
 * @copyright (c) 2013-2015 Applidyne Australia Pty. Ltd. - All rights reserved.
 */

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */
#include "state_event.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//! Statically declare the standard signals as events with the
/// dynamic part 0 (i.e. poolId = 0 & useCount = 0)
/// Make sure that this is sync with the standard signal enum.

PUBLIC const StateEvent
stateEventReserved[] =
{
  {(Signal)STATE_EMPTY_SIGNAL,     { 0, 0 }},
  {(Signal)STATE_ENTRY_SIGNAL,     { 0, 0 }},
  {(Signal)STATE_EXIT_SIGNAL,      { 0, 0 }},
  {(Signal)STATE_INIT_SIGNAL,      { 0, 0 }},
  {(Signal)STATE_TIMEOUT1_SIGNAL,  { 0, 0 }},
  {(Signal)STATE_TIMEOUT2_SIGNAL,  { 0, 0 }},
  {(Signal)STATE_TIMEOUT3_SIGNAL,  { 0, 0 }},
  {(Signal)STATE_STEP1_SIGNAL,     { 0, 0 }},
  {(Signal)STATE_STEP2_SIGNAL,     { 0, 0 }},
  {(Signal)STATE_USER_SIGNAL,      { 0, 0 }}
};

/* ----- End ---------------------------------------------------------------- */
