#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* #define USE_FULL_ASSERT    1U */

#ifdef __cplusplus
extern "C" {
#endif
void _Error_Handler( char *, int );

#define Error_Handler() _Error_Handler( __FILE__, __LINE__ )
#ifdef __cplusplus
}
#endif

#include "stm32f4xx.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_utils.h"

#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0 ( (uint32_t)0x00000007 ) /*!< 0 bit  for pre-emption priority, \
                                                           4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1 ( (uint32_t)0x00000006 ) /*!< 1 bit  for pre-emption priority, \
                                                           3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2 ( (uint32_t)0x00000005 ) /*!< 2 bits for pre-emption priority, \
                                                           2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3 ( (uint32_t)0x00000004 ) /*!< 3 bits for pre-emption priority, \
                                                           1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4 ( (uint32_t)0x00000003 ) /*!< 4 bits for pre-emption priority, \
                                                           0 bit  for subpriority */
#endif

#endif /* __MAIN_H__ */
