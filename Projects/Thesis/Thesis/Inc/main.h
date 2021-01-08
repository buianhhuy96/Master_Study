/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_ll_i2c.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_dma.h"

#include "stm32f4xx_ll_exti.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define En2A_TIM2_1_Pin GPIO_PIN_0
#define En2A_TIM2_1_GPIO_Port GPIOA
#define En2B_TIM2_3_Pin GPIO_PIN_2
#define En2B_TIM2_3_GPIO_Port GPIOA
#define En1A_TIM3_1_Pin GPIO_PIN_6
#define En1A_TIM3_1_GPIO_Port GPIOA
#define PIN_3_A_Pin GPIO_PIN_7
#define PIN_3_A_GPIO_Port GPIOA
#define PIN_3_B_Pin GPIO_PIN_5
#define PIN_3_B_GPIO_Port GPIOC
#define En1B_TIM3_3_Pin GPIO_PIN_0
#define En1B_TIM3_3_GPIO_Port GPIOB
#define PIN_4_B_Pin GPIO_PIN_1
#define PIN_4_B_GPIO_Port GPIOB
#define PIN_4_A_Pin GPIO_PIN_7
#define PIN_4_A_GPIO_Port GPIOE
#define PWM4_TIM1_1_Pin GPIO_PIN_9
#define PWM4_TIM1_1_GPIO_Port GPIOE
#define PWM3_TIM1_4_Pin GPIO_PIN_14
#define PWM3_TIM1_4_GPIO_Port GPIOE
#define En4A_TIM4_1_Pin GPIO_PIN_12
#define En4A_TIM4_1_GPIO_Port GPIOD
#define En4B_TIM4_3_Pin GPIO_PIN_14
#define En4B_TIM4_3_GPIO_Port GPIOD
#define PWM2_TIM1_2_Pin GPIO_PIN_9
#define PWM2_TIM1_2_GPIO_Port GPIOA
#define PWM1_TIM1_3_Pin GPIO_PIN_10
#define PWM1_TIM1_3_GPIO_Port GPIOA
#define PIN_2_A_Pin GPIO_PIN_6
#define PIN_2_A_GPIO_Port GPIOD
#define PIN_2_B_Pin GPIO_PIN_3
#define PIN_2_B_GPIO_Port GPIOB
#define PIN_1_B_Pin GPIO_PIN_5
#define PIN_1_B_GPIO_Port GPIOB
#define PIN_1_A_Pin GPIO_PIN_7
#define PIN_1_A_GPIO_Port GPIOB

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
