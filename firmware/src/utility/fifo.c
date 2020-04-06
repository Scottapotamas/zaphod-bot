/**
 * @file      fifo.c
 *
 * @ingroup   utility
 *
 * @brief     Basic buffer based FIFO facility
 *
 * @author    Marco Hess <marcoh@applidyne.com.au>
 *
 * @copyright (c) 2015 Applidyne Australia Pty. Ltd. - All rights reserved.
 */

/* ----- System Includes ---------------------------------------------------- */


/* ----- Local Includes ----------------------------------------------------- */

#include "fifo.h"

/* ----- Private Prototypes ------------------------------------------------- */

PRIVATE uint32_t
fifo_next( fifo_t * restrict f, uint32_t index );

/* ----- Public Functions --------------------------------------------------- */

/** This initializes the FIFO structure with the given buffer and size */

PUBLIC void
fifo_init( fifo_t * restrict f, uint8_t * buf, uint32_t buf_size )
{
     f->head     = 0;
     f->tail     = 0;
     f->capacity = buf_size;
     f->buf      = buf;
}

/* -------------------------------------------------------------------------- */

/** Returns the amount of data in use in the fifo.
  * This can be at max the capacity less 1
  */

PUBLIC uint32_t
fifo_size( fifo_t * restrict f )
{
    return f->capacity - 1;
}

/* -------------------------------------------------------------------------- */

/** Returns the amount of data in use in the fifo.
  * This can be at max the capacity less 1
  */

PUBLIC uint32_t
fifo_used( fifo_t * restrict f )
{
    if( f->head != f->tail )
    {
        if( f->head > f->tail )
        {
            return f->head - f->tail;
        }
        else
        {
            return f->head - f->tail + f->capacity;
        }
    }
    return 0;
}

/* -------------------------------------------------------------------------- */

/** Returns the amount of data in a sequential run in the fifo
  * i.e doesn't count bytes which cross over the overflow boundary
  * This can be at max the capacity less 1
  */

PUBLIC uint32_t
fifo_used_linear( fifo_t * restrict f )
{
    if( f->head != f->tail )
    {
        if( f->head > f->tail )
        {
            return f->head - f->tail;
        }
        else
        {
            return f->capacity - f->tail;   // from tail to end of buffer
        }
    }
    return 0;
}

/* -------------------------------------------------------------------------- */

/** Returns the amount of data free in the fifo. */

PUBLIC uint32_t
fifo_free( fifo_t * restrict f )
{
    return fifo_size( f ) - fifo_used( f );
}

/* -------------------------------------------------------------------------- */

/** Write a byte to the FIFO. Return true when OK */

PUBLIC bool
fifo_put( fifo_t * restrict f, const uint8_t ch )
{
    const uint32_t new_head = fifo_next( f, f->head );
    if( new_head != f->tail )
    {
        f->buf[f->head] = ch;
        f->head = new_head;
        return true;    /* successfully added to queue */
    }
    return false; //no more room
}

/* -------------------------------------------------------------------------- */

/** Get a byte from the FIFO. Return NULL when empty */

PUBLIC uint8_t *
fifo_get( fifo_t * restrict f )
{
    if( f->tail != f->head )
    {
        uint8_t * ch = &f->buf[f->tail];
        f->tail = fifo_next( f, f->tail );
        return ch;
    }
    return NULL;
}

/* -------------------------------------------------------------------------- */

/** Peek a byte from the FIFO. Return NULL when empty */

PUBLIC uint8_t *
fifo_peek( fifo_t * restrict f )
{
    if( f->tail != f->head )
    {
        uint8_t * ch = &f->buf[f->tail];
        return ch;
    }
    return NULL;
}

/* -------------------------------------------------------------------------- */

/** Writes up to nbytes bytes to the FIFO
 *  If the head runs in to the tail, not all bytes are written
 *  The number of bytes written is returned
 */

PUBLIC uint32_t
fifo_write( fifo_t * restrict f, const uint8_t * buf, uint32_t nbytes )
{
     uint32_t count = 0;
     for( uint32_t i = 0; i < nbytes; i++ )
     {
         if( fifo_put( f, buf[i] ) )
         {
             count++;
         }
         else
         {
             break;
         }
    }
    return count;
}

/* -------------------------------------------------------------------------- */

/** Reads nbytes bytes from the FIFO. The number of bytes read is returned */

PUBLIC uint32_t
fifo_read( fifo_t * restrict f, uint8_t * buf, uint32_t nbytes )
{
     uint32_t count = 0;
     for( uint32_t i = 0; i < nbytes; i++ )
     {
         uint8_t * p = fifo_get( f );
         if( p )
         {
             buf[i] = *p;
             count++;
         }
         else
         {
             break;
         }
     }
     return count;
}

/* -------------------------------------------------------------------------- */

/** Returns a pointer to the tail position. If the nbytes length is illegal, returns null */

PUBLIC uint32_t*
fifo_get_tail_ptr( fifo_t * restrict f, uint32_t nbytes )
{
    uint32_t *ptr = NULL;

    if( nbytes <= fifo_used_linear(f)  )
    {
        ptr = &f->buf[f->tail];
    }

    return ptr;
}

/* -------------------------------------------------------------------------- */

/** Moves/flushes the tail forward by nbytes */

PUBLIC uint32_t
fifo_skip( fifo_t * restrict f, uint32_t nbytes )
{
    if( nbytes <= fifo_used_linear(f) )
    {
        f->tail += nbytes;

        if( f->tail >= f->capacity )
        {
            f->tail = 0;
        }

        return nbytes;
    }

    return 0;
}

/* ----- Private Functions -------------------------------------------------- */

PRIVATE uint32_t
fifo_next( fifo_t * restrict f, uint32_t index )
{
    uint32_t current = index;
    current++;
    if( current >= f->capacity )
    {
        current = 0;
    }
    return current;
}

/* ----- End ---------------------------------------------------------------- */
