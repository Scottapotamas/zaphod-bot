/**
 * @file      debounce.h
 *
 * @brief     Generic digital inpout debouncer. Uses a number of samples and
 *            the result is following the input state when the number of samples
 *            all agree on the new state.
 *
 * @author    Marco Hess <marcoh@applidyne.com.au>
 *
 * @copyright (c) 2015 Applidyne Australia Pty. Ltd. - All rights reserved.
 */

#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#ifdef  __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

#include <stdint.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* ----- Defines ------------------------------------------------------------ */

/** Number of samples that have to agree before the debounced output follows */
#define  DEBOUNCE_NUM_SAMPLES   3

/* ----- Types -------------------------------------------------------------- */

typedef struct
{
    uint8_t index;                          /** Circular buffer index */
    uint8_t debounced;                      /** Last debounced result */
    uint8_t samples[DEBOUNCE_NUM_SAMPLES];  /** Number of samples to hold */
} DebounceStatus_t;

/* -------------------------------------------------------------------------- */

/**
 * @brief      Digital input debouncer. Enforces the output state follows
 *             the input state when all samples agree to the same state.
 *             Simply needs to be called regularly with the current input
 *             state.
 * @param[in]  status    Structure with history of debounce status
 * @param[in]  sample    Up to 8 parallel digital input bits being sampled.
 * @return     Debounced digital input bits.
 */

PUBLIC uint8_t
debounce( DebounceStatus_t * status, uint8_t sample );

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* DEBOUNCE_H */

