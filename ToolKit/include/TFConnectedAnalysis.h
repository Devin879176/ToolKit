#ifndef _TFCONNECTED_
#define _TFCONNECTED_

#include "TFVATrafficComp.h"

/**@file         TFConnectedAnalysis.h
   @author       Yue Ma
   @date         20160329
   @description  ��ͨ������ͷ�ļ�
*/

//��ͨ��ڵ�
typedef struct SZCCLNode
{
	bool bIsRoot;           //�ýڵ��Ƿ�Ϊ���ڵ�
	int  iRowPos;           //������
	int  iColPos_begin;     //��ʼ��������
	int  iColPos_end;       //��ֹ��������

	SZCCLNode* pRoot;           //���ڵ�
	SZCCLNode* pNext;           //��һ���ڵ�

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
	//ARectRes:��ͨ����
	//nValidArea:�����ֵ��������ֵ����ͨ�򷵻�
	void GetConnectedArea(int ncx, int ncy, int nSrcPitch, unsigned char* pSrc, bRectVector& ARectRes, int nValidArea );

	/////�ֽ����ο�
	void SplitBigRect(int ncx, int ncy, unsigned char* pSrc, bRectVector& ARectRes);

	//////���ô󳵡�С����С��Χ
	void SetCarSize(float  fAveSizeX, float  fAveSizeY);

	///////�󳵡�С����С��Χ
	float m_fAveSizeX = 0;
	float m_fAveSizeY = 0;
};
#endif