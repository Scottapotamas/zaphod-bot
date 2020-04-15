#ifndef AVERAGE_SHORT_H
#define AVERAGE_SHORT_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* ----- Defines ------------------------------------------------------------ */

#define AVERAGE_SHORT_MAX_SPAN ( 60 )

/* ----- Types -------------------------------------------------------------- */

/** Structure to keep track of short term averages
  * (e.g. 60 sec of once per second). Declare the below structure and
  * use average_short_init to reset the structure and set the span required.
  * Then call average short with a pointer to the storage struct and the new
  * value to be averaged. Average value of all values of var is returned and
  * also available in the structure.
  */
typedef struct
{
    uint16_t span;      /* Number of values to average */
    uint16_t counter;   /* Current number of samples available */
    uint16_t index;     /* Index in sample buffer */
    uint16_t average;   /* Current average */
    uint16_t last;      /* Last added value */
    uint16_t deviation; /* Spread between the minimum and maximum values */
    uint32_t sum;       /* Sum of samples */
    uint16_t buffer[AVERAGE_SHORT_MAX_SPAN];
} AverageShort_t;

/* -------------------------------------------------------------------------- */

/** Reset an averaging buffer and set the required span (number of values)
 *  over which to average).
 *
 *  Span has to be equal or less than AVERAGE_SHORT_SPAN.
 */

PUBLIC void
average_short_init( AverageShort_t *me, uint16_t span );

/* -------------------------------------------------------------------------- */

/** Add a new value to the buffer and get the average so far returned. After
 *  AVERAGE_SHORT_SPAN entries, it becomes a running average of the last
 *  AVERAGE_SHORT_SPAN values.
 */

PUBLIC uint16_t
average_short_update( AverageShort_t *me, uint16_t new );

/* -------------------------------------------------------------------------- */

PUBLIC uint16_t
average_short_get_count( AverageShort_t *me );

/* -------------------------------------------------------------------------- */

/** Get the current average from the samples buffer */

PUBLIC uint16_t
average_short_get_average( AverageShort_t *me );

/* -------------------------------------------------------------------------- */

/** Get the last added value from the samples buffer */

PUBLIC uint16_t
average_short_get_last( AverageShort_t *me );

/* -------------------------------------------------------------------------- */

/** Get the deviation from average on the current samples buffer */

PUBLIC uint16_t
average_short_get_deviation( AverageShort_t *me );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* AVERAGE_SHORT_H */
