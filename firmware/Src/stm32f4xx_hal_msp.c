#include "main.h"

/**
  * Initializes the Global MSP.
  */

void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  // MemoryManagement_IRQn interrupt configuration
  HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);

  // BusFault_IRQn interrupt configuration
  HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);

  // UsageFault_IRQn interrupt configuration
  HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);

  // SVCall_IRQn interrupt configuration
  HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);

  // DebugMonitor_IRQn interrupt configuration
  HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);

  // PendSV_IRQn interrupt configuration
  HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);

  // SysTick_IRQn interrupt configuration
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

}
