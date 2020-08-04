#include "TFVIBE.h"

static int g_randIdx = 0;
const static unsigned short g_randNums[] =
{
	871, 28819, 21850, 10966, 17631, 30073, 15608, 6726, 26602, 2951, 905, 6564, 10460, 25290, 10675, 13454, 31789, 6125, 11078, 16927, 15578, 14602, 16580, 12388, 6664, 3264, 23073, 9236, 2026, 20358, 14023, 16507,
	14310, 23358, 16093, 11192, 23539, 27918, 457, 9000, 14662, 32107, 32243, 13346, 20013, 24182, 5580, 27877, 3899, 5260, 22032, 6606, 32677, 7448, 29000, 25295, 10476, 6060, 5266, 23836, 13756, 1361, 31140, 25895,
	13333, 4635, 23055, 21711, 17064, 7424, 10177, 27092, 11652, 29161, 11982, 28815, 15857, 27146, 1136, 1760, 16796, 26223, 26444, 2919, 3317, 22744, 29942, 3319, 31958, 20167, 22712, 20039, 15205, 13015, 22415, 12779,
	19515, 14415, 23927, 8835, 9158, 9463, 28648, 19251, 19947, 27947, 10657, 13395, 26608, 22061, 9124, 6247, 29082, 4800, 28546, 1361, 14738, 3570, 1752, 7621, 26893, 22585, 21530, 3838, 29356, 27329, 14479, 3759,
	26785, 13573, 13213, 30013, 32276, 10397, 3911, 18475, 14277, 14166, 13812, 18646, 4082, 30600, 5490, 23394, 4475, 25252, 13114, 23028, 11844, 25101, 22006, 10273, 21723, 7721, 26688, 6823, 2395, 17335, 30893, 1883,
	1424, 13155, 31498, 4451, 9307, 20375, 12005, 1445, 7256, 23701, 20304, 23423, 19903, 9923, 28641, 11705, 10120, 25413, 11005, 16312, 26579, 32465, 1366, 25332, 15238, 20159, 10931, 20652, 312, 22624, 26608, 19416,
	13134, 8849, 1605, 8031, 15715, 17808, 27595, 19594, 16622, 11534, 9531, 32184, 3358, 16517, 15609, 4436, 19977, 26059, 850, 20741, 14086, 4584, 11266, 10532, 11351, 23589, 25850, 29811, 12300, 18277, 19040, 12300,
	5667, 13752, 23510, 11159, 17396, 14894, 22271, 2496, 32461, 15593, 6975, 9449, 3545, 9590, 31427, 11291, 13126, 32606, 7362, 3139, 16550, 19703, 21762, 31131, 19094, 31872, 5277, 11622, 16286, 13165, 22533, 11232,
	26201, 25600, 13894, 9845, 1750, 24857, 30091, 20017, 17213, 25679, 18660, 10872, 25526, 14911, 6982, 18423, 6534, 2340, 3028, 21466, 32123, 26025, 32604, 30527, 19851, 10747, 27429, 1771, 11704, 17185, 15592, 23356,
	28727, 26768, 1052, 25695, 10448, 29178, 16502, 20163, 3976, 16232, 31153, 19406, 30334, 26504, 1163, 21198, 22291, 8264, 6416, 28210, 14925, 5539, 7706, 26874, 126, 8891, 19993, 28780, 19488, 8489, 6416, 32265,
	5124, 17041, 26592, 7615, 8364, 10361, 9417, 25696, 30965, 11867, 11566, 10835, 504, 6648, 4248, 24202, 22067, 9712, 16638, 1459, 6116, 6800, 6212, 14771, 17078, 26859, 22273, 15696, 16543, 31788, 133, 30765,
	17929, 13610, 14363, 28412, 14640, 17469, 907, 3055, 10443, 21841, 5859, 19311, 5614, 16952, 3447, 8465, 5423, 16197, 13351, 10438, 2824, 13846, 18656, 30802, 1916, 17073, 21350, 9471, 1277, 1747, 31568, 20882,
	3726, 18308, 32596, 22654, 4389, 2287, 12740, 26200, 23635, 7281, 7769, 6283, 5670, 21744, 15664, 29773, 9807, 21870, 22290, 18897, 23680, 11741, 32504, 22462, 22040, 14901, 17614, 17121, 26370, 30661, 1400, 13854,
	2522, 17606, 31763, 16045, 6994, 15926, 30845, 14467, 25813, 12260, 24343, 24333, 14954, 19143, 21961, 22052, 12250, 5523, 16967, 16190, 10517, 19342, 32147, 12001, 18898, 7584, 24772, 5726, 2166, 2474, 3845, 30139,
	26679, 15385, 8416, 27121, 7809, 12219, 5309, 27480, 10136, 32721, 10405, 20575, 3712, 8293, 332, 26752, 27674, 28894, 16975, 17267, 24975, 23761, 31674, 30882, 4593, 32538, 4301, 734, 24352, 31532, 31744, 1104,
	28139, 165, 5188, 1714, 13688, 11558, 14220, 3007, 7654, 16481, 32396, 487, 29232, 22131, 24000, 29000, 10581, 7285, 22449, 29914, 19126, 13133, 9332, 21481, 29119, 13514, 29299, 20425, 13513, 3825, 9328, 31173,
	29503, 10641, 12486, 9560, 20224, 2590, 1522, 19104, 21765, 27067, 6227, 27919, 6468, 29081, 32046, 23140, 18900, 4480, 14069, 21977, 32081, 9387, 5833, 926, 16514, 22745, 22333, 10375, 10041, 2977, 21597, 4610,
	25723, 4614, 1263, 14923, 11745, 7755, 6392, 31971, 28222, 14345, 24962, 28478, 2946, 31358, 26013, 12731, 17375, 3368, 18596, 19674, 26154, 2706, 26049, 28330, 27011, 18799, 17587, 7296, 10296, 15333, 19407, 11502,
	16871, 22827, 21317, 7667, 26851, 17750, 32165, 7024, 7897, 11120, 31138, 18907, 9396, 32203, 4373, 10551, 8637, 4244, 10565, 9284, 17927, 17067, 6245, 8179, 27658, 10418, 29790, 7964, 32142, 28261, 16079, 20083,
	8139, 25129, 4690, 3257, 27342, 24295, 13570, 17199, 12318, 2074, 13370, 8784, 5280, 3111, 28063, 5823, 440, 24814, 10592, 2687, 12721, 11934, 10684, 18018, 23391, 23757, 21445, 1988, 16642, 30152, 21860, 7800,
	9838, 21543, 27806, 9987, 29289, 20133, 13333, 13576, 32597, 22063, 6354, 516, 24335, 14907, 23873, 29756, 5654, 1883, 19837, 4595, 4592, 13330, 2254, 13535, 24265, 4070, 1137, 4574, 24670, 10422, 11159, 26854,
	4633, 6734, 16559, 28986, 4733, 31801, 25576, 21988, 32197, 25046, 25330, 22111, 23496, 8367, 13829, 24468, 9504, 20741, 20005, 12552, 9099, 15536, 6308, 25171, 12689, 12326, 23528, 23764, 7528, 25049, 20847, 7587,
	13076, 25542, 8463, 21447, 12754, 21322, 8579, 11945, 12471, 15160, 547, 28873, 13973, 23594, 16883, 6832, 2334, 20244, 6111, 16934, 30540, 13855, 12357, 27044, 8960, 28596, 25524, 27660, 3561, 32726, 19188, 20632,
	28071, 9142, 21574, 6928, 2864, 17281, 15852, 27713, 12659, 13949, 6636, 1706, 28476, 3412, 16146, 2936, 12376, 22192, 19252, 949, 29177, 4615, 6841, 2492, 5724, 17593, 22876, 9968, 7080, 25944, 4142, 14765,
	14874, 4422, 21722, 30591, 11615, 16518, 32301, 24466, 11585, 27596, 470, 12650, 22918, 22742, 24426, 15316, 7445, 255, 15531, 6173, 19547, 17938, 5897, 9601, 749, 26972, 11881, 22758, 1135, 30981, 3366, 13515,
	9396, 10144, 20821, 32123, 31526, 16896, 6990, 32134, 25961, 14157, 17768, 28273, 7483, 26456, 18691, 22957, 26015, 11052, 29900, 1478, 4922, 19430, 22588, 17373, 29693, 23494, 2146, 12632, 23044, 9605, 8675, 16856,
	19902, 9705, 11327, 9579, 11087, 17304, 16981, 24324, 12082, 14634, 9258, 7976, 13859, 25985, 5606, 14057, 13372, 30301, 12319, 14104, 10078, 8491, 1370, 4023, 2258, 24094, 16591, 17325, 560, 22913, 8813, 1213,
	27008, 3916, 31774, 19385, 18592, 17653, 5196, 16627, 29670, 21896, 4516, 2293, 29701, 1018, 21530, 18802, 18229, 18363, 24365, 31356, 15758, 18314, 14700, 22518, 29877, 30349, 25913, 1869, 14011, 1953, 22221, 17763,
	16450, 10991, 2940, 12492, 12770, 18880, 9786, 1085, 12491, 13458, 13663, 21825, 31403, 30570, 1447, 11054, 28882, 18541, 9853, 600, 24212, 17578, 889, 3972, 32494, 28475, 14119, 22432, 1389, 9868, 31498, 28593,
	11852, 1018, 22752, 30988, 6620, 22944, 296, 11721, 30505, 17005, 27362, 4469, 16859, 30842, 8342, 6663, 5978, 26011, 24215, 25877, 26422, 8757, 26249, 19784, 709, 22095, 11297, 29710, 20494, 12523, 16172, 5006,
	9189, 27030, 5843, 11489, 1880, 62, 8733, 26239, 27722, 12100, 16821, 12023, 5470, 17362, 3822, 30692, 13591, 20591, 5124, 7452, 2109, 28112, 22949, 19498, 28546, 32241, 28079, 32767, 19557, 9573, 18236, 25828,
	9559, 28395, 11228, 14515, 21788, 19777, 31265, 31559, 18805, 28477, 32097, 817, 5372, 6681, 11959, 19116, 22607, 32271, 1867, 2260, 18032, 14629, 18387, 27410, 18540, 31819, 22874, 731, 1083, 1699, 11072, 15258
};

/* get a random number in [0, RAND_MAX] */
#define RAND()   (g_randNums[(g_randIdx++) & 1023])
#define RAND4()  (RAND() & 3)
#define RAND8()  (RAND() & 7)   /* get a random number in [0, 7]  */
#define RAND16() (RAND() & 15)  /* get a random number in [0, 15] */

/* default parameters */
#define SAMPLES_PER_PIXEL      10          /* number of samples per pixel */
#define SPHERE_RADIUS          20          /* radius of the sphere        */
#define CLOSE_SAMPLES          2           /* number of close samples for being part of the background */
#define SUBSAMPLING            8           /* amount of random subsampling */

#define SQR(x)          ((x) * (x))

#define VAL_BG            0            /* background pixel */
#define VAL_FG          255            /* foreground pixel */
#define VAL_UNDEFINE   0x0F            /* undefined pixel */

#define COLOR_VERSION   1

#ifdef COLOR_VERSION
#define COLOR_DIST(pa, pb)   ( SQR((int)(pa)[0]-(pb)[0]) + SQR((int)(pa)[1]-(pb)[1]) + SQR((int)(pa)[2]-(pb)[2]) )
#define SET_COLOR(pa, pb)    { (pa)[0] = (pb)[0]; (pa)[1] = (pb)[1]; (pa)[2] = (pb)[2]; }
#else
#define COLOR_DIST(pa, pb)   ( SQR((int)(pa)[0]-(pb)[0]) )
#define SET_COLOR(pa, pb)    { (pa)[0] = (pb)[0]; }
#endif

CTFVIBE::CTFVIBE()
{
	m_bBeginFrames = true;
	m_vibeParams.samplesPerPixel = SAMPLES_PER_PIXEL;
	m_vibeParams.sphereRadius = SPHERE_RADIUS;
	m_vibeParams.closeSamples = CLOSE_SAMPLES;
	m_vibeParams.subSampling = SUBSAMPLING;
}

CTFVIBE::~CTFVIBE()
{
	if (m_bgModel.samples != NULL){
		delete[]m_bgModel.samples;
		m_bgModel.samples = NULL;
	}
}

void CTFVIBE::Init(int nWidth, int nHeight)
{
	if (m_ncx != nWidth || m_ncy != nHeight || m_bgModel.samples == NULL){
		m_ncx = nWidth;
		m_ncy = nHeight;
		
		m_bgModel.width = nWidth;
		m_bgModel.height = nHeight;
		m_bgModel.bytesPerPixel = m_vibeParams.samplesPerPixel * 3;
		m_bgModel.bytesPerLine = m_bgModel.bytesPerPixel * m_bgModel.width;
		m_bgModel.params = m_vibeParams;

		if (m_bgModel.samples != NULL){
			delete[]m_bgModel.samples;
			m_bgModel.samples = NULL;
		}

		m_bgModel.samples = new unsigned char[m_bgModel.height * m_bgModel.bytesPerLine];

		m_bBeginFrames = true;
	}
}

void CTFVIBE::InitBgModel(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int i_sampleNum)
{
	int x = 0, y = 0;
	int nbOffset[8];
	int cn = 3;
	int step = (int)nScanSrc * cn;
	int i = 0;

	nbOffset[0] = -step - cn;
	nbOffset[1] = -step;
	nbOffset[2] = -step + cn;
	nbOffset[3] = cn;
	nbOffset[4] = step + cn;
	nbOffset[5] = step;
	nbOffset[6] = step - cn;
	nbOffset[7] = -cn;

	for (y = 1; y < m_bgModel.height - 1; y++)
	{
		unsigned char *bg = m_bgModel.samples + y * m_bgModel.bytesPerLine + m_bgModel.bytesPerPixel;
		unsigned char *img = pSrc + y * step + cn;
		for (x = 1; x < m_bgModel.width - 1; x++, bg += m_bgModel.bytesPerPixel, img += cn)
		{
			unsigned char *bgp = bg;
			for (i = 0; i < i_sampleNum; i++, bgp += cn)
			{
				unsigned char *p = img + nbOffset[RAND8()];
				SET_COLOR(bgp, p);
			}
		}
	}
}

void CTFVIBE::DetectImpl(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, bool beUpdataBackGround)
{
	int x = 0, y = 0;
	int nbOffset[8];
	int cn = 3;
	int step = (int)nScanSrc*cn;
	int bgstep = m_bgModel.bytesPerLine;
	int bgcn = m_bgModel.bytesPerPixel;
	const int R = SQR(m_bgModel.params.sphereRadius);

	nbOffset[0] = -bgstep - bgcn;
	nbOffset[1] = -bgstep;
	nbOffset[2] = -bgstep + bgcn;
	nbOffset[3] = bgcn;
	nbOffset[4] = bgstep + bgcn;
	nbOffset[5] = bgstep;
	nbOffset[6] = bgstep - bgcn;
	nbOffset[7] = -bgcn;

	for (y = 1; y < m_bgModel.height - 1; y++)
	{
		unsigned char *bg = m_bgModel.samples + y * m_bgModel.bytesPerLine + m_bgModel.bytesPerPixel;
		unsigned char *img = pSrc + y * step + cn;
		unsigned char *fg = pDst + y * m_bgModel.width + 1;

		for (x = 1; x < m_bgModel.width - 1; x++, bg += m_bgModel.bytesPerPixel, img += cn, fg++)
		{
			unsigned char *bgp = bg;

			{
				int i = 0;
				int count = 0;
				while (count < m_bgModel.params.closeSamples && i < m_bgModel.params.samplesPerPixel)
				{
					int dist = COLOR_DIST(img, bgp);
					if (dist < R)
					{
						count++;
					}

					i++;
					bgp += cn;
				}

				*fg = (count >= m_bgModel.params.closeSamples) ? VAL_BG : VAL_FG;
			}

			if (*fg == VAL_BG && beUpdataBackGround)
			{
				if (0 == RAND4())
				{
					int sampleIdx = RAND() % m_bgModel.params.samplesPerPixel;
					bgp = bg + sampleIdx * cn;
					SET_COLOR(bgp, img);
				}

				if (0 == RAND4())
				{
					int nb = RAND8();
					int sampleIdx = RAND() % m_bgModel.params.samplesPerPixel;
					bgp = bg + nbOffset[nb] + sampleIdx * cn;
					SET_COLOR(bgp, img);
				}
			}
		}
	}

	{
		unsigned char *fg = pDst + m_bgModel.width;
		for (y = 1; y < m_bgModel.height - 1; y++, fg += m_bgModel.width)
		{
			fg[0] = fg[1];
			fg[m_bgModel.width - 1] = fg[m_bgModel.width - 2];
		}

		fg = pDst;
		memcpy(fg, fg + m_bgModel.width, m_bgModel.width);
		fg = pDst + m_bgModel.width * (m_bgModel.height - 1);
		memcpy(fg, fg - m_bgModel.width, m_bgModel.width);
	}
}

void CTFVIBE::Detect(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst, bool beUpdataBackGround)
{
	if (nWidth == 0 || nHeight == 0 || pSrc == NULL)
		return;	

	Init(nWidth, nHeight);

	if (m_bBeginFrames)
	{
		InitBgModel(nWidth, nHeight, nScanSrc, pSrc, m_vibeParams.samplesPerPixel);
		m_bBeginFrames = false;
	}
	else
	{
		DetectImpl(nWidth, nHeight, nScanSrc, pSrc, nScanDst, pDst, beUpdataBackGround);
		DilateAndErode(nWidth, nHeight, nScanDst, pDst, nScanDst, pDst);
	}
}

void CTFVIBE::Replay()
{
	m_bBeginFrames = true;
}

void CTFVIBE::DilateAndErode(int nWidth, int nHeight, int nScanSrc, unsigned char* pSrc, int nScanDst, unsigned char* pDst)
{
	Mat img(nHeight, nWidth, CV_8UC1, pSrc);
	Mat out;
	medianBlur(img, img, 5);

	Mat ele = getStructuringElement(MORPH_RECT, Size(5, 5), Point(2, 2));
	dilate(img, img, ele);
	erode(img, img, ele);
	
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