//#include <io.h>
//#include <memory>
//#include <iostream>
//#include <fstream> 
//#include <math.h>
//#include <windows.h>
//#include <direct.h>
//#include <time.h>
//#include <tchar.h>
//#include "cv.hpp"
//
///**
//* 获取指定路径下的图片或者视频文件
//* @param[path][输入的路径]
//* @param[files][存储指定目录下item]
//* @param[category][要获取的的item类型，0表示图片，1表示视频]
//* @return[成功返回0，失败返回-1]
//*/
//int GetItems(std::string path, std::vector<std::string>& files, int category)
//{
//	//文件句柄
//	intptr_t   hFile = 0;
//	//文件信息
//	struct _finddata_t fileinfo;
//
//	std::string p;
//
//	if (category == 0){
//
//		if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)  {
//
//			do {
//				//如果是目录,迭代之
//				//如果不是,加入列表
//				if ((fileinfo.attrib  &  _A_SUBDIR)) {
//					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
//						GetItems(p.assign(path).append("\\").append(fileinfo.name), files, category);
//				}
//				else {
//					if (strstr(fileinfo.name, "png") != NULL || strstr(fileinfo.name, "bmp") != NULL || \
//						strstr(fileinfo.name, "jpg") != NULL || strstr(fileinfo.name, "tif") != NULL  \
//						)
//						files.push_back(p.assign(path).append("\\").append(fileinfo.name));
//				}
//			} while (_findnext(hFile, &fileinfo) == 0);
//			_findclose(hFile);
//		}
//
//		return 0;
//	}
//	else if (category == 1){
//
//		if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)  {
//
//			do {
//				//如果是目录,迭代之
//				//如果不是,加入列表
//				if ((fileinfo.attrib  &  _A_SUBDIR)) {
//					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
//						GetItems(p.assign(path).append("\\").append(fileinfo.name), files, category);
//				}
//				else {
//					if (
//						strstr(fileinfo.name, "avi") != NULL || strstr(fileinfo.name, "mp4") != NULL || \
//						strstr(fileinfo.name, "dav") != NULL)
//						files.push_back(p.assign(path).append("\\").append(fileinfo.name));
//				}
//			} while (_findnext(hFile, &fileinfo) == 0);
//			_findclose(hFile);
//		}
//
//		return 0;
//	}
//	else{
//		std::cout << "input category param failed!" << std::endl;
//
//		return -1;
//	}
//}
//
//
//
//std::string SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
//{
//	std::string::size_type pos1, pos2;
//	pos2 = s.find(c);
//	pos1 = 0;
//	while (std::string::npos != pos2)
//	{
//		v.push_back(s.substr(pos1, pos2 - pos1));
//
//		pos1 = pos2 + c.size();
//		pos2 = s.find(c, pos1);
//	}
//	if (pos1 != s.length())
//		v.push_back(s.substr(pos1));
//
//	return *v.rbegin();
//}
//
//#define SHOW 1
//
//int main(){
//
//	std::string imageDir = "F:\\Project\\TrafficData\\pic\\shelter\\fuwuqu";
//	std::vector<std::string> names;
//	int type = 0;
//	int res = GetItems(imageDir, names, type);
//	int cannyThresh = 50;
//
//	cv::Mat grayImg;
//	cv::Mat img;
//	cv::Mat cannyImg;
//	cv::Mat binImg;
//	std::vector<std::vector<cv::Point>> contours;
//	std::vector<cv::Vec4i> hierarchy;
//	int binThresh = 50;
//
//	for (int i = 0; i < names.size(); i++){
//
//		img = cv::imread(names[i]);
//
//		std::cout << "cols:" << img.cols << " rows:" << img.rows << std::endl;
//
//		cv::cvtColor(img, grayImg, CV_RGB2GRAY);
//		cv::threshold(grayImg, binImg, binThresh, 255, cv::THRESH_BINARY);
//		cv::dilate(binImg, binImg, cv::Mat(), cv::Point(-1, -1), 2);
//		cv::Canny(binImg, cannyImg, cannyThresh, 2 * cannyThresh);
//		cv::Mat edgeBox = cv::Mat::zeros(img.size(), img.type());
//		cv::Mat contourBox = cv::Mat::zeros(img.size(), img.type());
//		cv::dilate(cannyImg, cannyImg, cv::Mat(), cv::Point(-1, -1), 2);
//
//		// 找到所有轮廓并且存储在序列中
//		cv::findContours(cannyImg.clone(), contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
//
//
//		for (int i = 0; i < contours.size(); i++){
//			if (cv::contourArea(contours[i]) > 5000){
//				for (int k = 0; k < contours[i].size() - 1; k++){
//					cv::Point point1 = contours[i][k];
//					cv::Point point2 = contours[i][k + 1];
//					cv::line(contourBox, point1, point2, CvScalar(255, 255, 255), 1);
//				}
//			}
//		}
//
//
//#if SHOW
////	cv::imshow("canny:", cannyImg);
////	cv::imshow("edgeBox:", edgeBox);
//	cv:imshow("contourBox:", contourBox);
//
//	cv::imshow("org", img);
//  //cv::imshow("binImg", binImg);
//	cv::waitKey(0);
//#endif 
//
//
//	}
//
//}