/* ----- System Includes ---------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "hal_adc.h"
#include "qassert.h"
#include "app_config.h"
#include "adc.h"
#include "average_short.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ------------------------- Average ---------------------------------------- */

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
PRIVATE HalAdcState_t  hal_adc3;

/* ------------------------- Functions -------------------------------------- */

PUBLIC void
hal_adc_init( void )
{
    memset( &hal_adc1,     0, sizeof( hal_adc1 ) );
    memset( &hal_adc3,     0, sizeof( hal_adc3 ) );
    memset( &adc_peaks,    0, sizeof( adc_peaks ) );
    memset( &adc_rate,     0, sizeof( adc_rate ) );
    memset( &adc_enabled,  0, sizeof( adc_enabled ) );
    memset( &adc_dma,      0, sizeof( adc_dma ) );
    memset( &adc_channels, 0, sizeof( adc_channels ) );
    memset( &adc_averages, 0, sizeof( adc_averages ) );

    average_short_init( &adc_averages[HAL_ADC_INPUT_VBACKUPBAT],  32 );
    average_short_init( &adc_averages[HAL_ADC_INPUT_VREFINT],     32 );
    average_short_init( &adc_averages[HAL_ADC_INPUT_TEMPERATURE], 32 );
    average_short_init( &adc_averages[HAL_ADC_INPUT_PWR_I5],      32 );
    average_short_init( &adc_averages[HAL_ADC_INPUT_PWR_I3V3],    32 );
    average_short_init( &adc_averages[HAL_ADC_INPUT_M1_CURRENT],  32 );
    average_short_init( &adc_averages[HAL_ADC_INPUT_M2_CURRENT],  10 );
    average_short_init( &adc_averages[HAL_ADC_INPUT_PWR_VBAT],    32 );
    average_short_init( &adc_averages[HAL_ADC_INPUT_SPARE],       32 );
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

    /* Check if ADC1 needs enabling */
    bool enabled = false;
    hal_adc1.rate = UINT16_MAX;
    for( uint8_t chan = HAL_ADC_INPUT_VBACKUPBAT;
                 chan < HAL_ADC_INPUT_M1_CURRENT;
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
        /* ADC 1 */
        bool enabled = false;
        for( uint8_t chan = HAL_ADC_INPUT_VBACKUPBAT;
                     chan < HAL_ADC_INPUT_M1_CURRENT;
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
    /* ADC 1 */
    if( hal_adc1.running )
    {
        if( hal_adc1.done )
        {
            hal_adc1.tick++;
            if( hal_adc1.tick > hal_adc1.rate )
            {
                hal_adc1.tick = 0;
                HAL_ADC_Stop_DMA( &hadc1 );
                HAL_ADC_Start_DMA( &hadc1, &adc_dma[HAL_ADC_INPUT_VBACKUPBAT], 5 );
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
        memcpy( &adc_channels[HAL_ADC_INPUT_VBACKUPBAT],
                &adc_dma[HAL_ADC_INPUT_VBACKUPBAT],
                5 * sizeof( adc_channels[0] ) );

        /* Run them though the averaging */
        for( uint8_t chan = HAL_ADC_INPUT_VBACKUPBAT;
                     chan < HAL_ADC_INPUT_M1_CURRENT;
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

/* ----- End ---------------------------------------------------------------- */
