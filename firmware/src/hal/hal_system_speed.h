#ifndef HAL_SYSTEM_SPEED_H
#define HAL_SYSTEM_SPEED_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* ----- Public Functions --------------------------------------------------- */

/* PLL configuration */
#define RCC_PLLM_MASK ( (uint32_t)0x0000003F )
#define RCC_PLLM_POS  ( 0U )
#define RCC_PLLN_MASK ( (uint32_t)0x00007FC0 )
#define RCC_PLLN_POS  ( 6U )
#define RCC_PLLP_MASK ( (uint32_t)0x00030000 )
#define RCC_PLLP_POS  ( 16U )
#define RCC_PLLQ_MASK ( (uint32_t)0x0F000000 )
#define RCC_PLLQ_POS  ( 24U )
#define RCC_PLLR_MASK ( (uint32_t)0x70000000 )
#define RCC_PLLR_POS  ( 28U )

typedef struct
{
    uint32_t PLLM;    // PLL M parameter. Between 2 and 63.
    uint32_t PLLN;    // PLL N parameter. Between 192 and 432.
    uint32_t PLLP;    // PLL P parameter. Use 2, 4, 6 or 8.
    uint32_t PLLQ;    // PLL Q parameter. Between 2 and 15.
    uint32_t PLLR;    // PLL R parameter. Between 2 and 7. Only available on STM32F446 devices.
} SystemSpeed_RCC_PLL_t;

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_system_speed_set_pll( SystemSpeed_RCC_PLL_t *pll_settings );

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_system_speed_get_pll( SystemSpeed_RCC_PLL_t *pll_settings );

/* -------------------------------------------------------------------------- */

PUBLIC bool
hal_system_speed_pll_ready( void );

/* -------------------------------------------------------------------------- */

PUBLIC bool
hal_system_speed_hsi_ready( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_system_speed_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_system_speed_sleep( void );

/* -------------------------------------------------------------------------- */

PUBLIC float
hal_system_speed_get_load( void );

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
hal_system_speed_get_speed( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_system_speed_high( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_system_speed_low( void );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_SYSTEM_SPEED_H */
