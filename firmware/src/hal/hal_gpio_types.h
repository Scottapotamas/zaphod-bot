#ifndef HAL_GPIO_TYPES_H
#define HAL_GPIO_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* ----- Defines ------------------------------------------------------------ */

/** Readability macros to remind us which is a HIGH level or LOW level */
#define GPIO_HIGH true
#define GPIO_LOW  false

/** Map the pin number into a bit mask */
#define HAL_GPIO_PIN_MASK( pin_nr ) ( ( uint16_t )( 1U << ( pin_nr ) ) )

/** Port Number */
typedef enum
{
    PORT_A = ( 0 ),
    PORT_B = ( 1 ),
    PORT_C = ( 2 ),
    PORT_D = ( 3 ),
    PORT_E = ( 4 ),
    PORT_F = ( 5 ),
    PORT_G = ( 6 ),
    PORT_H = ( 7 ),
    PORT_I = ( 8 ),
    PORT_J = ( 9 ),
    PORT_K = ( 10 ),
} HalGpioPortNr_t;

/** Pin Numbers */
typedef enum
{
    PIN_0,
    PIN_1,
    PIN_2,
    PIN_3,
    PIN_4,
    PIN_5,
    PIN_6,
    PIN_7,
    PIN_8,
    PIN_9,
    PIN_10,
    PIN_11,
    PIN_12,
    PIN_13,
    PIN_14,
    PIN_15,
} HalGpioPinNr_t;

typedef struct
{
    unsigned mode    : 3;      /* GPIO Mode (Analog, Input, Output Push Pull, etc. ) */
    unsigned port    : 4;      /* GPIO Port */
    unsigned pin     : 5;      /* Pin Number within port */
    unsigned initial : 1;      /* Initialisation level for outputs */
} HalGpioDef_t;

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_GPIO_TYPES_H */
