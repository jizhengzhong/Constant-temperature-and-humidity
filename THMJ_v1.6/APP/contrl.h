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
#ifndef __CONTRL_H__
#define __CONTRL_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "ADCSample.h"
#include "flow.h"

/* USER CODE BEGIN Includes */


/* USER CODE END Includes */


/* USER CODE BEGIN Private defines */



/* USER CODE END Private defines */


	
	
#define MAX_WORK_PROGRAM_COUNT				6
#define PROGRAM_TYPE_COUNT					  1


#define PAR_VERSION_ID      0x555C
	
#define PAR_POS_VER_ID       0                                                   //0
#define PAR_POS_UPDATE_TIME           (PAR_POS_VER_ID+1)                         //1
#define PAR_POS_UPDATE                (PAR_POS_UPDATE_TIME+1)                    //2
#define PAR_POS_Run_Stop_Command   	        (PAR_POS_UPDATE+1)                   //3
                                                                                 
#define PAR_POS_CUSTOM_TEMP_SET_STERI     	(PAR_POS_Run_Stop_Command+1)         //4
#define PAR_POS_CUSTOM_TIME_SET_STERI     	(PAR_POS_CUSTOM_TEMP_SET_STERI+1)    //5
#define PAR_POS_CUSTOM_EXHAUST_SET        	(PAR_POS_CUSTOM_TIME_SET_STERI+1)    //6
#define PAR_POS_CUSTOM_EXHAUST_MODE_SET   	(PAR_POS_CUSTOM_EXHAUST_SET+1)       //7

#define PAR_POS_CUSTOM_BLANCE_TIME_SET    	(PAR_POS_CUSTOM_EXHAUST_MODE_SET+1)  //8
#define PAR_POS_CUSTOM_DRY_TEMP_SET       	(PAR_POS_CUSTOM_BLANCE_TIME_SET+1)   //9
#define PAR_POS_CUSTOM_DRY_TIME_SET       	(PAR_POS_CUSTOM_DRY_TEMP_SET+1)      //10

#define PAR_POS_LIQUID_STERI_TEMP_SET     	(PAR_POS_CUSTOM_DRY_TIME_SET+1)      //11
#define PAR_POS_LIQUID_STERI_TIME_SET     	(PAR_POS_LIQUID_STERI_TEMP_SET+1)    //12
#define PAR_POS_LIQUID_KEEP_WARM_TEMP_SET   (PAR_POS_LIQUID_STERI_TIME_SET+1)    //13
#define PAR_POS_LIQUID_KEEP_WARM_TIME_SET   (PAR_POS_LIQUID_KEEP_WARM_TEMP_SET+1)//14
#define PAR_POS_LIQUID_BLANCE_TIME_SET      (PAR_POS_LIQUID_KEEP_WARM_TIME_SET+1)//15

#define PAR_POS_MELT_TEMP_SET               (PAR_POS_LIQUID_BLANCE_TIME_SET+1)   //16
#define PAR_POS_MELT_TIME_SET               (PAR_POS_MELT_TEMP_SET+1)            //17
#define PAR_POS_MELT_KEEP_WARM_TEMP_SET     (PAR_POS_MELT_TIME_SET+1)            //18
#define PAR_POS_MELT_KEEP_WARM_TIME_SET     (PAR_POS_MELT_KEEP_WARM_TEMP_SET+1)  //19


#define PAR_POS_SOLID_STERI_TEMP_SET        (PAR_POS_MELT_KEEP_WARM_TIME_SET+1)  //20
#define PAR_POS_SOLID_STERI_TIME_SET        (PAR_POS_SOLID_STERI_TEMP_SET+1)     //21
#define PAR_POS_SOLID_EXHAUST_TEMP_SET      (PAR_POS_SOLID_STERI_TIME_SET+1)     //22
#define PAR_POS_SOLID_EXHAUST_MODE_SET      (PAR_POS_SOLID_EXHAUST_TEMP_SET+1)   //23
#define PAR_POS_SOLID_DRY_TEMP_SET          (PAR_POS_SOLID_EXHAUST_MODE_SET+1)   //24
#define PAR_POS_SOLID_DRY_TIME_SET          (PAR_POS_SOLID_DRY_TEMP_SET+1)       //25

#define PAR_POS_FABRIC_STERI_TEMP_SET       (PAR_POS_SOLID_DRY_TIME_SET+1)       //26
#define PAR_POS_FABRIC_STERI_TIME_SET       (PAR_POS_FABRIC_STERI_TEMP_SET+1)    //27
#define PAR_POS_FABRIC_EXHAUST_TEMP_SET     (PAR_POS_FABRIC_STERI_TIME_SET+1)    //28
#define PAR_POS_FABRIC_EXHAUST_MODE_SET     (PAR_POS_FABRIC_EXHAUST_TEMP_SET+1)  //29
#define PAR_POS_FABRIC_DRY_TEMP_SET         (PAR_POS_FABRIC_EXHAUST_MODE_SET+1)  //30
#define PAR_POS_FABRIC_DRY_TIME_SET         (PAR_POS_FABRIC_DRY_TEMP_SET+1)      //31
#define PAR_POS_FABRIC_BLANCE_TIME_SET      (PAR_POS_FABRIC_DRY_TIME_SET+1)      //32

#define PAR_POS_RUN_PROGRAM_TYPE    		    (PAR_POS_FABRIC_BLANCE_TIME_SET+1)   //33

#define PAR_POS_OBJECT_SENSOR_CALIBRATION   (PAR_POS_RUN_PROGRAM_TYPE+1)         //34
#define PAR_POS_STEAM_SENSOR_CALIBRATION    (PAR_POS_OBJECT_SENSOR_CALIBRATION+1) //35
#define PAR_POS_PRESSURE_SENSOR_CALIBRATION (PAR_POS_STEAM_SENSOR_CALIBRATION+1)  //36
#define PAR_POS_WATER_TEMP_DISPLAY          (PAR_POS_PRESSURE_SENSOR_CALIBRATION+1) //37

#define PAR_POS_ALTITUDE_SET                (PAR_POS_WATER_TEMP_DISPLAY+1)    //38
#define PAR_POS_TEMP_BOIL                   (PAR_POS_WATER_TEMP_DISPLAY+1)    //38

#define PAR_POS_TEMP_ERROR_E11              (PAR_POS_TEMP_BOIL+1)                 //39
#define PAR_POS_STERI_COUNT_CORRECT   		  (PAR_POS_TEMP_ERROR_E11+1)            //40

#define DRY_FUNCTION_BIT0                    0
#define OBJECT_FUNCTION_BIT1                 1

#define PAR_POS_FUNCTION_SET    		        (PAR_POS_STERI_COUNT_CORRECT+1)       //41
#define PAR_POS_P_SET  		                  (PAR_POS_FUNCTION_SET+1)          //42
#define PAR_POS_I_SET  	                    (PAR_POS_P_SET+1)                     //43
#define PAR_POS_D_SET  	                    (PAR_POS_I_SET+1)                     //44
#define PAR_POS_AR_SET                      (PAR_POS_D_SET+1)                     //45
#define PAR_POS_T_SET 	                    (PAR_POS_AR_SET+1)                    //46
#define PAR_POS_DRY_AR 	                    (PAR_POS_T_SET+1)                     //47
#define PAR_POS_DRY_T 	                    (PAR_POS_DRY_AR+1)                    //48
#define PAR_POS_DRY_P 	                    (PAR_POS_DRY_T+1)                     //49
#define PAR_POS_DRY_I	                      (PAR_POS_DRY_P+1)                     //50
#define PAR_POS_DRY_D	                      (PAR_POS_DRY_I+1)                     //51
#define PAR_POS_WATER_LEVEL_CHECK	          (PAR_POS_DRY_D+1)                     //52
#define PAR_POS_WATER_F0_SWITCH	            (PAR_POS_WATER_LEVEL_CHECK+1)         //53
#define PAR_POS_UNLOCK_TEMP	                (PAR_POS_WATER_F0_SWITCH+1)           //54
#define PAR_POS_WATER_SENSOR_CALIBRATION    (PAR_POS_UNLOCK_TEMP+1)               //55
#define PAR_POS_PRE_HEAT_TEMP               (PAR_POS_WATER_SENSOR_CALIBRATION+1)  //56
#define PAR_POS_PRE_HEAT_TIME               (PAR_POS_PRE_HEAT_TEMP+1)             //57
#define PAR_POS_PRO_CHOICE_CONFIRM          (PAR_POS_PRE_HEAT_TIME+1)             //58
#define PAR_POS_FILTER                		  (PAR_POS_PRO_CHOICE_CONFIRM+1)        		//59

#define PAR_POS_END                         (PAR_POS_FILTER+1)        //59


#define PAR_POS_CURRENT_YEAR				        0     									   						//0
#define PAR_POS_CURRENT_MONTH               (PAR_POS_CURRENT_YEAR+1)              //1
#define PAR_POS_CURRENT_DAY                 (PAR_POS_CURRENT_MONTH+1)             //2
#define PAR_POS_CURRENT_HOUR                (PAR_POS_CURRENT_DAY+1)               //3
#define PAR_POS_CURRENT_MINUTE              (PAR_POS_CURRENT_HOUR+1)              //4
#define PAR_POS_CURRENT_SECOND              (PAR_POS_CURRENT_MINUTE+1)            //5
#define PAR_POS_FAN_AR                 		  (PAR_POS_CURRENT_SECOND+1)        		//6 

/* USER CODE BEGIN Prototypes */

#define  DoorOpen_State      1 
#define  DoorClose_State     0
#define  Unlock_State        1 
#define  Lock_State          0


#define AirAdmissionOFF 		 	   HAL_GPIO_WritePin(AirAdmissionCtrl_GPIO_Port, AirAdmissionCtrl_Pin, GPIO_PIN_RESET)
#define AirAdmissionON	  	   HAL_GPIO_WritePin(AirAdmissionCtrl_GPIO_Port, AirAdmissionCtrl_Pin, GPIO_PIN_SET)
#define HEAT_OFF 		  		 HAL_GPIO_WritePin(GPIOC, Heat_Ctrl_Pin, GPIO_PIN_RESET)
#define HEAT_ON 	       	 HAL_GPIO_WritePin(GPIOC, Heat_Ctrl_Pin, GPIO_PIN_SET)
#define Dry_OFF         	 HAL_GPIO_WritePin(Dry_Ctrl_GPIO_Port, Dry_Ctrl_Pin, GPIO_PIN_RESET)
#define Dry_ON             HAL_GPIO_WritePin(Dry_Ctrl_GPIO_Port, Dry_Ctrl_Pin, GPIO_PIN_SET)

#define CFK_POWER_ON        HAL_GPIO_WritePin(Ctr_CKF_GPIO_Port,Ctr_CKF_Pin, GPIO_PIN_SET)
#define CFK_POWER_OFF       HAL_GPIO_WritePin(Ctr_CKF_GPIO_Port,Ctr_CKF_Pin, GPIO_PIN_RESET)

#define DrainON           HAL_GPIO_WritePin(Water_Output_Ctrl_GPIO_Port,Water_Output_Ctrl_Pin, GPIO_PIN_SET)
#define DrainOFF          HAL_GPIO_WritePin(Water_Output_Ctrl_GPIO_Port,Water_Output_Ctrl_Pin, GPIO_PIN_RESET)

#define High_Level_Check    HAL_GPIO_ReadPin(Water_High_Level_Check_GPIO_Port,Water_High_Level_Check_Pin)
#define Low_Level_Check     HAL_GPIO_ReadPin(Water_Low_Level_Check_GPIO_Port,Water_Low_Level_Check_Pin)

#define Lock_State_Check    HAL_GPIO_ReadPin(Lock_State_GPIO_Port,Lock_State_Pin)
#define Door_State_Check    HAL_GPIO_ReadPin(Door_State_GPIO_Port,Door_State_Pin)
#define Dry_Protect_Check   HAL_GPIO_ReadPin(Dry_Protect_State_GPIO_Port,Dry_Protect_State_Pin)

#define Fan_OFF             HAL_GPIO_WritePin(Ctr_FAN_GPIO_Port, Ctr_FAN_Pin, GPIO_PIN_RESET)
#define Fan_ON             HAL_GPIO_WritePin(Ctr_FAN_GPIO_Port, Ctr_FAN_Pin, GPIO_PIN_SET)

#define Lock_PowerON        HAL_GPIO_WritePin(Ctr_Lock_GPIO_Port, Ctr_Lock_Pin, GPIO_PIN_SET)
#define Lock_PowerOFF       HAL_GPIO_WritePin(Ctr_Lock_GPIO_Port, Ctr_Lock_Pin, GPIO_PIN_RESET)

#define ALM_OFF    			  HAL_GPIO_WritePin(Ctr_Beep_GPIO_Port, Ctr_Beep_Pin, GPIO_PIN_SET)
#define ALM_ON     			  HAL_GPIO_WritePin(Ctr_Beep_GPIO_Port, Ctr_Beep_Pin, GPIO_PIN_RESET)

typedef union {
    uint16_t uVal;
    uint16_t iVal;
    uint8_t  uBytes[2];
    uint8_t  iBytes[2];
} partab_type;

extern partab_type ParTab[PAR_POS_END + 2]; //工作参数
extern partab_type ParReceive[PAR_POS_END + 2]; //接收参数
extern partab_type ParSaved[PAR_POS_END + 2]; //接收已存储的参数
extern partab_type ParUnSaved[PAR_POS_FAN_AR+2];
//extern  uint8_t    ParReadSt[PAR_POS_END + 2]; //指示上位机已读该参数

//温度度标志位位段定义
typedef struct
{   
    u8 Flag_Alm:1;  //温度报警标志位
    u8 Flag_Down:1;  //关断加热标志
    u8 Flag_Over:1; //温度溢出标志位 
  	u8 Flag_SteriRunTime:1; //灭菌状态
    
    u8 Flag_HeatOut:1;  //加热输出标志位 
    u8 Flag_Protect:1;  //温度保护标志，1:停止加热 0:允许加热
    u8 Flag_DisIsum:1;  //禁止加热积分累积  
}
Para_Flag_STRUCT;


//控制器器标志位位段定义
typedef struct
{ 
#define      EnRun         1
#define      DisRun        0  
    u8 Flag_EnRun;       //控制器运行停止标志位
    u8 Flag_TempRun:1;   //控制器停止温度控制标志位
	  StatePara Flag;

}
CONTROL_STRUCT;

extern CONTROL_STRUCT CONTROL;//控制器位段变量  
typedef union 
{
	unsigned char keepwarm;
	unsigned char dry_temp;
} keepwarm_or_drytemp;

typedef union 
{
	unsigned short int keepwarm;
	unsigned short int dry_temp;
} keepwarm_or_drytemp_time;

typedef enum
{
  Custom=0,
	Liquid,
	Melt,
	Solid,
	Fabric,
	PreHeat
}TypePara;

typedef struct 
{
  TypePara type  ;
	unsigned char SetPoint;
  unsigned char SteriTimes;
 	unsigned char steri;					  // 灭菌温度
	keepwarm_or_drytemp mode;					  // 保温温度
	unsigned char exhaust;				  // 排气温度
	unsigned char exhmode;          // 排气模式
	unsigned char balance_time;     
	unsigned short int steri_time;		    // 灭菌时间
  keepwarm_or_drytemp_time dry_kwarm_time;		    // 保温时间
	
}parameter;

typedef struct {
    uint16_t attr;
    int16_t  min;
    int16_t max;
    int16_t init;
    uint8_t code[20];
} partab_attr_type;



extern parameter Procedure;
extern Para_Flag_STRUCT TEMP;
extern Para_Flag_STRUCT Hum;

extern u16 TimMs_ModbusWait ; //通讯间隔时间
extern u16 Time_Base1ms     ; //1毫秒时基
extern u16 Time_Base1min    ; //1分钟时基
extern u16 Time_5min        ; //5分钟计时
extern u8  Time_PowerInital ; //上电初始化计时


extern u16 Time_DisIsum     ; //加热积分禁止计时
extern u32 Time_runsec      ; //编程模式运行时间秒
extern u16 Time_runmin      ; //编程模式运行时间分
extern u16 Time_Out;          //通讯超时计时
extern u16 Time_TimeMill    ; //设定时间毫秒计时
extern u16 Time_TimeMinute  ; //设定时间秒钟计时
extern s16 Temp_SetDiff;//与上一次设定值差值



void Alarm(void);
void HeatControl(PID_Para *PID_Parameter,InterParaTwo *ParaCtrl,
								 Para_Flag_STRUCT *Para_Flag_Structment);
void Temp_Hea_State(u8 state);
void Hum_Hea_State(u8 state);
void Power_Save(void);

void Fan_Ctrl(void);	

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

