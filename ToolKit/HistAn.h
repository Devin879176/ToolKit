#include <iostream>
#include "cv.hpp"
#include <FileKit.h>

#define SHOW 0

class HistAn :FileKit
{

	HistAn(cv::Mat& rgbImg);
	~HistAn();

	void GrayHist(cv::Mat grayImg);
	void rgbHist(std::vector<cv::Mat> rgbImgList);
	void hsvHist(std::vector<cv::Mat> hsvImgList);
	
private:
	cv::Mat grayImg;
	std::vector<cv::Mat> rgbImgList;   //通道分离后的RGB
	std::vector<cv::Mat> hsvImgList;
};


