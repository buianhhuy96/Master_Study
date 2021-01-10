#include "main.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h" 
#include "stm32f4xx_hal_conf.h"
#include "DefineAll.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_ll_tim.h"

#define FORWARD 0
#define BACKWARD 1

#define PIN_1_A GPIOB,GPIO_PIN_7  
#define PIN_1_B GPIOB,GPIO_PIN_5
#define PIN_2_A GPIOD,GPIO_PIN_6
#define PIN_2_B GPIOB,GPIO_PIN_3
#define PIN_3_A GPIOA,GPIO_PIN_7	
#define PIN_3_B GPIOC,GPIO_PIN_5
#define PIN_4_A GPIOE,GPIO_PIN_7
#define PIN_4_B GPIOB,GPIO_PIN_1

#define ENPIN_1_A GPIOA,GPIO_PIN_6	
#define ENPIN_1_B GPIOB,GPIO_PIN_0	
#define ENPIN_2_A GPIOA,GPIO_PIN_0	//PA
#define ENPIN_2_B GPIOA,GPIO_PIN_2
#define ENPIN_3_A GPIOC,GPIO_PIN_6	//PA
#define ENPIN_3_B GPIOC,GPIO_PIN_8
#define ENPIN_4_A GPIOD,GPIO_PIN_12	//PD
#define ENPIN_4_B GPIOD,GPIO_PIN_14

#define PWM1 TIM_CHANNEL_3
#define PWM2 TIM_CHANNEL_2
#define PWM3 TIM_CHANNEL_4
#define PWM4 TIM_CHANNEL_1

#define delta_x 0.05f
#define delta_y 0.05f
#define delta_t 0.05f

#define step_x 0.02f
#define step_y 0.02f
#define step_t 0.02f

#define threshold 1800.0f	
#define threshold2 1800.0f	
#define threshold0 5.0f
#define FirstCycles 	10.0f

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim8;

UART_HandleTypeDef huart3;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM8_Init(void);
static void MX_TIM6_Init(void);

void createMessage(float* val);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void Start_wheel(uint16_t NUM, float SPEED);                               
void DISABLE_ALL_IT(void);
void ENABLE_ALL_IT(void);
void ENCODER_READ_Start(void);

bool Task_2ms_FLAG = TRUE; 
bool active_test_flag = 0;
bool Controller_ver2 = 1;
bool compass_activation_flag = 1;
int16_t Task=-1;
uint16_t Mode=0;
float pos_receive_2[4][3]=
{
	{0,0,0},
	{1,0,0},
	{1,1,0},
	{0,1,0}
};

// Data transmiting 
uint8_t full_message[15];
uint8_t connected[]="!";
uint8_t Rx_indx, Rx_data, Rx_Buffer[100], data_receiving_complete,Connected;

int Mess_wait=0;

// Kalman Filter
float p[5];
float _err_measure;
float _q;
float _current_estimate;
float _kalman_gain;

float _err_measureX;
float _qX;
float _current_estimateX;
float _kalman_gainX;
 
// Vehicle parameters
float v1_scaled;
float v2_scaled;
float v3_scaled;
float v4_scaled;
int16_t HMC5883Ldata[3]; 
float angle;
float theta;
float last_theta; 
float offset;
float FirstCall_theta=10;

float S1[3];
float S2[3];
float prev_S2[3];

// PWM for 4 wheels
float duty_cycle_wheel_1;
float duty_cycle_wheel_2;
float duty_cycle_wheel_3;
float duty_cycle_wheel_4;

// Positioning and Navigation
float speed[3] = {0,0,0};
float prev_speed[3] = {0,0,0};
float current_location[3] = {0,0,0};
float prev_location[3] = {0,0,0};
float prev_location_theta[3] = {0,0,0};
float received_destination[3]={0,0,0};
float destination_setpoint[3]={0,0,0};
float prev_destination_setpoint[3];
float diff_destination_setpoint[3];
float motor_torque[4];
float u[3];
float q2[3]={0,0,0};
float speed_setpoint[3]= {0,0,0};

//////////// PID
float error[4];
float pre_error[4];
float I[4];
float Dt[4];
float PWM_ON_1=0;
float PWM_ON_2=0;
float PWM_ON_3=0;
float PWM_ON_4=0; 

// Dynamic Control Surface
float x2_set[3];
float x2n_set[3];
float dx2_set[3];

//Timer 3 pin PA6
//Timer 3 pin PA7
int32_t en_1_A=0;
int32_t en_1_A_temp=0;
uint16_t state_1=0;
uint16_t prestate_1=0;
uint16_t pre2state_1=0;
uint16_t clockwise_1=0;
uint16_t clockwise_1_wait=0;
uint16_t OvFl_1=0;
uint16_t FirstCall_1 = 1;
float v1=0;
float v1_set=0;
//Timer 2 pin PA2
//Timer 2 pin PA3
int32_t en_2_A=0;
int32_t en_2_A_temp=0;
uint16_t state_2=0;
uint16_t prestate_2=0;
uint16_t pre2state_2=0;
uint16_t clockwise_2=0;
uint16_t clockwise_2_wait=0;
uint16_t OvFl_2=0;
uint16_t FirstCall_2 = 1;
float v2=0;
float v2_set=0;
//Timer 5 pin PA0
//Timer 5 pin PA1
int32_t en_3_A=0;
int32_t en_3_A_temp=0;
uint16_t state_3=0;
uint16_t prestate_3=0;
uint16_t pre2state_3=0;
uint16_t clockwise_3=0;
uint16_t clockwise_3_wait=0;
uint16_t OvFl_3=0;
uint16_t FirstCall_3 = 1;
float v3=0;
float v3_set=0;

//Timer 4 pin PD13
//Timer 4 pin PD14
int32_t en_4_A=0;
int32_t en_4_A_temp=0;
uint16_t state_4=0;
uint16_t prestate_4=0;
uint16_t pre2state_4=0;
uint16_t clockwise_4=0;
uint16_t clockwise_4_wait=0;
uint16_t OvFl_4=0;
uint16_t FirstCall_4 = 1;
int cycle_4 = 0;
float v4=0;
float v4_set=0;
float tmp;
float t;
void Delay(uint32_t delay);
uint8_t x_reached = 0;
uint8_t y_reached = 0;
uint8_t t_reached = 0;
float w;
float v;
	int q=1;
uint8_t FirstCall_u=10;
int main(void)
{
	
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	SystemClock_Config();
	// Initializes timer for program flow and GPIO for controlling
	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();
	MX_USART3_UART_Init();
	MX_TIM4_Init();
	MX_TIM1_Init();
	MX_TIM8_Init();
	MX_TIM6_Init();
	//Encoder Input Capture init
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_3);
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_3);
	HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_3);
	HAL_TIM_IC_Start_IT(&htim8,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim8,TIM_CHANNEL_3);
	HAL_TIM_Base_Start_IT(&htim6);
	
	//HMC5883L
	HMC5883L_Initialize();
	
	//PWM initialize
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);		//Wheel 2 PC6
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);		//Wheel 3 PC7
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);		//Wheel 1 PC8
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);		//Wheel 4 PC9
	
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_TIM_Base_Start_IT(&htim8);
					
	//Kalman Filter Setup
	SimpleKalmanFilter(700,700,0.01);
	SimpleKalmanFilterX(7.5,7.5,0.2 );
	// 
	HAL_UART_Receive_IT(&huart3, &Rx_data, 1);
	DISABLE_ALL_IT();

	while (1)
	{	
		// This loop is only for testing purposes
		if (!active_test_flag)
		{
			if (FirstCall_u==0)
			{
				Start_wheel(1, duty_cycle_wheel_1);
				Start_wheel(2, duty_cycle_wheel_2);
				Start_wheel(3, duty_cycle_wheel_3);
				Start_wheel(4, duty_cycle_wheel_4);
			}
		}
	}
}

/** This function control program flow with 4 different tasks **/		
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
		 
	HAL_TIM_Base_Stop(&htim6);
	if (htim->Instance==TIM6)
	{
		t+=1;
		
		if (FirstCall_u > 0) FirstCall_u--;
		if ((Task_2ms_FLAG == TRUE)&&(!active_test_flag)) 	Task++;
		else Task++;
		// Task 0: read destination set from control center and handle calculation
		if (Task == 0)
		{
			DISABLE_ALL_IT();
			
			if (data_receiving_complete == 1) 
			{
				for (int i=0;i<30;i++)
				{
					if (Connected == 1)
					{
							if ((Rx_Buffer[i]==120) && (Rx_Buffer[i+1] <46) &&
								(Rx_Buffer[i+2] < 58) && (Rx_Buffer[i+3] < 58) &&
								(Rx_Buffer[i+4] < 58) && (Rx_Buffer[i+1] > 42)&& 
								(Rx_Buffer[i+2] > 47)&& (Rx_Buffer[i+3] > 47) && 
								(Rx_Buffer[i+4] > 47))
							{ 
								received_destination[0]= ((Rx_Buffer[i+2]-48)*100+(Rx_Buffer[i+3]-48)*10 \
												+(Rx_Buffer[i+4]-48))*(44-Rx_Buffer[i+1]);
								received_destination[0]= received_destination[0]/100;
							}
							if ((Rx_Buffer[i]==121) && (Rx_Buffer[i+1] <46) &&
								(Rx_Buffer[i+2] < 58) && (Rx_Buffer[i+3] < 58) &&
								(Rx_Buffer[i+4] < 58) && (Rx_Buffer[i+1] > 42)&& 
								(Rx_Buffer[i+2] > 47)&& (Rx_Buffer[i+3] > 47) && 
								(Rx_Buffer[i+4] > 47))
							{
								received_destination[1]=((Rx_Buffer[i+2]-48)*100+(Rx_Buffer[i+3]-48)*10 \
												+(Rx_Buffer[i+4]-48))*(44-Rx_Buffer[i+1]);
								received_destination[1]=received_destination[1]/100;
							}
							if ((Rx_Buffer[i]==116) && (Rx_Buffer[i+1] <46) && 
								(Rx_Buffer[i+2] < 58) && (Rx_Buffer[i+3] < 58) &&
								(Rx_Buffer[i+4] < 58) && (Rx_Buffer[i+1] > 42)&& 
								(Rx_Buffer[i+2] > 47)&& (Rx_Buffer[i+3] > 47) && 
								(Rx_Buffer[i+4] > 47))
							{
								received_destination[2]=((Rx_Buffer[i+2]-48)*100+(Rx_Buffer[i+3]-48)*10 \
												+(Rx_Buffer[i+4]-48))*(44-Rx_Buffer[i+1]);
								received_destination[2]=received_destination[2]/100;
							}
						
							
					}
				}
		
				data_receiving_complete=0;
			}
			// Calculate received data into actual destination setpoint
			if ((current_location[0] < (received_destination[0]-delta_x))||
				(current_location[0]> (received_destination[0]+delta_x))) 
			{
				destination_setpoint[0] += 
								(float) step_x*(received_destination[0]-current_location[0]) \
								/fabs(received_destination[0]-current_location[0]); 
				x_reached = 0;
			}
			else 
			{
				destination_setpoint[0]=received_destination[0]; 
				x_reached = 1;
			}
			if ((current_location[1] < (received_destination[1]-delta_y))||
				(current_location[1]> (received_destination[1]+delta_y))) 
			{
				destination_setpoint[1] += 
								(float) step_y*(received_destination[1]-current_location[1]) \
								/fabs(received_destination[1]-current_location[1]); 
				y_reached = 0;
			}
			else 
			{
				destination_setpoint[1]=received_destination[1]; 
				y_reached = 1;
			}
			
			if ((current_location[2] - received_destination[2]) > PI)
			{
				float pos_receive_theta;
				pos_receive_theta = received_destination[2] + 2*PI;
				if ((current_location[2] < (pos_receive_theta-delta_t))||
					(current_location[2]> (pos_receive_theta+delta_t))) 
				{
					destination_setpoint[2] = destination_setpoint[2]+ (float) step_t; 
					t_reached = 0;
				}
				else {destination_setpoint[2]=pos_receive_theta; t_reached = 1;}
			}
			else if ((current_location[2] - received_destination[2]) < -PI)
			{
					float pos_receive_theta;
				pos_receive_theta = received_destination[2] - 2*PI;
				if ((current_location[2] < (pos_receive_theta-delta_t))||
					(current_location[2]> (pos_receive_theta+delta_t))) 
				{
					destination_setpoint[2] = destination_setpoint[2]- (float) step_t; 
					t_reached = 0;
				}
				else 
				{
					destination_setpoint[2]=pos_receive_theta; 
					t_reached = 1;
				}
			}
			else 
			{
				if ((current_location[2] < (received_destination[2]-delta_t))||
					(current_location[2]> (received_destination[2]+delta_t))) 
				{
					destination_setpoint[2] += 
								(float) step_t*(received_destination[2]-current_location[2]) \
								/fabs(received_destination[2]-current_location[2]); 
					t_reached = 0;
				}
				else {
					destination_setpoint[2]=received_destination[2]; t_reached = 1;
				}
			}
			
		}
		// Task 1: Start reading compass data
		else if (Task == 1)
		{
			DISABLE_ALL_IT();
			if(FirstCall_theta != 0)
			{
			
				offset= HMC5883L_GetHeading(0);
				tmp=0;
				FirstCall_theta--;
			}
			else 
			{
				tmp= HMC5883L_GetHeading(0);	
				tmp=tmp-offset;
			}

		}
		// Task 2: Start reading encoder and calculate vehicle theta based on compass data
		else if (Task == 2)
		{
			theta = (((theta < (5*PI/180)) && (tmp >(355*PI/180))) || 
					  ((tmp < (5*PI/180)) && (theta >(355*PI/180))))?tmp:updateEstimateX(tmp,theta);
			if(compass_activation_flag)
			{				
				prev_location_theta[2]=current_location[2];
				current_location[2]=2*PI-theta;
				if (current_location[2] < 0) current_location[2] = current_location[2] + 2*PI;
				if (current_location[2] > 2*PI) current_location[2] = current_location[2] - 2*PI;
			}
			ENCODER_READ_Start();		
		}
		// Task 3: Control the vehicle to move 
		else if (Task == 3) 
		{
			DISABLE_ALL_IT();
			Controller();
			if (!active_test_flag)	xyspeed();
			else
			{
				speed[0] = speed_setpoint[0];
				speed[1] = speed_setpoint[1];
				speed[2] = speed_setpoint[2];
			}
			locateDeadReckoning();
			Setpoint();
			UpdateSetpoint();
			xyspeed_inv();
			
	
			pwmCalculation();
			prev_speed[0] = speed[0];
			prev_speed[1] = speed[1];
			prev_speed[2] = speed[2];
			if (Connected == 2) Connected=1;
			else if (Connected == 1) 
			{
				if(Mess_wait==0)
				{
					createMessage(current_location); 
					HAL_UART_Transmit(&huart3, full_message, sizeof(full_message),100);
					Mess_wait=1;
				}	
				else
					Mess_wait--;
			}
			
			Task = -1;
		}
	}
	HAL_TIM_Base_Start_IT(&htim6);
	
		
	// Handle overflow of reading encoder using input capture 		
	if (Task == 2)
	{
		if (htim->Instance==TIM2)
		{
			OvFl_2++;
			if (OvFl_2>threshold0)	v2=0;
			else if (v2!=0)
			{
				en_2_A=updateEstimate((en_2_A_temp+OvFl_2*65535),en_2_A,2);
				if	(clockwise_2 == 1)		v2=timerscale/fabs(en_2_A);
				else v2=-timerscale/fabs(en_2_A); 
			}
		}
		if (htim->Instance==TIM3)
		{
			OvFl_1++;
			if (OvFl_1>threshold0) v1=0;
			else if (v1!=0)
			{
				en_1_A=updateEstimate((en_1_A_temp+OvFl_1*65535),en_1_A,1);
				if	(clockwise_1 == 1)		v1=timerscale/fabs(en_1_A);
				else v1=-timerscale/fabs(en_1_A); 
			}	
			
		}
		if (htim->Instance==TIM4)
		{
			OvFl_4++;
			if (OvFl_4>threshold0) v4=0;
				else if (v4!=0)
			{
				en_4_A=updateEstimate((en_4_A_temp+OvFl_4*65535),en_4_A,4);
				if	(clockwise_4 == 1)		v4=timerscale/fabs(en_4_A);
				else v4=-timerscale/fabs(en_4_A); 
			}	
		}
		if (htim->Instance==TIM8)
		{
			OvFl_3++;
			
			if (OvFl_3>threshold0) v3=0;

			else if (v3!=0)
			{
				en_3_A=updateEstimate((en_3_A_temp+OvFl_3*65535),en_3_A,3);
				if	(clockwise_3 == 1)		v3=timerscale/fabs(en_3_A);
				else v3=-timerscale/fabs(en_3_A); 
			}
		}
	}
}

/** This function create message of current_location of vehicle **/
void createMessage(float* val)
{
	int8_t* mes_x=malloc(6*sizeof(char));
	int8_t* mes_y=malloc(6*sizeof(char));
	int8_t* mes_t=malloc(6*sizeof(char));
	
	mes_x[0]=(unsigned int)72;
	if (val[0] < 0) mes_x[1]=(int)(-3);
	else mes_x[1]=(int)(-5);
	mes_x[2]=(unsigned int)(fabs(val[0]));
	mes_x[3]=((unsigned int)(fabs(val[0])*10)%10);
	mes_x[4]=((unsigned int)(fabs(val[0])*100)%10);
	
	mes_y[0]=(unsigned int)73;
	if (val[1] < 0) mes_y[1]=(int)(-3);
	else mes_y[1]=(int)(-5);
	mes_y[2]=(unsigned int)(fabs(val[1]));
	mes_y[3]=((unsigned int)(fabs(val[1])*10)%10);
	mes_y[4]=((unsigned int)(fabs(val[1])*100)%10);
	
	mes_t[0]=(unsigned int)68;
	if (val[2] < 0) mes_t[1]=(int)(-3);
	else mes_t[1]=(int)(-5);
	mes_t[2]=(unsigned int)(fabs(val[2]));
	mes_t[3]=((unsigned int)(fabs(val[2])*10)%10);
	mes_t[4]=((unsigned int)(fabs(val[2])*100)%10);
	for (int i=0;i<=4;i++){
		full_message[i] = mes_x[i]+48;
		full_message[i+5] = mes_y[i]+48;
		full_message[i+10] = mes_t[i]+48;
	}
	free(mes_x);
	free(mes_y);
	free(mes_t);
}

/** This function handle receiving message of destination **/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)  
{
    uint8_t index;
    if (huart->Instance == USART3)  //current UART
        {
			 //clear Rx_Buffer before receiving new full_message 		
			if (Rx_indx==0) {for (index=0;index<100;index++) Rx_Buffer[index]=0;}  
			//if received data different from ascii 13 (enter)
			if (Rx_data != 35) 
            {
				Rx_Buffer[Rx_indx++]=Rx_data;    //add data to Rx_Buffer
            }
			else            //if received data = 13
			{
				if (Connected != 1)
				{
					for (int k=0;k<30;k++)
					{
						if (Rx_Buffer[k]==33)
						{
							HAL_UART_Transmit(&huart3, connected, sizeof(connected),100);
							Connected=2;
						}
					}
				}
				if(Rx_indx==15)
				{
					
					data_receiving_complete=1;//transfer complete, data is ready to read
				}
				Rx_indx=0;
				
			}
				
			HAL_UART_Receive_IT(&huart3, &Rx_data, 1);   //activate UART receive interrupt every time
        }

}

/** This function handle reading ENCODER and 
	calculate wheel speed clockwise and reverse clockwise in 4 stages of input capture **/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
			//////////////////////////////////////ENCODER 3
	if (htim->Instance==TIM8)
	{
			//w=__HAL_TIM_GetCompare(&htim8,TIM_CHANNEL_1);
			if ((HAL_GPIO_ReadPin(ENPIN_3_A)==0) && (HAL_GPIO_ReadPin(ENPIN_3_B)==0)) state_3=3;
			if ((HAL_GPIO_ReadPin(ENPIN_3_A)==0) && (HAL_GPIO_ReadPin(ENPIN_3_B)==1)) state_3=0;
			if ((HAL_GPIO_ReadPin(ENPIN_3_A)==1) && (HAL_GPIO_ReadPin(ENPIN_3_B)==1)) state_3=1;
			if ((HAL_GPIO_ReadPin(ENPIN_3_A)==1) && (HAL_GPIO_ReadPin(ENPIN_3_B)==0)) state_3=2;
			switch(clockwise_3)
			{
				case 0:
				{
					if ((state_3==1)&&(prestate_3==3))
					{  		
						en_3_A_temp=__HAL_TIM_GetCompare(&htim8,TIM_CHANNEL_1);	
						prestate_3=2;
						pre2state_3=3;
					}
					if ((state_3==2)&&(prestate_3==0))
					{  	
						if (FirstCall_3  > 0)  FirstCall_3--;
						else if (FirstCall_3 == 0)
						{								
							en_3_A=updateEstimate(en_3_A_temp,en_3_A,3);
							v3=-timerscale/(fabs(en_3_A)+OvFl_3*65535);
						}								
						__HAL_TIM_SetCounter(&htim8,0);
						prestate_3=3;
						pre2state_3=0;
						OvFl_3=0;
					}
					if ((state_3==3)&&(prestate_3==1))
					{  		
						en_3_A_temp=__HAL_TIM_GetCompare(&htim8,TIM_CHANNEL_1);		
						prestate_3=0;
						pre2state_3=1;
					}
					if ((state_3==0)&&(prestate_3==2))
					{  		
						en_3_A=updateEstimate((en_3_A_temp),en_3_A,3);
						v3=-timerscale/(fabs(en_3_A)+OvFl_3*65535);			
						__HAL_TIM_SetCounter(&htim8,0);
						prestate_3=1;
						pre2state_3=2;
						OvFl_3=0;
					}
					if (state_3==prestate_3)
					{
							clockwise_3_wait=1;
							
					}
					if ((clockwise_3_wait==1) && (state_3 == pre2state_3))
					{
						clockwise_3=1;
						FirstCall_3 = FirstCycles;
					}
					break;
				}
				case 1:
				{
					
					if ((state_3==1)&&(prestate_3==3))
					{  		
						en_3_A_temp=__HAL_TIM_GetCompare(&htim8,TIM_CHANNEL_3);	
						prestate_3=0;
						pre2state_3=3;
					}
					if ((state_3==2)&&(prestate_3==0))
					{  		
						if (FirstCall_3 > 0)  FirstCall_3--;
						else if (FirstCall_3 == 0)
						{			
						en_3_A=updateEstimate((en_3_A_temp),en_3_A,3);
						v3=timerscale/(fabs(en_3_A)+OvFl_3*65535);
						}								
						__HAL_TIM_SetCounter(&htim8,0);
						prestate_3=1;
						pre2state_3=0;
						OvFl_3=0;
					}
					if ((state_3==3)&&(prestate_3==1))
					{  		
						en_3_A_temp=__HAL_TIM_GetCompare(&htim8,TIM_CHANNEL_3);		
						prestate_3=2;
						pre2state_3=1;
					}
					if ((state_3==0)&&(prestate_3==2))
					{  		
						en_3_A=updateEstimate((en_3_A_temp),en_3_A,3);
						v3=timerscale/(fabs(en_3_A)+OvFl_3*65535);			
						__HAL_TIM_SetCounter(&htim8,0);
						prestate_3=3;
						pre2state_3=2;
						OvFl_3=0;
					}
					if (state_3==prestate_3)
					{
							clockwise_3_wait=1;
					}
					if ((clockwise_3_wait==1) && (state_3 == pre2state_3))
					{
						clockwise_3=0;
						FirstCall_3 = FirstCycles;
					}
					break;
				};
			}
		}

	//////////////////////////////////////ENCODER 1
	else if (htim->Instance==TIM3)
	{				
		if ((HAL_GPIO_ReadPin(ENPIN_1_A)==0) && (HAL_GPIO_ReadPin(ENPIN_1_B)==0) ) {state_1=3;}
		if ((HAL_GPIO_ReadPin(ENPIN_1_A)==0) && (HAL_GPIO_ReadPin(ENPIN_1_B)==1) ) {state_1=0;} 
		if ((HAL_GPIO_ReadPin(ENPIN_1_A)==1) && (HAL_GPIO_ReadPin(ENPIN_1_B)==1) ) {state_1=1;} 
		if ((HAL_GPIO_ReadPin(ENPIN_1_A)==1) && (HAL_GPIO_ReadPin(ENPIN_1_B)==0) ) state_1=2;
		switch(clockwise_1)
		{
			case 0:
			{
				if ((state_1==1)&&(prestate_1==3))
				{  		
					en_1_A_temp=HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_1);	
					prestate_1=2;
					pre2state_1=3;
				}
				if ((state_1==2)&&(prestate_1==0))
				{  	
					//t++;
					if (FirstCall_1  > 0)  FirstCall_1--;
					else if (FirstCall_1 == 0)  
					{								
						en_1_A=updateEstimate(en_1_A_temp,en_1_A,1);
						
						v1=-timerscale/(fabs(en_1_A)+OvFl_1*65535);
					}								
					__HAL_TIM_SetCounter(&htim3,0);
					prestate_1=3;
					pre2state_1=0;
					OvFl_1=0;
				}
				if ((state_1==3)&&(prestate_1==1))
				{  		
					en_1_A_temp=HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_1);		
					prestate_1=0;
					pre2state_1=1;
				}
				if ((state_1==0)&&(prestate_1==2))
				{  		
					en_1_A=updateEstimate(en_1_A_temp,en_1_A,1);
					v1=-timerscale/(fabs(en_1_A)+OvFl_1*65535);			
					__HAL_TIM_SetCounter(&htim3,0);
					prestate_1=1;
					pre2state_1=2;
					OvFl_1=0;
				}
				if (state_1==prestate_1)
				{
					clockwise_1_wait=1;
						
				}
				if ((clockwise_1_wait==1) && (state_1 == pre2state_1))
				{
					clockwise_1=1;
					FirstCall_1 = FirstCycles;
				}
				break;
			}
			case 1:
			{
				
				if ((state_1==1)&&(prestate_1==3))
				{  		
					en_1_A_temp=HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_3);	
					prestate_1=0;
					pre2state_1=3;
				}
				if ((state_1==2)&&(prestate_1==0))
				{  		
					if (FirstCall_1  > 0)  FirstCall_1--;
					else if (FirstCall_1 == 0)
					{			
						en_1_A=updateEstimate(en_1_A_temp,en_1_A,1);
						v1=timerscale/(fabs(en_1_A)+OvFl_1*65535);
					}								
					__HAL_TIM_SetCounter(&htim3,0);
					prestate_1=1;
					pre2state_1=0;
					OvFl_1=0;
				}
				if ((state_1==3)&&(prestate_1==1))
				{  		
					en_1_A_temp=HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_3);		
					prestate_1=2;
					pre2state_1=1;
				}
				if ((state_1==0)&&(prestate_1==2))
				{  		
					en_1_A=updateEstimate(en_1_A_temp,en_1_A,1);
					v1=timerscale/(fabs(en_1_A)+OvFl_1*65535);			
					__HAL_TIM_SetCounter(&htim3,0);
					prestate_1=3;
					pre2state_1=2;
					OvFl_1=0;
				}
				if (state_1==prestate_1)
				{
					clockwise_1_wait=1;
				}
				if ((clockwise_1_wait==1) && (state_1 == pre2state_1))
				{
					clockwise_1=0;
					FirstCall_1 = FirstCycles;
				}
				break;
			};

		}
	 
	}

	 ///////////////////////////////////////ENCODER 2
	 else if (htim->Instance==TIM2)
	{				
		if ((HAL_GPIO_ReadPin(ENPIN_2_A)==0) && (HAL_GPIO_ReadPin(ENPIN_2_B)==0) ) state_2=3;
		if ((HAL_GPIO_ReadPin(ENPIN_2_A)==0) && (HAL_GPIO_ReadPin(ENPIN_2_B)==1) ) state_2=0; 
		if ((HAL_GPIO_ReadPin(ENPIN_2_A)==1) && (HAL_GPIO_ReadPin(ENPIN_2_B)==1) ) state_2=1; 
		if ((HAL_GPIO_ReadPin(ENPIN_2_A)==1) && (HAL_GPIO_ReadPin(ENPIN_2_B)==0) ) state_2=2;
		switch(clockwise_2)
		{
			case 0:
			{
				if ((state_2==1)&&(prestate_2==3))
				{  		
					en_2_A_temp=HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_1);	
					prestate_2=2;
					pre2state_2=3;
				}
				if ((state_2==2)&&(prestate_2==0))
				{  	
					if (FirstCall_2 > 0)  FirstCall_2--;
					else if (FirstCall_2 == 0)
					{								
					en_2_A=updateEstimate(en_2_A_temp,en_2_A,2);
						
					v2=-timerscale/(fabs(en_2_A)+OvFl_2*65535);
					}								
					__HAL_TIM_SetCounter(&htim2,0);
					prestate_2=3;
					pre2state_2=0;
					OvFl_2=0;
				}
				if ((state_2==3)&&(prestate_2==1))
				{  		
					en_2_A_temp=HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_1);		
					prestate_2=0;
					pre2state_2=1;
				}
				if ((state_2==0)&&(prestate_2==2))
				{  		
					en_2_A=updateEstimate(en_2_A_temp,en_2_A,2);
					v2=-timerscale/(fabs(en_2_A)+OvFl_2*65535);			
					__HAL_TIM_SetCounter(&htim2,0);
					prestate_2=1;
					pre2state_2=2;
					OvFl_2=0;
				}
				if (state_2==prestate_2)
				{
					clockwise_2_wait=1;
				}
				if ((clockwise_2_wait==1) && (state_2 == pre2state_2))
				{
					clockwise_2=1;
					FirstCall_2 = FirstCycles;
				}
				break;
			}
			case 1:
			{
				
				if ((state_2==1)&&(prestate_2==3))
				{  		
					en_2_A_temp=HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_3);	
					prestate_2=0;
					pre2state_2=3;
				}
				if ((state_2==2)&&(prestate_2==0))
				{  		
					if (FirstCall_2 > 0)  FirstCall_2--;
					else if (FirstCall_2 == 0)
					{			
					en_2_A=updateEstimate(en_2_A_temp,en_2_A,2);
					v2=timerscale/(fabs(en_2_A)+OvFl_2*65535);
					}								
					__HAL_TIM_SetCounter(&htim2,0);
					prestate_2=1;
					pre2state_2=0;
					OvFl_2=0;
				}
				if ((state_2==3)&&(prestate_2==1))
				{  		
					en_2_A_temp=HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_3);		
					prestate_2=2;
					pre2state_2=1;
				}
				if ((state_2==0)&&(prestate_2==2))
				{  		
					en_2_A=updateEstimate(en_2_A_temp,en_2_A,2);
					v2=timerscale/(fabs(en_2_A)+OvFl_2*65535);			
					__HAL_TIM_SetCounter(&htim2,0);
					prestate_2=3;
					pre2state_2=2;
					OvFl_2=0;
				}
				if (state_2==prestate_2)
				{
					clockwise_2_wait=1;
				}
				if ((clockwise_2_wait==1) && (state_2 == pre2state_2))
				{
					clockwise_2=0;
					FirstCall_2 = FirstCycles;
				}
				break;
			};

		}
	 
	}

		

	//////////////////////////////////////ENCODER 4
	else if (htim->Instance==TIM4)
	{	
		if ((HAL_GPIO_ReadPin(ENPIN_4_A)==0) && (HAL_GPIO_ReadPin(ENPIN_4_B)==0) ) state_4=3;
		if ((HAL_GPIO_ReadPin(ENPIN_4_A)==0) && (HAL_GPIO_ReadPin(ENPIN_4_B)==1) ) state_4=0;
		if ((HAL_GPIO_ReadPin(ENPIN_4_A)==1) && (HAL_GPIO_ReadPin(ENPIN_4_B)==1) ) state_4=1;
		if ((HAL_GPIO_ReadPin(ENPIN_4_A)==1) && (HAL_GPIO_ReadPin(ENPIN_4_B)==0) ) state_4=2;
		switch(clockwise_4)
		{
			case 0:
			{
				if ((state_4==1)&&(prestate_4==3))
				{  		
						en_4_A_temp=HAL_TIM_ReadCapturedValue(&htim4,TIM_CHANNEL_1);	
						prestate_4=2;
						pre2state_4=3;
				}
				if ((state_4==2)&&(prestate_4==0))
				{  	
						if (FirstCall_4 > 0)  FirstCall_4--;
						else if (FirstCall_4 == 0)  
						{								
						en_4_A=updateEstimate(en_4_A_temp,en_4_A,3);
							
						v4=-timerscale/(fabs(en_4_A)+OvFl_4*65535);
						}								
						__HAL_TIM_SetCounter(&htim4,0);
						prestate_4=3;
						pre2state_4=0;
						OvFl_4=0;
				}
				if ((state_4==3)&&(prestate_4==1))
				{  		
						en_4_A_temp=HAL_TIM_ReadCapturedValue(&htim4,TIM_CHANNEL_1);		
						prestate_4=0;
						pre2state_4=1;
				}
				if ((state_4==0)&&(prestate_4==2))
				{  		
						en_4_A=updateEstimate(en_4_A_temp,en_4_A,3);
						v4=-timerscale/(fabs(en_4_A)+OvFl_4*65535);			
						__HAL_TIM_SetCounter(&htim4,0);
						prestate_4=1;
						pre2state_4=2;
						OvFl_4=0;	
				}
				if (state_4==prestate_4)
				{
						clockwise_4_wait=1;
						
				}
				if ((clockwise_4_wait==1) && (state_4 == pre2state_4))
				{
					clockwise_4=1;
					FirstCall_4 = FirstCycles;
				}
				break;
			}
			case 1:
			{
				
				if ((state_4==1)&&(prestate_4==3))
				{  		
						en_4_A_temp=HAL_TIM_ReadCapturedValue(&htim4,TIM_CHANNEL_3);	
						prestate_4=0;
						pre2state_4=3;
				}
				if ((state_4==2)&&(prestate_4==0))
				{  		
						if (FirstCall_4 > 0)  FirstCall_4--;
						else if (FirstCall_4 == 0)
						{			
						en_4_A=updateEstimate(en_4_A_temp,en_4_A,3);
						v4=timerscale/(fabs(en_4_A)+OvFl_4*65535);
						}								
						__HAL_TIM_SetCounter(&htim4,0);
						prestate_4=1;
						pre2state_4=0;
						OvFl_4=0;
				}
				if ((state_4==3)&&(prestate_4==1))
				{  		
						en_4_A_temp=HAL_TIM_ReadCapturedValue(&htim4,TIM_CHANNEL_3);		
						prestate_4=2;
						pre2state_4=1;
				}
				if ((state_4==0)&&(prestate_4==2))
				{  		
						en_4_A=updateEstimate(en_4_A_temp,en_4_A,3);
						v4=timerscale/(fabs(en_4_A)+OvFl_4*65535);			
						__HAL_TIM_SetCounter(&htim4,0);
						prestate_4=3;
						pre2state_4=2;
						OvFl_4=0;
				}
				if (state_4==prestate_4)
				{
						clockwise_4_wait=1;
				}
				if ((clockwise_4_wait==1) && (state_4 == pre2state_4))
				{
					clockwise_4=0;
					FirstCall_4 = FirstCycles;
				}
				break;
			};
	
		}
	
	}
	

}

/** This function control PWM for 4 wheels **/
void Start_wheel(uint16_t NUM, float SPEED)
{
	switch (NUM)
	{
		case 2:
		{
			if (SPEED > threshold) 
			{
				HAL_GPIO_WritePin(PIN_1_A,GPIO_PIN_SET);
				HAL_GPIO_WritePin(PIN_1_B,GPIO_PIN_RESET);
			}
			else if ((SPEED < -threshold) ||  ((SPEED < -threshold2) && 
					 ((v1 != 0) || (v3 != 0) || (v4 != 0) )))
			{
				HAL_GPIO_WritePin(PIN_1_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(PIN_1_B,GPIO_PIN_SET);
			}
			else 
			{
				HAL_GPIO_WritePin(PIN_1_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(PIN_1_B,GPIO_PIN_RESET);
				SPEED=0;
			}
			__HAL_TIM_SET_COMPARE(&htim1,PWM1,(int) fabs(SPEED));
			break;
		}
		case 1:
		{
			if (SPEED > threshold) 
			{
				HAL_GPIO_WritePin(PIN_2_A,GPIO_PIN_SET);
				HAL_GPIO_WritePin(PIN_2_B,GPIO_PIN_RESET);
			}
			else if ((SPEED < -threshold) ||  ((SPEED < -threshold2) && 
					 ((v2 != 0) || (v3 != 0) || (v4 != 0) )))
			{
				HAL_GPIO_WritePin(PIN_2_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(PIN_2_B,GPIO_PIN_SET);
			}
			else 
			{
				HAL_GPIO_WritePin(PIN_2_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(PIN_2_B,GPIO_PIN_RESET);
				SPEED=0;
			}
			__HAL_TIM_SET_COMPARE(&htim1,PWM2,(int) fabs(SPEED));
			
			break;
		}
		case 3:
		{
			if (SPEED > threshold)
			{
				HAL_GPIO_WritePin(PIN_3_A,GPIO_PIN_SET);
				HAL_GPIO_WritePin(PIN_3_B,GPIO_PIN_RESET);
			}
			else if (SPEED < -threshold)
			{
				HAL_GPIO_WritePin(PIN_3_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(PIN_3_B,GPIO_PIN_SET);
			}
			else 
			{
				HAL_GPIO_WritePin(PIN_3_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(PIN_3_B,GPIO_PIN_RESET);
				SPEED=0;
			}
			
			
			__HAL_TIM_SET_COMPARE(&htim1,PWM3,(int) fabs(SPEED));
		
			break;
		}
		case 4:
		{
			if (SPEED > threshold)
			{
				HAL_GPIO_WritePin(PIN_4_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(PIN_4_B,GPIO_PIN_SET);
			}
			else if (SPEED < -threshold)
			{
				HAL_GPIO_WritePin(PIN_4_A,GPIO_PIN_SET);
				HAL_GPIO_WritePin(PIN_4_B,GPIO_PIN_RESET);
			
			}
			else 
			{
				HAL_GPIO_WritePin(PIN_4_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(PIN_4_B,GPIO_PIN_RESET);
				SPEED=0;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            	
			}
			
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,(int) fabs(SPEED));
		
			break;
		}
	}
}



void ENCODER_READ_Start(void)
{
	prestate_1 = 0;
	prestate_2 = 0;
	prestate_3 = 0;
	prestate_4 = 0;
	FirstCall_1 = 1;
	FirstCall_2 = 1;
	FirstCall_3 = 1;
	FirstCall_4 = 1;
	ENABLE_ALL_IT();
}

void DISABLE_ALL_IT(void)
{
	HAL_TIM_IC_Stop(&htim2,TIM_CHANNEL_1);
	HAL_TIM_IC_Stop(&htim2,TIM_CHANNEL_3);
	HAL_TIM_IC_Stop(&htim3,TIM_CHANNEL_1);
	HAL_TIM_IC_Stop(&htim3,TIM_CHANNEL_3);
	HAL_TIM_IC_Stop(&htim4,TIM_CHANNEL_1);
	HAL_TIM_IC_Stop(&htim4,TIM_CHANNEL_3);
	HAL_TIM_IC_Stop(&htim8,TIM_CHANNEL_1);
	HAL_TIM_IC_Stop(&htim8,TIM_CHANNEL_3);
	
}
void ENABLE_ALL_IT(void)
{
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_3);
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_3);
	HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_3);
	HAL_TIM_IC_Start_IT(&htim8,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim8,TIM_CHANNEL_3);
	__HAL_TIM_SetCounter(&htim2,0);
	__HAL_TIM_SetCounter(&htim3,0);
	__HAL_TIM_SetCounter(&htim4,0);
	__HAL_TIM_SetCounter(&htim8,0);
}

void Delay(uint32_t delay)
{
	delay=delay*17;
	while(delay--)
	{	
	}
}
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  LL_I2C_InitTypeDef I2C_InitStruct;

  LL_GPIO_InitTypeDef GPIO_InitStruct;

  /**I2C1 GPIO Configuration  
  PB8   ------> I2C1_SCL
  PB9   ------> I2C1_SDA 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8|LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* I2C1 interrupt Init */
  NVIC_SetPriority(I2C1_EV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(I2C1_EV_IRQn);
  NVIC_SetPriority(I2C1_ER_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(I2C1_ER_IRQn);

    /**I2C Initialization 
    */
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.ClockSpeed = 100000;
  I2C_InitStruct.DutyCycle = LL_I2C_DUTYCYCLE_2;
  I2C_InitStruct.OwnAddress1 = 0;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C1, &I2C_InitStruct);

  LL_I2C_SetOwnAddress2(I2C1, 0);

  LL_I2C_DisableOwnAddress2(I2C1);

  LL_I2C_DisableGeneralCall(I2C1);

  LL_I2C_EnableClockStretching(I2C1);

}

/* TIM1 init function */
static void MX_TIM1_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 419;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 10000;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim1);

}

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_IC_InitTypeDef sConfigIC;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 6;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 15;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM3 init function */
static void MX_TIM3_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_IC_InitTypeDef sConfigIC;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 6;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 15;
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM4 init function */
static void MX_TIM4_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_IC_InitTypeDef sConfigIC;

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 6;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_IC_Init(&htim4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 15;
  if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM6 init function */
static void MX_TIM6_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 83;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 9999;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM8 init function */
static void MX_TIM8_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_IC_InitTypeDef sConfigIC;

  htim8.Instance = TIM8;
  htim8.Init.Prescaler = 6;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 65535;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  if (HAL_TIM_IC_Init(&htim8) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 15 ;
  if (HAL_TIM_IC_ConfigChannel(&htim8, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_IC_ConfigChannel(&htim8, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART3 init function */
static void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PIN_3_A_GPIO_Port, PIN_3_A_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PIN_3_B_GPIO_Port, PIN_3_B_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, PIN_4_B_Pin|PIN_2_B_Pin|PIN_1_B_Pin|PIN_1_A_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PIN_4_A_GPIO_Port, PIN_4_A_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PIN_2_A_GPIO_Port, PIN_2_A_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PE4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PIN_3_A_Pin */
  GPIO_InitStruct.Pin = PIN_3_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(PIN_3_A_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PIN_3_B_Pin */
  GPIO_InitStruct.Pin = PIN_3_B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(PIN_3_B_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PIN_4_B_Pin PIN_2_B_Pin PIN_1_B_Pin PIN_1_A_Pin */
  GPIO_InitStruct.Pin = PIN_4_B_Pin|PIN_2_B_Pin|PIN_1_B_Pin|PIN_1_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PIN_4_A_Pin */
  GPIO_InitStruct.Pin = PIN_4_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(PIN_4_A_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PIN_2_A_Pin */
  GPIO_InitStruct.Pin = PIN_2_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(PIN_2_A_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
