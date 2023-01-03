/* ----- System Includes ---------------------------------------------------- */

#include <string.h>
#include <math.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "attractor_sequence.h"

/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */

PUBLIC void
attractor_sequence_init( void )
{

}

/* -------------------------------------------------------------------------- */

/*
 * Move creation process
 *
 * If we're not at home,
 *  Grab the current position
 * Otherwise
 *  Generate a random position to start from
 *
 * Run x iterations of attractor from current position to calculate future positions on the path
 *  Keep track of the current time
 *  New points are done by adding equal measures of time
 * Create a sliding catmull rom out of these points
 * Calculate length of catmull rom
 * Calculate the move duration in order to satisfy a target velocity
 * Clamp move duration to be > 5ms or something?
 *
 * Add the move to the planner queue
 *
 * */

/* -------------------------------------------------------------------------- */



/* -------------------------------------------------------------------------- */


/* ----- End ---------------------------------------------------------------- */
