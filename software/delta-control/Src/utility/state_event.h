/**
 * @file event.h
 *
 * @brief
 * This header file declares the 'Event' type for use by by the task
 * and state machine processing modules.
 * In addition this module also declares the standard types for
 * use in the state machines and also statically allocates the storage
 * for the events corresponding to these standard signals.
 *
 * @author  Marco Hess <marcoh@applidyne.com.au>
 *
 * @copyright (c) 2013-2015 Applidyne Australia Pty. Ltd. - All rights reserved.
 */

#ifndef STATE_EVENT_H
#define STATE_EVENT_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */
#include <stdint.h>
#include <stdbool.h>

/* ----- Local Includes ----------------------------------------------------- */
#include "global.h"

/* ----------------------- Typedefs -----------------------------------------*/

//! Signal only has to be 8-bits which allows for up to 255 different
/// signals to be specified.
typedef uint8_t Signal;

/* -------------------------------------------------------------------------- */

/** The Dynamic part of an event structure keeps track of the memory pool
 *  from which the event was allocated and also allows some housekeeping
 *  info for the event subscription module. */
typedef struct Dynamic Dynamic;
struct Dynamic
{
    unsigned poolId:2;      ///< Pool number from which event was allocated
                            ///< (allows up to 3 pools to be used)
    unsigned useCount:6;    ///< Number of times the event was already propagated
                            ///< (up to the number of tasks)
};

/* -------------------------------------------------------------------------- */

//! Base Event stucture. Additional application specific events can be
/// declared with this struct declared as the first member named super
/// like in:
///
/// typedef struct HostAckEvent
/// {
///   Event      super;
///   int8u      ack_type;
/// } HostAckEvent;
///
/// Also with a range of Event structures defined like this allocate memory
/// pools with the size of the most common used structures (including the
/// largest one.

typedef struct StateEvent StateEvent;
struct StateEvent
{
  Signal  signal;         //!< signal of the event instance
  Dynamic dynamic;        //!< event house keeping info
                          //!< (static/dynamic/reference count)
};

/* -------------------------------------------------------------------------- */

//! Standard signals Definitions - Remember when you modify these enums
/// that you also modify the declaration of stateEventReserved[]!!!!

enum
{
  STATE_EMPTY_SIGNAL = 0, //!< internal pseudo signal for traversing state tree
  STATE_ENTRY_SIGNAL,     //!< signal for coding state entry actions
  STATE_EXIT_SIGNAL,      //!< signal for coding state exit actions
  STATE_INIT_SIGNAL,      //!< signal for coding state initial entry
  STATE_TIMEOUT1_SIGNAL,  //!< signal used by time events
  STATE_TIMEOUT2_SIGNAL,  //!< signal used by time events
  STATE_TIMEOUT3_SIGNAL,  //!< signal used by time events
  STATE_STEP1_SIGNAL,     //!< signal used to transfer control within a state
  STATE_STEP2_SIGNAL,     //!< signal used to transfer control within a state
  STATE_USER_SIGNAL       //!< first signal that can be used in user applications
};

/* -------------------------------------------------------------------------- */

//! Event memory for standard signals so they don't need to be
/// dynamically allocated.
extern const StateEvent stateEventReserved[];

/* ----- End ---------------------------------------------------------------- */

#ifdef    __cplusplus
}
#endif
#endif // STATE_EVENT_H


