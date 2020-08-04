#ifndef HISTAN_H 
#define  HISTAN_H

#include "cv.hpp"
#include <iostream>
#include <FileKit.h>

#define SHOW 1

class HistAn
{
public:
	void Init(cv::Mat& rgbImg);	
	int GrayHist();
	int rgbHist();
	int hsvHist();
	
private:
	cv::Mat grayImg;
	std::vector<cv::Mat> rgbImgList;   //通道分离后的RGB
	std::vector<cv::Mat> hsvImgList;
};


#endif