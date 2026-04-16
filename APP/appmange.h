#ifndef  __appmanage_H
#define  __appmanage_H

#include <stdint.h>
#include "main.h"
#include "stdbool.h"


void PrintStart(void) ;
void PrintString(char *pString);

typedef struct {
    float Temperature;
    float Humidity;

} SensorADC;

typedef struct {
    u8 TimeMode: 1; 
//#define FixMode						0
//#define CurrentModePro 		1
    u8 CurrentModeState: 1; //模式选择 0：定制模式  1：程序模式
    u8 Cnd: 1; //控温方式 0：时间到进入下一段 1：温度到进入下一段
#define      EnRun         1
#define      DisRun        0
    u8 Flag_EnRun: 1;      //控制器运行停止标志位
	  u8 PowerOnRun :1;      //断电恢复功能
    u8 ColdState: 1;
	  u8 DoorRunControl:1;
	  u8 LightState :1;
    unsigned char EnTime: 1;
    unsigned char EnTimeEnd: 1;
} TypeCommon;


typedef struct {
    unsigned char bHeatOut: 1; //加热输出标志位
    unsigned char bDisIsum: 1; //禁止加热积分累积
    unsigned char bAlm:  1;  //温度报警标志位
    unsigned char bOver: 1;
    unsigned char bProtect: 1;
    unsigned char bDowm: 1;
    unsigned char bRun:  1;
    unsigned char RunSetBit: 1;
	  unsigned char bStable:1;
} TypeBit;


typedef struct {
    short T; //控制周期
    short P; //比例带
    short I; //积分时间
    short d; //微分时间
    short OutValue;
    int ISumVaule;
    float Target;
    float Measure;
    float Display;
    float LastMeasure;
    float DFVaule;
    float Fliter;
    float SetDiff;
	  unsigned char DiffStableSecond;
    unsigned char gama;
    unsigned char AR;
    unsigned char Time_HeatCycle;
    TypeBit Bit;
    unsigned short  Time_5min;
    unsigned short  Time_HeatCounter;
    void (*HeatCallback)(bool);
} PidParam;

typedef enum {
	  bNoError    = 0,
    bStateTempH = 1<<0,
	  bStateHumiH = 1<<1,
    bStateTempL = 1<<2,
	  bStateHumiL = 1<<3,
		bStateDoor  = 1<<4,
	  bStateWaterL = 1<<5,
		bStateWaterH = 1<<6,
} StateAlarmEnum;

typedef enum {
#define EVIdEnumCount  2
    TemperatureID = 0,
    HumidityID,
} EVIdEnum;

typedef enum {
#define 	EVEnumCount  4
    GetSensorValueID = 0,
    DisplaySensorValueID,
    TempParaUpdataID,
    HumiParaUpdataID,
	  TempStableID,
		HumiStableID,
} EVEnum;

typedef enum {
   FixedModeID=0, 
	 ProgramModeID
} RunMode;

typedef struct {
#define   TypeID       2
    PidParam  Parameter[TypeID];
    unsigned char   FanOutTime;
	  unsigned char   PrintLastSecond;
	  float SubTemperature;
	  float LstSubTemperature;
	  float SubHumidity;
	  float LstSubHumidity;
    unsigned int RunTime;
	  unsigned int SetRunTime;
	  uint16_t Compressor_Delay_time;
		uint16_t DeforstingmilliSecond;
	  uint16_t DeforstingIntervalSecond;
	  uint32_t PrintmilliSecond;
    TypeCommon	Flag;

			unsigned char bOnceCompressorStart:1;
		  unsigned char bDoorState:1;
} TypeCtrl;

extern TypeCtrl SensorType;

extern void Control(PidParam* PID_Parameter, TypeCommon *pTypeCommon);
#endif


