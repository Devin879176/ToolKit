#pragma once
#ifndef _TRAFFIC_COMP_H_
#define _TRAFFIC_COMP_H_

#include <stdio.h>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <list>
#include <math.h>
#include <time.h>
#include <algorithm>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "TFVATrafficMacro.h"


using namespace std;
using namespace cv;

#define  NAREANUM 15

static void ShowWindow(std::string title, cv::Mat img, int isShow)
{
	if (isShow == 0)
		return;
	imshow(title, img);
	cvWaitKey(1);
}

union FloatRand
{
	struct
	{
		unsigned long Frac : 23;
		unsigned long Exp : 8;
		unsigned long Signed : 1;
	} BitArea;
	float Value;
	unsigned long Binary; /* for debug only */
};

static unsigned long long num = 1;
static float GetFloatRand()
{
	int nt = time(0) % 100000;
    unsigned long long value = time(0) * (num++);

	srand(value);

	union FloatRand r;
	r.BitArea.Signed = 0;
	r.BitArea.Exp = 1;
	r.BitArea.Frac = (rand() * rand()) % 0x800000;
	if (r.BitArea.Frac == 0x7FFFFF)
		r.BitArea.Exp = 0x7D;
	else if (r.BitArea.Frac == 0)
		r.BitArea.Exp = 0x7E;
	else
		r.BitArea.Exp = 0x7E;

		int temp = rand() % 2;
	if (temp == 0)
		temp = -1;

	return r.Value*temp;
}

static int alarmCount = 0;
static void GetTrafficEventID(char *cEventID)
{
	if (cEventID == NULL)
		return;

	time_t timer;//time_t就是long int 类型
	timer = time(NULL);
	struct tm* current_time = localtime(&timer);

	if (alarmCount >= 999)
		alarmCount = 0;

	sprintf(cEventID, "%04d%02d%02d%02d%02d%02d%03d",
		current_time->tm_year + 1900, current_time->tm_mon + 1, current_time->tm_mday,
		current_time->tm_hour, current_time->tm_min, current_time->tm_sec, alarmCount++);
}


#define TFABS(K)((K)>=0?(K):-(K))
static bool IsSamePTZ(TF_PTZ *ptz1, TF_PTZ *ptz2)
{
	if (TFABS(ptz1->wPanPos - ptz2->wPanPos)<10
		&& TFABS(ptz1->wTiltPos - ptz2->wTiltPos)<5
		&& TFABS(ptz1->wZoomPos - ptz2->wZoomPos)<2)
		return true;
	else
		return false;
}

#endif
