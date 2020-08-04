#ifndef _TFCONNECTED_
#define _TFCONNECTED_

#include "TFVATrafficComp.h"

/**@file         TFConnectedAnalysis.h
   @author       Yue Ma
   @date         20160329
   @description  连通区域检测头文件
*/

//连通域节点
typedef struct SZCCLNode
{
	bool bIsRoot;           //该节点是否为根节点
	int  iRowPos;           //行坐标
	int  iColPos_begin;     //起始点列坐标
	int  iColPos_end;       //终止点列坐标

	SZCCLNode* pRoot;           //根节点
	SZCCLNode* pNext;           //下一个节点

	SZCCLNode()
	{
		bIsRoot       = true;
		iRowPos       = -1;
		iColPos_begin = -1;
		iColPos_end   = -1;
		pRoot         = NULL;
		pNext         = NULL;
	}

	void Add(SZCCLNode* pItem)
	{
		pItem->bIsRoot = false;

		SZCCLNode* p = this;
		while( p->pNext != NULL )
		{
			p = p->pNext;
		}
		p->pNext = pItem;

		if(pRoot == NULL)
		{
			pItem->pRoot = this;
		}
		else
		{
			pItem->pRoot = this->pRoot;
		}
	}

	void AddEx(SZCCLNode* pItem)
	{
		pItem->bIsRoot = false;

		SZCCLNode* p = this;
		while( p->pNext != NULL )
		{
			p = p->pNext;
		}
		p->pNext = pItem;

		//
		pItem->pRoot = this->pRoot;
		while(pItem->pNext != NULL)
		{
			pItem->pNext->pRoot = this->pRoot;
			pItem = pItem->pNext;
		}
	}

}ZCCLNode;


class CTFConnectedAnalysis
{
public:
	CTFConnectedAnalysis();
	virtual ~CTFConnectedAnalysis();
public:
	//ARectRes:连通域结果
	//nValidArea:面积阈值，大于阈值的连通域返回
	void GetConnectedArea(int ncx, int ncy, int nSrcPitch, unsigned char* pSrc, bRectVector& ARectRes, int nValidArea );

	/////分解大矩形框
	void SplitBigRect(int ncx, int ncy, unsigned char* pSrc, bRectVector& ARectRes);

	//////设置大车、小车大小范围
	void SetCarSize(float  fAveSizeX, float  fAveSizeY);

	///////大车、小车大小范围
	float m_fAveSizeX = 0;
	float m_fAveSizeY = 0;
};
#endif