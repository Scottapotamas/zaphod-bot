/**
 * @file      fifo.h
 *
 * @ingroup   utility
 *
 * @brief     Basic buffer based FIFO capability.
 *
 * @author    Marco Hess <marcoh@applidyne.com.au>
 *
 * @copyright (c) 2015 Applidyne Australia Pty. Ltd. - All rights reserved.
 */

#ifndef FIFO_H
#define FIFO_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* ----- Types -------------------------------------------------------------- */

typedef struct
{
     uint8_t * buf;
     uint32_t  head;
     uint32_t  tail;
     uint32_t  capacity;
} fifo_t;

/* ----- Public Functions --------------------------------------------------- */

/** This initializes the FIFO structure with the given buffer and size */

PUBLIC void
fifo_init( fifo_t * restrict f, uint8_t * buf, uint32_t buf_size );

/* -------------------------------------------------------------------------- */

/** Returns the capacity of the fifo. */

PUBLIC uint32_t
fifo_size( fifo_t * restrict f );

/* -------------------------------------------------------------------------- */

/** Returns the amount of data used in the fifo. */

PUBLIC uint32_t
fifo_used( fifo_t * restrict f );

/* -------------------------------------------------------------------------- */

/** Returns the amount of data in the fifo _in_ a sequential block of memory. */

PUBLIC uint32_t
fifo_used_linear( fifo_t * restrict f );

/* -------------------------------------------------------------------------- */

/** Returns the amount of free space in the fifo. */

PUBLIC uint32_t
fifo_free( fifo_t * restrict f );

/* -------------------------------------------------------------------------- */

/** Write a byte to the FIFO. Return true when OK */

PUBLIC bool
fifo_put( fifo_t * restrict f, const uint8_t ch );

/* -------------------------------------------------------------------------- */

/** Get a byte from the FIFO. Return NULL when empty */

PUBLIC uint8_t *
fifo_get( fifo_t * restrict f );

/* -------------------------------------------------------------------------- */

/** Peek a byte from the FIFO. Return NULL when empty */

PUBLIC uint8_t *
fifo_peek( fifo_t * restrict f );

/* -------------------------------------------------------------------------- */

/** Writes up to nbytes bytes to the FIFO
 *  If the head runs in to the tail, not all bytes are written
 *  The number of bytes written is returned
 */

PUBLIC uint32_t
fifo_write( fifo_t * restrict f, const uint8_t * buf, uint32_t nbytes );

/* -------------------------------------------------------------------------- */

/** Reads nbytes bytes from the FIFO. The number of bytes read is returned */

PUBLIC uint32_t
fifo_read( fifo_t * restrict f, uint8_t * buf, uint32_t nbytes );

/* -------------------------------------------------------------------------- */

/** Get the pointer to the tail, if the proposed nbytes is legal
 *  Only use this function while promising that underlying data isn't mutated
 */

PUBLIC uint32_t*
fifo_get_tail_ptr( fifo_t * restrict f, uint32_t nbytes );

/* -------------------------------------------------------------------------- */

/** Moves the tail forwards by nbytes, functionally skipping data (partial flush)
 *  Returns number bytes flushed
 */

PUBLIC uint32_t
fifo_skip( fifo_t * restrict f, uint32_t nbytes );

/* ----- End ---------------------------------------------------------------- */

#ifdef    __cplusplus
}
#endif
#endif /* FIFO_H */
