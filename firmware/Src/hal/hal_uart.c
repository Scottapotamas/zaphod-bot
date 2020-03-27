/* ----- System Includes ---------------------------------------------------- */

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include "qassert.h"
#include "hal_uart.h"
#include "hal_gpio.h"
#include "stm32f4xx_hal.h"
#include "fifo.h"

/* ----- Private Data ------------------------------------------------------- */

//DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ----- Defines ------------------------------------------------------------ */

#warning "Fix cludge in fifo size due to eUI not being respectful of output processing time"
#define  HAL_UART_RX_FIFO_SIZE      1024
#define  HAL_UART_TX_FIFO_SIZE      1024

#define UART_EXTERNAL_BAUD	115200
#define UART_INTERNAL_BAUD	115200
#define UART_MODULE_BAUD	115200

/* ----- Types -------------------------------------------------------------- */

typedef struct
{
    HalUartPort_t       port;
    HalUartRxCallback_t rx_callback;
    UART_HandleTypeDef  usart;
    fifo_t              tx_fifo;
    uint8_t             tx_buffer[HAL_UART_TX_FIFO_SIZE];
    fifo_t              rx_fifo;
    uint8_t             rx_buffer[HAL_UART_RX_FIFO_SIZE];
} HalUart_t;

/* ----- Variables ---------------------------------------------------------- */

PRIVATE HalUart_t hal_uart[HAL_UART_NUM_PORTS];

/* ----- Private Functions -------------------------------------------------- */

PRIVATE void
hal_uart_init_usart1_io_and_isr( void );

PRIVATE void
hal_uart_deinit_usart1_io_and_isr( void );

PRIVATE void
hal_uart_init_usart2_io_and_isr( void );

PRIVATE void
hal_uart_deinit_usart2_io_and_isr( void );

PRIVATE void
hal_uart_init_uart5_io_and_isr( void );

PRIVATE void
hal_uart_deinit_uart5_io_and_isr( void );

/* -------------------------------------------------------------------------- */
/* --- UART INTERFACE                                                     --- */
/* -------------------------------------------------------------------------- */


PUBLIC void
hal_uart_init( HalUartPort_t port )
{
    HalUart_t * h = &hal_uart[port];
    memset( h, 0, sizeof( HalUart_t ) );

    switch( port )
    {
        case HAL_UART_PORT_EXTERNAL:
            h->port = HAL_UART_PORT_EXTERNAL;
            fifo_init( &h->tx_fifo, h->tx_buffer, HAL_UART_TX_FIFO_SIZE );
            fifo_init( &h->rx_fifo, h->rx_buffer, HAL_UART_RX_FIFO_SIZE );

            h->usart.Instance = UART5;
			hal_uart_set_baudrate( h->port, UART_EXTERNAL_BAUD );
			hal_uart_init_uart5_io_and_isr();
            break;

        case HAL_UART_PORT_INTERNAL:
            h->port = HAL_UART_PORT_INTERNAL;
            fifo_init( &h->tx_fifo, h->tx_buffer, HAL_UART_TX_FIFO_SIZE );
            fifo_init( &h->rx_fifo, h->rx_buffer, HAL_UART_RX_FIFO_SIZE );

			h->usart.Instance = USART1;
			hal_uart_set_baudrate( h->port, UART_INTERNAL_BAUD );
			hal_uart_init_usart1_io_and_isr();
            break;

        case HAL_UART_PORT_MODULE:
            h->port = HAL_UART_PORT_MODULE;
            fifo_init( &h->tx_fifo, h->tx_buffer, HAL_UART_TX_FIFO_SIZE );
            fifo_init( &h->rx_fifo, h->rx_buffer, HAL_UART_RX_FIFO_SIZE );

			h->usart.Instance = USART2;
			hal_uart_set_baudrate( h->port, UART_MODULE_BAUD );
			hal_uart_init_usart2_io_and_isr();
            break;

        default:
            break;
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_uart_deinit( HalUartPort_t port )
{
	HalUart_t * h = &hal_uart[port];
	switch( port )
	{
		case HAL_UART_PORT_EXTERNAL:
			h->port = HAL_UART_PORT_EXTERNAL;
			h->usart.Instance = UART5;
			hal_uart_deinit_uart5_io_and_isr();
			break;

		case HAL_UART_PORT_INTERNAL:
			h->port = HAL_UART_PORT_INTERNAL;
			h->usart.Instance = USART1;
			hal_uart_deinit_usart1_io_and_isr();
			break;

		case HAL_UART_PORT_MODULE:
			h->port = HAL_UART_PORT_MODULE;
			h->usart.Instance = USART2;
			hal_uart_deinit_usart2_io_and_isr();
			break;

		default:
			break;
	}
}

/* -------------------------------------------------------------------------- */

/** Activate the UART peripheral */

//PUBLIC void
//hal_uart_activate( HalUartPort_t port )
//{
//
//}

/* -------------------------------------------------------------------------- */

/** Deactivate the UART peripheral */

//PUBLIC void
//hal_uart_deactivate( HalUartPort_t port )
//{
//    HalUart_t * h = &hal_uart[port];
//
//    __HAL_UART_DISABLE_IT( &h->usart, UART_IT_TXE );
//    __HAL_UART_DISABLE_IT( &h->usart, UART_IT_RXNE );
//    HAL_UART_DeInit( &h->usart );
//}

/* -------------------------------------------------------------------------- */

/* Reset the UART baudrate */

PUBLIC void
hal_uart_reinit( HalUartPort_t port )
{
    HalUart_t * h = &hal_uart[port];

    if( h->usart.Instance )
    {
        __HAL_UART_DISABLE_IT( &h->usart, UART_IT_RXNE );
        HAL_UART_Init( &h->usart );
        __HAL_UART_ENABLE_IT( &h->usart, UART_IT_RXNE );
    }
}

/* -------------------------------------------------------------------------- */

/* Change the UART baudrate */

PUBLIC void
hal_uart_set_baudrate( HalUartPort_t port, uint32_t baudrate )
{
    HalUart_t * h = &hal_uart[port];

    h->usart.Init.BaudRate     = baudrate;
    h->usart.Init.WordLength   = UART_WORDLENGTH_8B;
    h->usart.Init.StopBits     = UART_STOPBITS_1;
    h->usart.Init.Parity       = UART_PARITY_NONE;
    h->usart.Init.Mode         = UART_MODE_TX_RX;
    h->usart.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    h->usart.Init.OverSampling = UART_OVERSAMPLING_16;

    HAL_UART_Init( &h->usart );
    __HAL_UART_ENABLE_IT( &h->usart, UART_IT_RXNE );
}

/* -------------------------------------------------------------------------- */

/* Get the UART baudrate */

PUBLIC uint32_t
hal_uart_get_baudrate( HalUartPort_t port )
{
    HalUart_t * h = &hal_uart[port];

    return h->usart.Init.BaudRate;
}
/* -------------------------------------------------------------------------- */

PUBLIC void
hal_uart_tx_start( HalUartPort_t port )
{
    HalUart_t * h = &hal_uart[port];

    if( ! __HAL_UART_GET_IT_SOURCE( &h->usart, UART_IT_TXE ) )
    {
        /* Interrupt disabled, kick start it */
        uint8_t c;
        fifo_read( &h->tx_fifo, &c, 1 );
        h->usart.Instance->DR = c;
        __HAL_UART_ENABLE_IT( &h->usart, UART_IT_TXE );
    }
}

/* -------------------------------------------------------------------------- */

/* Non-blocking send for a single character to the UART tx FIFO queue.
 * Returns true when successful. false when queue was full.
 */

PUBLIC bool
hal_uart_put( HalUartPort_t port, uint8_t ch )
{
    HalUart_t * h = &hal_uart[port];

    CRITICAL_SECTION_VAR();
    CRITICAL_SECTION_START();
    uint32_t sent = fifo_write( &h->tx_fifo, &ch, 1 );
    CRITICAL_SECTION_END();
    hal_uart_tx_start( port );
    return (sent == 1);
}

/* -------------------------------------------------------------------------- */

/** Wait until FIFO cleared. */

PUBLIC void
hal_uart_flush( HalUartPort_t port )
{
    CRITICAL_SECTION_VAR();

    HalUart_t * h = &hal_uart[port];
    uint32_t used;
    do
    {
        CRITICAL_SECTION_START();
        used = fifo_used( &h->tx_fifo );
        CRITICAL_SECTION_END();
    } while( used > 0 );
}

/* -------------------------------------------------------------------------- */

/* Non-blocking send for a single character to the UART tx FIFO queue.
 * Returns true when successful. false when queue was full.
 */

PUBLIC bool
hal_uart_put_blocking( HalUartPort_t port, uint8_t ch )
{
    HalUart_t * h = &hal_uart[port];

    uint32_t timeout = 100000;
    while( ( --timeout > 0) && ( fifo_free( &h->tx_fifo ) == 0 ) ) {}
    uint32_t sent = fifo_write( &h->tx_fifo, &ch, 1 );
    hal_uart_tx_start( port );
    return (sent == 1);
}

/* -------------------------------------------------------------------------- */

/* Blocking printf to the UART.
 * To be used within assert and fault handlers to print fault
 * information.
 */

PUBLIC void
hal_uart_printf_direct( HalUartPort_t port, const char * format, ... )
{
    HalUart_t * h = &hal_uart[port];
    char    buffer[255];
    uint16_t len;
    va_list args;

    va_start( args, format );
    len = (uint16_t)vsnprintf( buffer, sizeof(buffer), format, args );
    va_end( args );


    HAL_UART_Transmit( &h->usart, (uint8_t*)buffer, len, 0xFFFF );
}

/* -------------------------------------------------------------------------- */

/* Non-blocking send for a number of characters to the UART tx FIFO queue.
 * Returns the number of characters written to the queue. When less than
 * length, characters were dropped.
 */

PUBLIC uint32_t
hal_uart_write( HalUartPort_t port, const uint8_t * data, uint32_t length )
{
    HalUart_t * h = &hal_uart[port];

    uint32_t sent = fifo_write( &h->tx_fifo, data, length );
    hal_uart_tx_start( port );
    return sent;
}

/* -------------------------------------------------------------------------- */

/* Register a receive handler.
 * Called directly from UART ISR. When this is used, there is no buffering
 * available and the hal_usart_get is no longer functional.
 */

PUBLIC void
hal_uart_set_rx_handler( HalUartPort_t port, HalUartRxCallback_t handler )
{
    HalUart_t * h = &hal_uart[port];
    CRITICAL_SECTION_VAR();
    CRITICAL_SECTION_START();
    h->rx_callback = handler;
    CRITICAL_SECTION_END();
}

/* -------------------------------------------------------------------------- */

/* Returns number of available characters in the RX FIFO queue. */

PUBLIC uint32_t
hal_uart_rx_data_available( HalUartPort_t port )
{
    HalUart_t * h = &hal_uart[port];

    return fifo_used( &h->rx_fifo );
}

/* -------------------------------------------------------------------------- */

/* Retrieve a single byte from the rx FIFO queue.
 * Returns 0 when no data is available or when rx callback is in use.
 */

PUBLIC uint8_t
hal_uart_rx_get( HalUartPort_t port )
{
    HalUart_t * h = &hal_uart[port];

    uint8_t c = 0;
    CRITICAL_SECTION_VAR();
    CRITICAL_SECTION_START();
    fifo_read( &h->rx_fifo, &c, 1 );
    CRITICAL_SECTION_END();
    return c;
}

/* -------------------------------------------------------------------------- */

/* Retrieve a number of bytes from the rx FIFO queue up to
 * buffer length. Returns the number of bytes actually read.
 */

PUBLIC uint32_t
hal_uart_read( HalUartPort_t port, uint8_t * data, uint32_t maxlength )
{
    HalUart_t * h = &hal_uart[port];
    uint32_t len;
    CRITICAL_SECTION_VAR();
    CRITICAL_SECTION_START();
    len = fifo_read( &h->rx_fifo, data, maxlength );
    CRITICAL_SECTION_END();
    return len;
}

/* -------------------------------------------------------------------------- */

PRIVATE void
hal_uart_init_usart1_io_and_isr( void )
{
    __HAL_RCC_USART1_CLK_ENABLE();

    hal_gpio_init_alternate( _AUX_UART_RX, GPIO_MODE_AF_PP, GPIO_AF7_USART1, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_PULLUP );
    hal_gpio_init_alternate( _AUX_UART_TX, GPIO_MODE_AF_PP, GPIO_AF7_USART1, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_PULLUP );

    // UART1 interrupt
    HAL_NVIC_SetPriority( USART1_IRQn, 12, 0 );
    HAL_NVIC_EnableIRQ( USART1_IRQn );
}

/* -------------------------------------------------------------------------- */

PRIVATE void
hal_uart_deinit_usart1_io_and_isr( void )
{
    HAL_NVIC_DisableIRQ( USART1_IRQn );

    hal_gpio_disable_pin( _AUX_UART_RX );
    hal_gpio_disable_pin( _AUX_UART_TX );

    __HAL_RCC_USART2_CLK_DISABLE();
}

/* -------------------------------------------------------------------------- */

PRIVATE void
hal_uart_init_usart2_io_and_isr( void )
{
    __HAL_RCC_USART2_CLK_ENABLE();

    hal_gpio_init_alternate( _CARD_UART_RX, GPIO_MODE_AF_PP, GPIO_AF7_USART2, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_PULLUP );
    hal_gpio_init_alternate( _CARD_UART_TX, GPIO_MODE_AF_PP, GPIO_AF7_USART2, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_PULLUP );

    hal_gpio_init_alternate( _CARD_UART_CTS, GPIO_MODE_AF_PP, GPIO_AF7_USART2, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_NOPULL );
    hal_gpio_init_alternate( _CARD_UART_RTS, GPIO_MODE_AF_PP, GPIO_AF7_USART2, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_NOPULL );

    // USART2 interrupt
    HAL_NVIC_SetPriority( USART2_IRQn, 12, 0 );
    HAL_NVIC_EnableIRQ( USART2_IRQn );
}

/* -------------------------------------------------------------------------- */

PRIVATE void
hal_uart_deinit_usart2_io_and_isr( void )
{
    HAL_NVIC_DisableIRQ( USART2_IRQn );

    hal_gpio_disable_pin( _CARD_UART_RX );
    hal_gpio_disable_pin( _CARD_UART_TX );
    hal_gpio_disable_pin( _CARD_UART_CTS );
    hal_gpio_disable_pin( _CARD_UART_RTS );

    __HAL_RCC_USART2_CLK_DISABLE();
}

/* -------------------------------------------------------------------------- */

PRIVATE void
hal_uart_init_uart5_io_and_isr( void )
{
    __HAL_RCC_UART5_CLK_ENABLE();

    hal_gpio_init_alternate( _EXT_OUTPUT_0, GPIO_MODE_AF_PP, GPIO_AF8_UART5, GPIO_SPEED_FREQ_MEDIUM, GPIO_PULLUP );
    hal_gpio_init_alternate( _EXT_INPUT_0, 	GPIO_MODE_AF_PP, GPIO_AF8_UART5, GPIO_SPEED_FREQ_MEDIUM, GPIO_PULLUP );

    // UART5 interrupt
    HAL_NVIC_SetPriority( UART5_IRQn, 12, 0 );
    HAL_NVIC_EnableIRQ( UART5_IRQn );
}

/* -------------------------------------------------------------------------- */

PRIVATE void
hal_uart_deinit_uart5_io_and_isr( void )
{
    HAL_NVIC_DisableIRQ( UART5_IRQn );

    hal_gpio_disable_pin( _EXT_OUTPUT_0 );
    hal_gpio_disable_pin( _EXT_INPUT_0 );

    __HAL_RCC_UART5_CLK_DISABLE();
}

/* -------------------------------------------------------------------------- */

PRIVATE void
hal_usart_irq_handler( HalUart_t * h )
{
    /* RECEIVE HANDLING */
    if( __HAL_UART_GET_FLAG( &h->usart, UART_FLAG_RXNE ) )
    {
        uint8_t c = (uint8_t)h->usart.Instance->DR;

        if( h->rx_callback )
        {
            (*h->rx_callback)( h->port, c );
        }
        else
        {
            fifo_write( &h->rx_fifo, &c, 1 );
        }
    }

    /* TRANSMIT HANDLING */
    if( __HAL_UART_GET_FLAG( &h->usart, UART_FLAG_TXE ) )
    {
        if( fifo_used( &h->tx_fifo ) > 0 )
        {
            /* Transmit next available character */
            uint8_t c;
            fifo_read( &h->tx_fifo, &c, 1 );
            h->usart.Instance->DR = c;
        }
        else
        {
            /* Stop transmit interrupts */
            __HAL_UART_DISABLE_IT( &h->usart, UART_IT_TXE );
        }
    }
}

/* -------------------------------------------------------------------------- */

PRIVATE HalUart_t *
hal_uart_port( USART_TypeDef * usart )
{
    for( HalUartPort_t p = 0; p < HAL_UART_NUM_PORTS; p++)
    {
        if( hal_uart[p].usart.Instance == usart )
        {
            return &hal_uart[p];
        }
    }
    return NULL;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
USART1_IRQHandler( void )
{
    hal_usart_irq_handler( hal_uart_port( USART1 ) );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
USART2_IRQHandler( void )
{
    hal_usart_irq_handler( hal_uart_port( USART2 ) );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
UART5_IRQHandler( void )
{
    hal_usart_irq_handler( hal_uart_port( UART5 ) );
}


/* ----- End ---------------------------------------------------------------- */

