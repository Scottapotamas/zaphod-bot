#ifndef AVERAGE_FLOAT_H
#define AVERAGE_FLOAT_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* ----- Defines ------------------------------------------------------------ */

#define AVERAGE_FLOAT_MAX_SPAN ( 60 ) /* Seconds */

/* ----- Types -------------------------------------------------------------- */

/** Structure to keep track of short term averages
  * (e.g. 60 sec of every second). Declare the below structure and
  * use average_float_init to reset the structure and set the span required.
  * Then call average short with a pointer to the storage struct and the new
  * value to be averaged. Average value of all values of var is returned and
  * also available in the structure.
  */
typedef struct
{
    uint16_t span;      /* Number of values to average */
    uint16_t counter;   /* Current number of samples available */
    uint16_t index;     /* Index in sample buffer */
    float    average;   /* Current average */
    float    last;      /* Last added value */
    float    deviation; /* Spread between the minimum and maximum values */
    float    sum;       /* Sum of samples */
    float    buffer[AVERAGE_FLOAT_MAX_SPAN];
} AverageFloat_t;

/* -------------------------------------------------------------------------- */

/** Reset an averaging buffer and set the required span (number of values)
 *  over which to average).
 *
 *  Span has to be equal or less than AVERAGE_SHORT_SPAN.
 */

PUBLIC void
average_float_init( AverageFloat_t *me, uint16_t span );

/* -------------------------------------------------------------------------- */

/** Add a new value to the buffer and get the average so far returned. After
 *  AVERAGE_SHORT_SPAN entries, it becomes a running average of the last
 *  AVERAGE_SHORT_SPAN values.
 */

PUBLIC float
average_float_update( AverageFloat_t *me, float new );

/* -------------------------------------------------------------------------- */

PUBLIC uint16_t
average_float_get_count( AverageFloat_t *me );

/* -------------------------------------------------------------------------- */

/** Get the current average from the samples buffer */

PUBLIC float
average_float_get_average( AverageFloat_t *me );

/* -------------------------------------------------------------------------- */

/** Get the last added value from the samples buffer */

PUBLIC float
average_float_get_last( AverageFloat_t *me );

/* -------------------------------------------------------------------------- */

/** Get the deviation from average on the current samples buffer */

PUBLIC float
average_float_get_deviation( AverageFloat_t *me );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* AVERAGE_FLOAT_H */
