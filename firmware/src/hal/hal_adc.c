/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "stm32f4xx_ll_adc.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_bus.h"

#include "hal_adc.h"

#include "qassert.h"
#include "app_config.h"
#include "average_short.h"

/* ---------------- Lower Level Peripheral ---------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

PRIVATE uint32_t hal_channels[] = 	{
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
/* ---------------- Higher Level Processing ---------------------------------- */

typedef struct
{
    bool     running;
    bool     done;
    uint16_t tick;
    uint16_t rate;
} HalAdcState_t;

PRIVATE uint32_t       adc_dma[HAL_ADC_INPUT_NUM];      // 'Raw' DMA copy area for ADC readings
PRIVATE uint32_t       adc_channels[HAL_ADC_INPUT_NUM]; // Destination 'userspace' ADC readings

PRIVATE uint16_t       adc_rate[HAL_ADC_INPUT_NUM];     // Track the requested rate per-channel
PRIVATE uint8_t        adc_enabled[HAL_ADC_INPUT_NUM];  // Track if each channel is needed or not
PRIVATE uint32_t       adc_peaks[HAL_ADC_INPUT_NUM];    // Track highest value
PRIVATE AverageShort_t adc_averages[HAL_ADC_INPUT_NUM]; // Track average value

PRIVATE HalAdcState_t  hal_adc1;                        // Track behaviour for our peripheral (running, set rate etc)

/* ------------------------- Functions -------------------------------------- */

PUBLIC void
hal_adc_init( void )
{
	//setup the higher level handling of ADC readings
	memset( &hal_adc1,     0, sizeof( hal_adc1 ) );
	memset( &adc_peaks,    0, sizeof( adc_peaks ) );
	memset( &adc_rate,     0, sizeof( adc_rate ) );
	memset( &adc_enabled,  0, sizeof( adc_enabled ) );
	memset( &adc_dma,      0, sizeof( adc_dma ) );
	memset( &adc_channels, 0, sizeof( adc_channels ) );
	memset( &adc_averages, 0, sizeof( adc_averages ) );

	average_short_init( &adc_averages[HAL_ADC_INPUT_VREFINT], 	  32 );
	average_short_init( &adc_averages[HAL_ADC_INPUT_M1_CURRENT],  50 );
	average_short_init( &adc_averages[HAL_ADC_INPUT_M2_CURRENT],  50 );
	average_short_init( &adc_averages[HAL_ADC_INPUT_M3_CURRENT],  50 );
	average_short_init( &adc_averages[HAL_ADC_INPUT_M4_CURRENT],  50 );
	average_short_init( &adc_averages[HAL_ADC_INPUT_VOLT_SENSE],  32 );
	average_short_init( &adc_averages[HAL_ADC_INPUT_TEMP_PCB],    32 );
	average_short_init( &adc_averages[HAL_ADC_INPUT_TEMP_REG],    32 );
	average_short_init( &adc_averages[HAL_ADC_INPUT_TEMP_EXT],    32 );
	average_short_init( &adc_averages[HAL_ADC_INPUT_TEMP_INTERNAL], 32 );

    hal_adc1.running = false;
    hal_adc1.done = false;


    // Configure DMA channel for ADC
    NVIC_SetPriority(DMA2_Stream0_IRQn, 3); /* DMA IRQ lower priority than ADC IRQ */
    NVIC_EnableIRQ(DMA2_Stream0_IRQn);

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);

    LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_0, LL_DMA_CHANNEL_0);

    LL_DMA_ConfigTransfer(DMA2,
                          LL_DMA_STREAM_0,
                          LL_DMA_DIRECTION_PERIPH_TO_MEMORY |
                          LL_DMA_MODE_CIRCULAR              |
                          LL_DMA_PERIPH_NOINCREMENT         |
                          LL_DMA_MEMORY_INCREMENT           |
                          LL_DMA_PDATAALIGN_WORD        |
                          LL_DMA_MDATAALIGN_WORD        |
                          LL_DMA_PRIORITY_MEDIUM               );

    // Configure the DMA transfer buffer
    LL_DMA_ConfigAddresses(DMA2,
                           LL_DMA_STREAM_0,
                           LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
                           (uint32_t)&adc_dma[HAL_ADC_INPUT_M1_CURRENT],
                           LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

    // Set DMA transfer size
    LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_0, HAL_ADC_INPUT_NUM);

    // Enable DMA transfer interruption: transfer complete
    LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_0);

    // Enable DMA transfer interruption: half transfer
    LL_DMA_EnableIT_HT(DMA2, LL_DMA_STREAM_0);

    // Enable DMA transfer interruption: transfer error
    LL_DMA_EnableIT_TE(DMA2, LL_DMA_STREAM_0);

    // Enable the DMA transfer */
    LL_DMA_EnableStream(DMA2,LL_DMA_STREAM_0);


    //Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
    NVIC_SetPriority(ADC_IRQn, 2); /* ADC IRQ greater priority than DMA IRQ */
    NVIC_EnableIRQ(ADC_IRQn);

    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);

    if(__LL_ADC_IS_ENABLED_ALL_COMMON_INSTANCE() == 0)
    {
        LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_CLOCK_SYNC_PCLK_DIV2);
        LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_TEMPSENSOR | LL_ADC_PATH_INTERNAL_VREFINT);
    }

    if (LL_ADC_IsEnabled(ADC1) == 0)
    {
        LL_ADC_SetResolution(ADC1, LL_ADC_RESOLUTION_12B);
        LL_ADC_SetResolution(ADC1, LL_ADC_DATA_ALIGN_RIGHT);
        LL_ADC_SetSequencersScanMode(ADC1, LL_ADC_SEQ_SCAN_ENABLE);
    }

    if (LL_ADC_IsEnabled(ADC1) == 0)
    {
        LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);
        // LL_ADC_REG_SetTriggerEdge(ADC1, LL_ADC_REG_TRIG_EXT_RISING);
        LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);
        LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
        // LL_ADC_REG_SetFlagEndOfConversion(ADC1, LL_ADC_REG_FLAG_EOC_SEQUENCE_CONV);

        // Set ADC group regular sequencer length and scan direction
        LL_ADC_REG_SetSequencerLength(ADC1, LL_ADC_REG_SEQ_SCAN_ENABLE_10RANKS);
        // LL_ADC_REG_SetSequencerDiscont(ADC1, LL_ADC_REG_SEQ_DISCONT_DISABLE);

        //Configure each ADC channel with its rank in the sequencer, speed, etc.
        for( uint8_t hal_config_channel = 0; hal_config_channel < HAL_ADC_INPUT_NUM; hal_config_channel++ )
        {
            LL_ADC_REG_SetSequencerRanks(ADC1, hal_ranks[hal_config_channel], hal_channels[hal_config_channel]);
            LL_ADC_SetChannelSamplingTime(ADC1, hal_channels[hal_config_channel], LL_ADC_SAMPLINGTIME_480CYCLES);
        }
    }

    LL_ADC_EnableIT_EOCS(ADC1);
    LL_ADC_EnableIT_OVR(ADC1);
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
hal_adc_valid( HalAdcInput_t input )
{
    REQUIRE( input < HAL_ADC_INPUT_NUM );
    return adc_averages[input].counter > 0;
}

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
hal_adc_read( HalAdcInput_t input )
{
    REQUIRE( input < HAL_ADC_INPUT_NUM );
    return (uint32_t)average_short_get_last( &adc_averages[input] );
}

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
hal_adc_read_avg( HalAdcInput_t input )
{
    REQUIRE( input < HAL_ADC_INPUT_NUM );
    return (uint32_t)average_short_get_average( &adc_averages[input] );
}

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
hal_adc_read_peak( HalAdcInput_t input )
{
    REQUIRE( input < HAL_ADC_INPUT_NUM );
    return adc_peaks[input];
}

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_adc_start( HalAdcInput_t input, uint16_t poll_rate_ms )
{
    // Increment reference count for this input
    adc_enabled[input] = (uint8_t)(adc_enabled[input] + 1);

    // Clear the peak history
    adc_peaks[input] = 0;

    // Set the interval to poll this
    adc_rate[input] = poll_rate_ms;

    // Check if ADC needs enabling
    bool enabled = false;
    hal_adc1.rate = UINT16_MAX;

    for( uint8_t chan = HAL_ADC_INPUT_M1_CURRENT;
                 chan < HAL_ADC_INPUT_VREFINT;
                 chan++ )
    {
        if( adc_enabled[chan] > 0 )
        {
            hal_adc1.rate = MIN( hal_adc1.rate, adc_rate[chan] );
            enabled |= true;
        }
    }

    if( !hal_adc1.running && enabled )
    {
        if( LL_ADC_IsEnabled(ADC1) == 0 )
        {
            LL_ADC_Enable(ADC1);

            hal_adc1.running = true;
            hal_adc1.done    = true;   // Allow first conversion
        }
    }

}

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_adc_stop( HalAdcInput_t input )
{
    REQUIRE( input < HAL_ADC_INPUT_NUM );

    // Decrement reference count for this input
    if( adc_enabled[input] > 0 )
    {
        adc_enabled[input] = (uint8_t)(adc_enabled[input] - 1);
    }

    // If channel is OFF, scan for ADC controller to switch off
    if( adc_enabled[input] == 0 )
    {
        bool enabled = false;
        for( uint8_t chan = HAL_ADC_INPUT_M1_CURRENT;
                     chan < HAL_ADC_INPUT_VREFINT;
                     chan++ )
        {
            if( adc_enabled[chan] > 0 )
            {
                enabled = true;
                break;
            }
        }

        // If no longer enabled, turn off the sampling DMA
        if( !enabled )
        {
            hal_adc1.running = false;

            if( LL_ADC_IsEnabled(ADC1) == 1 )
            {
                LL_ADC_Disable(ADC1);
            }

            // TODO disable ADC DMA when stopped?
        }

    }
}

/* -------------------------------------------------------------------------- */

/** Timer tick to trigger a ADC conversion cycle */

PUBLIC void
hal_adc_tick( void )
{
    if( hal_adc1.running )
    {
        if( hal_adc1.done )
        {
            hal_adc1.tick++;
            if( hal_adc1.tick > hal_adc1.rate )
            {
                hal_adc1.tick = 0;

//                LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_0);
//                LL_DMA_EnableStream(DMA2,LL_DMA_STREAM_0);

                if( LL_ADC_IsEnabled(ADC1) == 1 )
                {
                    LL_ADC_REG_StartConversionSWStart(ADC1);
                }

                hal_adc1.done = false;
            }
        }
    }

}

/* -------------------------------------------------------------------------- */

void ADC_IRQHandler(void)
{
    // ADC group regular overrun caused the ADC interruption
    if( LL_ADC_IsActiveFlag_OVR(ADC1) != 0 )
    {
        LL_ADC_ClearFlag_OVR(ADC1); // Clear flag ADC group regular overrun

        // Disable overrun interrupt
        LL_ADC_DisableIT_OVR(ADC1);

        // TODO Gracefully recover when ADC overrun error occurs
        asm("nop");
    }
}

void DMA2_Stream0_IRQHandler(void)
{
    // DMA half transfer caused the DMA interruption
    if(LL_DMA_IsActiveFlag_HT0(DMA2) == 1)
    {
        LL_DMA_ClearFlag_HT0(DMA2); // Clear flag DMA half transfer

        // Freeze the first half of the DMA samples
        memcpy( &adc_channels[HAL_ADC_INPUT_M1_CURRENT],
                &adc_dma[HAL_ADC_INPUT_M1_CURRENT],
                (HAL_ADC_INPUT_NUM / 2) * sizeof(adc_channels[0]));

        /* Run them though the averaging */
        for( uint8_t chan = HAL_ADC_INPUT_M1_CURRENT;
             chan < (HAL_ADC_INPUT_NUM)/2;
             chan++ )
        {
            average_short_update(&adc_averages[chan], (uint16_t) adc_channels[chan]);
            adc_peaks[chan] = MAX(adc_peaks[chan], adc_channels[chan]);
        }

    }

    // DMA transfer complete caused the DMA interruption
    if(LL_DMA_IsActiveFlag_TC0(DMA2) == 1)
    {

        LL_DMA_ClearFlag_TC0(DMA2); // Clear flag DMA transfer complete

        // Freeze the second half of the DMA samples
        memcpy( &adc_channels[HAL_ADC_INPUT_NUM / 2],
                &adc_dma[HAL_ADC_INPUT_NUM / 2],
                (HAL_ADC_INPUT_NUM / 2) * sizeof(adc_channels[0]));

        /* Run them though the averaging */
        for( uint8_t chan = HAL_ADC_INPUT_NUM / 2;
             chan < (HAL_ADC_INPUT_NUM);
             chan++ )
        {
            average_short_update(&adc_averages[chan], (uint16_t) adc_channels[chan]);
            adc_peaks[chan] = MAX(adc_peaks[chan], adc_channels[chan]);
        }

        hal_adc1.done = true;
    }

    // DMA transfer error caused the DMA interruption
    if(LL_DMA_IsActiveFlag_TE0(DMA2) == 1)
    {
        LL_DMA_ClearFlag_TE0(DMA2); // Clear flag DMA transfer error

        // TODO Handle adc DMA errors?
        asm("nop");
    }
}

/* ----- End ---------------------------------------------------------------- */
