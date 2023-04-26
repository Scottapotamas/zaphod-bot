#ifndef HAL_UART_H
#define HAL_UART_H

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

typedef enum
{
    HAL_UART_PORT_EXTERNAL,
    HAL_UART_PORT_INTERNAL,
    HAL_UART_PORT_MODULE,
    HAL_UART_NUM_PORTS
} HalUartPort_t;

/* -------------------------------------------------------------------------- */

/** Initialise a UART */

PUBLIC void
hal_uart_init( HalUartPort_t port, uint32_t baudrate );

/* -------------------------------------------------------------------------- */

/** Deinit the UARTs */

PUBLIC void
hal_uart_global_deinit( void );

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
hal_uart_write( HalUartPort_t port, const uint8_t *data, uint32_t length );

PUBLIC uint32_t
hal_uart_read( HalUartPort_t port, uint8_t *data, uint32_t maxlength );

/* -------------------------------------------------------------------------- */

void UART5_IRQHandler( void );

void USART1_IRQHandler( void );

void USART2_IRQHandler( void );

/* -------------------------------------------------------------------------- */

void DMA1_Stream0_IRQHandler( void );

void DMA1_Stream7_IRQHandler( void );

void DMA2_Stream2_IRQHandler( void );

void DMA2_Stream7_IRQHandler( void );

void DMA1_Stream5_IRQHandler( void );

void DMA1_Stream6_IRQHandler( void );

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_UART_H */
