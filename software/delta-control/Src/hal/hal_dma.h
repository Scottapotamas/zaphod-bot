#ifndef HAL_DMA_H
#define HAL_DMA_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "stm32f4xx_hal.h"
#include "global.h"

/* ----- Types ------------------------------------------------------------- */

/* ------------------------- Functions Prototypes --------------------------- */

PUBLIC void
hal_dma_init( void );

/* -------------------------------------------------------------------------- */

/** Start DMA based ADC conversions */

PUBLIC void
hal_dma_start( void );

/* -------------------------------------------------------------------------- */

/** Stop DMA based ADC conversions */

PUBLIC void
hal_dma_stop( void );

/* -------------------------------------------------------------------------- */

/** DMA memory to memory transfer handles */

extern void _Error_Handler(char*, int);


/* ------------------------- End -------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_DMA_H */
