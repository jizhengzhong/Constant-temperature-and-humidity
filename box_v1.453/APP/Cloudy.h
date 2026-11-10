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
#define NTempMeasure    0
#define NHumiMeasure    1
#define NSetTemp        2
#define NSetHumi        3
#define NRunTime        4
#define NSetTime        5
#define NSubTemp        6
#define NSubHumi        7
#define NLevWater       8
#define NHeatPower      9
#define NError         10	
#define NHumiPower     11
#define RunType         NHumiPower+1
#define NEnd           RunType+1

uint16_t Second;
MB_StructDef Frame;
CallSendback Updata;
}CloudyCtr;

extern CloudyCtr Cloudy;
 void ReceiveProcess(void);
uint8_t ParaUpdataCheck(void);
#endif
