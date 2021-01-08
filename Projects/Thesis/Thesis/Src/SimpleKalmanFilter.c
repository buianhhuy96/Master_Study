/* 
 * SimpleKalmanFilter - a Kalman Filter implementation for single variable models.
 * Created by Denys Sene, January, 1, 2017.
 * Released under MIT License - see LICENSE file for details.
 */


#include "DefineAll.h"

	 
extern float p[5];
	
extern float _err_measure;
extern float _q;
extern float _current_estimate;
extern float _kalman_gain;
extern float _err_measureX;
extern float _qX;
extern float _current_estimateX;
extern float _kalman_gainX;
 

	
	
void SimpleKalmanFilter(float mea_e, float est_e, float q)
{
  _err_measure=mea_e;
	for (int i=1;i<5;i++)
  {p[i]=est_e;}
  _q = q;
}

void SimpleKalmanFilterX(float mea_e, float est_e, float q)
{
  _err_measureX=mea_e;
	p[0]=est_e;
  _qX = q;
}

float updateEstimate(float mea, float last_mea,int device)
{
	if (device != 0)
	{
  _kalman_gain = p[device]/(p[device] + _err_measure);
  _current_estimate = last_mea + _kalman_gain * (mea - last_mea);
  p[device] =  (1 - _kalman_gain)* p[device] + fabs(last_mea-_current_estimate)*_q;
	}
	else
	{
	_kalman_gain = p[0]/(p[0] + _err_measureX);
  _current_estimate = last_mea + _kalman_gain * (mea - last_mea);
  p[0] =  (1 - _kalman_gain)* p[0] + fabs(last_mea-_current_estimate)*_qX;

	}
  return _current_estimate;
}

float updateEstimateX(float mea, float last_mea)
{
  _kalman_gainX = p[0]/(p[0] + _err_measureX);
  _current_estimateX = last_mea + _kalman_gainX * (mea - last_mea);
  p[0] =  (1 - _kalman_gainX)* p[0] + fabs(last_mea-_current_estimate)*_qX;


  return _current_estimateX;
}

void setProcessNoise(float q)
{
  _q=q;
}

float getKalmanGain() {
  return _kalman_gain;
}

