#ifndef _VIBE_H_
#define _VIBE_H_

#include "TFVATrafficComp.h"

typedef struct {
	int samplesPerPixel;           /* number of samples per pixel */
	int sphereRadius;              /* radius of the sphere        */
	int closeSamples;              /* number of close samples for being part of the background */
	int subSampling;               /* amount of random subsampling */
} VIBE_PARAMS;

typedef struct VIBE_BGMODEL{
	int width, height;             /* image width, height          */
	int bytesPerPixel;             /* The byte count per pixel     */
	int bytesPerLine;              /* The byte count per scan line */
	unsigned char *samples;        /* background model             */
	unsigned char *foreground;     /* the foreground image         */
	VIBE_PARAMS    params;         /* keep the parameters to be the same for whole sequences */
	VIBE_BGMODEL(){
		samples = NULL;
		foreground = NULL;
	}
} VIBE_BGMODEL_;

class CTFVIBE
{
public:
	CTFVIBE();
	virtual ~CTFVIBE();

	void Detect(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, bool beUpdataBackGround);

	void Replay();

private:
	void Init(int nWidth, int nHeight);
	void InitBgModel(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int i_sampleNum);
	void DetectImpl(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, bool beUpdataBackGround);
	void DilateAndErode(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst);

private:
	bool      m_bBeginFrames;
	int       m_ncx;
	int       m_ncy;
	VIBE_PARAMS  m_vibeParams;
	VIBE_BGMODEL m_bgModel;
};

#endif //_VIBE_H_