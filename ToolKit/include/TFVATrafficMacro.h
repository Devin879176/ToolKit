#ifndef  __TFVATRAFFICMACRO_H__
#define  __TFVATRAFFICMACRO_H__
#include <functional>
#include <vector>
#include <list>

#ifdef OS_LINUX
#include "string.h"
#define STOKE "/"
#else
#define STOKE "\\"
#endif

//小于0的返回值为系统错误，大于等于0返回的是当前状态，不归为错误
#define TF_SP_PARAM_ERROR           -6   //参数错误
#define TF_SP_TOOLARGE              -5   //图像分辨率过大，不支持
#define TF_SP_ERROR                 -4   //程序错误
#define TF_SP_NO_MEMORY             -3   //内存错误
#define TF_SP_NULL_POINT            -2   //空指针错误
#define TF_SP_LoadXML_ERROR         -1   //加载模型失败
#define TF_SP_OK                     0   //正常
#define TF_SP_NOVIDEO                1   //无视频信号
#define TF_SP_SHAKE                  2   //抖动
#define TF_SP_FOG                    3   //雾天
#define TF_SP_NOROAD                 4   //非路面
#define TF_SP_SCENE_CHANGE           5   //场景切换
#define TF_SP_FG_CHANGE_TOOLARGE     6   //前景突然变化
#define TF_SP_SNOW                   7   //雪天
#define TF_SP_RAIN                   8   //雨天

////一些范围宏定义
#define TF_SP_CARLIFECOUNT_MAX  50		//最大的跟踪车辆数
#define TF_SP_ALARMCOUNT_MAX    50		//一次最大的事件上报数
#define TF_SP_MAX_ROADNUM		15		//支持的最大的单方向车道数
#define TF_SP_MAX_OBJNUM       200      //单图检测最大目标个数
#define TF_SP_MAX_POINT_NUM     10      //单区域最大点数
#define TF_SP_MAX_ALAEMCARNUM    5      //报警时车辆个数

//支持的数据格式
#define TF_SP_MAXPICWIDTH		2560	//支持的最大输入图片宽
#define TF_SP_MAXPICHEIGHT	    1920	//支持的最大输入图片高

#define TF_SP_MINPICWIDTH	     200    //支持的最小输入图片宽
#define TF_SP_MINPICHEIGHT		 188	//支持的最小输入图片高

#define TF_SP_PICWIDTH		    352	    //算法处理的图片宽
#define TF_SP_PICHEIGHT		    288	    //算法处理的图片高


#define TF_CROWDED_DETECT                   0x0001       //拥堵
#define TF_STOP_DETECT                      0x0002       //停车
#define TF_CRASH_DETECT                     0x0004       //撞车
#define TF_RETROGRADE_MOTION                0x0008       //逆行
#define TF_THROWING_OBJECT                  0x0010       //抛洒物
#define TF_PEDESTRIAN_DETECT                0x0020       //行人
#define TF_ABNORMAL_DRIVING                 0x0040       //机动车驶离
#define TF_SMOKE_DETECT                     0x0080       //烟火
#define TF_LANDSIDE_DETECT                  0x0100       //塌方
#define TF_NONMOTOR_DETECT                  0x0200       //非机动车
#define TF_EMERGENCY_DETECT                 0x0400       //应急车道占用
#define TF_DEPARTURE_DETECT                 0x0800       //变道
#define TF_PEDESTRIAN_CROSSING              0x1000       //行人穿越
#define TF_ROADBLOCK_DETECT                 0x2000       //路障
#define TF_ROADDAMAGE_DETECT                0x4000       //道路损毁
#define TF_ROLLOVER_DETECT                  0x8000       //翻车

#define TF_FLAG_INPUTVIDEO              0   //输入视频
#define TF_FLAG_CHANGE                  1   //切换通道
#define TF_FLAG_INPUTFG                 2   //输入前景

typedef char                tf_int8;
typedef short               tf_int16;
typedef int                 tf_int32;
typedef long long           tf_int64;
typedef unsigned char       tf_uint8;
typedef unsigned short      tf_uint16;
typedef unsigned int        tf_uint32;
typedef unsigned long long  tf_uint64;

enum ETF_Detect_MODE
{
	ETF_DETECT_H = 0,   //车流速度水平情况
	ETF_DETECT_V = 1,   //车流速度垂直情况
};

//运动目标检测算法设置
enum ETF_MOD_MODE
{
	ETF_MOD_CODE = 0,   //codebook
	ETF_MOD_GMM  = 1,   //gmm
	ETF_MOD_VIBE = 2,   //vibe
	ETF_MOD_VEP  = 3    //vep
};

//目标跟踪算法设置
enum ETF_TRACK_MODE
{
	ETF_TRACK_TRAJ = 0,   //轨迹
	ETF_TRACK_KCF = 1     //KCF
};

//场景类型定义
enum ETF_SCENE_MODE
{
	ETF_SCENE_MODE_UNKNOWN = -1,  //未知
	ETF_SCENE_MODE_NIGHT = 0,     //夜晚场景
	ETF_SCENE_MODE_DAY = 1,       //白天场景
	ETF_SCENE_MODE_TUNNEL = 2     //隧道场景
};

//多标签分类返回值
#ifndef TF_MULTILABEL_RESULT_
#define TF_MULTILABEL_RESULT_
typedef struct TF_MULTILABEL_VALUE_
{
	int   multiResultType;        //多标签分类类型
	float multiFResultValue;      //多标签分类置信度
} TF_MULTILABEL_VALUE;

#define MaxMultiLabelNum  5      //多标签最大支持的类别数

#endif

//算法配置结构体
#ifndef TF_ALG_PARAM_
#define TF_ALG_PARAM_
typedef struct TF_ALG_INPARAM_
{
	int  closedFog;      //雾天 [0-开启 1-关闭]
	int  closedScene;    //非路面 [0-开启 1-关闭]
	int  trafficInfoInterval;  //交通流量上报间隔
	int  trafficEventInterval; //交通事件上报间隔
	char cFunctionList[128];  //授权事件类型
	int  gpuCount;            //GPU个数
	int  nChnPerGPU;          //单GPU处理路数
	char workPath[256];      //工作目录
	int  eventTimeSpan[20];  ////事件上报间隔
} TF_ALG_INPARAM;
#endif

//输入信息结构
#ifndef TF_SP_INPARAM_
#define TF_SP_INPARAM_
typedef struct TF_SP_INPARAM
{
	int nSceneMode;                   //场景模型
	int nRoadInfo;                    //道路状况（0正常,1匝道,2立交桥,3停车带,4服务区,5收费站，6市内交通，7坡道，8弯道，9隧道）
	int nLaneUpOrDown;	              //离相机近的车道是上行车道还是下行车道 0上行1下行2未知
	int nUpRoadNum;                   //上行车道数
	int nUpLinesNum;                  //上行车道线数目
	int nUpMedianStripNum;            //上行隔离带数目
	int nDownRoadNum;                 //下行车道数
	int nDownLinesNum;                //下行车道线数目
	int nDownMedianStripNum;          //下行隔离带数目
	int nReSpeed;                     //道路参考速度值（预留）

	int nWidth;                       //图像宽度
	int nHeight;                      //图像高度
	int nScan;                        //图像跨距

	int nROI_X;                       //停车巡检开关
	int nROI_Y;                       //清洁人员开关
	int nROI_W;                       //运维车辆开关
	int nROI_H;                       //路面区域开关

	ETF_MOD_MODE    eModMode;         //前景算法设置
	ETF_TRACK_MODE  eTrackMode;       //撞车算法设置
	ETF_Detect_MODE eDetectMode;      //
	
	int nSensibility;                 //背景差分阈值

	float fEdge_threshold;            //停车是否结合车检开关
	int   nStop_duration;             //停车时长
	int   nStop_frame_count;          //停车累计帧数

	float fCrowd_occupy_threshold;       //行人检测阈值
	float fCrowd_high_occupy_threshold;  //非机动车检测阈值
	float fCrowd_level_threshold;        //拥堵是否结合车检开关

	float landslide_threshold;        //塌方的阈值
	float rockfall_threshold;         //落石的阈值
	float spill_threshold;            //抛洒物的阈值
	float crash_flow_threshold;       //撞车流量阈值

	char filePath[256];                 //模型文件路径
	char logPath[256];                  //LOG文件路径

	char sunUpTime[10];     //日出时间 [07:00]
	char sunSetTime[10];    //日落时间 [18:00]

	char eventType[30];     //事件类型
	char cameraID[30];      //相机ID

	//以下是预留接口
	tf_int64 i64back1;       //流量上报时间间隔
	tf_int64 i64back2;       //事件上报时间间隔
	tf_int64 i64back3;       
	tf_int32 iback1;		 //通道号
	tf_int32 iback2;         //开启交通参数计算标志[0-关闭,1-开启]
	tf_int32 iback3;         //开启的交通事件检测功能[1-拥堵 2-停车 4-撞车 8-逆行 16-抛撒物 32-行人穿越 64-机动车驶离]
}TF_SP_INPARAM;
#endif

#ifndef TF_POINT_
#define TF_POINT_
typedef struct TF_POINT_
{
	int nX;               
	int nY;            
}TF_POINT;
#endif

#ifndef TF_DetectPolygon
#define TF_DetectPolygon

typedef struct TF_AREA_
{
	bool isLine;    //线或区域
	int  nEventType; //事件类型[0-所有事件 1-交通参数 2-拥堵 3-停车 4-撞车 5-逆行 6-抛洒物 7-行人 8-机动车驶离 9-烟火 10-塌方 11-非机动车]
	int  nDirection; //上行或下行
	int  nPoint;    //点个数
	TF_POINT pPoint[TF_SP_MAX_POINT_NUM]; //坐标
}TF_AREA;

typedef struct TF_PTZINFO_
{
	long  wPanPos;   //水平参数
	long  wTiltPos;  //垂直参数
	long  wZoomPos;  //变倍参数
}TF_PTZ;

typedef struct TF_DetectPolygon
{
	int WnWidth;
	int WndHeigh;
	TF_PTZ  PTZ;
	std::vector<TF_AREA>  Areas;
}TF_DetectPolygonEx;
#endif


#ifndef TF_IMAGE_
#define TF_IMAGE_
typedef struct TF_IMAGE_
{
	int nWidth;                 //图像宽度
	int nHeight;                //图像高度
	tf_uint8* memBufRGB;	    //缓冲区首地址
	int  memBufRGBSize;			//缓冲区大小
	tf_uint64 lTimeStamp;       //获取视频帧时的时间戳
	tf_uint64 nDataType;        //数据类型，1：流量图(静态采样图)；2：动态采样图；3.原始图; 4.灰度图
	ETF_SCENE_MODE nSceneMode;  //场景类型，白天，晚上或者隧道
	TF_PTZ sPtzInfo;            //相机PTZ值
}TF_IMAGE;
#endif

//矩形区域
#ifndef SRECT_
#define SRECT_
typedef struct SRECT_
{
	int top;
	int bottom;
	int left;
	int right;
}SRect;
#endif//_SRECT_

//检测结果
#ifndef DETECT_INFO_
#define DETECT_INFO_
typedef struct DETECT_INFO_
{
	int    objectID;
	float  confidence;
	SRect  location;
}DETECT_INFO;
#endif//DETECT_INFO_

//外接矩形结构体
struct CBoundBox
{
	int nbeginX;
	int nbeginY;
	int nw;
	int nh;
	int nconnetedArea;

	CBoundBox()
	{
		nbeginX = 0;
		nbeginY = 0;
		nw = 0;
		nh = 0;
		nconnetedArea = 0;
	}

	CBoundBox(int beginX, int beginY, int w, int h, int connetedArea)
	{
		nbeginX = beginX;
		nbeginY = beginY;
		nw = w;
		nh = h;
		nconnetedArea = connetedArea;
	}

	CBoundBox(const CBoundBox& other)
	{
		*this = other;
	}

	CBoundBox& operator =(const CBoundBox &other)
	{
		nbeginX = other.nbeginX;
		nbeginY = other.nbeginY;
		nw = other.nw;
		nh = other.nh;
		nconnetedArea = other.nconnetedArea;

		return *this;
	}
};
typedef std::vector<CBoundBox> bRectVector;

//输出车辆结构
struct TF_CAR_OutObj
{
	int iCarType;			//车辆类型   1大车  2小车
	int iLaneNum;			//车道号		 [1-10]	
	int iUporDown;			//上行还是下行  1上行 2下行
	float fCarSpeed;		//车辆速度
	SRect rt;               //目标位置 
	TF_CAR_OutObj()
	{
		iCarType = 0;
		iLaneNum = 0;
		iUporDown = 0;
		fCarSpeed = 0.0f;
	}

	TF_CAR_OutObj(int CarType, int LaneNum, int UporDown, float CarSpeed)
	{
		iCarType = CarType;
		iLaneNum = LaneNum;
		iUporDown = UporDown;
		fCarSpeed = CarSpeed;
	}
};

typedef std::vector<TF_CAR_OutObj> aVecCarObj;

//占有率
#ifndef TF_Occupancy_Info_
#define TF_Occupancy_Info_
typedef struct TF_Occupancy_Info_
{
	bool  bValid;                                  //返回值是否有效

	float foccupancy;                              //总占有率
	float foccupancy1;                             //上行占有率
	float foccupancy2;                             //下行占有率
	float fRoadOccupancy[2][TF_SP_MAX_ROADNUM];    //占有率分车道数据
	float ComprehensiveValue[2]; 	               //综合值，上下行

	float ftimeInterval;                           //总车头时距
	float fTimeIntervalUp;                         //上行车头时距
	float fTimeIntervalDown;                       //下行车头时距
	float fRoadTimeInterval[2][TF_SP_MAX_ROADNUM]; //车头时距分车道数据

	int   iCarNumUp;                               //上行车辆排队个数
	int   iCarNumDown;                             //下行车辆排队个数
	int   iRoadCarNum[2][TF_SP_MAX_ROADNUM];       //车辆排队个数分车道数据

} TF_Occupancy_Info;
#endif

//事件对象
#ifndef TF_Alarm_Obj_
#define TF_Alarm_Obj_

const char C_CAR_COLOR_TYPE[][20] = { "黑色", "白色", "灰色", "红色", "黄色", "蓝色", "绿色", "未知颜色" };
const char C_CAR_TYPE_TYPE[][20] = { "行人", "非机动车", "轿车", "小型客车", "中型客车", "大型客车", "小型货车", "中型货车", "重型货车", "危化品车", "养护车", "养护人员", "警察" };

const char C_Event_TYPE_DESCRIB[][20] = { "正常", "拥堵", "停车", "雾天", "烟火", "逆行", "非路面", "撞车", "抛洒物", "行人", "机动车驶离", "塌方", "非机动车", "无信号", "视频抖动", "雪天", "转动", "镜头有雨", "告警结束", "晴天", "应急车道占用", "压线", "行人穿越", "路障", "路面损毁", "翻车", "图像质量"};
const char C_Event_TYPE_DESCRIBEN[][20] = { "0", "crowding", "parking", "4", "fire", "retrograde", "2", "crash", "littering", "pedestrian", "leaving", "landside", "nonmotor", "1", "3" , "5", "6", "7","end" ,"11", "emergency", "departure", "crossing", "roadblock", "damage", "rollover", "quality"};

typedef struct TF_Alarm_Obj_
{
	int nAlarmType;         //事件类型【0正常 1拥堵  2停车 3雾天 4烟火 5逆行 6非路面 7撞车 8抛洒物 9行人 10机动车驶离 11塌方 12非机动车 13无信号 14视频抖动 15雪天 16转动 17镜头有雨 18告警结束 19晴天 20应急车道占用 21变道 22行人穿越 23路障 24路面损毁 25翻车 26图像质量】
	int nAlarmDirection;	//事件上下行【1上行 2下行】(对于场景:1恢复正常,2变为异常)
	int nAlarmRoadNum;		//事件车道号
	int nAlarmLevel;		//事件等级
	tf_uint64 nAlarmTime;   //事件发生的时间
	int    rtAlarmRTCount;  //报警时的车辆个数
	SRect  rtAlarmRT[TF_SP_MAX_ALAEMCARNUM];  //区域
	int nCarType[TF_SP_MAX_ALAEMCARNUM];      //车型
	int nColorType[TF_SP_MAX_ALAEMCARNUM];    //车辆颜色
	TF_IMAGE *pAlarmImage;  //报警时图像
	char videoDiagnose[20];	//图像质量
	char strEventID[20];    //事件ID
}TF_Alarm_Obj;
#endif

//事件结构
#ifndef TF_Alarm_Info_
#define TF_Alarm_Info_
typedef struct TF_Alarm_Info_
{
	TF_Alarm_Obj  alarmObj[TF_SP_ALARMCOUNT_MAX];
	int nAlarmCount;
}TF_Alarm_Info;
#endif

//路面分割结果
#ifndef TF_Road_Locate_
#define TF_Road_Locate_
#define  ROADPOINTNUM 5 
#define  MAXLINENUM 10
typedef struct TF_Road_Locate_Struct
{
	/////路面区域信息
	float fLocate[ROADPOINTNUM * 2 ];
	float fLocateExt[2];

	////字体位置信息，原始检测结果
	SRect rtFont;
	////绿化带位置信息
	SRect rtGreeBelt;
    ////车道线上行个数,4个车道5条线，nUpLineNum = 5
	int nUpLineNum;
	////车道线下行个数
	int nDownLineNum;
	////车道线信息，拟合结果 上行
	float ptUpBeginX[MAXLINENUM];
	float ptUpBeginY[MAXLINENUM];
	float ptUpEndX[MAXLINENUM];
	float ptUpEndY[MAXLINENUM];
	//下行
	float ptDownBeginX[MAXLINENUM];
	float ptDownBeginY[MAXLINENUM];
	float ptDownEndX[MAXLINENUM];
	float ptDownEndY[MAXLINENUM];
	/////拌线信息
	float TripLine[4];
	/////上下行分割点
	float fMiddleX, fMiddleY;
	//相机位置
	unsigned int nCamLoc;
}TF_Road_Locate;
#endif

typedef struct TF_Detect_Info_
{
	int   detectedPersonNum;
	int   detectedCarNum;
	int   detectedMotoBikeNum;
	int   detectedWindowNum;
	int   detectedCarheadNum;
	int   detectedPlateNum;
	int   detectedTurnoverNum;
	int   detectedTurnoverTireNum;

	float fPersonScore[TF_SP_MAX_OBJNUM];
	float fCarScore[TF_SP_MAX_OBJNUM];
	float fMotoBike[TF_SP_MAX_OBJNUM];
	float fWindowScore[TF_SP_MAX_OBJNUM];
	float fCarheadScore[TF_SP_MAX_OBJNUM];
	float fPlateScore[TF_SP_MAX_OBJNUM];
	float fTurnoverScore[TF_SP_MAX_OBJNUM];
	float fTurnoverTireScore[TF_SP_MAX_OBJNUM];

	/////车型
	int carLabel[TF_SP_MAX_OBJNUM];
	////车辆颜色
	int colorType[TF_SP_MAX_OBJNUM];

	SRect personRect[TF_SP_MAX_OBJNUM];
	SRect carRect[TF_SP_MAX_OBJNUM];
	SRect motoBikeRect[TF_SP_MAX_OBJNUM];

	/////车窗
	SRect window[TF_SP_MAX_OBJNUM];
	/////车头
	SRect carhead[TF_SP_MAX_OBJNUM];
	////车牌
	SRect plate[TF_SP_MAX_OBJNUM];

	/////翻车
	SRect Turnover[TF_SP_MAX_OBJNUM];

	/////翻车轮胎
	SRect TurnoverTire[TF_SP_MAX_OBJNUM];
}TF_Detect_Info;

//EventDetStruct
#ifndef TF_EventDetInfo_
#define TF_EventDetInfo_
#define TF_EventType_Alarm     0
#define TF_EventType_CBack     1
#define TF_EventType_Road      2
#define TF_EventType_Detect    3
#define TF_EventType_Scene     4
#define TF_EventType_Weather   5
#define TF_EventType_LANDSIDE  6   //塌方
typedef union
{	
	struct
	{
		int alarmObjNum;
		TF_Alarm_Obj  alarmObj[TF_SP_ALARMCOUNT_MAX];
	}ALARM;	

	struct
	{
		int   nFrameIndex;
		int   nDataType;   
		int   nResultType;
		float fResultValue;
		int multiLabelNum;
		TF_MULTILABEL_VALUE multiLabelValue[MaxMultiLabelNum];
	}CBACK;

	struct
	{
		bool bRoadSegOK;
		TF_Road_Locate  RoadLocateObj;
		TF_Detect_Info  RoadDetectObj;
		TF_IMAGE  *pMaskImage;
	}ROAD;	

	struct
	{
		int             nDataType;
		ETF_SCENE_MODE nSceneMode;
		int             nframeNum;                
		TF_Detect_Info  DetectedResults[5];
		TF_IMAGE       *pDetectImage;
	}DETECT;	
}TF_DetInfo_Union;

typedef struct
{
	int OutFlag;
	int type;
	TF_DetInfo_Union value;
}TF_EventDetInfo;

#endif
typedef struct tag_EmergLineArea
{
	int nPtNum;
	TF_POINT ptArea[10];
}EmergLineAreaStruct;

//算法主接口输入输出结构
#ifndef CTFVATrafficInOutParam_
#define CTFVATrafficInOutParam_
typedef struct CTFVATrafficInOutParam_
{
	int nObjNum;					//当前帧运动目标数目	[out]
	int nVehicleFlow1;				//上行当前流量			[out]
	int nVehicleFlow2;				//下行当前流量			[out]
	int nBigFlow1;					//上行流量				[out]
	int nBigFlow2;					//下行流量				[out]
	int nSmallFlow1;				//上行流量				[out]
	int nSmallFlow2;				//下行流量				[out]
	float fVec1;					//上行方向的断面车速	[out]
	float fVec2;					//下行方向的断面车速	[out]
	float fAveVec1;                 //上行方向平均车速      [out]
	float fAveVec2;                 //下行方向平均车速      [out]
	float fBigVec1;                 //上行方向大车平均车速  [out]
	float fBigVec2;                 //下行方向大车平均车速  [out]
	float fSmallVec1;               //上行方向小车平均车速  [out]
	float fSmallVec2;               //下行方向小车平均车速  [out]
	float fEntropy;					//熵值返回				[out]
	float fDisEntropy;			    //行人阈值			    [out]
	float fDisDisEntropy;			//非机动车阈值			[out]

	float fLandslide;			    //塌方阈值			    [out]
	float fRockfall;			    //落石阈值			    [out]
	float fSpill;			        //抛洒物阈值			[out]

	float fDetLineTheta;			//车道线角度返回		    [out]
	tf_uint64 nNowTime;				//系统处理当前帧时候的时间	[in]
	int nTrafficJamlev;             //当前帧拥堵等级         [out]
	int nTrafficJamlev1;            //当前帧上行拥堵等级         [out]
	int nTrafficJamlev2;            //当前帧下行拥堵等级         [out]
	int nJamLevRes;                 //整理后输出拥堵等级     [out]		//上报1 2 3 4 5级  1 2 3 级算作事件
	int nJamLevRes1;                //上行整理后输出第一个方向拥堵等级     [out]		//上报1 2 3 4 5级  1 2 3 级算作事件
	int nJamLevRes2;                //下行整理后输出第二个方向拥堵等级     [out]		//上报1 2 3 4 5级  1 2 3 级算作事件
	tf_uint64 nJamTime;                //拥堵时间(秒)           [out]
	tf_uint64 nJamTime1;               //上行拥堵时间(秒)           [out]
	tf_uint64 nJamTime2;               //下行拥堵时间(秒)           [out]
	int nJamLen;                    //拥堵长度(米)           [out]
	float foccupancy;               //总占有率
	float foccupancy1;              //上行占有率
	float foccupancy2;              //下行占有率

	int nUpRoadNum;                //上行车道数
	int nUpLinesNum;               //上行车道线数目
	int nUpMedianStripNum;         //上行隔离带数目
	int nUpBeginx[15];             //上行车道线起点x坐标
	int nUpBeginy[15];             //上行车道线起点y坐标
	int nUpEndx[15];               //上行车道线终点x坐标
	int nUpEndy[15];               //上行车道线终点y坐标

	int nDownRoadNum;              //下行车道数
	int nDownLinesNum;             //下行车道线数目
	int nDownMedianStripNum;       //下行隔离带数目
	int nDownBeginx[15];           //下行车道线起点x坐标
	int nDownBeginy[15];           //下行车道线起点y坐标
	int nDownEndx[15];             //下行车道线终点x坐标
	int nDownEndy[15];             //下行车道线终点y坐标
	
	int nVehicleFlowRoad[2][10];  //断面流量分车道数据 //[0] 上行数据  [1]下行数据
	int nBigFlowRoad[2][10];  	//大车断面流量分车道数据	
	int nSmallFlowRoad[2][10];  //小车车断面流量分车道数据
	
	float fVecRoad[2][10];	 //平均速度分车道数据
	float fVecBigRoad[2][10];  //大车平均速度分车道数据	
	float fVecSmallRoad[2][10];  //小车车平均速度分车道数据	
	float fRoadOccupancy[2][10];  //占有率分车道数据	
	int  nRodeJamLevRes[2][10];  //拥堵分车道数据

	bool beVideoJitter; //视频是否抖动
	bool beKeyFrame;   //是否关键帧，场景切换的标志
	int  nCameraLoc;   //相机安装位置 0道路左侧 1道路右侧 2道路中间

	ETF_SCENE_MODE eSceneMode;  //场景类型
	bool bDetectPedestrian;  //是否检测行人
	bool bDetectLandslideEtc;  //是否检测塌方、落石、抛洒物
	bool bEmergencyLine;/////是否开启应急车道占用事件
	bool bFanChe;////是否开启翻车事件
	float fNightStopCar;			//夜间停车阈值			[out]

	bool bRoadLocate ;////是否路面信息
	TF_Road_Locate  RoadLocateObj;

	bool bActive;  //是否展示

	int  nEventType;     //事件类型
	int  nRoadStatus;    //路面状态

	/////路面区域点个数
	int nRoadAreaPtNum;
	////区域,传入比例
	//std::vector<float> fAreaInfo;
	float fAreaInfo[TF_SP_MAX_POINT_NUM*2];

	/////应急车道信息
	int nEmergLineAreaNum;
	EmergLineAreaStruct EmergLineArea[5];

	/////车道信息
	TF_Road_Locate  LaneArea;

}CTFVATrafficInOutParam;

#endif

#endif
