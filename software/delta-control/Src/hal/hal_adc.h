#ifndef HAL_ADC_H
#define HAL_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include "stm32f4xx_hal.h"

/* ----- Types ------------------------------------------------------------- */

typedef enum
{
    HAL_ADC_INPUT_M1_CURRENT,   /* ADC1-15 - RANK 1 */
    HAL_ADC_INPUT_M2_CURRENT,   /* ADC1-14 - RANK 2 */
    HAL_ADC_INPUT_M3_CURRENT,  	/* ADC1-7 - RANK 3 */
    HAL_ADC_INPUT_M4_CURRENT,   /* ADC1-6 - RANK 4 */
    HAL_ADC_INPUT_VOLT_SENSE,   /* ADC1-13 - RANK 5 */
    HAL_ADC_INPUT_TEMP_PCB,   	/* ADC1-10 - RANK 1 */
    HAL_ADC_INPUT_TEMP_REG,   	/* ADC1-11 - RANK 2 */
    HAL_ADC_INPUT_TEMP_EXT,     /* ADC1-12 - RANK 3 */
	HAL_ADC_INPUT_TEMP_INTERNAL,  /* ADC1-TEMP - RANK 4 */
	HAL_ADC_INPUT_VREFINT,        /* ADC1-VREF - RANK 4 */
    HAL_ADC_INPUT_NUM
} HalAdcInput_t;

extern ADC_HandleTypeDef hadc1;

/* ------------------------- Functions Prototypes --------------------------- */

PUBLIC void
hal_adc_init( void );

/* -------------------------------------------------------------------------- */

/** Return true when there are active samples in the averaging buffer */

PUBLIC bool
hal_adc_valid( HalAdcInput_t input );

/* -------------------------------------------------------------------------- */

/** Last converted ADC read */

PUBLIC uint32_t
hal_adc_read( HalAdcInput_t input );

/* -------------------------------------------------------------------------- */

/** Averaged ADC read */

PUBLIC uint32_t
hal_adc_read_avg( HalAdcInput_t input );

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
hal_adc_read_peak( HalAdcInput_t input );

/* -------------------------------------------------------------------------- */

/** Start DMA based ADC conversions */

PUBLIC void
hal_adc_start( HalAdcInput_t input, uint16_t poll_rate_ms );

/* -------------------------------------------------------------------------- */

/** Stop DMA based ADC conversions */

PUBLIC void
hal_adc_stop( HalAdcInput_t input );

/* -------------------------------------------------------------------------- */

/** Timer tick to trigger a ADC conversion cycle */

PUBLIC void
hal_adc_tick( void );

/* -------------------------------------------------------------------------- */

/** STM32 HAL error callback */

extern void _Error_Handler(char *, int);

/* ------------------------- End -------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_ADC_H */
