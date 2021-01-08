/* 
 * SimpleKalmanFilter - a Kalman Filter implementation for single variable models.
 * Created by Denys Sene, January, 1, 2017.
 * Released under MIT License - see LICENSE file for details.
 */

#ifndef SimpleKalmanFilter_h
#define SimpleKalmanFilter_h

	 



  void SimpleKalmanFilter(float mea_e, float est_e, float q);
	void SimpleKalmanFilterX(float mea_e, float est_e, float q);
  float updateEstimate(float mea, float last_mea, int device);
	float updateEstimateX(float mea, float last_mea);
  void setMeasurementError(float mea_e);
  void setEstimateError(float est_e);
  void setProcessNoise(float q);
  float getKalmanGain(void);
  

 


#endif
