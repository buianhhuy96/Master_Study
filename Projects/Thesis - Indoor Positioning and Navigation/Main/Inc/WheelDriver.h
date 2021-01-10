#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_conf.h"
#ifndef WheelDriver_h
#define WheelDriver_h
	
#define FORWARD 0
#define BACKWARD 1
#define PIN_1_A GPIO_PIN_3
#define PIN_1_B GPIO_PIN_7
#define PIN_2_A GPIO_PIN_6
#define PIN_2_B GPIO_PIN_5
#define PIN_3_A GPIO_PIN_2	
#define PIN_3_B GPIO_PIN_1
#define PIN_4_A GPIO_PIN_4
#define PIN_4_B GPIO_PIN_3
#define timerscale 21000000
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim8;
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM5_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM8_Init(void);
static void MX_TIM3_Init(void);

	void Start_wheel(uint16_t NUM, uint16_t DI, uint16_t SPEED0);

#endif
