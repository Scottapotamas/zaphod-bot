/**
 * @file    bitset.h
 *
 * @brief   This module provides support for setting and clearing bits in a
 *          32bit variable to be used as flags. Flags are numbered 1 to 32 and
 *          when bitsetHighest returns 0 none of the flags are set.
 *
 * @author  Marco Hess <marcoh@applidyne.com.au>
 *
 * @copyright (c) 2013-2018 Applidyne Australia Pty. Ltd. - All rights reserved.
 */

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "bitset.h"

/* ----------------------- Private Data ------------------------------------- */

/** Log 2 lookup table to determine the highest set bit from a byte value. */
PRIVATE const uint8_t bitsetLookup[] =
{
    0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
};

/* ----- Public Functions --------------------------------------------------- */

PUBLIC uint8_t
bitsetHighest( const BitSet_t * bitPattern )
{
    register uint8_t bitNumber = 0;

    if( *bitPattern )
    {
        if( *bitPattern & 0xFF000000 )
        {
            bitNumber = (uint8_t)(bitsetLookup[*bitPattern >> 24] + 24U);
        }
        else if( *bitPattern & 0xFF0000 )
        {
            bitNumber = (uint8_t)(bitsetLookup[*bitPattern >> 16] + 16U);
        }
        else if( *bitPattern & 0xFF00 )
        {
            bitNumber = (uint8_t)(bitsetLookup[*bitPattern >> 8] + 8U);
        }
        else
        {
            bitNumber = (uint8_t)bitsetLookup[*bitPattern & 0xFF];
        }
    }
    return bitNumber;
}

/* ----- End ---------------------------------------------------------------- */
