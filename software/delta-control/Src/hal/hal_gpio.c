/* ----- Module include ----------------------------------------------------- */

#include "hal_gpio.h"

/* ----- Local Includes ----------------------------------------------------- */

#include "stm32f4xx_hal.h"
#include "hal_gpio_types.h"
#include "qassert.h"

/* ----- Types -------------------------------------------------------------- */

/* ----- Private Function Declarations -------------------------------------- */


/* ----- Private Function Declarations -------------------------------------- */

/** Map the port nr to a STM32 GPIO_TypeDef */

PRIVATE GPIO_TypeDef *
hal_gpio_mcu_port( HalGpioPortNr_t port_nr );

/** Enable peripheral clock */

PRIVATE void
hal_gpio_mcu_rcc_clock_enable( const HalGpioPortNr_t port_nr );

/** Init as input */

PRIVATE void
hal_gpio_mcu_init_as_input( HalGpioPortNr_t port_nr,
                            HalGpioPinNr_t  pin_nr );

/** Init as input with pullup */

PRIVATE void
hal_gpio_mcu_init_as_input_with_pullup( HalGpioPortNr_t port_nr,
                                        HalGpioPinNr_t  pin_nr );

/** Init as open drain output */

PRIVATE void
hal_gpio_mcu_init_as_output_od( HalGpioPortNr_t port_nr,
                                HalGpioPinNr_t  pin_nr,
                                bool            initial_state );

/** Init as push pull output */

PRIVATE void
hal_gpio_mcu_init_as_output_pp( HalGpioPortNr_t port_nr,
                                HalGpioPinNr_t  pin_nr,
                                bool            initial_state );

/* ----- Private Data ------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */



/* ----- Grooming Trap MkIII I/O Map ---------------------------------------- */

/** For our own HAL GPIO layer, define enum labels that incorporate
 *  port and pin numbers combined in the one enum label.
 *  Port numbers are in the high byte and pin number are in the low byte
 */
const HalGpioDef_t HalGpioHardwareMap[] =
{
    /* --- IO --- */

    [ _AUX_GPIO_0          ] = { .mode = MODE_INPUT,  .port = PORT_B, .pin = PIN_4, .initial = 0 },
    [ _AUX_GPIO_1          ] = { .mode = MODE_INPUT,  .port = PORT_B, .pin = PIN_5, .initial = 0 },
    [ _AUX_GPIO_2          ] = { .mode = MODE_INPUT,  .port = PORT_B, .pin = PIN_6, .initial = 0 },
    [ _AUX_GPIO_3          ] = { .mode = MODE_INPUT,  .port = PORT_B, .pin = PIN_7, .initial = 0 },

    /* --- STATUS LEDS --- */

    [ _STATUS_0          ] = { .mode = MODE_OUT_PP, .port = PORT_A, .pin = PIN_6, .initial = 0 },
    [ _STATUS_1          ] = { .mode = MODE_OUT_PP, .port = PORT_A, .pin = PIN_7, .initial = 0 },
    [ _STATUS_2          ] = { .mode = MODE_OUT_PP, .port = PORT_A, .pin = PIN_8, .initial = 0 },

    /* --- USB VBUS --- */

    [ _USB_PWR_EN        ] = { .mode = MODE_OUT_PP,   .port = PORT_C, .pin = PIN_4, .initial = 0 },

    /* --- BUTTONS --- */

    [ _BTN_0             ] = { .mode = MODE_INPUT, .port = PORT_C, .pin = PIN_7 },
    [ _BTN_1             ] = { .mode = MODE_INPUT, .port = PORT_A, .pin = PIN_15 },

};

/* ----- Public Function Implementations ------------------------------------ */

PUBLIC void
hal_gpio_configure_mcu_defaults( void )
{
    for( HalGpioPortPin_t portpin = 0;
                          portpin < _NUMBER_OF_GPIO_PORT_PINS;
                          portpin++ )
    {
        const HalGpioDef_t *m = &HalGpioHardwareMap[portpin];
        hal_gpio_init( portpin, m->mode, m->initial );
    }
}

/* -------------------------------------------------------------------------- */

/** @brief Configure mode of pin */

PUBLIC void
hal_gpio_init( HalGpioPortPin_t gpio_port_pin_nr,
               HalGpioMode_t    mode,
               bool             initial_state )
{
    const HalGpioDef_t *m = &HalGpioHardwareMap[gpio_port_pin_nr];

    ENSURE( m->port <= PORT_H );
    ENSURE( m->pin <= PIN_15 );

    hal_gpio_mcu_rcc_clock_enable( m->port );

    switch( mode )
    {
        case MODE_INPUT:
            hal_gpio_mcu_init_as_input( m->port, m->pin );
            break;

        case MODE_INPUT_PU:
            hal_gpio_mcu_init_as_input_with_pullup( m->port, m->pin );
            break;

        case MODE_OUT_OD:
            hal_gpio_mcu_init_as_output_od( m->port, m->pin, initial_state );
            break;

        case MODE_OUT_PP:
            hal_gpio_mcu_init_as_output_pp( m->port, m->pin, initial_state );
            break;
    }

}

/* -------------------------------------------------------------------------- */

/** @brief read the I/O pin. Return true when pin is high */

PUBLIC bool
hal_gpio_read_pin( HalGpioPortPin_t gpio_port_pin_nr )
{
    const HalGpioDef_t *m = &HalGpioHardwareMap[gpio_port_pin_nr];

    //return true when high
    return HAL_GPIO_ReadPin( hal_gpio_mcu_port( m->port ), HAL_GPIO_PIN_MASK( m->pin ) ) == GPIO_PIN_SET;
}

/* -------------------------------------------------------------------------- */

/** @brief Write the I/O pin. Pin is set high when 'on' == true */

PUBLIC void
hal_gpio_write_pin( HalGpioPortPin_t gpio_port_pin_nr, bool on )
{
    const HalGpioDef_t *m = &HalGpioHardwareMap[gpio_port_pin_nr];

	HAL_GPIO_WritePin( hal_gpio_mcu_port( m->port ), HAL_GPIO_PIN_MASK( m->pin ), on ? GPIO_PIN_SET : GPIO_PIN_RESET );
}

/* -------------------------------------------------------------------------- */

/** @brief Toggle the I/O pin */

PUBLIC void
hal_gpio_toggle_pin( HalGpioPortPin_t gpio_port_pin_nr )
{
    const HalGpioDef_t *m = &HalGpioHardwareMap[gpio_port_pin_nr];

    HAL_GPIO_TogglePin( hal_gpio_mcu_port( m->port ), HAL_GPIO_PIN_MASK( m->pin ) );
}

/* ----- Private Function Implementations ----------------------------------- */


/** Map the port nr to a STM32 GPIO_TypeDef */

PRIVATE GPIO_TypeDef *
hal_gpio_mcu_port( HalGpioPortNr_t port_nr )
{
    GPIO_TypeDef * port = 0;

    switch( port_nr )
    {
        case PORT_A: port = GPIOA; break;
        case PORT_B: port = GPIOB; break;
        case PORT_C: port = GPIOC; break;
        case PORT_D: port = GPIOD; break;
        case PORT_E: port = GPIOE; break;
        case PORT_F: port = GPIOF; break;
        case PORT_G: port = GPIOG; break;
        case PORT_H: port = GPIOH; break;
        default:     ASSERT( false );
    }
    return port;
}

/* -------------------------------------------------------------------------- */

PRIVATE void
hal_gpio_mcu_rcc_clock_enable( const HalGpioPortNr_t port_nr )
{
    switch( port_nr )
    {
        case PORT_A: __HAL_RCC_GPIOA_CLK_ENABLE(); break;
        case PORT_B: __HAL_RCC_GPIOB_CLK_ENABLE(); break;
        case PORT_C: __HAL_RCC_GPIOC_CLK_ENABLE(); break;
        case PORT_D: __HAL_RCC_GPIOD_CLK_ENABLE(); break;
        case PORT_E: __HAL_RCC_GPIOE_CLK_ENABLE(); break;
        case PORT_F: __HAL_RCC_GPIOF_CLK_ENABLE(); break;
        case PORT_G: __HAL_RCC_GPIOG_CLK_ENABLE(); break;
        case PORT_H: __HAL_RCC_GPIOH_CLK_ENABLE(); break;
        default:                                   break;
    }
}

/* -------------------------------------------------------------------------- */

/** @brief Configure Input pin without pull up */

PRIVATE void
hal_gpio_mcu_init_as_input( HalGpioPortNr_t port_nr,
                            HalGpioPinNr_t  pin_nr )
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin  = HAL_GPIO_PIN_MASK( pin_nr );
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init( hal_gpio_mcu_port( port_nr ),
                   &GPIO_InitStruct );
}

/* -------------------------------------------------------------------------- */

/** @brief Configure as Input pin with pullups */

PRIVATE void
hal_gpio_mcu_init_as_input_with_pullup( HalGpioPortNr_t port_nr,
                                        HalGpioPinNr_t  pin_nr )
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin  = HAL_GPIO_PIN_MASK( pin_nr );
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init( hal_gpio_mcu_port( port_nr ),
                   &GPIO_InitStruct );
}

/* -------------------------------------------------------------------------- */

/** @brief Configure as output */

PRIVATE void
hal_gpio_mcu_init_as_output_od( HalGpioPortNr_t port_nr,
                                HalGpioPinNr_t  pin_nr,
                                bool            initial_state )
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin   = HAL_GPIO_PIN_MASK( pin_nr );
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init( hal_gpio_mcu_port( port_nr ),
                   &GPIO_InitStruct);

    HAL_GPIO_WritePin( hal_gpio_mcu_port( port_nr ),
                       HAL_GPIO_PIN_MASK( pin_nr ),
					   initial_state ? GPIO_PIN_SET : GPIO_PIN_RESET );
}

/* -------------------------------------------------------------------------- */

/** @brief Configure as output */

PRIVATE void
hal_gpio_mcu_init_as_output_pp( HalGpioPortNr_t port_nr,
                                HalGpioPinNr_t  pin_nr,
                                bool            initial_state )
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin   = HAL_GPIO_PIN_MASK( pin_nr );
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init( hal_gpio_mcu_port( port_nr ),
                   &GPIO_InitStruct);

    HAL_GPIO_WritePin( hal_gpio_mcu_port( port_nr ),
                       HAL_GPIO_PIN_MASK( pin_nr ),
					   initial_state ? GPIO_PIN_SET : GPIO_PIN_RESET );
}

/* ----- End ---------------------------------------------------------------- */
