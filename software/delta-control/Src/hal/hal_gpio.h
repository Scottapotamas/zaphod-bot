#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

#include <stdbool.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include "stm32f4xx_hal.h"
#include "qassert.h"

/* ----- Defines ------------------------------------------------------------ */

/** Readability macros to remind us which is a HIGH level or LOW level */

#define GPIO_HIGH       true
#define GPIO_LOW        false

/* ----- Types ------------------------------------------------------------- */

/** Operational mode for pins */

typedef enum
{
    MODE_INPUT = 0, /* Normal input */
    MODE_INPUT_PU,  /* Input with pullup */
    MODE_OUT_PP,    /* Push-Pull Output */
    MODE_OUT_OD     /* Open drain Output */
} HalGpioMode_t;

/** Enum with all the GPIO pins defined. */

typedef enum
{
    /* --- IO --- */

    _AUX_GPIO_0,
    _AUX_GPIO_1,
    _AUX_GPIO_2,
    _AUX_GPIO_3,

    /* --- BUTTONS --- */

    _BTN_0,             /* Input: Left Button */
    _BTN_1,             /* Input: Right Button */

    /* --- STATUS LEDS --- */

    _STATUS_0,            /* Output: LED RED */
    _STATUS_1,            /* Output: LED GREEN */
	_STATUS_2,

    /* --- USB --- */

    _USB_PWR_EN,          /* Output: USB Power Enable */

    /* --- Defining Total Entries --- */
    _NUMBER_OF_GPIO_PORT_PINS

} HalGpioPortPin_t;

/* -------------------------------------------------------------------------- */

/** Configure all internal default I/O configurations */

PUBLIC void
hal_gpio_configure_mcu_defaults( void );

/* -------------------------------------------------------------------------- */

/** Configure an indicated pin for the give mode */

PUBLIC void
hal_gpio_init( HalGpioPortPin_t gpio_port_pin_nr, HalGpioMode_t mode, bool initial );

/* -------------------------------------------------------------------------- */

/** Return true when input pin is high, false when input is low */

PUBLIC bool
hal_gpio_read_pin( HalGpioPortPin_t gpio_port_pin_nr );

/* -------------------------------------------------------------------------- */

/** Set output pin high when on = true, low when on = false */

PUBLIC void
hal_gpio_write_pin( HalGpioPortPin_t gpio_port_pin_nr, bool on );

/* -------------------------------------------------------------------------- */

/** Toggle the current output state */

PUBLIC void
hal_gpio_toggle_pin( HalGpioPortPin_t gpio_port_pin_nr );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_GPIO_H */
