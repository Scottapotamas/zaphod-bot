/* -------------------------------------------------------------------------- */

#include <string.h>

#include "hal_adc.h"
#include "qassert.h"

#include "stm32f4xx_ll_adc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_dma.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

PRIVATE uint32_t hal_channels[] = {
    LL_ADC_CHANNEL_15,
    LL_ADC_CHANNEL_14,
    LL_ADC_CHANNEL_7,
    LL_ADC_CHANNEL_6,
    LL_ADC_CHANNEL_13,
    LL_ADC_CHANNEL_10,
    LL_ADC_CHANNEL_11,
    LL_ADC_CHANNEL_12,
    LL_ADC_CHANNEL_TEMPSENSOR,
    LL_ADC_CHANNEL_VREFINT,
};

PRIVATE uint32_t hal_ranks[] = {
    LL_ADC_REG_RANK_1,
    LL_ADC_REG_RANK_2,
    LL_ADC_REG_RANK_3,
    LL_ADC_REG_RANK_4,
    LL_ADC_REG_RANK_5,
    LL_ADC_REG_RANK_6,
    LL_ADC_REG_RANK_7,
    LL_ADC_REG_RANK_8,
    LL_ADC_REG_RANK_9,
    LL_ADC_REG_RANK_10,
    LL_ADC_REG_RANK_11,
    LL_ADC_REG_RANK_12,
    LL_ADC_REG_RANK_13,
    LL_ADC_REG_RANK_14,
    LL_ADC_REG_RANK_15,
    LL_ADC_REG_RANK_16
};

/* -------------------------------------------------------------------------- */

PRIVATE void hal_adc_configure_dma( void );
PRIVATE void hal_adc_configure( void );

PRIVATE uint32_t adc_dma[HAL_ADC_INPUT_NUM];         // 'Raw' DMA copy area for ADC readings
PRIVATE ADCDataCallback user_callback = NULL;

/* -------------------------------------------------------------------------- */

PUBLIC void hal_adc_init( ADCDataCallback callback )
{
    memset( &adc_dma, 0, sizeof( adc_dma ) );

    user_callback = callback;

    hal_adc_configure_dma();
    hal_adc_configure();
}

/* -------------------------------------------------------------------------- */

PRIVATE void hal_adc_configure_dma( void )
{
    // Configure DMA channel for ADC
    NVIC_SetPriority( DMA2_Stream0_IRQn, 10 ); /* DMA IRQ lower priority than ADC IRQ */
    NVIC_EnableIRQ( DMA2_Stream0_IRQn );

    LL_AHB1_GRP1_EnableClock( LL_AHB1_GRP1_PERIPH_DMA2 );

    LL_DMA_SetChannelSelection( DMA2, LL_DMA_STREAM_0, LL_DMA_CHANNEL_0 );

    LL_DMA_ConfigTransfer( DMA2,
                           LL_DMA_STREAM_0,
                           LL_DMA_DIRECTION_PERIPH_TO_MEMORY | LL_DMA_MODE_CIRCULAR | LL_DMA_PERIPH_NOINCREMENT | LL_DMA_MEMORY_INCREMENT | LL_DMA_PDATAALIGN_WORD | LL_DMA_MDATAALIGN_WORD | LL_DMA_PRIORITY_MEDIUM );

    // Configure the DMA transfer buffer
    LL_DMA_ConfigAddresses( DMA2,
                            LL_DMA_STREAM_0,
                            LL_ADC_DMA_GetRegAddr( ADC1, LL_ADC_DMA_REG_REGULAR_DATA ),
                            (uint32_t)&adc_dma[HAL_ADC_INPUT_M1_CURRENT],
                            LL_DMA_DIRECTION_PERIPH_TO_MEMORY );

    // Set DMA transfer size
    LL_DMA_SetDataLength( DMA2, LL_DMA_STREAM_0, HAL_ADC_INPUT_NUM );

    // Enable DMA transfer interruption: transfer complete
    LL_DMA_EnableIT_TC( DMA2, LL_DMA_STREAM_0 );

    // Enable DMA transfer interruption: half transfer
    LL_DMA_EnableIT_HT( DMA2, LL_DMA_STREAM_0 );

    // Enable DMA transfer interruption: transfer error
    LL_DMA_EnableIT_TE( DMA2, LL_DMA_STREAM_0 );

    // Enable the DMA transfer
    LL_DMA_EnableStream( DMA2, LL_DMA_STREAM_0 );

}

/* -------------------------------------------------------------------------- */

PRIVATE void hal_adc_configure( void )
{
    // Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
    NVIC_SetPriority( ADC_IRQn, 9 );    /* ADC IRQ needs greater priority than DMA IRQ */
    NVIC_EnableIRQ( ADC_IRQn );

    LL_APB2_GRP1_EnableClock( LL_APB2_GRP1_PERIPH_ADC1 );

    if( __LL_ADC_IS_ENABLED_ALL_COMMON_INSTANCE() == 0 )
    {
        LL_ADC_SetCommonClock( __LL_ADC_COMMON_INSTANCE( ADC1 ), LL_ADC_CLOCK_SYNC_PCLK_DIV2 );
        LL_ADC_SetCommonPathInternalCh( __LL_ADC_COMMON_INSTANCE( ADC1 ), LL_ADC_PATH_INTERNAL_TEMPSENSOR | LL_ADC_PATH_INTERNAL_VREFINT );
    }

    if( LL_ADC_IsEnabled( ADC1 ) == 0 )
    {
        LL_ADC_SetResolution( ADC1, LL_ADC_RESOLUTION_12B );
        LL_ADC_SetResolution( ADC1, LL_ADC_DATA_ALIGN_RIGHT );
        LL_ADC_SetSequencersScanMode( ADC1, LL_ADC_SEQ_SCAN_ENABLE );
    }

    if( LL_ADC_IsEnabled( ADC1 ) == 0 )
    {
        // TODO consider supporting a timer based trigger option
        LL_ADC_REG_SetTriggerSource( ADC1, LL_ADC_REG_TRIG_SOFTWARE );
        // LL_ADC_REG_SetTriggerEdge(ADC1, LL_ADC_REG_TRIG_EXT_RISING);

        LL_ADC_REG_SetContinuousMode( ADC1, LL_ADC_REG_CONV_SINGLE );
        LL_ADC_REG_SetDMATransfer( ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED );
        // LL_ADC_REG_SetFlagEndOfConversion(ADC1, LL_ADC_REG_FLAG_EOC_SEQUENCE_CONV);

        // Set ADC group regular sequencer length and scan direction
        LL_ADC_REG_SetSequencerLength( ADC1, LL_ADC_REG_SEQ_SCAN_ENABLE_10RANKS );
        // LL_ADC_REG_SetSequencerDiscont(ADC1, LL_ADC_REG_SEQ_DISCONT_DISABLE);

        //Configure each ADC channel with its rank in the sequencer, speed, etc.
        for( uint8_t hal_config_channel = 0; hal_config_channel < HAL_ADC_INPUT_NUM; hal_config_channel++ )
        {
            LL_ADC_REG_SetSequencerRanks( ADC1, hal_ranks[hal_config_channel], hal_channels[hal_config_channel] );
            LL_ADC_SetChannelSamplingTime( ADC1, hal_channels[hal_config_channel], LL_ADC_SAMPLINGTIME_480CYCLES );
        }
    }

    LL_ADC_EnableIT_EOCS( ADC1 );
    LL_ADC_EnableIT_OVR( ADC1 );
}

/* -------------------------------------------------------------------------- */

PUBLIC void hal_adc_start( void )
{
    if( LL_ADC_IsEnabled( ADC1 ) == 0 )
    {
        LL_ADC_Enable( ADC1 );
    }

    // LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_0);
    // LL_DMA_EnableStream(DMA2,LL_DMA_STREAM_0);

    if( LL_ADC_IsEnabled( ADC1 ) == 1 )
    {
        if( LL_ADC_REG_IsTriggerSourceSWStart( ADC1 ) == 1 )
        {
            LL_ADC_REG_StartConversionSWStart( ADC1 );
        }
        else
        {
            // Assumes external trigger source is already configured and running,
            // this just allows the ADC to trigger on said source
            LL_ADC_REG_StartConversionExtTrig( ADC1, LL_ADC_REG_TRIG_EXT_RISING );
        }
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC void hal_adc_stop( void )
{
    if( LL_ADC_IsEnabled( ADC1 ) == 1 )
    {
        LL_ADC_Disable( ADC1 );
    }

    if( LL_ADC_REG_IsTriggerSourceSWStart( ADC1 ) == 0 )
    {
        LL_ADC_REG_StopConversionExtTrig( ADC1 );
    }
}

/* -------------------------------------------------------------------------- */

void ADC_IRQHandler( void )
{
    // ADC group regular overrun caused the ADC interruption
    if( LL_ADC_IsActiveFlag_OVR( ADC1 ) != 0 )
    {
        LL_ADC_ClearFlag_OVR( ADC1 );    // Clear flag ADC group regular overrun
        LL_ADC_DisableIT_OVR( ADC1 );    // Disable overrun interrupt

        ASSERT(false);
    }
}

void DMA2_Stream0_IRQHandler( void )
{
    // DMA half transfer caused the DMA interruption
    if( LL_DMA_IsActiveFlag_HT0( DMA2 ) == 1 )
    {
        LL_DMA_ClearFlag_HT0( DMA2 );    // Clear flag DMA half transfer

        // Handle the first half of the DMA samples
        if( user_callback )
        {
            user_callback( HAL_ADC_INPUT_M1_CURRENT, adc_dma[HAL_ADC_INPUT_M1_CURRENT] );
            user_callback( HAL_ADC_INPUT_M2_CURRENT, adc_dma[HAL_ADC_INPUT_M2_CURRENT] );
            user_callback( HAL_ADC_INPUT_M3_CURRENT, adc_dma[HAL_ADC_INPUT_M3_CURRENT] );
            user_callback( HAL_ADC_INPUT_M4_CURRENT, adc_dma[HAL_ADC_INPUT_M4_CURRENT] );
            user_callback( HAL_ADC_INPUT_VOLT_SENSE, adc_dma[HAL_ADC_INPUT_VOLT_SENSE] );
        }
    }

    // DMA transfer complete caused the DMA interruption
    if( LL_DMA_IsActiveFlag_TC0( DMA2 ) == 1 )
    {
        LL_DMA_ClearFlag_TC0( DMA2 );    // Clear flag DMA transfer complete

        // Handle the second half of the DMA samples
        if( user_callback )
        {
            user_callback( HAL_ADC_INPUT_TEMP_PCB, adc_dma[HAL_ADC_INPUT_TEMP_PCB] );
            user_callback( HAL_ADC_INPUT_TEMP_REG, adc_dma[HAL_ADC_INPUT_TEMP_REG] );
            user_callback( HAL_ADC_INPUT_TEMP_EXT, adc_dma[HAL_ADC_INPUT_TEMP_EXT] );
            user_callback( HAL_ADC_INPUT_TEMP_INTERNAL, adc_dma[HAL_ADC_INPUT_TEMP_INTERNAL] );
            user_callback( HAL_ADC_INPUT_VREFINT, adc_dma[HAL_ADC_INPUT_VREFINT] );
        }
    }

    // DMA transfer error caused the DMA interruption
    if( LL_DMA_IsActiveFlag_TE0( DMA2 ) == 1 )
    {
        LL_DMA_ClearFlag_TE0( DMA2 );    // Clear flag DMA transfer error
        ASSERT(false);
    }
}

/* -------------------------------------------------------------------------- */