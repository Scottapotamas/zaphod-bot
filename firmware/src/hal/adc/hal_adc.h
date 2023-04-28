#ifndef HAL_ADC_H
#define HAL_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

typedef enum
{
    HAL_ADC_INPUT_M1_CURRENT,    /* ADC1-15   - RANK 1 */
    HAL_ADC_INPUT_M2_CURRENT,    /* ADC1-14   - RANK 2 */
    HAL_ADC_INPUT_M3_CURRENT,    /* ADC1-7    - RANK 3 */
    HAL_ADC_INPUT_M4_CURRENT,    /* ADC1-6    - RANK 4 */
    HAL_ADC_INPUT_VOLT_SENSE,    /* ADC1-13   - RANK 5 */
    HAL_ADC_INPUT_TEMP_PCB,      /* ADC1-10   - RANK 6 */
    HAL_ADC_INPUT_TEMP_REG,      /* ADC1-11   - RANK 7 */
    HAL_ADC_INPUT_TEMP_EXT,      /* ADC1-12   - RANK 8 */
    HAL_ADC_INPUT_TEMP_INTERNAL, /* ADC1-TEMP - RANK 9 */
    HAL_ADC_INPUT_VREFINT,       /* ADC1-VREF - RANK 10 */
    HAL_ADC_INPUT_NUM
} HalAdcInput_t;

/* -------------------------------------------------------------------------- */

PUBLIC void hal_adc_init( void );

/* -------------------------------------------------------------------------- */

/** Last converted ADC read */
PUBLIC uint32_t hal_adc_read( HalAdcInput_t input );

/* -------------------------------------------------------------------------- */

/** Start DMA based ADC conversions */
PUBLIC void hal_adc_start( void );

/* -------------------------------------------------------------------------- */

/** Stop DMA based ADC conversions */
PUBLIC void hal_adc_stop( void );

/* -------------------------------------------------------------------------- */

void ADC_IRQHandler( void );
void DMA2_Stream0_IRQHandler( void );

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_ADC_H */
