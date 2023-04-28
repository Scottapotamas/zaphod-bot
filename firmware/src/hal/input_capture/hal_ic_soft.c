/* -------------------------------------------------------------------------- */

#include <string.h>

#include "average_short.h"
#include "hal_gpio.h"
#include "hal_ic_soft.h"
#include "hal_systick.h"
#include "qassert.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* -------------------------------------------------------------------------- */

typedef struct
{
    bool     running;
    uint16_t tick;
    uint16_t rate;
} HalSoftICState_t;

PRIVATE uint16_t       soft_ic_rate[HAL_IC_SOFT_NUM];
PRIVATE uint8_t        soft_ic_enabled[HAL_IC_SOFT_NUM];
PRIVATE AverageShort_t soft_ic_averages[HAL_IC_SOFT_NUM];

PRIVATE bool     soft_ic_last_state[HAL_IC_SOFT_NUM];
PRIVATE uint32_t soft_ic_timer[HAL_IC_SOFT_NUM];
PRIVATE uint16_t soft_ic_duration[HAL_IC_SOFT_NUM];

PRIVATE uint8_t soft_ic_gpio[] = {
    _FAN_TACHO,
};

PRIVATE HalSoftICState_t hal_soft_ic1;

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_ic_soft_init( void )
{
    // Setup the higher level handling of ADC readings
    memset( &hal_soft_ic1, 0, sizeof( hal_soft_ic1 ) );
    memset( &soft_ic_rate, 0, sizeof( soft_ic_rate ) );
    memset( &soft_ic_enabled, 0, sizeof( soft_ic_enabled ) );
    memset( &soft_ic_averages, 0, sizeof( soft_ic_averages ) );
    memset( &soft_ic_last_state, 0, sizeof( soft_ic_last_state ) );
    memset( &soft_ic_timer, 0, sizeof( soft_ic_timer ) );
    memset( &soft_ic_duration, 0, sizeof( soft_ic_duration ) );

    average_short_init( &soft_ic_averages[HAL_IC_SOFT_HALL], 5 );
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
hal_ic_soft_valid( HalSoftICInput_t input )
{
    REQUIRE( input < HAL_IC_SOFT_NUM );
    return soft_ic_averages[input].counter > 0;
}

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
hal_ic_soft_read( HalSoftICInput_t input )
{
    REQUIRE( input < HAL_IC_SOFT_NUM );
    return (uint32_t)average_short_get_last( &soft_ic_averages[input] );
}

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
hal_ic_soft_read_avg( HalSoftICInput_t input )
{
    REQUIRE( input < HAL_IC_SOFT_NUM );
    return (uint32_t)average_short_get_average( &soft_ic_averages[input] );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_ic_soft_start( HalSoftICInput_t input, uint16_t poll_rate_ms )
{
    /* Increment reference count for this input */
    soft_ic_enabled[input] = ( uint8_t )( soft_ic_enabled[input] + 1 );

    /* Set the interval to poll this */
    soft_ic_rate[input] = poll_rate_ms;

    /* Check if ADC needs enabling */
    bool enabled      = false;
    hal_soft_ic1.rate = UINT16_MAX;

    for( uint8_t chan = 0; chan < HAL_IC_SOFT_NUM; chan++ )
    {
        if( soft_ic_enabled[chan] > 0 )
        {
            hal_soft_ic1.rate = MIN( hal_soft_ic1.rate, soft_ic_rate[chan] );
            enabled |= true;
        }
    }

    if( !hal_soft_ic1.running && enabled )
    {
        hal_soft_ic1.running = true;
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_ic_soft_stop( HalSoftICInput_t input )
{
    REQUIRE( input < HAL_IC_SOFT_NUM );

    /* Decrement reference count for this input */
    if( soft_ic_enabled[input] > 0 )
    {
        soft_ic_enabled[input] = ( uint8_t )( soft_ic_enabled[input] - 1 );
    }

    /* If a given channel is OFF, scan for soft input capture controller to switch off */
    if( soft_ic_enabled[input] == 0 )
    {
        bool enabled = false;
        for( uint8_t chan = 0; chan < HAL_IC_SOFT_NUM; chan++ )
        {
            if( soft_ic_enabled[chan] > 0 )
            {
                enabled = true;
                break;
            }
        }

        /* If no longer enabled, turn off this 'module' */
        if( !enabled )
        {
            hal_soft_ic1.running = false;
        }
    }
}

/* -------------------------------------------------------------------------- */

/** Timer tick calls this, checks pins and calculates edge width */

PUBLIC void
hal_ic_soft_tick( void )
{
    // Only sample when something wants us to run
    if( hal_soft_ic1.running )
    {
        hal_soft_ic1.tick++;

        // See if time to sample has arrived (at rate of the fastest channel)
        if( hal_soft_ic1.tick > hal_soft_ic1.rate )
        {
            hal_soft_ic1.tick = 0;

            //check all the managed pins
            for( uint8_t chan = 0; chan < HAL_IC_SOFT_NUM; chan++ )
            {
                //only perform capture on the pin if something(s) wants acquisitions
                if( soft_ic_enabled[chan] > 0 )
                {
                    //see what state the pin is in
                    if( hal_gpio_read_pin( soft_ic_gpio[chan] ) )
                    {
                        //check for the rising edge (compare to previous state)
                        if( soft_ic_last_state[chan] == false )
                        {
                            uint32_t timestamp     = hal_systick_get_ms();
                            soft_ic_duration[chan] = timestamp - soft_ic_timer[chan];

                            //update average of the time between rising edges
                            average_short_update( &soft_ic_averages[chan], (uint16_t)soft_ic_duration[chan] );

                            soft_ic_timer[chan] = timestamp;    //prepare for next edge
                        }

                        soft_ic_last_state[chan] = true;
                    }
                    else
                    {
                        soft_ic_last_state[chan] = false;
                    }
                }
            }
        }
    }
}

/* -------------------------------------------------------------------------- */
