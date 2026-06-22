/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLOW_H__
#define __FLOW_H__


#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/


/* USER CODE BEGIN Includes */

  #include "main.h"
  #include <stdbool.h>
/* USER CODE END Includes */


/* USER CODE BEGIN Private defines */



/* USER CODE END Private defines */
	

	typedef enum{
		RelayOFF,
		RelayON
	}RelaySt;
	typedef struct {
    int16_t ExhuTemp;
    int16_t Sv;
    void ( *pCallback)(uint16_t temp ,uint8_t Step, void(*pRellayctrl)(RelaySt State));
} flow_run_para;
typedef enum{
		HeatOFF=0,
		HeatState1,
	  HeatState2,
	  BalanceState,
	  ExhaustState,
	  FinishState1,
	  KeepWarmState,
	  FinishState2 ,
	  MeltState,
		ExhaustState2,
	  PreHeatBlance,
		  DryBlance
	}CurveSt;

	//温度度标志位位段定义
typedef struct
{   
    u8 E1:1;  
    u8 E2:1; 
    u8 E3:1; 
  	u8 E4:1; 
    u8 E5:1;  
    u8 E8:1; 
    u8 E9:1;  
    u8 E11:1; 
	
    u8 E12:1;
	#define ERROR_FLAG_ENABLE					1
	#define NO_ERROR                  0
	  u8 ErrorFlag;		
}ErrorPara;

typedef struct 
{
    u8 ReachSetPoint:1;
	  u8 KeepWarmPoint:1; 
  	u8 TenSecondFlag:1;	
	  u8 MinuteFlag:1;
	  u8 DryStartFlag:1;

}StatePara;

typedef struct 
{
 
	u16 SteriTime;
	u8  SteriTemp;

}ProgramModePara;

typedef struct 
{
		#define WORK_PROGRAM_FLOW_STATE_STANDBY		0x01
		#define WORK_PROGRAM_FLOW_STATE_HEAT1		  0x02
		#define WORK_PROGRAM_FLOW_STATE_HEAT2		  0x03
		#define WORK_PROGRAM_FLOW_STATE_PreHeat		0x04
		#define WORK_PROGRAM_FLOW_STATE_STERI	    0x05
	
		#define WORK_PROGRAM_FLOW_STATE_DRAIN		  0x06 //升温
		#define WORK_PROGRAM_FLOW_STATE_DRY		    0x07

	
		#define WORK_PROGRAM_FLOW_STATE_EXHAUST		0x08
		#define WORK_PROGRAM_FLOW_STATE_FINISH1		0x09
		#define WORK_PROGRAM_FLOW_STATE_FINISH2		0x0A
		#define WORK_PROGRAM_FLOW_STATE_ERROR		  0x0B
//		#define WORK_PROGRAM_FLOW_STATE_RISE1		  0x11 //升温
//		#define WORK_PROGRAM_FLOW_STATE_RISE2		  0x12 //升温	
    unsigned int SteriMinute;
	  unsigned int SteriCount;
	  unsigned int RunTime;
	  unsigned char LastSecond;
	  unsigned char TenSecond;

		unsigned char flow_state;
	  CurveSt CurveState;
	  StatePara StateParameter;
		unsigned char SteriSecond;
	  unsigned short int FanOutMs;
	  unsigned short int Pressure;
		unsigned short int LastPressure;
	/* 锅内压力 */

//	float DisplayPressure;    
	float DisplayWater;
	float DisplaySteam;
	float DisplayObject;

	#define FLOW_STATE_HEAT2_1					1
	#define FLOW_STATE_HEAT2_2					2
	#define FLOW_STATE_HEAT2_3					3
	#define FLOW_STATE_HEAT2_4					4
	#define FLOW_STATE_HEAT2_5					5
	#define FLOW_STATE_HEAT2_6					6
	#define FLOW_STATE_HEAT2_7					7
	#define FLOW_STATE_HEAT2_C1					5
	#define FLOW_STATE_HEAT2_C2					6
	#define FLOW_STATE_HEAT2_C3					7

	#define STATE_BALANCE_0				0
	#define FLOW_STATE_EXHAUST_1				1
	#define FLOW_STATE_EXHAUST_2				2
	#define FLOW_STATE_EXHAUST_3				3
	#define FLOW_STATE_EXHAUST_4				4

 unsigned char state;
	#define SENSOR_OBJECT_USED					0xaa
	unsigned char object_used;
  unsigned char BalanceFlag ;
	#define CHECK_LEVER_LOCK_FLAG				0x55
	unsigned char leverlock;

	float f0_value;//F0值
	ErrorPara Err;
} manager;

typedef  void (*EventFunEntry)( unsigned char evenId,manager *pFlowPcb);
typedef enum
{
	EV_StopState=0,
  EV_E1ALARM,
	EV_E2ALARM,
  EV_E3ALARM,
	EV_E4ALARM,
  EV_E5ALARM,
	EV_E8ALARM,
  EV_E9ALARM,
	EV_E11ALARM,
  EV_E12ALARM,
} EVId;
typedef struct
{

	EVId  EventId;				/* 功能ID */
	EventFunEntry ActFun;		/* 功能函数 */
} T_ACT_FUN;
extern manager RunFlow;
extern void RellayCtrl(RelaySt State);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

