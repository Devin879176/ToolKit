#pragma once

#ifndef __TFVA_FOREGROUND_DETECT_H__
#define __TFVA_FOREGROUND_DETECT_H__

#include "TFMOD.h"
#include "TFVIBE.h"
#include "TFVEP.h"
#include "TFConnectedAnalysis.h"

class CTFVAForegroundDetect 
{
public:
	CTFVAForegroundDetect();
	~CTFVAForegroundDetect();

	int detect(const Mat &i_rgb, const Mat &i_gray, uint64 i_frame, ETF_MOD_MODE i_mode, bool i_bUpdateBG, int i_nSensibility, bRectVector& o_aObjList, unsigned char* o_pFG);

	void Reset();
private:
	void SelectMod(ETF_MOD_MODE i_mode, ETF_MOD_MODE &mode);

	CTFCodebookMode m_CModCoodbook; //码本模型
	CTFMixtureGaussian m_CModMixGauss;  //混合高斯模型
	CTFVIBE  m_CModVibe;
	CTFVEP   m_CModVEP;
	CTFConnectedAnalysis m_CConnectedAnalysis; //连通域分析功能类
	ETF_MOD_MODE  m_mode;
	int m_nIndex;
};

#endif