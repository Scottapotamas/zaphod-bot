#ifndef OVERWATCH_H
#define OVERWATCH_H

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "observer.h"
#include "subject.h"

/* -------------------------------------------------------------------------- */

PUBLIC void overwatch_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC Subject * overwatch_get_subject( void );

/* -------------------------------------------------------------------------- */

PUBLIC Observer * overwatch_get_observer( void );

/* -------------------------------------------------------------------------- */

PUBLIC void overwatch_task( void* arg );

/* -------------------------------------------------------------------------- */

#endif    // OVERWATCH_H