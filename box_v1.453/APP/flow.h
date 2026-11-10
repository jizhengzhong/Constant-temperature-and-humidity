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
typedef enum{ 
       Deforst0  = 0,
	     Deforst1,
	     Deforst2,
	     Deforst3,
	     Deforst4,
	     ProhibitedDeforst
      } DeforstID;

typedef struct
{
  int16_t Temp;
	uint16_t  Interval;
	uint16_t  Time;
} Defrost;
typedef  void (*pFlowCallback)(Defrost* DefrostStruct);
typedef  void (*pFlowEventFunEntry)(DeforstID EventId,uint16_t * pTab,pFlowCallback pFUN);
typedef struct {
    DeforstID  EventId;				/* 功能ID */
    pFlowEventFunEntry pFun;		/* 功能函数 */
} FlowFunStructDef;




/* USER CODE END Private defines */
	

	typedef enum{
		RelayOFF,
		RelayON
	}RelaySt;


extern void RellayCtrl(RelaySt State);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

