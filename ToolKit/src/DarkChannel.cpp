#include <iostream>
#include "cv.hpp"
#include "DarkChannel.h"

DarkChannel::DarkChannel(cv::Mat orgImg){
	if (orgImg.empty()){
		std::cout << "image empty£¡" << std::endl;
		return;
	}

	if (orgImg.channels() != 3){
		std::cout << "no rgb image£¡" << std::endl;
		return;
	}
	m_DarkChannelImg = cv::Mat::zeros(cv::Size(orgImg.rows, orgImg.cols), CV_8UC1);
	m_RGBImg = orgImg;

}

int DarkChannel::ExtractDark(){

	for (int rowIndex = 0; rowIndex < m_RGBImg.rows; rowIndex++){
		for (int colIndex = 0; colIndex < m_RGBImg.cols; colIndex++){

			int b = m_RGBImg.at<cv::Vec3b>(rowIndex, colIndex)[0];
			int g = m_RGBImg.at<cv::Vec3b>(rowIndex, colIndex)[1];
			int r = m_RGBImg.at<cv::Vec3b>(rowIndex, colIndex)[2];

			m_DarkChannelImg.at<uchar>(rowIndex, colIndex) = b;
			if (g < b)
				m_DarkChannelImg.at<uchar>(rowIndex, colIndex) = g;
			if (r < g)
				m_DarkChannelImg.at<uchar>(rowIndex, colIndex) = r;

			/*std::cout << "(" << b << "," << g << "," << r << ")" << std::endl;
			std::cout << int(m_DarkImg.at<uchar>(rowIndex, colIndex)) << std::endl;*/
		}
	}

	MinFilter(15);

	/*for (int rowIndex = 0; rowIndex < rgbImg.rows; rowIndex++){
		for (int colIndex = 0; colIndex < rgbImg.cols; colIndex++){
		std::cout << int(m_DarkImg.at<uchar>(rowIndex, colIndex)) << std::endl;
		}
		}*/
	return 0;
}


int DarkChannel::MaxMinFilter(int kernel){

	int r = (kernel - 1) / 2;
	cv::Mat dst_ex;
	cv::copyMakeBorder(m_RGBImg, dst_ex, r, r, r, r, cv::BORDER_CONSTANT, cv::Scalar(255));
	for (int i = r; i < dst_ex.rows - r; i++)
	{
		for (int j = r; j < dst_ex.cols - r; j++)
		{
			int minVal = dst_ex.at<uchar>(i, j);
			for (int s = -r; s < r + 1; s++)
			{
				for (int t = -r; t < r + 1; t++)
				{
					if (dst_ex.at<uchar>(i + s, j + t) < minVal)
					{
						minVal = dst_ex.at<uchar>(i + s, j + t);
					}
				}
			}
			m_DarkChannelImg.at<uchar>(i - r, j - r) = minVal;
		}
	}
	return 0;
}


int DarkChannel::MinFilter(int ksize)
{
	int r = (ksize - 1) / 2;
	cv::Mat dst_ex(cv::Size(m_DarkChannelImg.rows + ksize - 1, m_DarkChannelImg.cols + ksize - 1), CV_8UC1);
	resMat = cv::Mat::zeros(cv::Size(m_DarkChannelImg.cols, m_DarkChannelImg.rows), CV_8UC1);
	copyMakeBorder(m_DarkChannelImg, dst_ex, r, r, r, r, cv::BORDER_DEFAULT);
	for (int row_index = 0; row_index < m_DarkChannelImg.rows; row_index++)
	{
		for (int col_index = 0; col_index < m_DarkChannelImg.cols; col_index++)
		{
			cv::Rect roi(col_index, row_index, ksize, ksize);
			//cv::Mat roi_of_image = dst_ex(roi);
			double minVal = 0, maxVal = 0;
			cv::Point minPt, maxPt;
			minMaxLoc(dst_ex(roi), &minVal, &maxVal, &minPt, &maxPt);
			resMat.at<uchar>(row_index, col_index) = minVal;
		}
	}
	return 0;
}