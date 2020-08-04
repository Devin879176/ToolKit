#include "TFForegroundDetect.h"

CTFVAForegroundDetect::CTFVAForegroundDetect()
{
	m_nIndex = 0;
}

CTFVAForegroundDetect::~CTFVAForegroundDetect()
{

}
void CTFVAForegroundDetect::Reset()
{
	if (m_mode == ETF_MOD_GMM)
	{
		m_CModMixGauss.Replay();
	}
	else if (m_mode == ETF_MOD_CODE)
	{
		m_CModCoodbook.Replay();
	}
	else if (m_mode == ETF_MOD_VIBE)
	{
		m_CModVibe.Replay();
	}
	else if (m_mode == ETF_MOD_VEP)
	{
		m_CModVEP.Replay();
	}
}

int CTFVAForegroundDetect::detect(const Mat &i_rgb, const Mat &i_gray, uint64 i_frame, ETF_MOD_MODE i_mode, bool i_bUpdateBG, int i_nSensibility, bRectVector& o_aObjList, unsigned char* o_pFG)
{
	int res = TF_SP_OK;
	
	m_mode = i_mode;

	if (m_mode == ETF_MOD_GMM)
	{
		if (i_frame < 1)
		{
			m_CModMixGauss.Replay();
		}

		if (m_nIndex == 0)
		{
			m_CModMixGauss.Detect(i_gray.cols, i_gray.rows, i_gray.cols, i_gray.data, i_gray.cols, o_pFG, i_bUpdateBG);
			m_nIndex++;
		}
		else
		{
			m_CModMixGauss.DetectOnly(i_gray.cols, i_gray.rows, i_gray.cols, i_gray.data, i_gray.cols, o_pFG);
			m_nIndex++;
			if (m_nIndex == 3)
			{
				m_nIndex = 0;
			}
		}
		int nSizeThre = i_gray.cols * i_gray.rows / 4000;
		m_CConnectedAnalysis.GetConnectedArea(i_gray.cols, i_gray.rows, i_gray.cols, o_pFG, o_aObjList, nSizeThre);
	}
	else if (m_mode == ETF_MOD_CODE)
	{
		if (i_frame < 1)
		{
			m_CModCoodbook.Replay();
		}
		
		if (m_nIndex == 0)
		{
			m_CModCoodbook.Detect(i_gray.cols, i_gray.rows, i_gray.cols, i_gray.data, i_gray.cols, o_pFG, i_nSensibility, i_bUpdateBG);
			m_nIndex++;
		}
		else
		{
			m_CModCoodbook.DetectOnly(i_gray.cols, i_gray.rows, i_gray.cols, i_gray.data, i_gray.cols, o_pFG, i_nSensibility);
			m_nIndex++;
			if (m_nIndex == 3)
			{
				m_nIndex = 0;
			}
		}
		int nSizeThre = i_gray.cols * i_gray.rows / 4000;
		m_CConnectedAnalysis.GetConnectedArea(i_gray.cols, i_gray.rows, i_gray.cols, o_pFG, o_aObjList, nSizeThre);
	}
	else if (m_mode == ETF_MOD_VIBE)
	{
		m_CModVibe.Detect(i_rgb.cols, i_rgb.rows, i_rgb.cols, i_rgb.data, i_rgb.cols, o_pFG, i_bUpdateBG);
		int nSizeThre = i_gray.cols * i_gray.rows / 4000;
		m_CConnectedAnalysis.GetConnectedArea(i_gray.cols, i_gray.rows, i_gray.cols, o_pFG, o_aObjList, nSizeThre);
	}
	else if (m_mode == ETF_MOD_VEP)
	{
		if (m_nIndex == 0)
		{
			m_CModVEP.Init(i_rgb.cols, i_rgb.rows);
			m_nIndex++;
		}

		m_CModVEP.Detect(i_rgb, o_aObjList, o_pFG);
	}

	return res;
}

void CTFVAForegroundDetect::SelectMod(ETF_MOD_MODE i_mode, ETF_MOD_MODE &mode)
{
	mode = i_mode;
}
