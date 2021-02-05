#include "CornerDetector.h"
#include <iostream>

CornerDetector::CornerDetector(const float scale, const int ksize) :
									scale_(scale), ksize_(ksize)
{
	if ((scale_ > MAX_SCALE) || (scale_ < MIN_SCALE))
	{
		std::cout << SCALE_INVALID_RANGE << std::endl;
		scale_ = DEFAULT_SCALE;
	} 
	else
	{
		scale_ = scale;
	} /* End of if((scale_ > MAX_SCALE) || (scale_ < MIN_SCALE)) */

	if ((ksize_ > MAX_SCALE) || (ksize_ < MIN_SCALE))
	{
		std::cout << KSIZE_INVALID_RANGE << std::endl;
		ksize_ = DEFAULT_KSIZE;
	}
	else
	{
		ksize_ = scale;
	} /* End of if((ksize_ > MAX_SCALE) || (ksize_ < MIN_SCALE))) */
}

CornerDetector::~CornerDetector(void)
{

}

void CornerDetector::setScale(const float scale) 
{
	if ((scale <= MAX_SCALE) && (scale >= MIN_SCALE))
	{
		scale_ = scale;
	} /* (scale <= MAX_SCALE) && (scale >= MIN_SCALE) */
	else
	{
		std::cout << SCALE_INVALID_RANGE << std::endl;		
	}
}

float CornerDetector::getScale(void) const
{
	return scale_;
}

void CornerDetector::setKsize(const int ksize)
{
	if ((ksize <= MAX_KSIZE) && (ksize >= MIN_KSIZE))
	{
		ksize_ = ksize;
	} /* (ksize <= MAX_KSIZE) && (ksize >= MIN_KSIZE) */	
	else
	{
		std::cout << KSIZE_INVALID_RANGE << std::endl;
	}
}

int CornerDetector::getKsize(void) const
{
	return ksize_;
}

cv::Mat detectCorners(cv::Mat img)
{
	cv::Mat img_gray;
	cv::cvtcolor(img,img_gray,cv::COLOR_BGR2GRAY);
	
	cv::Mat img_resized;
	cv::resize(img_gray, img_resized, Size(), scale_, scale_, CV_INTER_CUBIC);
	cv::Mat img_filtered;
	cv::medianBlur(img_resized, img_filtered, ksize_);
	cv::Mat img_corners;
	cv::cornerHarris(img_filtered, img_corners, HARRIS_BLOCK_SIZE, HARRIS_K_SIZE, HARRIS_K, BORDER_DEFAULT);
}
