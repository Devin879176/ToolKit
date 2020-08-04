#pragma once

#ifndef _TF_VDT_H
#define _TF_VDT_H

#include "TFVATrafficComp.h"

class CTFVEP
{
public:
	CTFVEP(void);
	~CTFVEP(void);

	void Init(int nWidth, int nHeight);

	void Detect(const cv::Mat &image, bRectVector& o_aObjList, unsigned char* pFGMask);

	void Replay();

private:
	uint64  m_frame_number;
	long long m_handle;
};

#endif