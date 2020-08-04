#ifndef DARKCHANEEL_H
#define DARKCHANEEL_H
#include "cv.hpp"


//提取图像暗通道
class DarkChannel{

public:
	DarkChannel(cv::Mat rgbImg);
	int ExtractDark();
	int MaxMinFilter(int kernel);
	int MinFilter(int ksize);
	cv::Mat resMat;
private:
	cv::Mat m_DarkChannelImg;
	cv::Mat m_RGBImg;
};

#endif