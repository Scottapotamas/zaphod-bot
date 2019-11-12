/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "hal_adc.h"
#include "hal_dma.h"
#include "hal_gpio.h"

#include "qassert.h"
#include "app_config.h"
#include "average_short.h"

/* ---------------- Lower Level Peripheral ---------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

PRIVATE uint32_t hal_channels[] = 	{
									ADC_CHANNEL_15,
									ADC_CHANNEL_14,
									ADC_CHANNEL_7,
									ADC_CHANNEL_6,
									ADC_CHANNEL_13,
									ADC_CHANNEL_10,
									ADC_CHANNEL_11,
									ADC_CHANNEL_12,
									ADC_CHANNEL_TEMPSENSOR,
									ADC_CHANNEL_VREFINT,
									};

/* ---------------- Higher Level Processing ---------------------------------- */

typedef struct
{
    bool     running;
    bool     done;
    uint16_t tick;
    uint16_t rate;
} HalAdcState_t;

PRIVATE uint16_t       adc_rate[HAL_ADC_INPUT_NUM];
PRIVATE uint8_t        adc_enabled[HAL_ADC_INPUT_NUM];
PRIVATE uint32_t       adc_dma[HAL_ADC_INPUT_NUM];
PRIVATE uint32_t       adc_channels[HAL_ADC_INPUT_NUM];
PRIVATE uint32_t       adc_peaks[HAL_ADC_INPUT_NUM];
PRIVATE AverageShort_t adc_averages[HAL_ADC_INPUT_NUM];

PRIVATE HalAdcState_t  hal_adc1;

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
	average_short_init( &adc_averages[HAL_ADC_INPUT_M1_CURRENT],  32 );
	average_short_init( &adc_averages[HAL_ADC_INPUT_M2_CURRENT],  32 );
	average_short_init( &adc_averages[HAL_ADC_INPUT_M3_CURRENT],  32 );
	average_short_init( &adc_averages[HAL_ADC_INPUT_M4_CURRENT],  32 );
	average_short_init( &adc_averages[HAL_ADC_INPUT_VOLT_SENSE],  32 );
	average_short_init( &adc_averages[HAL_ADC_INPUT_TEMP_PCB],    32 );
	average_short_init( &adc_averages[HAL_ADC_INPUT_TEMP_REG],    32 );
	average_short_init( &adc_averages[HAL_ADC_INPUT_TEMP_EXT],    32 );
	average_short_init( &adc_averages[HAL_ADC_INPUT_TEMP_INTERNAL], 32 );

	//Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	hadc1.Instance 						= ADC1;
	hadc1.Init.ClockPrescaler 			= ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc1.Init.Resolution 				= ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode 			= ENABLE;
	hadc1.Init.ContinuousConvMode 		= ENABLE;
	hadc1.Init.DiscontinuousConvMode 	= DISABLE;
	hadc1.Init.ExternalTrigConvEdge 	= ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv 		= ADC_SOFTWARE_START;
	hadc1.Init.DataAlign 				= ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion 			= 10;
	hadc1.Init.DMAContinuousRequests 	= DISABLE;
	hadc1.Init.EOCSelection 			= ADC_EOC_SEQ_CONV;

	//Initialise the ADC peripheral
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	//Configure each ADC channel with its rank in the sequencer, speed, etc.
	ADC_ChannelConfTypeDef sConfig;

	for(int hal_config_channel = 1;
			hal_config_channel < HAL_ADC_INPUT_NUM;
			hal_config_channel++)
	{
		sConfig.Rank = hal_config_channel;
		sConfig.Channel = hal_channels[hal_config_channel - 1];
		sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;

		if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}
	}

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
    /* Increment reference count for this input */
    adc_enabled[input] = (uint8_t)(adc_enabled[input] + 1);

    /* Clear the peak history */
    adc_peaks[input] = 0;

    /* Set the interval to poll this */
    adc_rate[input] = poll_rate_ms;

    /* Check if ADC needs enabling */
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
        hal_adc1.running = true;
        hal_adc1.done    = true;   /* Allow first conversion */
    }

}

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_adc_stop( HalAdcInput_t input )
{
    REQUIRE( input < HAL_ADC_INPUT_NUM );

    /* Decrement reference count for this input */
    if( adc_enabled[input] > 0 )
    {
        adc_enabled[input] = (uint8_t)(adc_enabled[input] - 1);
    }

    /* If channel is OFF, scan for ADC controller to switch off */
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

        /* If no longer enabled, turn off the sampling DMA */
        if( !enabled )
        {
            hal_adc1.running = false;
            HAL_ADC_Stop_DMA( &hadc1 );
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
                HAL_ADC_Stop_DMA( &hadc1 );
                HAL_ADC_Start_DMA( &hadc1, &adc_dma[HAL_ADC_INPUT_M1_CURRENT], HAL_ADC_INPUT_NUM );
                hal_adc1.done = false;
            }
        }
    }

}

/* -------------------------------------------------------------------------- */

void HAL_ADC_ConvCpltCallback( ADC_HandleTypeDef* hadc )
{
    if( hadc == &hadc1 )
    {
        /* Freeze the DMA collected samples */
        memcpy( &adc_channels[HAL_ADC_INPUT_M1_CURRENT],
                &adc_dma[HAL_ADC_INPUT_M1_CURRENT],
				HAL_ADC_INPUT_NUM * sizeof( adc_channels[0] ) );

        /* Run them though the averaging */
        for( uint8_t chan = HAL_ADC_INPUT_M1_CURRENT;
                     chan < HAL_ADC_INPUT_VREFINT;
                     chan++ )
        {
            average_short_update( &adc_averages[chan], (uint16_t)adc_channels[chan] );
            adc_peaks[chan] = MAX( adc_peaks[chan], adc_channels[chan] );
        }
        hal_adc1.done = true;
    }

}

/* -------------------------------------------------------------------------- */

void HAL_ADC_ErrorCallback( ADC_HandleTypeDef* hadc __attribute__((unused)) )
{
    //asm("nop");
}

//This is called by the depths of the STM32 HAL, and this function overrides ST's weakly defined one.
void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{
  if( adcHandle->Instance == ADC1 )
  {
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();

    //Initialise the GPIO as analog mode if they haven't been already
    	//this should be done as part of the default setup in the hal_gpio called during the hardware setup

    //ADC1 DMA setup
    hdma_adc1.Instance 					= DMA2_Stream0;
    hdma_adc1.Init.Channel 				= DMA_CHANNEL_0;
    hdma_adc1.Init.Direction 			= DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc 			= DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc 				= DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment 	= DMA_PDATAALIGN_WORD;
    hdma_adc1.Init.MemDataAlignment 	= DMA_MDATAALIGN_WORD;
    hdma_adc1.Init.Mode 				= DMA_CIRCULAR;
    hdma_adc1.Init.Priority 			= DMA_PRIORITY_LOW;
    hdma_adc1.Init.FIFOMode 			= DMA_FIFOMODE_DISABLE;

    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(adcHandle, DMA_Handle, hdma_adc1);
  }

}

//Also called in depths of the STM32 HAL, overrides ST's weakly defined one.
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{
  if( adcHandle->Instance == ADC1 )
  {
    //Disable the ADC clock
    __HAL_RCC_ADC1_CLK_DISABLE();

    //De-init IO
    hal_gpio_disable_pin( _SERVO_1_CURRENT	);
    hal_gpio_disable_pin( _SERVO_2_CURRENT	);
    hal_gpio_disable_pin( _SERVO_3_CURRENT	);
    hal_gpio_disable_pin( _SERVO_4_CURRENT	);
    hal_gpio_disable_pin( _TEMP_PCB_AMBIENT	);
    hal_gpio_disable_pin( _TEMP_PCB_PSU		);
    hal_gpio_disable_pin( _TEMP_EXTERNAL	);
    hal_gpio_disable_pin( _VOLTAGE_SENSE	);

    //Detach the DMA handler
    HAL_DMA_DeInit(adcHandle->DMA_Handle);
  }
}


/* ----- End ---------------------------------------------------------------- */
