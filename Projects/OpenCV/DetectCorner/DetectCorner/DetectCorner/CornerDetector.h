#ifndef CORNERDETECTOR_HH
#define CORNERDETECTOR_HH

#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>   
#include<string.h>

const float MAX_SCALE = 2.0;
const float MIN_SCALE = 0.5;
const float DEFAULT_SCALE = 1.0;
const std::string SCALE_INVALID_RANGE = "ERROR: Value of \"scale\" is not in valid range.";

const int MAX_KSIZE = 9;
const int MIN_KSIZE = 1;
const int DEFAULT_KSIZE = 3.0;
const std::string KSIZE_INVALID_RANGE = "ERROR: Value of \"ksize\" is not in valid range.";
#pragma once
class Corners
{
public:
	Corners(const cv::Mat& img);
	Corners(const cv::Mat& img,
			const float scale,
			const int ksize);
	~Corners();
	void set_scale(const float scale);
	float get_scale() const;
	void set_ksize(const int ksize);
	int get_ksize() const;
private:
	cv::Mat img_;
	float scale_;
	int ksize_;
};

#endif // CORNERDETECTOR_HH
