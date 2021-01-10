#ifndef DefineAll_h
#define DefineAll_h
/* Includes */   
#include<stdbool.h> 
#include<stdint.h> 
//#include "legacy.h"
//#include "ll_includes.h"
#include "stm32f4xx_hal_conf.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// General

#define timerscale 12000000.f
#define PI acos(-1.0)//3.14159265358979323846f
#define T 0.03f
#define Tp 0.15f
#define J 0.05f
#define D 0.1094f
#define r 0.03f
#define m 1.5f	
#define kT 0.8f		
#define VCC 7.9f		
#define R 2.6f	
#define Ri 1.0f	
#define k11 5.0f
#define k12 5.0f
#define k13 18.0f
#define k2 9.0f
#define Kp 80.0f
#define Ki 0.0f
#define Kd 0.0f
#define It 0.0f
// Kalman filter
void SimpleKalmanFilter(float mea_e, float est_e, float q);
void SimpleKalmanFilterX(float mea_e, float est_e, float q);
float updateEstimate(float mea, float last_mea, int device);
float updateEstimateX(float mea, float last_mea);
void setMeasurementError(float mea_e);
void setEstimateError(float est_e);
void setProcessNoise(float q);
float getKalmanGain(void);

// HMC5883L
#ifdef __cplusplus
 extern "C" {
#endif 
   	
#define TRUE 1
#define FALSE 0
#define HMC5883L_I2C   I2C1 

   
#define HMC5883L_ADDRESS            0x1E // this device only has one address
#define HMC5883L_DEFAULT_ADDRESS    (HMC5883L_ADDRESS<<1)

#define HMC5883L_RA_CONFIG_A        0x00
#define HMC5883L_RA_CONFIG_B        0x01
#define HMC5883L_RA_MODE            0x02
#define HMC5883L_RA_DATAX_H         0x03
#define HMC5883L_RA_DATAX_L         0x04
#define HMC5883L_RA_DATAY_H         0x05
#define HMC5883L_RA_DATAY_L         0x06
#define HMC5883L_RA_DATAZ_H         0x07
#define HMC5883L_RA_DATAZ_L         0x08
#define HMC5883L_RA_STATUS          0x09
#define HMC5883L_RA_ID_A            0x0A
#define HMC5883L_RA_ID_B            0x0B
#define HMC5883L_RA_ID_C            0x0C

#define HMC5883L_CRA_AVERAGE_BIT    6
#define HMC5883L_CRA_AVERAGE_LENGTH 2
#define HMC5883L_CRA_RATE_BIT       4
#define HMC5883L_CRA_RATE_LENGTH    3
#define HMC5883L_CRA_BIAS_BIT       1
#define HMC5883L_CRA_BIAS_LENGTH    2

#define HMC5883L_AVERAGING_1        0x00
#define HMC5883L_AVERAGING_2        0x01
#define HMC5883L_AVERAGING_4        0x02
#define HMC5883L_AVERAGING_8        0x03

#define HMC5883L_RATE_0P75          0x00
#define HMC5883L_RATE_1P5           0x01
#define HMC5883L_RATE_3             0x02
#define HMC5883L_RATE_7P5           0x03
#define HMC5883L_RATE_15            0x04
#define HMC5883L_RATE_30            0x05
#define HMC5883L_RATE_75            0x06

#define HMC5883L_BIAS_NORMAL        0x00
#define HMC5883L_BIAS_POSITIVE      0x01
#define HMC5883L_BIAS_NEGATIVE      0x02

#define HMC5883L_CRB_GAIN_BIT       7
#define HMC5883L_CRB_GAIN_LENGTH    3

#define HMC5883L_GAIN_1370          0x00
#define HMC5883L_GAIN_1090          0x01
#define HMC5883L_GAIN_820           0x02
#define HMC5883L_GAIN_660           0x03
#define HMC5883L_GAIN_440           0x04
#define HMC5883L_GAIN_390           0x05
#define HMC5883L_GAIN_330           0x06
#define HMC5883L_GAIN_220           0x07

#define HMC5883L_MODEREG_BIT        1
#define HMC5883L_MODEREG_LENGTH     2

#define HMC5883L_MODE_CONTINUOUS    0x00
#define HMC5883L_MODE_SINGLE        0x01
#define HMC5883L_MODE_IDLE          0x02

#define HMC5883L_STATUS_LOCK_BIT    1
#define HMC5883L_STATUS_READY_BIT   0


void HMC5883L_Initialize(void);
bool HMC5883L_TestConnection(void);

// CONFIG_A register
uint8_t HMC5883L_GetSampleAveraging(void);
void HMC5883L_SetSampleAveraging(uint8_t averaging);
uint8_t HMC5883L_GetDataRate(void);
void HMC5883L_SetDataRate(uint8_t rate);
uint8_t HMC5883L_GetMeasurementBias(void);
void HMC5883L_SetMeasurementBias(uint8_t bias);

// CONFIG_B register
uint8_t HMC5883L_GetGain(void);
void HMC5883L_SetGain(uint8_t gain);

// MODE register
uint8_t HMC5883L_GetMode(void);
void HMC5883L_SetMode(uint8_t mode);

// full_message* registers
float HMC5883L_GetHeading(int16_t offset);
// STATUS register
bool HMC5883L_GetLockStatus(void);
bool HMC5883L_GetReadyStatus(void);

void HMC5883L_WriteBits(uint8_t slaveAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t full_message);
void HMC5883L_WriteBit(uint8_t slaveAddr, uint8_t regAddr, uint8_t bitNum, uint8_t full_message);
void HMC5883L_ReadBits(uint8_t slaveAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *full_message); 
void HMC5883L_ReadBit(uint8_t slaveAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *full_message);

//void HMC5883L_I2C_Init(void);
void HMC5883L_I2C_ByteWrite(uint8_t slaveAddr, uint8_t* pBuffer, uint8_t WriteAddr);
void HMC5883L_I2C_BufferRead(uint8_t slaveAddr,uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);
	
// Deadreckoning

void xyspeed(void);
void locateDeadReckoning(void);
void xyspeed_inv(void);
extern float speed[3];
extern float prev_speed[3];
extern float current_location[3];
extern bool compass_activation_flag;
extern float theta;
extern float v1;
extern float v2;
extern float v3;
extern float v4;
extern float v1_scaled;
extern float v2_scaled;
extern float v3_scaled;
extern float v4_scaled;
extern bool Controller_ver2;
// DSC 
void Setpoint(void);
void UpdateSetpoint(void);
void Controller(void);
void pwmCalculation(void);
void Start_PWM(void);
extern float PWM_ON_1;
extern float PWM_ON_2;
extern float PWM_ON_3;
extern float PWM_ON_4; 
extern float theta;
extern float prev_location[3];
extern float prev_location_theta[3];
extern float destination_setpoint[3];
extern float prev_destination_setpoint[3];
extern float diff_destination_setpoint[3];
extern float motor_torque[4];
extern float speed[3];
extern float speed_setpoint[3];
extern float u[3];
extern float q2[3];
extern float x2_set[3];
extern float x2n_set[3];
extern float dx2_set[3];
extern float v1_set;
extern float v2_set;
extern float v3_set;
extern float v4_set;
extern float S1[3];
extern float S2[3];
extern float prev_S2[3];

///PID
void PID(void);
extern float error[4];
extern float pre_error[4];
extern float I[4];
extern float Dt[4];
extern float duty_cycle_wheel_1;
extern float duty_cycle_wheel_2;
extern float duty_cycle_wheel_3;
extern float duty_cycle_wheel_4;
#endif
