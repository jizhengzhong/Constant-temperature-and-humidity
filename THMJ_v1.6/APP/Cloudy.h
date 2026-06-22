#ifndef __CLOUDY_H__
#define __CLOUDY_H__
#include "stdint.h"





typedef union {
    unsigned short value;
    unsigned char  uBytes[2];
} BigLittleEnd;
typedef void (*CallSendback)(uint16_t addr,uint16_t data);
typedef struct{
	#define RegAddressHigh   2 
	#define RegAddressLow    3 
	#define DataAddressHigh  7 
	#define DataAddressLow   8
	#define CrcAddressHigh   9 
	#define CrcAddressLow    10 
	
	
///////////±ßÔµ²É¼¯/////////////
	#define EdgeCollAddrH   2
	#define EdgeCollAddrL   3
	#define EdgeCollCountH  4
	#define EdgeCollCountL  5
	#define EdgeCollBackCountL 6
	#define EdgeCollBackCountH 7
 int8_t SlaveAddr;
 int8_t Fun;
 int8_t RgeAddr_H;
 int8_t RegAddr_L;
 int8_t Leng_H;
 int8_t Leng_L;
 int8_t Data_H;
 int8_t Data_L;
 int8_t Crc_H;
 int8_t Crc_L;
}MB_StructDef;
typedef struct {
#define NMeasure    0
#define NCount      1
#define NWater      2
#define NPressur    3
#define NSetTemp    4
#define NSetTime    5
#define NSetKeepWarm   6
#define NKeepWarmTime  7
#define NLeftTime      8
#define NHeatState      9
#define NError         10	
#define NDeviceState   11
#define NEnd           NDeviceState+1
	
//#define LstMeasure    0
//#define LstCount      1
//#define LstWater      2
//#define LstPressur    3
//#define LstSetTemp    4
//#define LstSetTime    5
//#define LstSetKeepWarm   6
//#define LstKeepWarmTime  7
//#define LstLeftTime      8
//#define LstHeatTime      9
//#define LstError         10	
//#define LstEnd           LstError+1
//uint16_t LstUpdateValue[LstEnd];
	
MB_StructDef Frame;
CallSendback Updata;
}CloudyCtr;



#endif
