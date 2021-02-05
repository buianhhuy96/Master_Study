#ifndef CORNERDETECTOR_HH
#define CORNERDETECTOR_HH

#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>   
#include<string.h>

#define HARRIS_BLOCK_SIZE 3
#define HARRIS_K_SIZE 3
#define HARRIS_K 0.1

const float MAX_SCALE = 2.0;
const float MIN_SCALE = 0.5;
const float DEFAULT_SCALE = 1.0;
const std::string SCALE_INVALID_RANGE = "ERROR: Value of \"scale\" is not in valid range.";

const int MAX_KSIZE = 9;
const int MIN_KSIZE = 1;
const int DEFAULT_KSIZE = 3.0;
const std::string KSIZE_INVALID_RANGE = "ERROR: Value of \"ksize\" is not in valid range.";
#pragma once
class CornerDetector
{
public:
	CornerDetector(const float scale,
				   const int ksize);
	~CornerDetector(void);
	void setScale(const float scale);
	float getScale(void) const;
	void setKsize(const int ksize);
	int getKsize(void) const;
	cv::Mat detectCorners(cv::Mat img);
private:
	float scale_;
	int ksize_;
};

#endif // CORNERDETECTOR_HH
