#include "DefineAll.h"




float prev2_location[3]={0,0,0};

void Setpoint(void)
{	
	int i=0;
	for (i=0;i<3;i++)
	{
		diff_destination_setpoint[i]=(destination_setpoint[i] - prev_destination_setpoint[i]);
	}
	if (diff_destination_setpoint[2] > PI) diff_destination_setpoint[2] = diff_destination_setpoint[2] - 2*PI;
		else if (diff_destination_setpoint[2] < (-PI)) diff_destination_setpoint[2] =  2*PI + diff_destination_setpoint[2];
	for (i=0;i<3;i++)
	{
		diff_destination_setpoint[i]=diff_destination_setpoint[i]/T;
	}
}
void UpdateSetpoint(void)
{	
	int i=0;
	for (i=0;i<3;i++)
	{
		prev_destination_setpoint[i]=destination_setpoint[i];
	}
}

void Controller(void)
{
	int i;
	
	
	//float x2[3];
	
	
	for (i=0;i<3;i++)
	{
		diff_current_v_setpoint[i]=(-destination_setpoint[i]+current_location[i]);
		if (diff_current_v_setpoint[2] > PI) diff_current_v_setpoint[2] = diff_current_v_setpoint[2] - 2*PI;
		else if (diff_current_v_setpoint[2] < (-PI)) diff_current_v_setpoint[2] =  2*PI + diff_current_v_setpoint[2];
	}
	
	
	S1[0] = diff_current_v_setpoint[0]*cos(current_location[2]) + diff_current_v_setpoint[1]*sin(current_location[2]) ;
	S1[1] = (-1)*diff_current_v_setpoint[0]*sin(current_location[2]) + diff_current_v_setpoint[1]*cos(current_location[2]) ;
	S1[2] = diff_current_v_setpoint[2];
	
	x2n_set[0] = cos(current_location[2])*diff_destination_setpoint[0]+sin(current_location[2])*diff_destination_setpoint[1]-k11*S1[0];
	x2n_set[1] = -sin(current_location[2])*diff_destination_setpoint[0]+cos(current_location[2])*diff_destination_setpoint[1]- S1[1]*k12;
	x2n_set[2] = diff_destination_setpoint[2] - S1[2]*k13;
	if (Controller_ver2==1)
	{
		float a = T/(T+Tp);
		for (i=0;i<3;i++)
		{
			x2_set[i] = (1-a)*x2_set[i]+a*x2n_set[i];
		}
	}
	else 
	{
		x2_set[0] = cos(current_location[2])*diff_destination_setpoint[0] + sin(current_location[2])*diff_destination_setpoint[1] + k11*cos(current_location[2])*diff_current_v_setpoint[0] + sin(current_location[2])*diff_current_v_setpoint[1]*k12;
		x2_set[1] = -0.05*sin(current_location[2])+diff_destination_setpoint[1]*cos(current_location[2])-k11*sin(current_location[2])*diff_current_v_setpoint[0]+cos(current_location[2])*diff_current_v_setpoint[1]*k12;
		x2_set[2] = diff_destination_setpoint[2] + diff_current_v_setpoint[2]*k13;
		
	}
	for (i=0;i<3;i++)
	{
			dx2_set[i]=(x2n_set[i]-x2_set[i])/Tp; 
	}
	for (i=0;i<3;i++)
	{
		S2[i]=(speed[i]-x2_set[i]);
	}
	u[0] = dx2_set[0] - speed[1]*speed[2] - S2[0]*k2 ;
	u[1] = dx2_set[1] - (-1)*speed[0]*speed[2] - S2[1]*k2;
	u[2] = dx2_set[2]  - (S2[2])*k2 ;
		float k=cos(PI/4);
		float a = m*r/k;
		float b = J*r/D;
	motor_torque[0] = ( a*u[0] + a*u[1] + b*u[2])/4;
	motor_torque[1] = (-a*u[0] + a*u[1] + b*u[2])/4;
	motor_torque[2] = (-a*u[0] - a*u[1] + b*u[2])/4;
	motor_torque[3] = ( a*u[0] - a*u[1] + b*u[2])/4;
	float B_t[3];
	B_t[0] = (cos(current_location[2] + PI/4)*motor_torque[0] + cos(current_location[2] +3*PI/4)*motor_torque[1] + cos(current_location[2] - 3*PI/4)*motor_torque[2] + cos(current_location[2] - PI/4)*motor_torque[3])/r;
	B_t[1] = (sin(current_location[2] + PI/4)*motor_torque[0] + sin(current_location[2] +3*PI/4)*motor_torque[1] + sin(current_location[2] - 3*PI/4)*motor_torque[2] + sin(current_location[2] - PI/4)*motor_torque[3])/r;
	B_t[2] = D*(motor_torque[0] + motor_torque[1] + motor_torque[2] + motor_torque[3])/r;
	q2[0]=B_t[0]/m;
	q2[1]=B_t[1]/m;
	q2[2]=B_t[2]/J;
	
	prev_location[0]= q2[0]*T*T/2 + 2*prev_location[0] - prev2_location[0];
	prev_location[1]= q2[1]*T*T/2 + 2*prev_location[1] - prev2_location[1];
	prev_location[2]= q2[2]*T*T/2 + 2*prev_location[2] - prev2_location[2];
	
	speed_setpoint[0] = (prev_location[0] - prev2_location[0])/T;
	speed_setpoint[1] = (prev_location[1] - prev2_location[1])/T;
	speed_setpoint[2] = (prev_location[2] - prev2_location[2])/T;
	
	prev2_location[0]=prev_location[0];
	prev2_location[1]=prev_location[1];
	prev2_location[2]=prev_location[2];
		
	prev_S2[0]=S2[0];
	prev_S2[1]=S2[1];
	prev_S2[2]=S2[2];
	


}

void pwmCalculation(void)
{
	PWM_ON_1 = fabs(v1_set)*kT/D;
	PWM_ON_2 = fabs(v2_set)*kT/D;
	PWM_ON_3 = fabs(v3_set)*kT/D;
	PWM_ON_4 = fabs(v4_set)*kT/D;
	PWM_ON_1 = (PWM_ON_1*1.16f)*10000/VCC;
	PWM_ON_2 = (PWM_ON_2*1.16f)*10000/VCC;
	PWM_ON_3 = (PWM_ON_3)*10000/VCC;
	PWM_ON_4 = (PWM_ON_4)*10000/VCC;
	PID();

	if (v1_set < 0) PWM_ON_1 = -PWM_ON_1;
	if (v2_set < 0) PWM_ON_2 = -PWM_ON_2;
	if (v3_set < 0) PWM_ON_3 = -PWM_ON_3;
	if (v4_set < 0) PWM_ON_4 = -PWM_ON_4;
	Start_PWM();
}

void PID(void)
{
	
	error[0] = fabs(v1_set) - fabs(v1_scaled);
	I[0] = I[0] + error[0]*T;
	Dt[0] = (error[0] - pre_error[0])/T;
	PWM_ON_1 = PWM_ON_1 + (int) (error[0]*Kp + I[0]*Ki + Dt[0]*Kd);
	pre_error[0] = error[0];
	
	error[1] = fabs(v2_set) - fabs(v2_scaled);
	I[1] = I[1] + error[1]*T;
	Dt[1] = (error[1] - pre_error[1])/T;
	PWM_ON_2 = PWM_ON_2 + (int) (error[1]*(Kp-5) + I[1]*Ki + Dt[1]*Kd);
	pre_error[1] = error[1];
	
	error[2] = fabs(v3_set) - fabs(v3_scaled);
	I[2] = I[2] + error[2]*T;
	Dt[2] = (error[2] - pre_error[2])/T;
	PWM_ON_3 = PWM_ON_3 + (int) (error[2]*Kp + I[2]*Ki + Dt[2]*Kd);
	pre_error[2] = error[2];
	
	error[3] = fabs(v4_set) - fabs(v4_scaled);
	I[3] = I[3] + error[3]*T;
	Dt[3] = (error[3] - pre_error[3])/T;
	PWM_ON_4 = PWM_ON_4 + (int) (error[3]*(Kp-5) + I[3]*Ki + Dt[3]*Kd);
	pre_error[3] = error[3];
}
void Start_PWM(void)
{
	duty_cycle_wheel_1=PWM_ON_1;
	duty_cycle_wheel_2=PWM_ON_2;
	duty_cycle_wheel_3=PWM_ON_3;
	duty_cycle_wheel_4=PWM_ON_4;
}
