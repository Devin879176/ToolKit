#include <io.h>
#include <memory>
#include <iostream>
#include "HistAn.h"
#include "cv.hpp"

void HistAn::Init(cv::Mat& rgbImg){

	if (!rgbImg.empty()){
		if (rgbImg.channels() == 3){
			cv::cvtColor(rgbImg, grayImg, CV_BGR2GRAY);
			cv::split(rgbImg, rgbImgList);
		}
		else{
			grayImg = rgbImg.clone();
		}
	}
	else{
		std::cout << "image empty!" << std::endl;
		return;
	}
}

int HistAn::GrayHist(){

 	int channels[] = { 0 };
	const int histSize[] = { 256 };
	float range[] = { 0, 256 };
	const float* ranges[] = { range };
	cv::Mat hist;

	cv::calcHist(&grayImg, 1, channels, cv::Mat(), hist, 1, histSize, ranges, true, false);
	double maxValue = 0;
	minMaxLoc(hist, 0, &maxValue, 0, 0);
	int rows = cvRound(maxValue);
	cv::Mat histImage = cv::Mat::zeros(rows, 256, CV_8UC1);

	int black_thresh = 130;
	int low_sum = 0;
	for (int i = 0; i < 35; i++){
		low_sum += (int)(hist.at<float>(i, 0));
	}
	//float black_ratio = low_sum*1. 0 / cv::sum(hist)[0];
	////std::cout << "black_ratio:" << black_ratio << std::endl;
	//if (black_ratio > 0.3)
	//	return 0;

	int white_thresh = 180;
	//int up_sum = 0;
	//for (int i = white_thresh; i < 256; i++){
	//	up_sum += (int)(hist.at<float>(i, 0));
	//}
	//float white_ratio = low_sum*1.0 / cv::sum(hist)[0];
	////std::cout << "white_ratio:" << white_ratio << std::endl;
	//if (white_ratio > 0.0002)
	//	return 0;


#if SHOW
	histImage.col(black_thresh - 1).rowRange(cv::Range(0, rows)) = 255;
	histImage.col(black_thresh).rowRange(cv::Range(0, rows)) = 255;
	histImage.col(black_thresh + 1).rowRange(cv::Range(0, rows)) = 255;

	histImage.col(white_thresh - 1).rowRange(cv::Range(0, rows)) = 255;
	histImage.col(white_thresh).rowRange(cv::Range(0, rows)) = 255;
	histImage.col(white_thresh + 1).rowRange(cv::Range(0, rows)) = 255;

	for (int i = 0; i < 256; i++)
	{
		int temp = (int)(hist.at<float>(i, 0));
		if (temp)
		{
			histImage.col(i).rowRange(cv::Range(rows - temp, rows)) = 255;
		}
	}
	cv::Mat resizeImage;
	cv::resize(histImage, resizeImage, cv::Size(256, 256));
	cv::imshow("gray_img:", grayImg);
	cv::imshow("histImage:", resizeImage);
	cv::waitKey(0);
#endif
	return 0;
}


//void main(){
//
//	std::string fileName = "E:/data/fire/testImage/20200728_20200728000006_ÕºœÒ÷ ¡ø.jpg";
//	cv::Mat src = cv::imread(fileName);
//
//	cv::Mat gray;
//	cv::cvtColor(src, gray, CV_BGR2GRAY);
//
//	/*std::vector<cv::Mat> hsvList;
//	cv::split(dst, hsvList);*/
//	cv::imshow("gray", gray);
//	cv::waitKey(0);
//	//cv::CvtColor(src, dst, CV_BGR2HSV);
//
//}