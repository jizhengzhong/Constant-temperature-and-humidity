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
#ifndef __IO_MANAGE_H__
#define __IO_MANAGE_H__


#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/


/* USER CODE BEGIN Includes */

  #include "main.h"
/* USER CODE END Includes */


/* USER CODE BEGIN Private defines */

#define IN_EN_ST           0

/* USER CODE END Private defines */
	

	
    typedef union {
        uint16_t St;
        uint8_t Stv[2];

        struct {
            uint16_t in_YL : 1;
            uint16_t bit1 : 1;
            uint16_t in_LOCK_SW : 1;
            uint16_t bit3 : 1;
            uint16_t in_DOOR_SW : 1;
            uint16_t bit5 : 1;
            uint16_t in_CO2TANK1 : 1;
            uint16_t bit7 : 1;
            uint16_t in_CO2TANK2 : 1;
            uint16_t bit9 : 1;
            uint16_t bit10 : 1;
            uint16_t bit11 : 1;
            uint16_t bit12 : 1;
            uint16_t bit13 : 1;
            uint16_t bit14 : 1;
            uint16_t bit15 : 1;
        } bits;
    } sys_in_type;	

		    typedef union {
        uint16_t St;
        uint8_t Stv[2];

        struct {
            uint16_t out_ALARM : 1;  //报警
            uint16_t out_JSF_H : 1;  //进水阀
            uint16_t out_JQB_H : 1;  //进气泵
            uint16_t out_PSF_H : 1;  //排水阀
            uint16_t out_CKF_H : 1;  //常闭阀
            uint16_t out_FS_H : 1;   //风扇
            uint16_t out_LOCK_H : 1; //锁
            uint16_t out_CBF_H : 1;  //常闭阀
            uint16_t out_ZJR_H : 1;  //主加热
            uint16_t out_GZJR_H : 1; //干燥加热
            uint16_t bit10 : 1;  //
            uint16_t bit11 : 1;
            uint16_t bit12 : 1;
            uint16_t bit13 : 1;
            uint16_t bit14 : 1;
            uint16_t bit15 : 1;
        } bits;
    } sys_out_type;	
				
		
		    typedef struct {
        //数字输入
        sys_in_type InPut;
        sys_in_type InPutList[4];
        sys_in_type LostEdge;
        sys_in_type FoundEdge;
        sys_out_type OutPut;
        sys_out_type OutPutOld;
        //AD转换数值获取
        uint16_t Pt100AdValList[5]; //pt 测量AD
        uint16_t Pt100AdValIndex;
        uint16_t Pt100AdVal;
        uint16_t TcdMainAdValList[5]; //TCD 主测量
        uint16_t TcdMainAdValIndex;
        uint16_t TcdMainAdVal;
        uint16_t DigHumiAdValList[5]; //数字湿度
        uint16_t DigHumiAdValIndex;
        uint16_t DigHumiAdVal;
        uint16_t DigTempAdValList[5]; //数字温度
        uint16_t DigTempAdValIndex;
        uint16_t DigTempAdVal;
        uint16_t TcdMeasureAdValList[5]; //TCD 测量臂
        uint16_t TcdMeasureAdValIndex;
        uint16_t TcdMeasureAdVal;
        uint16_t TcdRefAdValList[5]; //TCD参考臂
        uint16_t TcdRefAdValIndex;
        uint16_t TcdRefAdVal;
        
        //模拟输入
        int32_t iTempMain;
        float fTempMain;
        int32_t iTempDoor;
        float fTempDoor;
        int32_t iCol;
        float fCol;
        float fHumi;
        
        float SHT85_fHumi;
        float SHT85_fTemp;
        
        //模拟输出
        int32_t AdjTempOutDuty;
        int32_t AdjColOutDuty;
        int32_t AdjTempPeriod;
        int32_t AdjColPeriod;
        int32_t AdjTempOutCnt;
        int32_t AdjColOutCnt;
    } sys_io_type;
			

	
		
    typedef struct {
        uint8_t ErrSt[4][16];
        uint8_t ErrStLast[4][16];
        uint8_t err_global;
        
        uint8_t sd_cd_st; //sd 插卡状态
        uint8_t sd_state; //sd 初始化完成
        
        uint8_t bLoginSt;
        uint32_t PowerUpTimeCnt; //单位s
        sys_io_type Io;

        //控制设定
        uint8_t bTempRun;
        int16_t TempMainSet;

        
        //运行控制
        uint8_t bSysRun;
        uint32_t TimeSet;
        uint32_t TimeCnt;
        
        //灭菌控制
        uint8_t bMjRun;
        uint32_t MjTimeSet;
//        uint32_t MjTimeCnt;
        //控制值 / 显示值
        int16_t TempMainDisp;           
        
    } sys_run_st_type;	


#ifdef __cplusplus
}
#endif

#endif 

