/* ----- System Includes ---------------------------------------------------- */

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_bus.h"

#include "global.h"
#include "hal_uart.h"
#include "hal_gpio.h"
#include "fifo.h"
#include "qassert.h"

/* ----- Private Data ------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ----- Defines ------------------------------------------------------------ */


#define  HAL_UART_RX_FIFO_SIZE      250
#define  HAL_UART_TX_FIFO_SIZE      250

#define HAL_UART_RX_DMA_BUFFER_SIZE 64

#define UART_EXTERNAL_BAUD	115200
#define UART_INTERNAL_BAUD	115200
#define UART_MODULE_BAUD	115200

/* ----- Types -------------------------------------------------------------- */

typedef struct
{
    HalUartPort_t   port;           // Human friendly enum name
    USART_TypeDef   *usart;

    // Keep track of the DMA periph/stream/channel values for this port
    DMA_TypeDef     *dma_peripheral;    // DMA1 or DMA2 object
    uint32_t         dma_stream_tx;
    uint32_t         dma_channel_tx;
    uint32_t         dma_stream_rx;
    uint32_t         dma_channel_rx;

    // User-space buffers are serviced outside IRQ
    fifo_t          tx_fifo;
    uint8_t         tx_buffer[HAL_UART_TX_FIFO_SIZE];
    uint8_t         tx_sneak_bytes;

    fifo_t          rx_fifo;
    uint8_t         rx_buffer[HAL_UART_RX_FIFO_SIZE];

    // Raw DMA buffer,
    volatile uint8_t dma_rx_buffer[HAL_UART_RX_DMA_BUFFER_SIZE];
    uint32_t         dma_rx_pos;

} HalUart_t;

/* ----- Variables ---------------------------------------------------------- */

PRIVATE HalUart_t hal_uart[HAL_UART_NUM_PORTS];

/* ----- Private Functions -------------------------------------------------- */

PRIVATE void
hal_uart_dma_init(HalUartPort_t port);

PRIVATE void
hal_uart_dma_irq_setup( DMA_TypeDef *DMAx, uint32_t stream, uint8_t preempt_priority, uint8_t sub_priority);

PRIVATE void
hal_uart_peripheral_init( USART_TypeDef *USARTx, uint32_t baudrate );

PRIVATE void
hal_uart_clear_dma_tx_flags( DMA_TypeDef *DMAx, uint32_t stream_tx );

PRIVATE void
hal_uart_start_tx(HalUart_t *h );

PRIVATE void
hal_uart_completed_tx(HalUart_t *h );

PRIVATE void
hal_usart_irq_rx_handler(HalUart_t *h);


/* ----- USART Interface ---------------------------------------------------- */

PUBLIC void
hal_uart_init( HalUartPort_t port )
{
    HalUart_t * h = &hal_uart[port];
    memset( h, 0, sizeof( HalUart_t ) );

    switch( port )
    {
        case HAL_UART_PORT_EXTERNAL:
            h->port = HAL_UART_PORT_EXTERNAL;
            h->usart = UART5;
            // See STM DMA Controller Description AN4031 Section 1.1.1 for Stream/Channel matrix
            h->dma_peripheral   = DMA1;
            h->dma_stream_tx    = LL_DMA_STREAM_7;
            h->dma_channel_tx   = LL_DMA_CHANNEL_4;
            h->dma_stream_rx    = LL_DMA_STREAM_0;
            h->dma_channel_rx   = LL_DMA_CHANNEL_4;

            fifo_init( &h->tx_fifo, h->tx_buffer, HAL_UART_TX_FIFO_SIZE );
            fifo_init( &h->rx_fifo, h->rx_buffer, HAL_UART_RX_FIFO_SIZE );

            LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART5);
            LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

            hal_gpio_init_alternate( _EXT_OUTPUT_0, LL_GPIO_OUTPUT_PUSHPULL, LL_GPIO_AF_8, LL_GPIO_SPEED_FREQ_VERY_HIGH, LL_GPIO_PULL_NO );
            hal_gpio_init_alternate( _EXT_INPUT_0, 	LL_GPIO_OUTPUT_PUSHPULL, LL_GPIO_AF_8, LL_GPIO_SPEED_FREQ_VERY_HIGH, LL_GPIO_PULL_NO );

            hal_uart_dma_init(HAL_UART_PORT_EXTERNAL);
            hal_uart_peripheral_init( h->usart, UART_EXTERNAL_BAUD );

            // Start it up
            LL_DMA_EnableStream(h->dma_peripheral, h->dma_stream_rx);     // rx stream
            LL_USART_Enable(h->usart);
            break;

        case HAL_UART_PORT_INTERNAL:
            h->port = HAL_UART_PORT_INTERNAL;
            h->usart = USART1;
            h->dma_peripheral   = DMA2;
            h->dma_stream_tx    = LL_DMA_STREAM_7;
            h->dma_channel_tx   = LL_DMA_CHANNEL_4;
            h->dma_stream_rx    = LL_DMA_STREAM_2;
            h->dma_channel_rx   = LL_DMA_CHANNEL_4;

            fifo_init( &h->tx_fifo, h->tx_buffer, HAL_UART_TX_FIFO_SIZE );
            fifo_init( &h->rx_fifo, h->rx_buffer, HAL_UART_RX_FIFO_SIZE );

            LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
            LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);

            hal_gpio_init_alternate( _AUX_UART_RX, LL_GPIO_OUTPUT_PUSHPULL, LL_GPIO_AF_7, LL_GPIO_SPEED_FREQ_VERY_HIGH, LL_GPIO_PULL_NO );
            hal_gpio_init_alternate( _AUX_UART_TX, LL_GPIO_OUTPUT_PUSHPULL, LL_GPIO_AF_7, LL_GPIO_SPEED_FREQ_VERY_HIGH, LL_GPIO_PULL_NO );

            hal_uart_dma_init(HAL_UART_PORT_INTERNAL);
            hal_uart_peripheral_init( h->usart, UART_INTERNAL_BAUD );

            LL_DMA_EnableStream(h->dma_peripheral, h->dma_stream_rx);     // rx stream
            LL_USART_Enable(h->usart);
            break;

        case HAL_UART_PORT_MODULE:
            h->port = HAL_UART_PORT_MODULE;
            h->usart = USART2;
            h->dma_peripheral   = DMA1;
            h->dma_stream_tx    = LL_DMA_STREAM_6;
            h->dma_channel_tx   = LL_DMA_CHANNEL_4;
            h->dma_stream_rx    = LL_DMA_STREAM_5;
            h->dma_channel_rx   = LL_DMA_CHANNEL_4;

            fifo_init( &h->tx_fifo, h->tx_buffer, HAL_UART_TX_FIFO_SIZE );
            fifo_init( &h->rx_fifo, h->rx_buffer, HAL_UART_RX_FIFO_SIZE );

            LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
            LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

            hal_gpio_init_alternate( _CARD_UART_RX, LL_GPIO_OUTPUT_PUSHPULL, LL_GPIO_AF_7, LL_GPIO_SPEED_FREQ_VERY_HIGH, LL_GPIO_PULL_NO );
            hal_gpio_init_alternate( _CARD_UART_TX, LL_GPIO_OUTPUT_PUSHPULL, LL_GPIO_AF_7, LL_GPIO_SPEED_FREQ_VERY_HIGH, LL_GPIO_PULL_NO );
//            hal_gpio_init_alternate( _CARD_UART_CTS, LL_GPIO_OUTPUT_PUSHPULL, LL_GPIO_AF_7, LL_GPIO_SPEED_FREQ_VERY_HIGH, LL_GPIO_PULL_NO );
//            hal_gpio_init_alternate( _CARD_UART_RTS, LL_GPIO_OUTPUT_PUSHPULL, LL_GPIO_AF_7, LL_GPIO_SPEED_FREQ_VERY_HIGH, LL_GPIO_PULL_NO );

            hal_uart_dma_init(HAL_UART_PORT_MODULE);
            hal_uart_peripheral_init( h->usart, UART_MODULE_BAUD );

            LL_DMA_EnableStream(h->dma_peripheral, h->dma_stream_rx);     // rx stream
            LL_USART_Enable(h->usart);
            break;

        default:
            break;
    }

}

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_uart_global_deinit( void )
{
    // Disable UART
    LL_USART_DeInit(UART5);
    LL_USART_DeInit(USART1);
    LL_USART_DeInit(USART2);

    // Disable DMA streams
    LL_DMA_DeInit(DMA1, LL_DMA_STREAM_5);
    LL_DMA_DeInit(DMA1, LL_DMA_STREAM_6);

}

/* -------------------------------------------------------------------------- */

/* Non-blocking send for a single character to the UART tx FIFO queue.
 * Returns true when successful. false when queue was full.
 */

PUBLIC bool
hal_uart_put( HalUartPort_t port, uint8_t ch )
{
    HalUart_t * h = &hal_uart[port];


    uint32_t sent = fifo_write( &h->tx_fifo, &ch, 1 );

    hal_uart_start_tx( h );
    return (sent == 1);
}

/* -------------------------------------------------------------------------- */

/** Wait until FIFO cleared. */

PUBLIC void
hal_uart_flush( HalUartPort_t port )
{
    HalUart_t * h = &hal_uart[port];
    uint32_t used;

    do
    {
        used = fifo_used( &h->tx_fifo );
    }
    while( used > 0 );
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
    hal_uart_start_tx( h );
    return (sent == 1);
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
    uint32_t sent = 0;

    if( fifo_free( &h->tx_fifo ) >= length )
    {
        sent = fifo_write( &h->tx_fifo, data, length );
        hal_uart_start_tx( h );
    }

    return sent;
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
 * Returns 0 when no data is available
 */

PUBLIC uint8_t
hal_uart_rx_get( HalUartPort_t port )
{
    HalUart_t * h = &hal_uart[port];

    uint8_t c = 0;
    fifo_read( &h->rx_fifo, &c, 1 );

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

    len = fifo_read( &h->rx_fifo, data, maxlength );

    return len;
}

/* -------------------------------------------------------------------------- */

PRIVATE void
hal_uart_dma_init(HalUartPort_t port)
{
    HalUart_t * h = &hal_uart[port];

    /* USART2_TX Init */
    LL_DMA_SetChannelSelection(h->dma_peripheral, h->dma_stream_tx, h->dma_channel_tx);
    LL_DMA_SetDataTransferDirection(h->dma_peripheral, h->dma_stream_tx, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
    LL_DMA_SetStreamPriorityLevel(h->dma_peripheral, h->dma_stream_tx, LL_DMA_PRIORITY_LOW);
    LL_DMA_SetMode(h->dma_peripheral, h->dma_stream_tx, LL_DMA_MODE_NORMAL);
    LL_DMA_SetPeriphIncMode(h->dma_peripheral, h->dma_stream_tx, LL_DMA_PERIPH_NOINCREMENT);
    LL_DMA_SetMemoryIncMode(h->dma_peripheral, h->dma_stream_tx, LL_DMA_MEMORY_INCREMENT);
    LL_DMA_SetPeriphSize(h->dma_peripheral, h->dma_stream_tx, LL_DMA_PDATAALIGN_BYTE);
    LL_DMA_SetMemorySize(h->dma_peripheral, h->dma_stream_tx, LL_DMA_MDATAALIGN_BYTE);
    LL_DMA_DisableFifoMode(h->dma_peripheral, h->dma_stream_tx);

    LL_DMA_SetPeriphAddress(h->dma_peripheral, h->dma_stream_tx, (uint32_t) &h->usart->DR);
    LL_DMA_EnableIT_TC(h->dma_peripheral, h->dma_stream_tx); /* Enable TX TC interrupt */

    hal_uart_dma_irq_setup( h->dma_peripheral, h->dma_stream_tx, 5, 1);

    /* USART2_RX Init */
    LL_DMA_SetChannelSelection(h->dma_peripheral, h->dma_stream_rx, h->dma_channel_rx);
    LL_DMA_SetDataTransferDirection(h->dma_peripheral, h->dma_stream_rx, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
    LL_DMA_SetStreamPriorityLevel(h->dma_peripheral, h->dma_stream_rx, LL_DMA_PRIORITY_LOW);
    LL_DMA_SetMode(h->dma_peripheral, h->dma_stream_rx, LL_DMA_MODE_CIRCULAR);
    LL_DMA_SetPeriphIncMode(h->dma_peripheral, h->dma_stream_rx, LL_DMA_PERIPH_NOINCREMENT);
    LL_DMA_SetMemoryIncMode(h->dma_peripheral, h->dma_stream_rx, LL_DMA_MEMORY_INCREMENT);
    LL_DMA_SetPeriphSize(h->dma_peripheral, h->dma_stream_rx, LL_DMA_PDATAALIGN_BYTE);
    LL_DMA_SetMemorySize(h->dma_peripheral, h->dma_stream_rx, LL_DMA_MDATAALIGN_BYTE);
    LL_DMA_DisableFifoMode(h->dma_peripheral, h->dma_stream_rx);

    LL_DMA_SetPeriphAddress(h->dma_peripheral, h->dma_stream_rx, (uint32_t) &h->usart->DR);
    LL_DMA_SetMemoryAddress(h->dma_peripheral, h->dma_stream_rx, (uint32_t) h->dma_rx_buffer);
    LL_DMA_SetDataLength(h->dma_peripheral, h->dma_stream_rx, HAL_UART_RX_DMA_BUFFER_SIZE);

    /* Enable HT & TC interrupts */
    LL_DMA_EnableIT_HT(h->dma_peripheral, h->dma_stream_rx);
    LL_DMA_EnableIT_TC(h->dma_peripheral, h->dma_stream_rx);

    hal_uart_dma_irq_setup( h->dma_peripheral, h->dma_stream_rx, 5, 0);
}

/* -------------------------------------------------------------------------- */

PRIVATE void
hal_uart_dma_irq_setup( DMA_TypeDef *DMAx, uint32_t stream, uint8_t preempt_priority, uint8_t sub_priority)
{
    int32_t irq_num = 0;

    if( DMAx == DMA1 )
    {
        switch( stream )
        {
            case LL_DMA_STREAM_0: irq_num = DMA1_Stream0_IRQn; break;
            case LL_DMA_STREAM_1: irq_num = DMA1_Stream1_IRQn; break;
            case LL_DMA_STREAM_2: irq_num = DMA1_Stream2_IRQn; break;
            case LL_DMA_STREAM_3: irq_num = DMA1_Stream3_IRQn; break;
            case LL_DMA_STREAM_4: irq_num = DMA1_Stream4_IRQn; break;
            case LL_DMA_STREAM_5: irq_num = DMA1_Stream5_IRQn; break;
            case LL_DMA_STREAM_6: irq_num = DMA1_Stream6_IRQn; break;
            case LL_DMA_STREAM_7: irq_num = DMA1_Stream7_IRQn; break;
            default:    ASSERT( false );
        }

        NVIC_SetPriority(irq_num, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), preempt_priority, sub_priority));
        NVIC_EnableIRQ(irq_num);
    }
    else if( DMAx == DMA2 )
    {
        switch( stream )
        {
            case LL_DMA_STREAM_0: irq_num = DMA2_Stream0_IRQn; break;
            case LL_DMA_STREAM_1: irq_num = DMA2_Stream1_IRQn; break;
            case LL_DMA_STREAM_2: irq_num = DMA2_Stream2_IRQn; break;
            case LL_DMA_STREAM_3: irq_num = DMA2_Stream3_IRQn; break;
            case LL_DMA_STREAM_4: irq_num = DMA2_Stream4_IRQn; break;
            case LL_DMA_STREAM_5: irq_num = DMA2_Stream5_IRQn; break;
            case LL_DMA_STREAM_6: irq_num = DMA2_Stream6_IRQn; break;
            case LL_DMA_STREAM_7: irq_num = DMA2_Stream7_IRQn; break;
            default:    ASSERT( false );
        }

        NVIC_SetPriority(irq_num, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), preempt_priority, sub_priority));
        NVIC_EnableIRQ(irq_num);
    }
}

/* -------------------------------------------------------------------------- */

PRIVATE void
hal_uart_peripheral_init( USART_TypeDef *USARTx, uint32_t baudrate )
{
    // USART Peripheral Setup
    LL_USART_InitTypeDef USART_InitStruct = {0};

    USART_InitStruct.BaudRate = baudrate;
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
    LL_USART_Init(USARTx, &USART_InitStruct);

    LL_USART_ConfigAsyncMode(USARTx);
    LL_USART_EnableDMAReq_TX(USARTx);
    LL_USART_EnableDMAReq_RX(USARTx);
    LL_USART_EnableIT_IDLE(USARTx);

    // Setup the USART interrupt priorities
    if( USARTx == USART1 )
    {
        NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0));
        NVIC_EnableIRQ(USART1_IRQn);
    }
    else if( USARTx == USART2 )
    {
        NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0));
        NVIC_EnableIRQ(USART2_IRQn);
    }
    else if( USARTx == UART5 )
    {
        NVIC_SetPriority(UART5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0));
        NVIC_EnableIRQ(UART5_IRQn);
    }

}

/* -------------------------------------------------------------------------- */

PRIVATE void hal_uart_clear_dma_tx_flags( DMA_TypeDef *DMAx, uint32_t stream_tx )
{
    // Clear all tx DMA interrupt flags
    switch( stream_tx )
    {
        case LL_DMA_STREAM_0:
            LL_DMA_ClearFlag_TC0(DMAx);
            LL_DMA_ClearFlag_HT0(DMAx);
            LL_DMA_ClearFlag_DME0(DMAx);
            LL_DMA_ClearFlag_FE0(DMAx);
            LL_DMA_ClearFlag_TE0(DMAx);
            break;
        case LL_DMA_STREAM_1:
            LL_DMA_ClearFlag_TC1(DMAx);
            LL_DMA_ClearFlag_HT1(DMAx);
            LL_DMA_ClearFlag_DME1(DMAx);
            LL_DMA_ClearFlag_FE1(DMAx);
            LL_DMA_ClearFlag_TE1(DMAx);
            break;
        case LL_DMA_STREAM_2:
            LL_DMA_ClearFlag_TC2(DMAx);
            LL_DMA_ClearFlag_HT2(DMAx);
            LL_DMA_ClearFlag_DME2(DMAx);
            LL_DMA_ClearFlag_FE2(DMAx);
            LL_DMA_ClearFlag_TE2(DMAx);
            break;
        case LL_DMA_STREAM_3:
            LL_DMA_ClearFlag_TC3(DMAx);
            LL_DMA_ClearFlag_HT3(DMAx);
            LL_DMA_ClearFlag_DME3(DMAx);
            LL_DMA_ClearFlag_FE3(DMAx);
            LL_DMA_ClearFlag_TE3(DMAx);
            break;
        case LL_DMA_STREAM_4:
            LL_DMA_ClearFlag_TC4(DMAx);
            LL_DMA_ClearFlag_HT4(DMAx);
            LL_DMA_ClearFlag_DME4(DMAx);
            LL_DMA_ClearFlag_FE4(DMAx);
            LL_DMA_ClearFlag_TE4(DMAx);
            break;
        case LL_DMA_STREAM_5:
            LL_DMA_ClearFlag_TC5(DMAx);
            LL_DMA_ClearFlag_HT5(DMAx);
            LL_DMA_ClearFlag_DME5(DMAx);
            LL_DMA_ClearFlag_FE5(DMAx);
            LL_DMA_ClearFlag_TE5(DMAx);
            break;
        case LL_DMA_STREAM_6:
            LL_DMA_ClearFlag_TC6(DMAx);
            LL_DMA_ClearFlag_HT6(DMAx);
            LL_DMA_ClearFlag_DME6(DMAx);
            LL_DMA_ClearFlag_FE6(DMAx);
            LL_DMA_ClearFlag_TE6(DMAx);
            break;
        case LL_DMA_STREAM_7:
            LL_DMA_ClearFlag_TC7(DMAx);
            LL_DMA_ClearFlag_HT7(DMAx);
            LL_DMA_ClearFlag_DME7(DMAx);
            LL_DMA_ClearFlag_FE7(DMAx);
            LL_DMA_ClearFlag_TE7(DMAx);
            break;
        default:
            ASSERT( false );
    }
}

/* ------------------------------------------------------------------*/

PRIVATE void
hal_uart_start_tx(HalUart_t *h )
{
    uint32_t primask;

    primask = __get_PRIMASK();
    __disable_irq();

    /* If transfer is not on-going */
    if (!LL_DMA_IsEnabledStream(h->dma_peripheral, h->dma_stream_tx))
    {
        h->tx_sneak_bytes = fifo_used_linear( &h->tx_fifo );

        // Limit maximum size to transmit at a time
        if (h->tx_sneak_bytes > 32)
        {
            h->tx_sneak_bytes = 32;
        }

        // Transmit remaining data
        if (h->tx_sneak_bytes > 0)
        {
            void* ptr = fifo_get_tail_ptr( &h->tx_fifo, h->tx_sneak_bytes );

            LL_DMA_SetDataLength(h->dma_peripheral, h->dma_stream_tx, h->tx_sneak_bytes);
            LL_DMA_SetMemoryAddress(h->dma_peripheral, h->dma_stream_tx, (uint32_t)ptr);

            hal_uart_clear_dma_tx_flags(h->dma_peripheral, h->dma_stream_tx);

            /* Start transfer */
            LL_DMA_EnableStream(h->dma_peripheral, h->dma_stream_tx);
        }
    }

    __set_PRIMASK(primask);
}

PRIVATE void
hal_uart_completed_tx(HalUart_t *h )
{
    fifo_skip( &h->tx_fifo, h->tx_sneak_bytes );
    hal_uart_start_tx( h );
}


/* ------------------------------------------------------------------*/

// Tracks data handled by RX DMA and passes data off for higher-level storage/parsing etc.
// Called when the RX DMA interrupts for half or full buffer fire, and when line-idle occurs

PRIVATE void
hal_usart_irq_rx_handler(HalUart_t *h)
{
    // Calculate current head index
    uint32_t current_pos = HAL_UART_RX_DMA_BUFFER_SIZE - LL_DMA_GetDataLength(h->dma_peripheral, h->dma_stream_rx);

    // Has DMA given us new data?
    if (current_pos != h->dma_rx_pos)
    {
        // Data hasn't hit the end yet
        if (current_pos > h->dma_rx_pos)
        {
            fifo_write( &h->rx_fifo, (const uint8_t *)&h->dma_rx_buffer[h->dma_rx_pos], current_pos - h->dma_rx_pos );
        }
        else    // circular buffer overflowed
        {
            // Read to the end of the buffer
            fifo_write( &h->rx_fifo, (const uint8_t *)&h->dma_rx_buffer[h->dma_rx_pos], DIM(h->dma_rx_buffer) - h->dma_rx_pos );

            // Read from the start of the buffer to the current head
            if (current_pos > 0)
            {
                fifo_write(&h->rx_fifo, (const uint8_t *)&h->dma_rx_buffer[0], current_pos );

            }
        }
    }
    // Remember the current head position
    h->dma_rx_pos = current_pos;

    // Check if we've reached the end of the buffer, move the head to the start
    if (h->dma_rx_pos == HAL_UART_RX_DMA_BUFFER_SIZE)
    {
        h->dma_rx_pos = 0;
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC void
UART5_IRQHandler( void )
{
    // Idle line interrupt occurs when the UART RX line has been high for more than one frame
    if (LL_USART_IsEnabledIT_IDLE(UART5) && LL_USART_IsActiveFlag_IDLE(UART5))
    {
        // Clear IDLE line flag
        LL_USART_ClearFlag_IDLE(UART5);

        // Check for data to process
        hal_usart_irq_rx_handler( &hal_uart[HAL_UART_PORT_EXTERNAL] );
    }
}

// RX
void
DMA1_Stream0_IRQHandler( void )
{
    // Half transfer complete
    if (LL_DMA_IsEnabledIT_HT(DMA1, LL_DMA_STREAM_0) && LL_DMA_IsActiveFlag_HT0(DMA1))
    {
        LL_DMA_ClearFlag_HT0(DMA1);

        // Check for data to process
        hal_usart_irq_rx_handler( &hal_uart[HAL_UART_PORT_EXTERNAL] );
    }

    // Full transfer complete
    if (LL_DMA_IsEnabledIT_TC(DMA1, LL_DMA_STREAM_0) && LL_DMA_IsActiveFlag_TC0(DMA1))
    {
        LL_DMA_ClearFlag_TC0(DMA1);
        hal_usart_irq_rx_handler( &hal_uart[HAL_UART_PORT_EXTERNAL] );
    }
}

// TX
void
DMA1_Stream7_IRQHandler( void )
{
    // Transfer complete
    if (LL_DMA_IsEnabledIT_TC(DMA1, LL_DMA_STREAM_7) && LL_DMA_IsActiveFlag_TC7(DMA1))
    {
        LL_DMA_ClearFlag_TC7(DMA1); // Clear transfer complete flag

        // Flush the data we just finished transferring, and send more as needed
        hal_uart_completed_tx(&hal_uart[HAL_UART_PORT_EXTERNAL]);
    }
}

/* -------------------------------------------------------------------------- */

void
USART1_IRQHandler( void )
{
    if (LL_USART_IsEnabledIT_IDLE(USART1) && LL_USART_IsActiveFlag_IDLE(USART1))
    {
        LL_USART_ClearFlag_IDLE(USART1);
        hal_usart_irq_rx_handler( &hal_uart[HAL_UART_PORT_INTERNAL]  );
    }
}

// RX
void
DMA2_Stream2_IRQHandler( void )
{
    // Half transfer complete
    if (LL_DMA_IsEnabledIT_HT(DMA2, LL_DMA_STREAM_2) && LL_DMA_IsActiveFlag_HT2(DMA2))
    {
        LL_DMA_ClearFlag_HT2(DMA2);
        hal_usart_irq_rx_handler( &hal_uart[HAL_UART_PORT_INTERNAL]  );
    }

    // Full transfer complete
    if (LL_DMA_IsEnabledIT_TC(DMA2, LL_DMA_STREAM_2) && LL_DMA_IsActiveFlag_TC2(DMA2))
    {
        LL_DMA_ClearFlag_TC2(DMA2);
        hal_usart_irq_rx_handler( &hal_uart[HAL_UART_PORT_INTERNAL]  );
    }
}

// TX
void
DMA2_Stream7_IRQHandler( void )
{
    if (LL_DMA_IsEnabledIT_TC(DMA2, LL_DMA_STREAM_7) && LL_DMA_IsActiveFlag_TC7(DMA2))
    {
        LL_DMA_ClearFlag_TC7(DMA2);
        hal_uart_completed_tx( &hal_uart[HAL_UART_PORT_INTERNAL] );
    }
}


/* -------------------------------------------------------------------------- */

void
USART2_IRQHandler( void )
{
    /* Check for IDLE line interrupt */
    if (LL_USART_IsEnabledIT_IDLE(USART2) && LL_USART_IsActiveFlag_IDLE(USART2))
    {
        LL_USART_ClearFlag_IDLE(USART2);
        hal_usart_irq_rx_handler(&hal_uart[HAL_UART_PORT_MODULE]);
    }
}

// RX
void
DMA1_Stream5_IRQHandler( void )
{
    // Half-transfer complete interrupt
    if (LL_DMA_IsEnabledIT_HT(DMA1, LL_DMA_STREAM_5) && LL_DMA_IsActiveFlag_HT5(DMA1))
    {
        LL_DMA_ClearFlag_HT5(DMA1);
        hal_usart_irq_rx_handler(&hal_uart[HAL_UART_PORT_MODULE]);
    }

    // Transfer-complete interrupt
    if (LL_DMA_IsEnabledIT_TC(DMA1, LL_DMA_STREAM_5) && LL_DMA_IsActiveFlag_TC5(DMA1))
    {
        LL_DMA_ClearFlag_TC5(DMA1);
        hal_usart_irq_rx_handler(&hal_uart[HAL_UART_PORT_MODULE]);
    }
}

// TX
void
DMA1_Stream6_IRQHandler( void )
{
    // Check transfer-complete interrupt
    if (LL_DMA_IsEnabledIT_TC(DMA1, LL_DMA_STREAM_6) && LL_DMA_IsActiveFlag_TC6(DMA1))
    {
        LL_DMA_ClearFlag_TC6(DMA1);
        hal_uart_completed_tx(&hal_uart[HAL_UART_PORT_MODULE]);
    }
}

/* ----- End ---------------------------------------------------------------- */

