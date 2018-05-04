/* ----- System Includes ---------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "hal_dma.h"


/* ------------------------- Functions -------------------------------------- */

PUBLIC void
hal_dma_init( void )
{
	/* DMA controller clock enable */
	__HAL_RCC_DMA2_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA2_Stream0_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_dma_start( void )
{
	//todo something useful here
}

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_dma_stop( void )
{
	//todo something useful here
}


/* ----- End ---------------------------------------------------------------- */
