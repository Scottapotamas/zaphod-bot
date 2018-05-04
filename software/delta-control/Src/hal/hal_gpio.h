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
	MODE_ANALOG,	/* Analog input mode */
    MODE_OUT_PP,    /* Push-Pull Output */
    MODE_OUT_OD     /* Open drain Output */
} HalGpioMode_t;

/** Enum with all the GPIO pins defined. See schematic for more detail */

typedef enum
{
    /* --- INTERNAL EXPANSION IO --- */
    _AUX_PWM_0,
    _AUX_PWM_1,
    _AUX_PWM_2,
    _AUX_ANALOG_0,
    _AUX_ANALOG_1,

	_AUX_SCL,
	_AUX_SDA,
	_AUX_UART_TX,
	_AUX_UART_RX,

    /* --- EXTERNAL EXPANSION IO --- */
    _EXT_INPUT_0,
    _EXT_OUTPUT_0,

    _EXT_INPUT_1,
    _EXT_OUTPUT_1,

    /* --- EXPANSION CARD --- */
	_CARD_POWER_EN,
	_CARD_UART_RX,
	_CARD_UART_TX,
	_CARD_UART_CTS,
	_CARD_UART_RTS,

    /* --- ONBOARD MISC --- */
	_BUZZER,

	_FAN_PWM,
	_FAN_TACHO,

    _TEMP_PCB_AMBIENT,
    _TEMP_PCB_PSU,
    _TEMP_EXTERNAL,
    _VOLTAGE_SENSE,

    /* --- BUTTONS --- */
    _BTN_0,
    _BTN_1,

    /* --- STATUS LEDS --- */
    _STATUS_0,
    _STATUS_1,
	_STATUS_2,

    /* --- USB --- */
    _USB_PWR_EN,
    _USB_ID_SPARE,

    /* --- SERVO IO --- */
    _SERVO_1_A,
    _SERVO_1_B,
    _SERVO_1_ENABLE,
    _SERVO_1_HLFB,
    _SERVO_1_CURRENT,
    _SERVO_1_CURRENT_FAULT,

    _SERVO_2_A,
    _SERVO_2_B,
    _SERVO_2_ENABLE,
    _SERVO_2_HLFB,
    _SERVO_2_CURRENT,
    _SERVO_2_CURRENT_FAULT,

    _SERVO_3_A,
    _SERVO_3_B,
    _SERVO_3_ENABLE,
    _SERVO_3_HLFB,
    _SERVO_3_CURRENT,
    _SERVO_3_CURRENT_FAULT,

    _SERVO_4_A,
    _SERVO_4_B,
    _SERVO_4_ENABLE,
    _SERVO_4_HLFB,
    _SERVO_4_CURRENT,
    _SERVO_4_CURRENT_FAULT,

    /* --- Defining Total Entries --- */
    _NUMBER_OF_GPIO_PORT_PINS

} HalGpioPortPin_t;

/* -------------------------------------------------------------------------- */

/** Configure all internal default I/O configurations */

PUBLIC void
hal_gpio_configure_defaults( void );

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
