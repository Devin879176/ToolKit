#pragma once

#include "cv.hpp"
#include "vector"
#include "string"
#include "iostream"

using namespace std;
using namespace cv;

class DecodeVideo{

public:
	DecodeVideo(vector<string> videoNameVec);

	int PopFrame(Mat* Frame);

private:
	int mVideoIndex = 0;
	int mVideoSize = 0;
	//int mTotalFrame = 0;
	//int mFrameCount = 0;
	VideoCapture curCapture;
	vector<string> mVideoNames;
};