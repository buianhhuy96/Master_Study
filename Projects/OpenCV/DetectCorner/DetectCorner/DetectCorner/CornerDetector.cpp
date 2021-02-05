#include "CornerDetector.h"
#include <iostream>

Corners::Corners(const cv::Mat& img) :
	img_(img)
{
	scale_ = DEFAULT_SCALE;
	ksize_ = DEFAULT_KSIZE;
}

Corners::Corners(const cv::Mat& img, 
				 const float scale,
				 const int ksize) :
	img_(img), scale_(scale), ksize_(ksize)
{
	if ((scale_ > MAX_SCALE) || (scale_ < MIN_SCALE))
	{
		std::cout << SCALE_INVALID_RANGE << std::endl;
		scale_ = DEFAULT_SCALE;
	} /* ((scale_ > MAX_SCALE) || (scale_ < MIN_SCALE)) */
	else
	{
		scale_ = scale;
	}

	if ((ksize_ > MAX_SCALE) || (ksize_ < MIN_SCALE))
	{
		std::cout << KSIZE_INVALID_RANGE << std::endl;
		ksize_ = DEFAULT_KSIZE;
	}
	else
	{
		ksize_ = scale;
	}
}

Corners::~Corners()
{

}

void Corners::set_scale(const float scale) 
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

float Corners::get_scale() const
{
	return scale_;
}

void Corners::set_ksize(const int ksize)
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

int Corners::get_ksize() const
{
	return ksize_;
}
