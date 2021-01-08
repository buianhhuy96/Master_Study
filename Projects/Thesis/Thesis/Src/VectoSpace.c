#include "DefineAll.h"



 

void xyspeed(void)
{
	v1_scaled = v1/(float) 2564.706;
	v2_scaled = v2/(float) 2564.706;;
	v3_scaled = v3/(float) 2564.706;;
	v4_scaled = v4/(float) 2564.706;;
	//float* xyspeed= (float *) calloc(3,sizeof(float));
	float* vehicle_speed = (float *) calloc(3,sizeof(float));
	float k=cos(PI/4);

	vehicle_speed[0]=k*((v1-v2)-v3+v4)/2;
	vehicle_speed[1]=k*((v1+v2)-v3-v4)/2;
	//vehicle_speed[2]=(v1+v2+v3+v4)/(4*(float)D);
	//vehicle_speed[0]*cos(0) - vehicle_speed[1]*sin(0);//
//	vehicle_speed[0]*sin(0) + vehicle_speed[1]*cos(0);//
	speed[0]=vehicle_speed[0]*cos(current_location[2]) - vehicle_speed[1]*sin(current_location[2]);
	speed[1]=vehicle_speed[1]*sin(current_location[2]) + vehicle_speed[1]*cos(current_location[2]);
	speed[2]=(v1_scaled+v2_scaled+v3_scaled+v4_scaled)/4/D;
	speed[0]=speed[0]/(float)	2564.706;
	speed[1]=speed[1]/(float) 2564.706;
	free(vehicle_speed);
	
}

void DRlocate(void)
{
	//float *newPosition= (float *) calloc(3,sizeof(float));
	current_location[0] = current_location[0]+ (float) T*(speed[0]+speed_old[0])/2;
	current_location[1] = current_location[1]+ (float) T*(speed[1]+speed_old[1])/2;
	if (!compass_activation_flag)
	{
		current_location[2] = current_location[2]+ (float) T*(speed[2]+speed_old[2])/2;
			if (current_location[2] < 0) current_location[2] = current_location[2] + 2*PI;
			if (current_location[2] > 2*PI) current_location[2] = current_location[2] - 2*PI;
	}
}

void xyspeed_inv(void)
{
	float* vehicle_speed = (float *) calloc(3,sizeof(float));
	vehicle_speed[0] = speed_set[0]*cos(current_location[2]) + speed_set[1]*sin(current_location[2]);
	vehicle_speed[1] = (-1)*speed_set[0]*sin(current_location[2]) + speed_set[1]*cos(current_location[2]);
	vehicle_speed[2] = speed_set[2];
		float k=cos(PI/4);
	v1_set = k*(vehicle_speed[0]+vehicle_speed[1]) + D*vehicle_speed[2];
	v2_set = k*((-1)*vehicle_speed[0]+vehicle_speed[1]) + D*vehicle_speed[2];
	v3_set = k*((-1)*vehicle_speed[0]-vehicle_speed[1]) + D*vehicle_speed[2];
	v4_set = k*(vehicle_speed[0]-vehicle_speed[1]) + D*vehicle_speed[2];
	free(vehicle_speed);
}


