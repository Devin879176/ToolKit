#include <io.h>
#include <iostream>
#include "cv.hpp"
#include "HistAn.h"
#include "FileKit.h"
#include "DarkChannel.h"
#include "DecodeVideo.h"
#include "TFForegroundDetect.h"


int main(){

	//std::string filePath = "E:\\data\\testDarkChannel";
	std::string filePath = "E:\\data\\fire\\tmp";
	std::vector<std::string> files;
	int label = 1;

	FileKit fileInstance;
	int res = fileInstance.getFiles(filePath, files, label);

	DecodeVideo  DecodeVideoIns(files);
	CTFVAForegroundDetect FGDetect;

	cv::Mat Frame;
	cv::Mat grayImg;
	//https://github.com/Devin879176/ToolKit.git
	int frameCount = 0;



	bRectVector aObjList;
	while (1){
		int res = DecodeVideoIns.PopFrame(&Frame);
		if (res){
			cv::resize(Frame, Frame, Size(352, 288));
			cv::cvtColor(Frame, grayImg, CV_RGB2GRAY);
			frameCount++;
			std::cout << "frameCount:" << frameCount << std::endl;
			cv::Mat fgImg = cv::Mat::zeros(Frame.rows, Frame.cols, CV_8UC1);
			FGDetect.detect(Frame, grayImg, frameCount, ETF_MOD_GMM, true, 0, aObjList, fgImg.data);
			//FGDetect.detect(Frame, grayImg, frameCount, ETF_MOD_VEP, true, 2, aObjList, fgImg.data);
			std::cout << "aObjList size:" << aObjList.size() << std::endl;

			cv::imshow("videoFrame", Frame);
			cv::imshow("fgImg", fgImg);
			cv::waitKey(1);
		}
		else{
			return 0;
		}
	}

	return 0;
}