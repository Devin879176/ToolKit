#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "TFVEP.h"
#include "libVDT.h"

CTFVEP::CTFVEP(void)
{
	m_handle = vdtCreate();
	m_frame_number = 0;
}

CTFVEP::~CTFVEP(void)
{
	vdtRelease(m_handle);
}

void CTFVEP::Init(int nWidth, int nHeight)
{
	vdtInit(m_handle, nWidth, nHeight);
}

void CTFVEP::Replay()
{
	vdtReset(m_handle);
}

void CTFVEP::Detect(const cv::Mat &image, bRectVector& o_aObjList, unsigned char* pFGMask)
{		
	VDT_RESULT  blobs;
	o_aObjList.clear();
	long res = vdtDetect(m_handle, image.cols, image.rows, image.data, pFGMask, &blobs);
	if (res == 0)
	{
		for (int i = 0; i < blobs.blobCount; i++)
		{
			CBoundBox rc;
			rc.nbeginX = blobs.blobInfo[i].x;
			rc.nbeginY = blobs.blobInfo[i].y;
			rc.nw = blobs.blobInfo[i].w;
			rc.nh = blobs.blobInfo[i].h;
			o_aObjList.push_back(rc);
		}
	}
}
