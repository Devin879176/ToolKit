#include "TFConnectedAnalysis.h"

using namespace cv;

bool SortByArea(CBoundBox &box1, CBoundBox &box2)
{
	return box1.nconnetedArea > box2.nconnetedArea;
}
CTFConnectedAnalysis::CTFConnectedAnalysis()
{

}

CTFConnectedAnalysis::~CTFConnectedAnalysis()
{

}
void CTFConnectedAnalysis::SetCarSize(float  fAveSizeX ,float  fAveSizeY  )
{
	 m_fAveSizeX = fAveSizeX;
	 m_fAveSizeY = fAveSizeY;
}
void CTFConnectedAnalysis::SplitBigRect(int ncx, int ncy, unsigned char* pSrc, bRectVector& aRectTmp)
{
	if (aRectTmp.empty())
		return;

	//sort(aRectTmp.begin(), aRectTmp.end(), SortByArea);

	Mat mat_src = Mat(ncy, ncx, CV_8UC1);
	memcpy(mat_src.data, pSrc, ncy*ncx);
 
	bRectVector aRectTmp1(aRectTmp.begin(), aRectTmp.end());
	bRectVector::iterator iter = aRectTmp1.begin();
	for (; iter != aRectTmp1.end(); iter++)
	{
		CBoundBox aItem = *iter;
		if (aItem.nw * aItem.nh > ncx * ncy * 0.3)//面积过大
		{
			float fbeginY[4] = { 0.0, 0.1, 0.3, 0.6 };
			float fbeginX[3] = { 0.0, 0.4, 0.7 };

			float fh[5] = { 0, 0.1, 0.2, 0.3, 0.4 };
			float fw[4] = { 0, 0.4, 0.3, 0.3 };

			for (int j = 0; j < 3; j++)/////宽度
				for (int i = 0; i < 4; i++)////高度
				{
					CBoundBox rtTemp;
					rtTemp.nbeginY = aItem.nbeginY + fbeginY[i] * aItem.nh;
					rtTemp.nbeginX = aItem.nbeginX + fbeginX[j] * aItem.nw;
					rtTemp.nh = fh[i + 1] * aItem.nh;
					rtTemp.nw = fw[j + 1] * aItem.nw;
					rtTemp.nconnetedArea = fh[i] * fw[j] * aItem.nconnetedArea;


					Rect roi(rtTemp.nbeginX, rtTemp.nbeginY, rtTemp.nw, rtTemp.nh);
					Mat roi_of_image = mat_src(roi);
					int nNonZero = countNonZero(roi_of_image);
					if (nNonZero > rtTemp.nw * rtTemp.nh * 0.5)
					{
#if 1
						if (abs(m_fAveSizeX) < 1e-6 && 
							abs(m_fAveSizeY)  < 1e-6)
						{	
							aRectTmp.push_back(rtTemp);
						}
						else/////根据汽车大小再切分
						{
							if (rtTemp.nw > m_fAveSizeX
								&&rtTemp.nh > m_fAveSizeY
								&&m_fAveSizeX > 5 && m_fAveSizeY > 5)
						    {
								//////可以分割成几个区域
								int nW = rtTemp.nw / m_fAveSizeX;/////宽度有几个
								int nH = rtTemp.nh / m_fAveSizeY;/////高度有几个

								for (int m = 0; m < nH; m ++ )
								{
									for (int k = 0; k < nW; k ++)
									{
										CBoundBox rtTempSmall;

										rtTempSmall.nbeginX = m * m_fAveSizeX;
										rtTempSmall.nbeginY = k * m_fAveSizeY;
										rtTempSmall.nw = m_fAveSizeX;
										rtTempSmall.nh = m_fAveSizeY;
										rtTempSmall.nconnetedArea = m_fAveSizeX * m_fAveSizeY;

										aRectTmp.push_back(rtTempSmall);
									}
								}
						    }
							else
							{
								aRectTmp.push_back(rtTemp);
							}
						}

#else
						////再切分成4块
						CBoundBox rtTempSmall;
						for (int m = 0; m < 2; m++)
							for (int k = 0; k < 2; k++)
							{
								rtTempSmall.nbeginX = rtTemp.nbeginX + m * 0.5 * rtTemp.nw;
								rtTempSmall.nbeginY = rtTemp.nbeginY + k * 0.5 * rtTemp.nh;
								rtTempSmall.nw = 0.5 * rtTemp.nw;
								rtTempSmall.nh = 0.5 * rtTemp.nh;
								rtTempSmall.nconnetedArea = rtTemp.nconnetedArea / 4;
								//rectangle(mat_src, Rect(rtTempSmall.nbeginX, rtTempSmall.nbeginY, rtTempSmall.nw, rtTempSmall.nh),
									//CV_RGB(255, 255, 255), 1);
								aRectTmp.push_back(rtTempSmall);
							}
#endif
					}

				}
		}
	}

	if (aRectTmp.size() > 0)
	{
		iter = aRectTmp.begin();
		for (; iter != aRectTmp.end();)
		{
			CBoundBox aItem = *iter;
			if ((aItem.nw * aItem.nh > ncx * ncy * 0.3)//面积过大
				|| aItem.nw > aItem.nh * 3)////扁平
			{
				iter = aRectTmp.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}
}
void CTFConnectedAnalysis::GetConnectedArea(int ncx, int ncy, int nSrcPitch, unsigned char* pSrc, bRectVector& ARectRes, int nValidArea )
{
	while( !ARectRes.empty() ){
		ARectRes.pop_back();
	}
	ARectRes.clear();

	bRectVector aRectTmp;

	while( !aRectTmp.empty() ){
		aRectTmp.pop_back();
	}
	aRectTmp.clear();

	if( ncx == 0 || ncy == 0 || nSrcPitch == 0 || pSrc == NULL )
		return;

	std::vector<ZCCLNode*> aCCLTreesVt;
	bool bNewLine       = true;
	ZCCLNode* pItem     = NULL;
	int  nLastLineCnt   = 0;  //上一行线段数目

	for( int i = 0; i < ncy; ++i )
	{
		unsigned char* ps = pSrc + i*nSrcPitch;
		int nSz0  = (int)aCCLTreesVt.size();
		bNewLine  = true;
		for(int j = 0; j < ncx; ++j)
		{
			if(ps[j] != 0)
			{
				if(bNewLine)
				{
					pItem = new ZCCLNode;
					aCCLTreesVt.push_back(pItem);
					pItem->iRowPos         = i;
					pItem->iColPos_begin   = j;
					pItem->iColPos_end     = j;	
					bNewLine               = false;
				}
				pItem->iColPos_end         = j;

			}
			else
			{
				if(!bNewLine)
				{
					bNewLine = true;
				}
			}
		}
		//
		//
		int nSz1 = (int)aCCLTreesVt.size();
		if(nSz0 > 0 && nSz1 - nSz0 > 0 && nLastLineCnt > 0)
		{
			//当前行首个线段位置 nSz0
			//上一行首个线段位置 nSz0 - nLastLineCnt
			//当前行线段个数
			int curLcn    = nSz1 - nSz0;
			for( int m = 0; m < curLcn; ++m )
			{
				ZCCLNode* pItem_cur     = aCCLTreesVt[nSz0 + m];
				int nNum   = 0; //连通数目
				for( int n = 0; n < nLastLineCnt; ++n)
				{
					ZCCLNode* pItem_lst = aCCLTreesVt[nSz0 - nLastLineCnt + n]; 
					//
					if( pItem_lst->iColPos_end + 1 < pItem_cur->iColPos_begin )
						continue;
					if( pItem_lst->iColPos_begin > pItem_cur->iColPos_end + 1 )
						break;
					//
					if(nNum == 0)
					{
						pItem_lst->Add(pItem_cur);
					}
					else
					{
						if(pItem_lst->pRoot == NULL)
						{
							pItem_cur->Add(pItem_lst);
						}
						else
						{
							if( pItem_cur->pRoot != pItem_lst->pRoot )
							{
								pItem_cur->AddEx(pItem_lst->pRoot);
							}
						}
					}
					nNum++;
				}
			}
		}
		//
		nLastLineCnt = nSz1 - nSz0;
	}

	//
	int sz = (int)aCCLTreesVt.size();
	for(int i = 0; i < sz; ++i)
	{
		if(aCCLTreesVt[i]->bIsRoot)
		{
			ZCCLNode* pItem = aCCLTreesVt[i];
			int l = pItem->iColPos_begin;
			int r = pItem->iColPos_end;
			int t = pItem->iRowPos;
			int b = pItem->iRowPos;
			int nArea = r - l + 1;
			while(pItem->pNext != NULL)
			{
				pItem  = pItem->pNext;
				nArea += pItem->iColPos_end - pItem->iColPos_begin + 1;

				if(l > pItem->iColPos_begin)
					l = pItem->iColPos_begin;

				if(r < pItem->iColPos_end)
					r = pItem->iColPos_end;

				if(t > pItem->iRowPos)
					t = pItem->iRowPos;

				if(b < pItem->iRowPos)
					b = pItem->iRowPos;
			}
			//

			if(nArea > nValidArea)
			{
				CBoundBox atom = CBoundBox(l,t,r-l,b-t,nArea);
				aRectTmp.push_back(atom);
			}
// 			else
// 			{
// 				ZCCLNode* pItem = aCCLTreesVt[i];
// 				do 
// 				{
// 					int l = pItem->iColPos_begin;
// 					int r = pItem->iColPos_end;
// 					int y = pItem->iRowPos;
// 					memset(pSrc + y*nSrcPitch + l, 0, r - l + 1);
// 					pItem = pItem->pNext;
// 				} while (pItem != NULL);
// 			}
		}
	}

	//////对得到的单个打矩形框进行处理
	//SplitBigRect(ncx, ncy, pSrc, aRectTmp);

	//矩形规整化，去除嵌套及重叠区域较大的矩形
	if ( !aRectTmp.empty() )
	{
		int nSize = aRectTmp.size();
		for(int i = 0; i < nSize-1; ++i)
		{
			bool bAdd  = true;
			CBoundBox rt = aRectTmp[i];
			for(int j = i + 1; j < nSize; ++j)
			{
				CBoundBox rt_r = aRectTmp[j];
				int rt_endx = rt.nbeginX+rt.nw;
				int re_r_endx = rt_r.nbeginX+rt_r.nw;
				int rt_endy = rt.nbeginY+rt.nh;
				int rt_r_endy = rt_r.nbeginY+rt_r.nh;


				//重叠区域计算
				int nl = max(rt.nbeginX,rt_r.nbeginX);
				int nr = min(rt_endx,re_r_endx);
				int nt = max(rt.nbeginY,rt_r.nbeginY);
				int nb = min(rt_endy,rt_r_endy);

				if ( nl < nr && nt < nb )
				{
					int nSameArea = (nr-nl)*(nb-nt);

					if ( nSameArea > rt.nh*rt.nw*0.9 || nSameArea > rt_r.nh*rt_r.nw*0.9 )
					{
						if (rt.nh*rt.nw < rt_r.nh*rt_r.nw)
						{
							bAdd = false;
							break;
						} 
						else
						{
							bAdd = false;
							aRectTmp[j] = rt;
							break;
						}
					} 
				}


// 			    if( rt.nbeginX >= rt_r.nbeginX && rt_endx <= re_r_endx && rt.nbeginY >= rt_r.nbeginY && rt_endy <= rt_r_endy )
// 			    {
// 			    	bAdd = false;
// 			    	break;
// 			    }
// 			    if( rt.nbeginX <= rt_r.nbeginX && rt_endx >= re_r_endx && rt.nbeginY <= rt_r.nbeginY && rt_endy >= rt_r_endy )
// 			    {
// 			    	bAdd = false;
// 			    	aRectTmp[j] = rt;
// 			    	break;
// 			    }


			}
			if(bAdd)
			{
				ARectRes.push_back(rt);
			}
		}
		CBoundBox rt_end = aRectTmp[nSize-1];
		ARectRes.push_back(rt_end);
	}




	while( !aRectTmp.empty() ){
		aRectTmp.pop_back();
	}
	aRectTmp.clear();


	//
	for(int i = 0; i < sz; ++i)
	{
		delete aCCLTreesVt[i];
	}
	aCCLTreesVt.clear();	
}