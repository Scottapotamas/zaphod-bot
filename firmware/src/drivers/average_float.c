/* ----- System Includes ---------------------------------------------------- */

#include <math.h>
#include <stdlib.h>
#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "average_float.h"
#include "global.h"
#include "qassert.h"

/* -------------------------------------------------------------------------- */

//DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* -------------------------------------------------------------------------- */

/** Reset an averaging buffer and set the required span (number of values)
 *  over which to average).
 *
 *  Span has to be equal or less than AVERAGE_SHORT_SPAN.
 */

PUBLIC void
average_float_init( AverageFloat_t *me, uint16_t span )
{
    /* span must sensible and fit in buffer */
    //REQUIRE( span > 0 );
    //REQUIRE( span <= AVERAGE_FLOAT_MAX_SPAN );

    /* Clear the structure */
    memset( me, 0, sizeof( AverageFloat_t ) );

    /* Init the span */
    me->span = span;
}

/* -------------------------------------------------------------------------- */

/** Add a new value to the buffer and get the average so far returned. After
 *  AVERAGE_FLOAT_MAX_SPAN entries, it becomes a running average of the last
 *  AVERAGE_FLOAT_MAX_SPAN values. It is expected to be updated one per second
 *  so for a total averaging over a minute.
 */

PUBLIC float
average_float_update( AverageFloat_t *me, float new )
{
    /* If we don't have a full span only calculate the average of the
     * entries we have so far, if the span is full, it turns into a
     * running average.
     */

    if( me->counter < me->span )
    {
        me->counter++;
    }
    else
    {
        /* With a full buffer, subtract the old index value from the sum */
        if( me->sum )
        {
            me->sum -= me->buffer[me->index];
        }
    }

    /* Remember this this new value as the last added */
    me->last = new;

    /* Add new value to index */
    me->buffer[me->index] = new;

    /* Add new index value to sum */
    me->sum += me->buffer[me->index];

    /* Recalculate the average */
    me->average = me->sum / me->counter;

    /* Prepare for next index */
    me->index++;
    if( me->index >= me->span )
    {
        me->index = 0;
    }

    return me->average;
}

/* -------------------------------------------------------------------------- */

/** Get the last added value from the samples buffer */

PUBLIC uint16_t
average_float_get_count( AverageFloat_t *me )
{
    return me->counter;
}

/* -------------------------------------------------------------------------- */

/** Get the last added value from the samples buffer */

PUBLIC float
average_float_get_average( AverageFloat_t *me )
{
    return me->average;
}

/* -------------------------------------------------------------------------- */

/** Get the last added value from the samples buffer */

PUBLIC float
average_float_get_last( AverageFloat_t *me )
{
    return me->last;
}

/* -------------------------------------------------------------------------- */

PUBLIC float
average_float_get_deviation( AverageFloat_t *me )
{
    /* Determine the spread of the values in the buffer */
    int32_t tmp = 0;
    for( uint16_t i = 0; i < me->span; i++ )
    {
        int32_t diff = me->buffer[i] - me->average;
        tmp += ( diff * diff );
    }
    me->deviation = (uint16_t)sqrtf( (float)tmp );

    return me->deviation;
}

/* ----- End ---------------------------------------------------------------- */
