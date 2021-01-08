
#include "WheelDriver.h"
#include "main.h"



void Start_wheel(uint16_t NUM, uint16_t DI, uint16_t SPEED)
{
	switch (NUM)
	{
	case 1:
	{
		switch (DI)
		{
			case FORWARD:
			{
				HAL_GPIO_WritePin(GPIOE,PIN_1_A,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOE,PIN_1_B,GPIO_PIN_RESET);
				break;
			}
			case BACKWARD:
			{
				HAL_GPIO_WritePin(GPIOE,PIN_1_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOE,PIN_1_B,GPIO_PIN_SET);
				break;
			}
		__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_3,SPEED);
	
		break;
	}
		case 2:
	{
		switch (DI)
		{
			case FORWARD:
			{
				HAL_GPIO_WritePin(GPIOE,PIN_2_A,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOE,PIN_2_B,GPIO_PIN_RESET);
				break;
			}
			case BACKWARD:
			{
				HAL_GPIO_WritePin(GPIOE,PIN_2_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOE,PIN_2_B,GPIO_PIN_SET);
				break;
			}
		__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,SPEED);
	
		break;
		}
	}
			case 3:
	{
		switch (DI)
		{
			case FORWARD:
			{
				HAL_GPIO_WritePin(GPIOE,PIN_3_A,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOE,PIN_3_B,GPIO_PIN_RESET);
				break;
			}
			case BACKWARD:
			{
				HAL_GPIO_WritePin(GPIOE,PIN_3_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOE,PIN_3_B,GPIO_PIN_SET);
				break;
			}
		__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_2,SPEED);
	
		break;
		}
	}
			case 4:
	{
		switch (DI)
		{
			case FORWARD:
			{
				HAL_GPIO_WritePin(GPIOE,PIN_4_A,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOE,PIN_4_B,GPIO_PIN_RESET);
				break;
			}
			case BACKWARD:
			{
				HAL_GPIO_WritePin(GPIOE,PIN_4_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOE,PIN_4_B,GPIO_PIN_SET);
				break;
			}
		__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_4,SPEED);
	
		break;
		}
	}
}
}
}