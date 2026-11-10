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
#ifndef __ADCSample_H__
#define __ADCSample_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define MAX_ANALOG_SAMPLING_NUMBER 10
	
#define 	PT_B1A1     0
#define 	PT_B2A2     1
//#define 	PT_B3A3     2
#define 	Hum_ADC     2
#define   ADC_Sense   3
#define 	Filter_Channel  ADC_Sense+1


/* USER CODE END Private defines */


#define   Heat_OFF      0
#define 	Heat_ON       1



/* USER CODE BEGIN Prototypes */


//typedef struct
//{
//    s16 AL;//超温偏差报警
//    s16 HL;//电磁阀偏差报警
//    s16 T; //控制周期
//    s16 P; //比例带
//    s16 I; //积分时间
//    s16 d; //微分时间
//    s16 Pb1;//第一路热敏电阻零位调整
//    s16 PK1;//满度调整
//	  s16 OutValue;
//	  s32 ISumVaule;
//	  float Target;
//	  float Measure;
//	  float LastMeasure;
//	  float DFVaule;
//	  u8 gama;
//   	u8 AR;
//	  u8 Flag_DisIsum:1;  //禁止加热积分累积 
//    u8 Time_HeatCycle	;
//	  u16 Time_HeatCounter;
//	  void (*HeatCallback)( u8);  
//}
//PID_Para;
////内部参数表二
//typedef struct
//{
////    s16 Pon;//断电恢复功能
//    s16 nod;//模式选择 0：禁止定时 1：允许定时 2：程序模式
//    s16 Cnd;//控温方式 0：时间到进入下一段 1：温度到进入下一段
////    s16 ndt;//定时模式选择
////    s16 Hn; //计时模式
////    s16 dE; //恒温计时偏差值
////    s16 rT; //总定时时间修正
////    s16 SPL;//量程下限
////    s16 SPH;//量程上限
////    s16 EP2;//是否启用温度保护功能
////    s16 SP2;//温度保护停止加热临界值
//}
//InterParaTwo;


//extern InterParaTwo  Hum_Ctrl;


//extern float Temp_Set;      //温度设定值

//extern PID_Para Temperature;	
//extern PID_Para Humidity;
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

