/**
 * @file      debounce.h
 *
 * @brief     Generic digital input debouncer. Uses a number of samples and
 *            the result is following the input state when the number of samples
 *            all agree on the new state.
 *
 * @author    Marco Hess <marcoh@applidyne.com.au>
 *
 * @copyright (c) 2015 Applidyne Australia Pty. Ltd. - All rights reserved.
 */

/* ----- Local Includes ----------------------------------------------------- */

#include "debounce.h"

/* ----------------------- Start implementation ----------------------------- */

PUBLIC uint8_t
debounce( DebounceStatus_t * status, uint8_t sample )
{
    uint8_t andSamples;
    uint8_t orSamples;
    uint8_t j;

    /* Circular buffer of samples */
    status->samples[status->index++] = sample;
    if( status->index >= DEBOUNCE_NUM_SAMPLES )
    {
        status->index = 0;
    }

    /* Filter samples */
    andSamples = status->samples[0];
    orSamples  = status->samples[0];
    for( j = 1; j < DEBOUNCE_NUM_SAMPLES; j++ )
    {
        andSamples &= status->samples[j];
        orSamples  |= status->samples[j];
    }

    status->debounced = (status->debounced & orSamples ) | andSamples;

    return (status->debounced);
}

/* ----- End ---------------------------------------------------------------- */
