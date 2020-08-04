#ifndef __TFMOD_H__
#define __TFMOD_H__

#include "TFVATrafficComp.h"

//区间结构体
struct CInterval
{
	int nbegin;
	int nend;

	CInterval( int begin, int end )
	{
		nbegin = begin;
		nend   = end;
	}

	CInterval(const CInterval& other)
	{
		*this = other;
	}

	CInterval& operator =(const CInterval &other)
	{
		nbegin = other.nbegin;
		nend   = other.nend;
		return *this;
	}
};

typedef std::list<CInterval*> aIntervalList;
typedef std::vector<CInterval*> bIntervalList;
typedef std::vector<CInterval> cIntervalList;


struct SCodeword
{
public:
	SCodeword( float nCentre, long nNum, long nMaxTimeInterval, long nLastTime, int nNoRenewTime )
	{
		m_nCentre          = nCentre;
		m_nNum             = nNum;
		m_nMaxTimeInterval = nMaxTimeInterval;
		m_nLastTime        = nLastTime;
		m_nNoRenewTime     = nNoRenewTime;
	}
public:
	float m_nCentre         ;
	long  m_nNum            ;
	long  m_nMaxTimeInterval;
	long  m_nLastTime       ;
	int   m_nNoRenewTime    ;
};

typedef std::list<SCodeword> Codebook;

//码本模型
class CTFCodebookMode
{
public:
	CTFCodebookMode();
	virtual ~CTFCodebookMode();

public:
	void Detect(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, int nSensibility, bool beUpdataBackGround );

	void DetectOnly(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, int nSensibility);
	//获得运动目标矩形列表
	void GetMoveObjRectList(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, bRectVector& RectList, int nCutTop, int nCutLeft);

	void Replay();
	

protected:
	void Init(int nWidth, int nHeight);
	void InitData(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst);

	void DetectImpl(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, int nSensibility, bool beUpdataBackGround);
	//只检测不做背景更新，为了提高效率
	void DetectImplOnly(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, int nSensibility);
	//膨胀
	void expand(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, int nRad);
	//腐蚀
	void corrode(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, int nRad);
	//先膨胀后腐蚀
	void DilateAndErode(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst);

protected:
	bool      m_bBeginFrames;
	long      m_nFrameCount;

	int       m_ncx;
	int       m_ncy;

	Codebook* m_pCodebook;
	unsigned char* m_pPrev;

	cIntervalList m_aIntervalList; 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct SGaussianAtom
{
	SGaussianAtom()
	{
		m_fAverage     = 0.0f;
		m_fSqure       = 0.0f;
		m_fWeight      = 0.0f;
		m_fWeightSqure = 0.0f;
		m_fEx          = 0.0f;
	}

	SGaussianAtom(float fAverage, float fSqure, float fWeight)
	{
		m_fAverage = fAverage;
		m_fSqure   = fSqure;
		m_fWeight  = fWeight;
	}

	SGaussianAtom(const SGaussianAtom& other){
		*this = other;
	}

	SGaussianAtom& operator = (const SGaussianAtom& other){
		m_fAverage      = other.m_fAverage;
		m_fSqure        = other.m_fSqure;
		m_fWeight       = other.m_fWeight;
		m_fWeightSqure  = other.m_fWeightSqure;
		m_fEx           = other.m_fEx;

		return *this;
	}

	float m_fAverage;
	float m_fSqure;
	float m_fWeight;
	float m_fWeightSqure;
	float m_fEx;
};

typedef std::vector<SGaussianAtom> SGaussianItem;
typedef std::vector<SGaussianItem*> SGaussianInfoVec;
typedef std::vector<SGaussianAtom>::iterator SGaussinAtomIt;

//混合高斯模型
class CTFMixtureGaussian 
{
public:
	CTFMixtureGaussian();
	virtual ~CTFMixtureGaussian();

public:
	void Detect(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, bool beUpdataBackGround);

	void DetectOnly(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst);

	void Replay();

protected:
	void Init(int nWidth, int nHeight);
	void InitData(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst);

	void DetectImpl(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, bool beUpdataBackGround);
	//只检测，不更新
	void DetectImplOnly(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst);
	//膨胀
	void expand(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, int nRad);
	//腐蚀
	void corrode(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, int nRad);
	//先膨胀后腐蚀
	void DilateAndErode(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst);

public:
	void static sort_ex( SGaussinAtomIt begin, SGaussinAtomIt end );
	
protected:
	bool      m_bBeginFrames;
	uint64    m_nFrameCount;
	int       m_nFrameThres;

	int       m_ncx;
	int       m_ncy;
	int       m_sz;
public:
	unsigned char* m_pPrev;

	SGaussianItem*   m_pGaussianItems;

};



#endif