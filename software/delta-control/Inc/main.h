/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define STATUS_2_Pin GPIO_PIN_2
#define STATUS_2_GPIO_Port GPIOE
#define STATUS_1_Pin GPIO_PIN_3
#define STATUS_1_GPIO_Port GPIOE
#define STATUS_0_Pin GPIO_PIN_4
#define STATUS_0_GPIO_Port GPIOE
#define FAN_TACH_Pin GPIO_PIN_5
#define FAN_TACH_GPIO_Port GPIOE
#define BUTTON_1_Pin GPIO_PIN_6
#define BUTTON_1_GPIO_Port GPIOE
#define BUTTON_0_Pin GPIO_PIN_13
#define BUTTON_0_GPIO_Port GPIOC
#define TEMP_PCB_Pin GPIO_PIN_0
#define TEMP_PCB_GPIO_Port GPIOC
#define TEMP_EXT_0_Pin GPIO_PIN_1
#define TEMP_EXT_0_GPIO_Port GPIOC
#define TEMP_EXT_1_Pin GPIO_PIN_2
#define TEMP_EXT_1_GPIO_Port GPIOC
#define V_SENSE_Pin GPIO_PIN_3
#define V_SENSE_GPIO_Port GPIOC
#define SERVO_4_HLFB_Pin GPIO_PIN_0
#define SERVO_4_HLFB_GPIO_Port GPIOA
#define SERVO_4_ENABLE_Pin GPIO_PIN_1
#define SERVO_4_ENABLE_GPIO_Port GPIOA
#define SERVO_4_A_Pin GPIO_PIN_2
#define SERVO_4_A_GPIO_Port GPIOA
#define SERVO_4_B_Pin GPIO_PIN_3
#define SERVO_4_B_GPIO_Port GPIOA
#define AUX_ANALOG_0_Pin GPIO_PIN_4
#define AUX_ANALOG_0_GPIO_Port GPIOA
#define AUX_ANALOG_1_Pin GPIO_PIN_5
#define AUX_ANALOG_1_GPIO_Port GPIOA
#define MOTOR_4_CURRENT_Pin GPIO_PIN_6
#define MOTOR_4_CURRENT_GPIO_Port GPIOA
#define MOTOR_3_CURRENT_Pin GPIO_PIN_7
#define MOTOR_3_CURRENT_GPIO_Port GPIOA
#define MOTOR_2_CURRENT_Pin GPIO_PIN_4
#define MOTOR_2_CURRENT_GPIO_Port GPIOC
#define MOTOR_1_CURRENT_Pin GPIO_PIN_5
#define MOTOR_1_CURRENT_GPIO_Port GPIOC
#define MOTOR_4_OC_Pin GPIO_PIN_0
#define MOTOR_4_OC_GPIO_Port GPIOB
#define MOTOR_3_OC_Pin GPIO_PIN_1
#define MOTOR_3_OC_GPIO_Port GPIOB
#define SERVO_3_HLFB_Pin GPIO_PIN_9
#define SERVO_3_HLFB_GPIO_Port GPIOE
#define SERVO_3_ENABLE_Pin GPIO_PIN_12
#define SERVO_3_ENABLE_GPIO_Port GPIOE
#define SERVO_3_A_Pin GPIO_PIN_13
#define SERVO_3_A_GPIO_Port GPIOE
#define SERVO_3_B_Pin GPIO_PIN_14
#define SERVO_3_B_GPIO_Port GPIOE
#define AUX_SCL_Pin GPIO_PIN_10
#define AUX_SCL_GPIO_Port GPIOB
#define AUX_SDA_Pin GPIO_PIN_11
#define AUX_SDA_GPIO_Port GPIOB
#define EXT_PWM_2_Pin GPIO_PIN_14
#define EXT_PWM_2_GPIO_Port GPIOB
#define EXT_PWM_1_Pin GPIO_PIN_15
#define EXT_PWM_1_GPIO_Port GPIOB
#define MOTOR_2_OC_Pin GPIO_PIN_10
#define MOTOR_2_OC_GPIO_Port GPIOD
#define MOTOR_1_OC_Pin GPIO_PIN_11
#define MOTOR_1_OC_GPIO_Port GPIOD
#define SERVO_2_HLFB_Pin GPIO_PIN_12
#define SERVO_2_HLFB_GPIO_Port GPIOD
#define SERVO_2_ENABLE_Pin GPIO_PIN_13
#define SERVO_2_ENABLE_GPIO_Port GPIOD
#define SERVO_2_A_Pin GPIO_PIN_14
#define SERVO_2_A_GPIO_Port GPIOD
#define SERVO_2_B_Pin GPIO_PIN_15
#define SERVO_2_B_GPIO_Port GPIOD
#define SERVO_1_HLFB_Pin GPIO_PIN_6
#define SERVO_1_HLFB_GPIO_Port GPIOC
#define SERVO_1_ENABLE_Pin GPIO_PIN_7
#define SERVO_1_ENABLE_GPIO_Port GPIOC
#define SERVO_1_A_Pin GPIO_PIN_8
#define SERVO_1_A_GPIO_Port GPIOC
#define SERVO_1_B_Pin GPIO_PIN_9
#define SERVO_1_B_GPIO_Port GPIOC
#define AUX_PWM_0_Pin GPIO_PIN_15
#define AUX_PWM_0_GPIO_Port GPIOA
#define EXT_OUTPUT_0_Pin GPIO_PIN_12
#define EXT_OUTPUT_0_GPIO_Port GPIOC
#define EXT_INPUT_1_Pin GPIO_PIN_0
#define EXT_INPUT_1_GPIO_Port GPIOD
#define EXT_OUTPUT_1_Pin GPIO_PIN_1
#define EXT_OUTPUT_1_GPIO_Port GPIOD
#define EXT_INPUT_0_Pin GPIO_PIN_2
#define EXT_INPUT_0_GPIO_Port GPIOD
#define MODULE_CTS_Pin GPIO_PIN_3
#define MODULE_CTS_GPIO_Port GPIOD
#define MODULE_RTS_Pin GPIO_PIN_4
#define MODULE_RTS_GPIO_Port GPIOD
#define MODULE_TX_Pin GPIO_PIN_5
#define MODULE_TX_GPIO_Port GPIOD
#define MODULE_RX_Pin GPIO_PIN_6
#define MODULE_RX_GPIO_Port GPIOD
#define MODULE_PWR_EN_Pin GPIO_PIN_7
#define MODULE_PWR_EN_GPIO_Port GPIOD
#define AUX_TX_Pin GPIO_PIN_6
#define AUX_TX_GPIO_Port GPIOB
#define AUX_RX_Pin GPIO_PIN_7
#define AUX_RX_GPIO_Port GPIOB
#define FAN_PWM_Pin GPIO_PIN_8
#define FAN_PWM_GPIO_Port GPIOB
#define BUZZER_PWM_Pin GPIO_PIN_9
#define BUZZER_PWM_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
