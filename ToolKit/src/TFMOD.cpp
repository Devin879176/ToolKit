#include "TFMOD.h"

CTFCodebookMode::CTFCodebookMode(void)
{
	m_bBeginFrames = true;
	m_nFrameCount = 0;
	m_ncx = 0;
	m_ncy = 0;
	m_pCodebook = NULL;
	m_pPrev = NULL;
}

CTFCodebookMode::~CTFCodebookMode(void)
{
	if ( m_pPrev != NULL )
	{
		delete[] m_pPrev;
		m_pPrev = NULL;
	}

	if( m_pCodebook != NULL )
	{
		delete[] m_pCodebook;
		m_pCodebook = NULL;
	}
}

void CTFCodebookMode::Init( int nWidth, int nHeight )
{
	if (m_ncx != nWidth || m_ncy != nHeight || m_pCodebook == NULL || m_pPrev == NULL){
		m_ncx = nWidth;
		m_ncy = nHeight;
		if( m_pCodebook != NULL ){
			delete[] m_pCodebook;
			m_pCodebook = NULL;
		}
		m_pCodebook = new Codebook[m_ncx*m_ncy];
		
		if (m_pPrev != NULL){
			delete[] m_pPrev;
			m_pPrev = NULL;
		}
		m_pPrev = new unsigned char[m_ncx*m_ncy];
	}
}

void CTFCodebookMode::InitData( int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst )
{
	if( m_bBeginFrames ){
		for (int i =0; i < nHeight; ++i){
			unsigned char* ps = pSrc + i*nScanSrc;
			Codebook* p = m_pCodebook + i*nWidth;
			unsigned char* pr = m_pPrev + i*nWidth;
			for (int j = 0; j < nWidth; ++j){
				SCodeword aItem(ps[j], 1, 1, 1, 0);
				p[j].push_back(aItem);
				pr[j] = ps[j];
			}
		}
		memset(pDst,0,nWidth*nHeight);
		m_nFrameCount++;
		m_bBeginFrames = false;
	}
}

void CTFCodebookMode::DetectImpl( int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, int nSensibility, bool beUpdataBackGround )
{
	int InitDistance = 105-nSensibility;   //105-70=35
	InitDistance = InitDistance < 0 ? 0 : InitDistance;
	int ThreNoRenewTime = 150;
	int ThreNum = 300;
	int ThreMaxTimeInterval = 150;

	float fRate = 0.1f;

	for(int i = 0; i < nHeight; ++i)
	{
		unsigned char* ps = pSrc + i*nScanSrc;
		unsigned char* pd = pDst + i*nScanDst;
		unsigned char* pr = m_pPrev + i*nWidth;
		Codebook* p = m_pCodebook + i*nWidth;
		for(int j = 0; j < nWidth; ++j)
		{
			if ( beUpdataBackGround )
			{//设定更新时候才更新背景
				std::list<SCodeword>::iterator pos_min;
				std::list<SCodeword>::iterator pos;
				double nMin = 1<<30;
				for(pos = p[j].begin(); pos != p[j].end(); ++pos)
				{
					SCodeword* pItem = &(*pos);
					if (nMin > fabs((ps[j] - pItem->m_nCentre)))
					{
						nMin = fabs((ps[j] - pItem->m_nCentre));
						pos_min = pos;
					}
				}

				if ( nMin < /*InitDistance*/(*pos_min).m_nCentre*fRate + 5 )
				{
					for(pos = p[j].begin(); pos != p[j].end(); ++pos)
					{
						SCodeword* pItem = &(*pos);
						if (pos == pos_min)
						{
							pItem->m_nCentre = (pItem->m_nCentre * pItem->m_nNum + ps[j]) / (pItem->m_nNum + 1);
							pItem->m_nNum += 1;
							pItem->m_nMaxTimeInterval = fmax(pItem->m_nMaxTimeInterval, m_nFrameCount - pItem->m_nLastTime);
							pItem->m_nLastTime = m_nFrameCount;
							pItem->m_nNoRenewTime = 0;
						} else{
							pItem->m_nNoRenewTime += 1;
						}
					}

				}
				else
				{
					for(pos = p[j].begin(); pos != p[j].end(); ++pos)
					{
						SCodeword* pItem = &(*pos);
						pItem->m_nNoRenewTime += 1;
					}

					SCodeword aItem( ps[j], 1, 1, m_nFrameCount, 0 );
					p[j].push_back(aItem);

				}


				for(pos = p[j].begin(); pos != p[j].end(); )
				{
					SCodeword* pItem = &(*pos);
					if (pItem->m_nNum < ThreNum && (pItem->m_nMaxTimeInterval > ThreMaxTimeInterval || pItem->m_nNoRenewTime > ThreNoRenewTime) && p[j].size() > 1)
					{
						//delete pItem;
						pos = p[j].erase(pos);
					}
					else
					{
						pos++;
					}
				}
			}
			else
			{//beUpdataBackGround为false时候，条件更新，路面亮度较暗且帧差小，进行更新
				if ( ps[j] < 50 && abs(ps[j]-pr[j]) < 10 )
				{
					std::list<SCodeword>::iterator pos_min;
					std::list<SCodeword>::iterator pos;
					double nMin = 1<<30;
					for(pos = p[j].begin(); pos != p[j].end(); ++pos)
					{
						SCodeword* pItem = &(*pos);
						if (nMin > fabs((ps[j] - pItem->m_nCentre)))
						{
							nMin = fabs((ps[j] - pItem->m_nCentre));
							pos_min = pos;
						}
					}

					if ( nMin < /*InitDistance*/(*pos_min).m_nCentre*fRate + 5 )
					{
						for(pos = p[j].begin(); pos != p[j].end(); ++pos)
						{
							SCodeword* pItem = &(*pos);
							if (pos == pos_min)
							{
								pItem->m_nCentre = (pItem->m_nCentre * pItem->m_nNum + ps[j]) / (pItem->m_nNum + 1);
								pItem->m_nNum += 1;
								pItem->m_nMaxTimeInterval = fmax(pItem->m_nMaxTimeInterval, m_nFrameCount - pItem->m_nLastTime);
								pItem->m_nLastTime = m_nFrameCount;
								pItem->m_nNoRenewTime = 0;
							} else{
								pItem->m_nNoRenewTime += 1;
							}
						}

					}
					else
					{
						for(pos = p[j].begin(); pos != p[j].end(); ++pos)
						{
							SCodeword* pItem = &(*pos);
							pItem->m_nNoRenewTime += 1;
						}

						SCodeword aItem( ps[j], 1, 1, m_nFrameCount, 0 );
						p[j].push_back(aItem);

					}


					for(pos = p[j].begin(); pos != p[j].end(); )
					{
						SCodeword* pItem = &(*pos);
						if (pItem->m_nNum < ThreNum && (pItem->m_nMaxTimeInterval > ThreMaxTimeInterval || pItem->m_nNoRenewTime > ThreNoRenewTime) && p[j].size() > 1)
						{
							//delete pItem;
							pos = p[j].erase(pos);
						}
						else
						{
							pos++;
						}
					}
				}

			}
			

			int Flag = 1;
			std::list<SCodeword>::iterator pos_maxnum;
			long MaxNum = 0;
			std::list<SCodeword>::iterator pos1;
			for( pos1 = p[j].begin(); pos1 != p[j].end(); ++pos1 )
			{
				SCodeword* pItem = &(*pos1);
				if (pItem->m_nNum > MaxNum)
				{
					MaxNum = pItem->m_nNum;
					pos_maxnum = pos1;
				}

				if (pItem->m_nNum >= ThreNum /*&& pItem->m_nNoRenewTime <= ThreNoRenewTime*/)
				{
					if (fabs((ps[j] - pItem->m_nCentre)) < /*InitDistance*/pItem->m_nCentre*fRate + 5)
					{
						Flag = 0;
					}
				} 
			}

			if( fabs( (ps[j]-(*pos_maxnum).m_nCentre) ) < /*InitDistance*/ (*pos_maxnum).m_nCentre*fRate + 5|| Flag == 0 )
			{
				pd[j] = 0;
			}
			else
			{
				pd[j] = 255;
			}

			pr[j] = ps[j];
		}
	}
}

void CTFCodebookMode::DetectImplOnly(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, int nSensibility)
{
	int InitDistance = 105 - nSensibility;   //105-70=35
	InitDistance = InitDistance < 0 ? 0 : InitDistance;
	int ThreNoRenewTime = 150;
	int ThreNum = 300;
	int ThreMaxTimeInterval = 150;

	float fRate = 0.1f;

	for (int i = 0; i < nHeight; ++i)
	{
		unsigned char* ps = pSrc + i*nScanSrc;
		unsigned char* pd = pDst + i*nScanDst;
		unsigned char* pr = m_pPrev + i*nWidth;
		Codebook* p = m_pCodebook + i*nWidth;
		for (int j = 0; j < nWidth; ++j)
		{
			int Flag = 1;
			std::list<SCodeword>::iterator pos_maxnum;
			long MaxNum = 0;
			std::list<SCodeword>::iterator pos1;
			for (pos1 = p[j].begin(); pos1 != p[j].end(); ++pos1)
			{
				SCodeword* pItem = &(*pos1);
				if (pItem->m_nNum > MaxNum)
				{
					MaxNum = pItem->m_nNum;
					pos_maxnum = pos1;
				}

				if (pItem->m_nNum >= ThreNum /*&& pItem->m_nNoRenewTime <= ThreNoRenewTime*/)
				{
					if (fabs((ps[j] - pItem->m_nCentre)) < /*InitDistance*/pItem->m_nCentre*fRate + 5)
					{
						Flag = 0;
					}
				}
			}

			if (fabs((ps[j] - (*pos_maxnum).m_nCentre)) < /*InitDistance*/ (*pos_maxnum).m_nCentre*fRate + 5 || Flag == 0)
			{
				pd[j] = 0;
			}
			else
			{
				pd[j] = 255;
			}

			pr[j] = ps[j];
		}
	}
}

void CTFCodebookMode::Detect( int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, int nSensibility, bool beUpdataBackGround )
{
	if(nWidth == 0 || nHeight == 0 || pSrc == NULL)
		return;

	//Init memory
	Init(nWidth, nHeight);

	if(m_bBeginFrames){//Init data
		InitData(nWidth, nHeight, nScanSrc, pSrc, nScanDst, pDst);
	}else{//detect
		DetectImpl( nWidth, nHeight, nScanSrc, pSrc, nScanDst, pDst, nSensibility, beUpdataBackGround );
		//expand(nWidth,nHeight,nScanDst,pDst,nScanDst,pDst,3);
		//corrode(nWidth,nHeight,nScanDst,pDst,nScanDst,pDst,3);
		DilateAndErode(nWidth, nHeight, nScanDst, pDst, nScanDst, pDst);
		m_nFrameCount++;
	}

}

void CTFCodebookMode::DetectOnly(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, int nSensibility)
{
	if (nWidth == 0 || nHeight == 0 || pSrc == NULL)
		return;

	//Init memory
	Init(nWidth, nHeight);

	if (m_bBeginFrames){//Init data
		InitData(nWidth, nHeight, nScanSrc, pSrc, nScanDst, pDst);
	}else{//detect
		DetectImplOnly(nWidth, nHeight, nScanSrc, pSrc, nScanDst, pDst, nSensibility);
		//expand(nWidth, nHeight, nScanDst, pDst, nScanDst, pDst, 3);
		//corrode(nWidth, nHeight, nScanDst, pDst, nScanDst, pDst, 3);
		DilateAndErode(nWidth, nHeight, nScanDst, pDst, nScanDst, pDst);
		m_nFrameCount++;
	}
}

void CTFCodebookMode::Replay()
{
	if (m_pPrev != NULL)
	{
		delete[] m_pPrev;
		m_pPrev = NULL;
	}

	if (m_pCodebook != NULL)
	{
		delete[] m_pCodebook;
		m_pCodebook = NULL;
	}

	m_bBeginFrames = true;
	m_nFrameCount = 0;
	m_ncx = 0;
	m_ncy = 0;
	m_pCodebook = NULL;
	m_pPrev = NULL;
}

void CTFCodebookMode::GetMoveObjRectList(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, bRectVector& RectList, int nCutTop, int nCutLeft)
{

	RectList.clear();
	m_aIntervalList.clear();

	int a = RectList.size();

	//横向分析
	bool beObjectEnd1 = false;
	int nBeginpos1 = 0;
	for (int i = 0; i < nHeight; ++i)
	{
		unsigned char* pS = pSrc + i*nScanSrc;
		int nLine = 0;
		for (int j = 0; j < nWidth; ++j)
		{
			if (pS[j] == 255)
				nLine++;
		}

		if (nLine < 5)
		{
			if (beObjectEnd1 && i - nBeginpos1 > 5)
			{
				CInterval pItem = CInterval(nBeginpos1, i);
				m_aIntervalList.push_back(pItem);
			}
			beObjectEnd1 = false;
		}
		else
		{
			if (!beObjectEnd1)
			{
				nBeginpos1 = i;
				beObjectEnd1 = true;
			}
			else if (i == nHeight - 1 && i - nBeginpos1 > 5)
			{
				CInterval pItem = CInterval(nBeginpos1, i);
				m_aIntervalList.push_back(pItem);
			}

		}
	}

	std::vector<CInterval>::iterator pos;

	//纵向分析
	for (pos = m_aIntervalList.begin(); pos != m_aIntervalList.end(); ++pos)
	{
		CInterval pItem = *pos;
		int nbegin = pItem.nbegin;
		int nend = pItem.nend;

		bool beObjectEnd2 = false;
		int nBeginpos2 = 0;
		for (int i = 0; i < nWidth; ++i)
		{
			unsigned char* pS = pSrc + i;
			int nCol = 0;
			for (int j = nbegin; j <= nend; ++j)
			{
				if (*(pS + j*nScanSrc) == 255)
					nCol++;
			}

			if (nCol < 5)
			{
				if (beObjectEnd2 && i - nBeginpos2 > 5)
				{
					CBoundBox pItem = CBoundBox(nBeginpos2 + nCutLeft, nbegin + nCutTop, i - nBeginpos2, nend - nbegin, (i - nBeginpos2)*(nend - nbegin));
					RectList.push_back(pItem);

				}
				beObjectEnd2 = false;
			}
			else
			{
				if (!beObjectEnd2)
				{
					nBeginpos2 = i;
					beObjectEnd2 = true;
				}
				else if (i == nWidth - 1 && i - nBeginpos2 > 5)
				{
					CBoundBox pItem = CBoundBox(nBeginpos2 + nCutLeft, nbegin + nCutTop, i - nBeginpos2, nend - nbegin, (i - nBeginpos2)*(nend - nbegin));
					RectList.push_back(pItem);
				}
			}

		}

	}

	for (int index = 0; index < (int)RectList.size(); index++)
	{
		CBoundBox pItem = RectList[index];
		int nBeginx = pItem.nbeginX - nCutLeft;
		int nBeginy = pItem.nbeginY - nCutTop;
		int nEndx = nBeginx + pItem.nw;
		int nEndy = nBeginy + pItem.nh;

		//顶部
		for (int i = nBeginy; i < nEndy; ++i)
		{
			unsigned char* pT = pSrc + i*nScanSrc;
			int nt = 0;
			for (int j = nBeginx; j <= nEndx; ++j)
			{
				if (pT[j] == 255)nt++;
			}

			if (nt < 5)
			{
				nBeginy++;
			}
			else
			{
				break;
			}
		}

		//底部
		for (int i = nEndy; i > nBeginy; --i)
		{
			unsigned char* pB = pSrc + i*nScanSrc;
			int nb = 0;
			for (int j = nBeginx; j <= nEndx; ++j)
			{
				if (pB[j] == 255)nb++;
			}

			if (nb < 5)
			{
				nEndy--;
			}
			else
			{
				break;
			}
		}

		//左侧
		for (int i = nBeginx; i < nEndx; ++i)
		{
			unsigned char* pL = pSrc + i;
			int nl = 0;
			for (int j = nBeginy; j <= nEndy; ++j)
			{
				if (*(pL + j*nScanSrc) == 255)nl++;
			}

			if (nl < 5)
			{
				nBeginx++;
			}
			else
			{
				break;
			}
		}

		//右侧
		for (int i = nEndx; i > nBeginx; --i)
		{
			unsigned char* pR = pSrc + i;
			int nr = 0;
			for (int j = nBeginy; j <= nEndy; ++j)
			{
				if (*(pR + j*nScanSrc) == 255)nr++;
			}

			if (nr < 5)
			{
				nEndx--;
			}
			else
			{
				break;
			}
		}

		pItem.nbeginX = nBeginx + nCutLeft;
		pItem.nbeginY = nBeginy + nCutTop;
		pItem.nw = nEndx - nBeginx;
		pItem.nh = nEndy - nBeginy;

		RectList[index] = pItem;
	}
}

void CTFCodebookMode::expand(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, int nRad)
{
	unsigned char* pTmp = new unsigned char[nWidth*nHeight];
	for ( int i = 0; i < nHeight; ++i )
	{
		unsigned char* pS = pSrc + i*nScanSrc;
		unsigned char* pT = pTmp + i*nWidth;
		memcpy(pT,pS,nWidth);
	}

	for ( int i = nRad; i < nHeight-nRad; ++i )
	{
		unsigned char* pS = pSrc + i*nScanSrc;
		unsigned char* pT = pTmp + i*nWidth;
		for ( int j = nRad; j < nWidth-nRad; ++j )
		{
			if (pT[j]==0)
			{
				for ( int m = i-nRad; m <= i+nRad; ++m )
				{
					bool bebreak = false;
					unsigned char* pSL = pSrc + m*nScanSrc;
					for ( int n = j-nRad; n <= j+nRad; ++n )
					{
						if ( pSL[n] == 255 )
						{
							pT[j] = 255;
							bebreak = true;
							break;
						}
					}

					if ( bebreak )
					{
						break;
					}
				}
			}
		}
	}

	for ( int i = 0; i < nHeight; ++i )
	{
		unsigned char* pD = pDst + i*nScanDst;
		unsigned char* pT = pTmp + i*nWidth;
		memcpy(pD,pT,nWidth);
	}

	if ( pTmp != NULL )
	{
		delete[] pTmp;
	}	
}

void CTFCodebookMode::corrode(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, int nRad)
{
	unsigned char* pTmp = new unsigned char[nWidth*nHeight];
	for ( int i = 0; i < nHeight; ++i )
	{
		unsigned char* pS = pSrc + i*nScanSrc;
		unsigned char* pT = pTmp + i*nWidth;
		memcpy(pT,pS,nWidth);
	}

	for ( int i = nRad; i < nHeight-nRad; ++i )
	{
		unsigned char* pS = pSrc + i*nScanSrc;
		unsigned char* pT = pTmp + i*nWidth;
		for ( int j = nRad; j < nWidth-nRad; ++j )
		{
			if (pT[j]==255)
			{
				for ( int m = i-nRad; m <= i+nRad; ++m )
				{
					bool bebreak = false;
					unsigned char* pSL = pSrc + m*nScanSrc;
					for ( int n = j-nRad; n <= j+nRad; ++n )
					{
						if ( pSL[n] == 0 )
						{
							pT[j] = 0;
							bebreak = true;
							break;
						}
					}

					if ( bebreak )
					{
						break;
					}
				}
			}
		}
	}

	for ( int i = 0; i < nHeight; ++i )
	{
		unsigned char* pD = pDst + i*nScanDst;
		unsigned char* pT = pTmp + i*nWidth;
		memcpy(pD,pT,nWidth);
	}

	if ( pTmp != NULL )
	{
		delete[] pTmp;
	}	
}

void CTFCodebookMode::DilateAndErode(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst)
{
	Mat img(nHeight, nWidth, CV_8UC1);

	for (int i = 0; i < nHeight; ++i)
	{
		unsigned char* pS = pSrc + i * nScanSrc;
		unsigned char* pD = img.ptr<unsigned char>(i);
		for (int j = 0; j < nWidth; ++j)
		{
			pD[j] = pS[j];
		}
	}
	//imwrite("D:/Images/src.jpg", img);
	Mat ele = getStructuringElement(MORPH_RECT, Size(7, 7), Point(3, 3));
	//Mat imgTmp(nHeight, nWidth, CV_8UC1);
	dilate(img, img, ele); //膨胀 
	erode(img, img, ele);  //腐蚀  

	//imwrite("D:/Images/res.jpg", img);

	for (int i = 0; i < nHeight; ++i)
	{
		unsigned char* pD = pDst + i * nScanDst;
		unsigned char* pS = img.ptr<unsigned char>(i);
		for (int j = 0; j < nWidth; ++j)
		{
			pD[j] = pS[j];	
		}
	}


}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CTFMixtureGaussian::CTFMixtureGaussian()
{
	m_bBeginFrames = true;
	m_nFrameCount  = 1;
	m_nFrameThres  = 2;

	m_ncx = 0;
	m_ncy = 0;
	m_sz  = 0;

	m_pGaussianItems = NULL;
	m_pPrev = NULL;

}

CTFMixtureGaussian::~CTFMixtureGaussian()
{
	if ( m_pPrev != NULL )
	{
		delete[] m_pPrev;
	}

	if( m_pGaussianItems != NULL )
	{
		delete[] &(m_pGaussianItems[0]);
		m_pGaussianItems = NULL;
	}
}

void CTFMixtureGaussian::Init(int nWidth, int nHeight)
{
	if( m_ncx != nWidth || m_ncy != nHeight || m_pGaussianItems == NULL ){
		m_ncx = nWidth;
		m_ncy = nHeight;

		if( m_pGaussianItems != NULL ){
			delete[] &(m_pGaussianItems[0]);
			m_pGaussianItems = NULL;
		}

		m_sz = m_ncx*m_ncy;
		m_pGaussianItems = new SGaussianItem[m_ncx*m_ncy];
		for(int i = 0; i < m_ncy; ++i){
			for(int j = 0; j < m_ncx; ++j){
				SGaussianItem* pItem = m_pGaussianItems + i*m_ncx + j;
				pItem->push_back(SGaussianAtom());
				pItem->push_back(SGaussianAtom());
				pItem->push_back(SGaussianAtom());
				pItem->push_back(SGaussianAtom());
				pItem->push_back(SGaussianAtom());


			}
		}


		m_pPrev = new unsigned char[m_ncx*m_ncy];

	}
}

void CTFMixtureGaussian::InitData(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst)
{
	float fInitsqure        = 400.0f;
	float fInitWeight       = 0.5;
	float fInitWeightSquare = fInitWeight * fInitWeight;

	if( m_bBeginFrames ){
		for (int i =0; i < nHeight; ++i){
			unsigned char* ps = pSrc + i*nScanSrc;
			unsigned char* pd = pDst + i*nScanDst;
			int   ns = i*nWidth;
			memset(pd,0,nWidth);
			for (int j = 0; j < nWidth; ++j){
				SGaussianItem* pItem = m_pGaussianItems + ns + j;
				int nValue = ps[j];
				for (int k = 0; k < 5; ++k){
					(*pItem)[k].m_fAverage += nValue;
				}
			}
		}

		
		m_nFrameCount++;

		if(m_nFrameCount > m_nFrameThres){
			for(int i = 0; i < nHeight; ++i){
				unsigned char* ps = pSrc + i*nScanSrc;
				unsigned char* pre = m_pPrev + i*nWidth;
				int   ns = i*nWidth;

				for(int j = 0; j < nWidth; ++j){
					SGaussianItem* pItem = m_pGaussianItems + ns + j;

					for (int k = 0; k < 5; ++k){
						float fMiu = (*pItem)[k].m_fAverage / m_nFrameThres;
						(*pItem)[k].m_fAverage         = fMiu;
						(*pItem)[k].m_fSqure           = fInitsqure;

						if (k == 0){
							(*pItem)[k].m_fWeight      = fInitWeight;
							(*pItem)[k].m_fWeightSqure = fInitWeightSquare;
						}else{
							(*pItem)[k].m_fWeight      = (1.0f - fInitWeight)/4.0f;
							(*pItem)[k].m_fWeightSqure = (*pItem)[k].m_fWeight * (*pItem)[k].m_fWeight;
						}
					}

					pre[j] = ps[j];
				}
			}
			m_bBeginFrames = false;
		}		
	}
}


class CZSort
{
public:
	bool operator()(  const SGaussianAtom& sAtomleft,   const SGaussianAtom& sAtomRight ) const
	{
		if( sAtomleft.m_fEx <=  sAtomRight.m_fEx )
			return false;
		else
			return true;
	}
};


void CTFMixtureGaussian::DetectImpl(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, bool beUpdataBackGround)
{
	float Initsqure = 400.0f;

	float fAlpha      = 0.007f;//0.02f;
	float fAlphaSqure = fAlpha * fAlpha;
	float fBeta       = 1.0f - fAlpha;
	float fBetaSqure  = fBeta * fBeta;
	float fThres      = 9.0f;//6.0f * 6.0f;
	//float fThres      = 3.0f * 3.0f;

	float fTmpMiu     = 0.0f;
	float fTmpOmiga   = 0.0f;

	for (int i = 0; i < nHeight; ++i)
	{
		unsigned char* ps = pSrc + i*nScanSrc;
		unsigned char* pd = pDst + i*nScanDst;
		unsigned char* pr = m_pPrev + i*nWidth;
		int   ns = i*nWidth;

		for (int j = 0; j < nWidth; ++j)
		{
			SGaussianItem* pItem   = m_pGaussianItems  + ns + j;
			int nValue   = ps[j];

			if ( beUpdataBackGround )
			{//设置更新时候才更新背景			
				int n = 0;
				for (int k =0; k < 5; ++k)
				{

					float fAver   = (*pItem)[k].m_fAverage;
					float fSqure  = (*pItem)[k].m_fSqure;
					float fWeight = (*pItem)[k].m_fWeight;
					float fMiu    = nValue - fAver;

					if (  fMiu*fMiu > fThres * (*pItem)[k].m_fSqure  )
					{
						(*pItem)[k].m_fWeight      *= fBeta;
						(*pItem)[k].m_fWeightSqure *= fBetaSqure;
						(*pItem)[k].m_fEx           = (*pItem)[k].m_fWeightSqure / fSqure;//(*pItem)[k].m_fSqure;
						n++;
					}
					else
					{
						float rate                 = fAlpha / fWeight;//(*pItem)[k].m_fWeight;
						float fOmiga               = 1.0f - rate;

						//(*pItem)[k].m_fWeight       = fBeta  * (*pItem)[k].m_fWeight + fAlpha;
						//(*pItem)[k].m_fWeightSqure  = (*pItem)[k].m_fWeight * (*pItem)[k].m_fWeight;

						fWeight                     = fBeta  * fWeight + fAlpha;
						(*pItem)[k].m_fWeight       = fWeight;
						(*pItem)[k].m_fWeightSqure  = fWeight * fWeight;

						//fTmpMiu                = fOmiga * (*pItem)[k].m_fAverage + rate * nValue;
						//fTmpOmiga                  = fOmiga * (*pItem)[k].m_fSqure + rate * fMiu * fMiu;

						fAver                     -= rate * ( fAver - nValue);
						fSqure                    -= rate * ( fSqure - fMiu * fMiu);

						(*pItem)[k].m_fAverage     = fAver;  //fTmpMiu;
						(*pItem)[k].m_fSqure       = fSqure; //fTmpOmiga;
						(*pItem)[k].m_fEx          = (*pItem)[k].m_fWeightSqure / fSqure;//fTmpOmiga;
					}
				}

				if (n == 5)
				{
					(*pItem)[4].m_fAverage      = float(nValue);
					(*pItem)[4].m_fSqure        = Initsqure;
					(*pItem)[4].m_fWeight      += fAlpha;
					(*pItem)[4].m_fWeightSqure  = (*pItem)[4].m_fWeight * (*pItem)[4].m_fWeight; 
					(*pItem)[4].m_fEx           = (*pItem)[4].m_fWeightSqure / Initsqure;
				}

				sort((*pItem).begin(), (*pItem).end(), CZSort());
				//sort_ex( (*pItem).begin(), (*pItem).end() );
			}
			else
			{//beUpdataBackGround为false时候，条件更新，路面亮度较暗且帧差小，进行更新
				if ( ps[j] < 50 && abs(ps[j]-pr[j]) < 10 )
				{
					int n = 0;
					for (int k =0; k < 5; ++k)
					{

						float fAver   = (*pItem)[k].m_fAverage;
						float fSqure  = (*pItem)[k].m_fSqure;
						float fWeight = (*pItem)[k].m_fWeight;
						float fMiu    = nValue - fAver;

						if (  fMiu*fMiu > fThres * (*pItem)[k].m_fSqure  )
						{
							(*pItem)[k].m_fWeight      *= fBeta;
							(*pItem)[k].m_fWeightSqure *= fBetaSqure;
							(*pItem)[k].m_fEx           = (*pItem)[k].m_fWeightSqure / fSqure;//(*pItem)[k].m_fSqure;
							n++;
						}
						else
						{
							float rate                 = fAlpha / fWeight;//(*pItem)[k].m_fWeight;
							float fOmiga               = 1.0f - rate;

							//(*pItem)[k].m_fWeight       = fBeta  * (*pItem)[k].m_fWeight + fAlpha;
							//(*pItem)[k].m_fWeightSqure  = (*pItem)[k].m_fWeight * (*pItem)[k].m_fWeight;

							fWeight                     = fBeta  * fWeight + fAlpha;
							(*pItem)[k].m_fWeight       = fWeight;
							(*pItem)[k].m_fWeightSqure  = fWeight * fWeight;

							//fTmpMiu                = fOmiga * (*pItem)[k].m_fAverage + rate * nValue;
							//fTmpOmiga                  = fOmiga * (*pItem)[k].m_fSqure + rate * fMiu * fMiu;

							fAver                     -= rate * ( fAver - nValue);
							fSqure                    -= rate * ( fSqure - fMiu * fMiu);

							(*pItem)[k].m_fAverage     = fAver;  //fTmpMiu;
							(*pItem)[k].m_fSqure       = fSqure; //fTmpOmiga;
							(*pItem)[k].m_fEx          = (*pItem)[k].m_fWeightSqure / fSqure;//fTmpOmiga;
						}
					}

					if (n == 5)
					{
						(*pItem)[4].m_fAverage      = float(nValue);
						(*pItem)[4].m_fSqure        = Initsqure;
						(*pItem)[4].m_fWeight      += fAlpha;
						(*pItem)[4].m_fWeightSqure  = (*pItem)[4].m_fWeight * (*pItem)[4].m_fWeight; 
						(*pItem)[4].m_fEx           = (*pItem)[4].m_fWeightSqure / Initsqure;
					}

					sort((*pItem).begin(), (*pItem).end(), CZSort());

				}

			}


			int n1 = 0;
			float s = 0;
			for (int k = 0; k < 5; ++k)
			{
				s += (*pItem)[k].m_fWeight;
				n1++;
				if (s >= 0.7)
				{
					break;
				}
			}

			int Flag = 0;
			for (int k = 0; k < n1; ++k)
			{
				float fMiu = nValue - (*pItem)[k].m_fAverage;
				if ( fMiu * fMiu <= fThres * (*pItem)[k].m_fSqure )
				{
					Flag = 1;
					break;
				}
			}

			if (Flag == 0 && fabs( nValue - (*pItem)[0].m_fAverage ) > (*pItem)[0].m_fAverage*0.15 )
			{
				pd[j] = 255;
			}
			else
			{
				pd[j] = 0;
			}
			pr[j] = ps[j];
		}
	}
}

void CTFMixtureGaussian::DetectImplOnly(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst)
{
	float Initsqure = 400.0f;

	float fAlpha = 0.007f;//0.02f;
	float fAlphaSqure = fAlpha * fAlpha;
	float fBeta = 1.0f - fAlpha;
	float fBetaSqure = fBeta * fBeta;
	float fThres = 9.0f;//6.0f * 6.0f;
	//float fThres      = 3.0f * 3.0f;

	float fTmpMiu = 0.0f;
	float fTmpOmiga = 0.0f;

	for (int i = 0; i < nHeight; ++i)
	{
		unsigned char* ps = pSrc + i*nScanSrc;
		unsigned char* pd = pDst + i*nScanDst;
		unsigned char* pr = m_pPrev + i*nWidth;
		int   ns = i*nWidth;

		for (int j = 0; j < nWidth; ++j)
		{
			SGaussianItem* pItem = m_pGaussianItems + ns + j;
			int nValue = ps[j];

			int n1 = 0;
			float s = 0;
			for (int k = 0; k < 5; ++k)
			{
				s += (*pItem)[k].m_fWeight;
				n1++;
				if (s >= 0.7)
				{
					break;
				}
			}

			int Flag = 0;
			for (int k = 0; k < n1; ++k)
			{
				float fMiu = nValue - (*pItem)[k].m_fAverage;
				if (fMiu * fMiu <= fThres * (*pItem)[k].m_fSqure)
				{
					Flag = 1;
					break;
				}
			}

			if (Flag == 0 && fabs(nValue - (*pItem)[0].m_fAverage) > (*pItem)[0].m_fAverage*0.15)
			{
				pd[j] = 255;
			}
			else
			{
				pd[j] = 0;
			}
			pr[j] = ps[j];
		}
	}
}


void CTFMixtureGaussian::Detect(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, bool beUpdataBackGround )
{
	if(nWidth == 0 || nHeight == 0 || pSrc == NULL)
		return;

	//Init memory
	Init(nWidth, nHeight);

	if(m_bBeginFrames){//Init data
		InitData(nWidth, nHeight, nScanSrc, pSrc, nScanDst, pDst);
	}else{//detect
		DetectImpl( nWidth, nHeight, nScanSrc, pSrc, nScanDst, pDst, beUpdataBackGround );
		//expand(nWidth,nHeight,nScanDst,pDst,nScanDst,pDst,3);
		//corrode(nWidth,nHeight,nScanDst,pDst,nScanDst,pDst,3);
		DilateAndErode(nWidth, nHeight, nScanDst, pDst, nScanDst, pDst);
	}
}

void CTFMixtureGaussian::DetectOnly(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst)
{
	if (nWidth == 0 || nHeight == 0 || pSrc == NULL)
		return;

	//Init memory
	Init(nWidth, nHeight);

	if (m_bBeginFrames){//Init data
		InitData(nWidth, nHeight, nScanSrc, pSrc, nScanDst, pDst);
	}
	else{//detect
		DetectImplOnly(nWidth, nHeight, nScanSrc, pSrc, nScanDst, pDst);
		//expand(nWidth, nHeight, nScanDst, pDst, nScanDst, pDst, 3);
		//corrode(nWidth, nHeight, nScanDst, pDst, nScanDst, pDst, 3);
		DilateAndErode(nWidth, nHeight, nScanDst, pDst, nScanDst, pDst);
	}
}

void CTFMixtureGaussian::Replay()
{
	if (m_pPrev != NULL)
	{
		delete[] m_pPrev;
	}

	if (m_pGaussianItems != NULL)
	{
		delete[] & (m_pGaussianItems[0]);
		m_pGaussianItems = NULL;
	}

	m_bBeginFrames = true;
	m_nFrameCount = 1;
	m_nFrameThres = 2;

	m_ncx = 0;
	m_ncy = 0;
	m_sz = 0;

	m_pGaussianItems = NULL;
	m_pPrev = NULL;
}


void CTFMixtureGaussian::sort_ex(SGaussinAtomIt begin, SGaussinAtomIt end)
{
	float fMax           = (*begin).m_fEx;
	SGaussinAtomIt itMax = begin;
	float fMin           = fMax;
	SGaussinAtomIt itMin = begin;


	SGaussinAtomIt it = begin;
	for( it =  begin + 1; it != end; ++it){

		if( fMax < (*it).m_fEx ){
			fMax  = (*it).m_fEx;
			itMax = it;
		}

		if( fMin >= (*it).m_fEx ){
			fMin  = (*it).m_fEx;
			itMin = it;
		}

	}

	SGaussianAtom sGAtom;


	if( itMin != itMax ){//执行交换动作
		//先操作最大项
		if( itMax > begin ){
			if( itMin != begin ){
				sGAtom     = *begin;
				*begin     = *itMax;
				*itMax     = sGAtom;
			}else{
				sGAtom     = *begin;
				*begin     = *itMax;
				*itMax     = sGAtom;

				sGAtom     = *(end - 1);
				*(end - 1) = *itMax;
				*itMax     = sGAtom;

				itMin      = end - 1;
			}
		}

		//操作最小项
		if( itMin < end -1 ){
			sGAtom             = *(end  - 1);
			*(end  - 1)        = *itMin;
			*itMin             = sGAtom;
		}
	}
}


void CTFMixtureGaussian::expand(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, int nRad)
{
	unsigned char* pTmp = new unsigned char[nWidth*nHeight];
	for ( int i = 0; i < nHeight; ++i )
	{
		unsigned char* pS = pSrc + i*nScanSrc;
		unsigned char* pT = pTmp + i*nWidth;
		memcpy(pT,pS,nWidth);
	}

	for ( int i = nRad; i < nHeight-nRad; ++i )
	{
		unsigned char* pS = pSrc + i*nScanSrc;
		unsigned char* pT = pTmp + i*nWidth;
		for ( int j = nRad; j < nWidth-nRad; ++j )
		{
			if (pT[j]==0)
			{
				for ( int m = i-nRad; m <= i+nRad; ++m )
				{
					bool bebreak = false;
					unsigned char* pSL = pSrc + m*nScanSrc;
					for ( int n = j-nRad; n <= j+nRad; ++n )
					{
						if ( pSL[n] == 255 )
						{
							pT[j] = 255;
							bebreak = true;
							break;
						}
					}

					if ( bebreak )
					{
						break;
					}
				}
			}
		}
	}

	for ( int i = 0; i < nHeight; ++i )
	{
		unsigned char* pD = pDst + i*nScanDst;
		unsigned char* pT = pTmp + i*nWidth;
		memcpy(pD,pT,nWidth);
	}

	if ( pTmp != NULL )
	{
		delete[] pTmp;
	}	
}

void CTFMixtureGaussian::corrode(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, int nRad)
{
	unsigned char* pTmp = new unsigned char[nWidth*nHeight];
	for ( int i = 0; i < nHeight; ++i )
	{
		unsigned char* pS = pSrc + i*nScanSrc;
		unsigned char* pT = pTmp + i*nWidth;
		memcpy(pT,pS,nWidth);
	}

	for ( int i = nRad; i < nHeight-nRad; ++i )
	{
		unsigned char* pS = pSrc + i*nScanSrc;
		unsigned char* pT = pTmp + i*nWidth;
		for ( int j = nRad; j < nWidth-nRad; ++j )
		{
			if (pT[j]==255)
			{
				for ( int m = i-nRad; m <= i+nRad; ++m )
				{
					bool bebreak = false;
					unsigned char* pSL = pSrc + m*nScanSrc;
					for ( int n = j-nRad; n <= j+nRad; ++n )
					{
						if ( pSL[n] == 0 )
						{
							pT[j] = 0;
							bebreak = true;
							break;
						}
					}

					if ( bebreak )
					{
						break;
					}
				}
			}
		}
	}

	for ( int i = 0; i < nHeight; ++i )
	{
		unsigned char* pD = pDst + i*nScanDst;
		unsigned char* pT = pTmp + i*nWidth;
		memcpy(pD,pT,nWidth);
	}

	if ( pTmp != NULL )
	{
		delete[] pTmp;
	}	
}

void CTFMixtureGaussian::DilateAndErode(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst)
{
	Mat img(nHeight, nWidth, CV_8UC1);

	for (int i = 0; i < nHeight; ++i)
	{
		unsigned char* pS = pSrc + i * nScanSrc;
		unsigned char* pD = img.ptr<unsigned char>(i);
		for (int j = 0; j < nWidth; ++j)
		{
			pD[j] = pS[j];
		}
	}

	//imwrite("D:/Images/src.jpg", img);
	Mat ele = getStructuringElement(MORPH_RECT, Size(7, 7), Point(3, 3));
	//Mat imgTmp(nHeight, nWidth, CV_8UC1);
	dilate(img, img, ele); //膨胀 
	erode(img, img, ele);  //腐蚀  

	//imwrite("D:/Images/res.jpg", img); 

	for (int i = 0; i < nHeight; ++i)
	{
		unsigned char* pD = pDst + i * nScanDst;
		unsigned char* pS = img.ptr<unsigned char>(i);
		for (int j = 0; j < nWidth; ++j)
		{
			pD[j] = pS[j];
		}
	}


}