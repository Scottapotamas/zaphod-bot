#ifndef EXPANSION_H
#define EXPANSION_H

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* ----- Defines ------------------------------------------------------------ */

/* ----- Types ------------------------------------------------------------- */

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
expansion_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
expansion_get_speed( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
expansion_request_target( int32_t position );

/* -------------------------------------------------------------------------- */

PUBLIC void
expansion_set_home( void );

/* -------------------------------------------------------------------------- */

PUBLIC int32_t
expansion_get_position( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
expansion_process( void );

/* -------------------------------------------------------------------------- */

#endif /* EXPANSION_H */