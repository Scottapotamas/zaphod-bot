/* ----- Local Includes ----------------------------------------------------- */

#include <stdio.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "uuid.h"
#include "hal_uuid.h"

/* ----- Private Variables -------------------------------------------------- */

PRIVATE char uuid_string[28] = { 0 };

/* -------------------------------------------------------------------------- */

PUBLIC const char *
uuid( void )
{
    if( uuid_string[ 0 ] == 0 )
    {
        snprintf( uuid_string,
                  sizeof( uuid_string ),
                  "%08lX-%08lX-%08lX",
                  HAL_UUID[0],
                  HAL_UUID[1],
                  HAL_UUID[2] );
    }
    return uuid_string;
}

/* ----- End ---------------------------------------------------------------- */

