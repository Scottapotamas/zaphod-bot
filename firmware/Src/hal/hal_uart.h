#ifndef HAL_UART_H
#define HAL_UART_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include "stm32f4xx_ll_gpio.h"

/* ----- Types ------------------------------------------------------------- */

typedef enum
{
    HAL_UART_PORT_EXTERNAL,
    HAL_UART_PORT_INTERNAL,
    HAL_UART_PORT_MODULE,
    HAL_UART_NUM_PORTS
} HalUartPort_t;

/* -------------------------------------------------------------------------- */
/* --- UART INTERFACE                                                     --- */
/* -------------------------------------------------------------------------- */

/** Initialise the UARTs */

PUBLIC void
hal_uart_init( HalUartPort_t port );

/* -------------------------------------------------------------------------- */

/** Deinit the UARTs */

PUBLIC void
hal_uart_deinit( HalUartPort_t port );

/* -------------------------------------------------------------------------- */

/* Reset the UART (for after clock speed change) */

PUBLIC void
hal_uart_reinit( HalUartPort_t port );

/* -------------------------------------------------------------------------- */

/** Activate the UART peripheral */

PUBLIC void
hal_uart_activate( HalUartPort_t port );

/* -------------------------------------------------------------------------- */

/** Deactivate the UART peripheral */

PUBLIC void
hal_uart_deactivate( HalUartPort_t port );

/* -------------------------------------------------------------------------- */

/** Configure the UART baudrate */

PUBLIC void
hal_uart_set_baudrate( HalUartPort_t port, uint32_t baudrate );

/* -------------------------------------------------------------------------- */

/** Get the current UART baudrate */

PUBLIC uint32_t
hal_uart_get_baudrate( HalUartPort_t port );

/* -------------------------------------------------------------------------- */

/* Non-blocking send for a single character to the UART tx FIFO queue.
 * Returns true when successful. false when queue was full.
 */

PUBLIC bool
hal_uart_put( HalUartPort_t port, uint8_t c );

/* -------------------------------------------------------------------------- */

/** Wait until FIFO cleared. */

PUBLIC void
hal_uart_flush( HalUartPort_t port );

/* -------------------------------------------------------------------------- */

/* Blocking send for a single character to the UART tx FIFO queue.
 * Can still return unsuccessfully when timing out.
 * Returns true when successful. false when queue was full.
 */

PUBLIC bool
hal_uart_put_blocking( HalUartPort_t port, uint8_t c );

/* -------------------------------------------------------------------------- */

/* Non-blocking send for a number of characters to the UART tx FIFO queue.
 * Returns true when successful. false when queue was full.
 */

PUBLIC uint32_t
hal_uart_write( HalUartPort_t port, const uint8_t * data, uint32_t length );

/* -------------------------------------------------------------------------- */

/* Returns number of available characters in the RX FIFO queue. */

PUBLIC uint32_t
hal_uart_rx_data_available( HalUartPort_t port );

/* -------------------------------------------------------------------------- */

/* Retrieve a single byte from the rx FIFO queue.
 * Returns 0 when no data is available or when rx callback is in use.
 */

PUBLIC uint8_t
hal_uart_rx_get( HalUartPort_t port );

/* -------------------------------------------------------------------------- */

/* Retrieve a number of bytes from the rx FIFO queue up to
 * buffer length. Returns the number of bytes actually read.
 */

PUBLIC uint32_t
hal_uart_read( HalUartPort_t port, uint8_t * buffer, uint32_t bufferlen );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_UART_H */
