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





#define PAR_VERSION_ID                          0x12C0

#define PAR_POS_VER_ID      								   0                                   //0  版本
#define PAR_POS_UPDATE_TIME           			  (PAR_POS_VER_ID+1)                   //1  更新时间
#define PAR_POS_UPDATE                			  (PAR_POS_UPDATE_TIME+1)              //2  更新程序
#define PAR_POS_Run_Stop_Command   	   			  (PAR_POS_UPDATE+1)                   //3                                                                             
#define PAR_POS_TEMP_SV     								  (PAR_POS_Run_Stop_Command+1)         //4
#define PAR_POS_HUMI_SV             				  (PAR_POS_TEMP_SV+1)   				       //5
#define PAR_POS_TEMP_STA_SECOND     				  (PAR_POS_HUMI_SV+1)                  //6  暂时未用
#define PAR_POS_HUMI_STA_SECOND             	(PAR_POS_TEMP_STA_SECOND+1)   				   //7  暂时未用
#define PAR_POS_RUN_TIME_SET         			    (PAR_POS_HUMI_STA_SECOND+1)              //8
#define PAR_POS_TEMP_Pb         			        (PAR_POS_RUN_TIME_SET+1)             //9
#define PAR_POS_TEMP_Pk         			        (PAR_POS_TEMP_Pb+1)                  //10
#define PAR_POS_HUMI_Pb         			        (PAR_POS_TEMP_Pk+1)                  //11
#define PAR_POS_HUMI_Pk         			        (PAR_POS_HUMI_Pb+1)                  //12
#define PAR_POS_TEMP_AL         			        (PAR_POS_HUMI_Pk+1)                  //13
#define PAR_POS_TEMP_AH         			        (PAR_POS_TEMP_AL+1)                  //14
#define PAR_POS_TEMP_FILTER         			    (PAR_POS_TEMP_AH+1)                  //15
#define PAR_POS_HUMI_AL         			        (PAR_POS_TEMP_FILTER+1)              //16
#define PAR_POS_HUMI_AH         			        (PAR_POS_HUMI_AL+1)                  //17
#define PAR_POS_HUMI_FILTER         			    (PAR_POS_HUMI_AH+1)                  //18
#define PAR_POS_HUMI_DIFF_COUNT         		  (PAR_POS_HUMI_FILTER+1)              //19
#define PAR_POS_TEMP_DIFF_COUNT         		 (PAR_POS_HUMI_DIFF_COUNT+1)          //20
#define PAR_POS_TEMP_P_SET  		             (PAR_POS_TEMP_DIFF_COUNT+1)           //21
#define PAR_POS_TEMP_I_SET  	               (PAR_POS_TEMP_P_SET+1)                //22
#define PAR_POS_TEMP_D_SET  	               (PAR_POS_TEMP_I_SET+1)                //23
#define PAR_POS_TEMP_AR_SET                  (PAR_POS_TEMP_D_SET+1)                //24
#define PAR_POS_TEMP_T_SET 	                 (PAR_POS_TEMP_AR_SET+1)               //25
#define PAR_POS_TEMP_GAMA_SET 	             (PAR_POS_TEMP_T_SET+1)                //26
#define PAR_POS_HUMI_AR 	                   (PAR_POS_TEMP_GAMA_SET+1)             //27
#define PAR_POS_HUMI_T 	                      (PAR_POS_HUMI_AR+1)                   //28
#define PAR_POS_HUMI_P 	                      (PAR_POS_HUMI_T+1)                    //29
#define PAR_POS_HUMI_I	                      (PAR_POS_HUMI_P+1)                    //30
#define PAR_POS_HUMI_D	                      (PAR_POS_HUMI_I+1)                    //31
#define PAR_POS_HUMI_GAMA	                    (PAR_POS_HUMI_D+1)                    //32
#define PAR_POS_RUN_CHOICE_MODE               (PAR_POS_HUMI_GAMA+1)                 //33  未使用
#define PAR_POS_HUMI_SENSOR_LOW_VOLTAGE       (PAR_POS_RUN_CHOICE_MODE+1)           //34
#define PAR_POS_HUMI_SENSOR_HIGH_VOLTAGE      (PAR_POS_HUMI_SENSOR_LOW_VOLTAGE+1)   //35

#define bCtrol_Time_Mode       				 0  //bit0 :断电恢复 
#define bCtrol_Run_Mode        				 1  //bit1:运行模式选择（定制/程式模式）
#define bCtrol_Temp_Mode       				 2  //bit2:温度控制位
#define bCtrol_Run_Stop_Mode   				 3  //bit:3:运行停止控制位
#define bCtrol_Power_On_Run_Mode       4  //bit4：排水模式控制位
#define bCtrol_Compress_Mode    			 5  //bit5:压缩机控制位 
#define bCtrol_Door_Check_Mode 				 6  //bit6:门检测控制位
#define bCtrol_Light           				 7  //bit7:照明灯控制位
#define bCtrol_Multi_Cali_Humi 				 8  //bit8:开启湿度多点校准
#define PAR_POS_bCTROL              	 			  (PAR_POS_HUMI_SENSOR_HIGH_VOLTAGE+1)  //36
#define PAR_POS_CMPRSSR_ON_TEMP_LOW           (PAR_POS_bCTROL+1)							//37
#define PAR_POS_CMPRSSR_ON_PROHIBITED           (PAR_POS_CMPRSSR_ON_TEMP_LOW+1)				//38  J禁止压缩机启动
#define PAR_POS_CMPRSSR_ON_HUMI_L             (PAR_POS_CMPRSSR_ON_PROHIBITED+1)				//39
#define PAR_POS_CMPRSSR_OFF_TEMP              (PAR_POS_CMPRSSR_ON_HUMI_L+1)				//40
#define PAR_POS_CMPRSSR_ON_DELAY              (PAR_POS_CMPRSSR_OFF_TEMP+1)  			//41
#define PAR_POS_ADD_WATER_DELAY               (PAR_POS_CMPRSSR_ON_DELAY+1)      			//42
#define PAR_POS_ADD_WATER_SET_RELAY           (PAR_POS_ADD_WATER_DELAY+1)      			//43

#define PAR_POS_ALMAb0                        (PAR_POS_ADD_WATER_SET_RELAY+1)      			//44
#define PAR_POS_TEMP_Pb1         			        (PAR_POS_ALMAb0+1)               //45
#define PAR_POS_TEMP_Pk1         			        (PAR_POS_TEMP_Pb1+1)                      //46
#define PAR_POS_HUMI_Pb1         			        (PAR_POS_TEMP_Pk1+1)                      //47
#define PAR_POS_HUMI_Pk1         			        (PAR_POS_HUMI_Pb1+1)                      //48

#define PAR_POS_PRO_RUN_ROW                    (PAR_POS_HUMI_Pk1+1)                     //49
#define PAR_POS_PRO_Set_Circle_Count          (PAR_POS_PRO_RUN_ROW+1)                   //50
//#define PAR_POS_TEMP_PROGRAM_MODE0            (PAR_POS_PRO_Set_Circle_Count+1) 					//51

//#define PAR_POS_HUMI_PROGRAM_MODE0            (PAR_POS_TEMP_PROGRAM_MODE0+1)    		    //52
#define PAR_POS_TIME_PROGRAM_MODE0             53
#define PAR_POS_PRO_INDEX                     (PAR_POS_TIME_PROGRAM_MODE0+1)             //54
#define PAR_POS_PRINT_INTERVAL                (PAR_POS_PRO_INDEX  +1)        //55


#define PAR_POS_PRO_TEMP_DISP1                 (PAR_POS_PRINT_INTERVAL+1)                 //56
#define PAR_POS_PRO_TEMP_DISP2                 (PAR_POS_PRO_TEMP_DISP1+1)              //57
#define PAR_POS_PRO_TEMP_DISP3                 (PAR_POS_PRO_TEMP_DISP2+1)              //58
#define PAR_POS_PRO_TEMP_DISP4                 (PAR_POS_PRO_TEMP_DISP3+1)              //59
#define PAR_POS_PRO_TEMP_DISP5                 (PAR_POS_PRO_TEMP_DISP4+1) 
#define PAR_POS_PRO_TEMP_DISP6                 (PAR_POS_PRO_TEMP_DISP5+1) 
#define PAR_POS_PRO_TEMP_DISP7                 (PAR_POS_PRO_TEMP_DISP6+1) 
#define PAR_POS_PRO_TEMP_DISP8                 (PAR_POS_PRO_TEMP_DISP7+1) 
#define PAR_POS_PRO_TEMP_DISP9                 (PAR_POS_PRO_TEMP_DISP8+1) 
#define PAR_POS_PRO_TEMP_DISP10                (PAR_POS_PRO_TEMP_DISP9+1) 
#define PAR_POS_PRO_TEMP_DISP11                (PAR_POS_PRO_TEMP_DISP10+1) 
#define PAR_POS_PRO_TEMP_DISP12                (PAR_POS_PRO_TEMP_DISP11+1)    //67

#define PAR_POS_PRO_TEMP_WISH1                 (PAR_POS_PRO_TEMP_DISP12+1)   //68
#define PAR_POS_PRO_TEMP_WISH2                 (PAR_POS_PRO_TEMP_WISH1+1)    //69
#define PAR_POS_PRO_TEMP_WISH3                 (PAR_POS_PRO_TEMP_WISH2+1)    //70
#define PAR_POS_PRO_TEMP_WISH4                 (PAR_POS_PRO_TEMP_WISH3+1)    //71
#define PAR_POS_PRO_TEMP_WISH5                 (PAR_POS_PRO_TEMP_WISH4+1) 
#define PAR_POS_PRO_TEMP_WISH6                 (PAR_POS_PRO_TEMP_WISH5+1) 
#define PAR_POS_PRO_TEMP_WISH7                 (PAR_POS_PRO_TEMP_WISH6+1) 
#define PAR_POS_PRO_TEMP_WISH8                 (PAR_POS_PRO_TEMP_WISH7+1) 
#define PAR_POS_PRO_TEMP_WISH9                 (PAR_POS_PRO_TEMP_WISH8+1) 
#define PAR_POS_PRO_TEMP_WISH10                 (PAR_POS_PRO_TEMP_WISH9+1) 
#define PAR_POS_PRO_TEMP_WISH11                 (PAR_POS_PRO_TEMP_WISH10+1) 
#define PAR_POS_PRO_TEMP_WISH12                 (PAR_POS_PRO_TEMP_WISH11+1)  //79

#define PAR_POS_PRO_HUMI_DISP1                 (PAR_POS_PRO_TEMP_WISH12+1)  //80
#define PAR_POS_PRO_HUMI_DISP2                 (PAR_POS_PRO_HUMI_DISP1+1) 
#define PAR_POS_PRO_HUMI_DISP3                 (PAR_POS_PRO_HUMI_DISP2+1) 
#define PAR_POS_PRO_HUMI_DISP4                 (PAR_POS_PRO_HUMI_DISP3+1) 
#define PAR_POS_PRO_HUMI_DISP5                 (PAR_POS_PRO_HUMI_DISP4+1) 
#define PAR_POS_PRO_HUMI_DISP6                 (PAR_POS_PRO_HUMI_DISP5+1) 
#define PAR_POS_PRO_HUMI_DISP7                 (PAR_POS_PRO_HUMI_DISP6+1) 
#define PAR_POS_PRO_HUMI_DISP8                 (PAR_POS_PRO_HUMI_DISP7+1) 
#define PAR_POS_PRO_HUMI_DISP9                 (PAR_POS_PRO_HUMI_DISP8+1) 
#define PAR_POS_PRO_HUMI_DISP10                (PAR_POS_PRO_HUMI_DISP9+1) 
#define PAR_POS_PRO_HUMI_DISP11                (PAR_POS_PRO_HUMI_DISP10+1) 
#define PAR_POS_PRO_HUMI_DISP12                (PAR_POS_PRO_HUMI_DISP11+1) //91

#define PAR_POS_PRO_HUMI_WISH1                 (PAR_POS_PRO_HUMI_DISP12+1) //92
#define PAR_POS_PRO_HUMI_WISH2                 (PAR_POS_PRO_HUMI_WISH1+1) 
#define PAR_POS_PRO_HUMI_WISH3                 (PAR_POS_PRO_HUMI_WISH2+1) 
#define PAR_POS_PRO_HUMI_WISH4                 (PAR_POS_PRO_HUMI_WISH3+1) 
#define PAR_POS_PRO_HUMI_WISH5                 (PAR_POS_PRO_HUMI_WISH4+1) 
#define PAR_POS_PRO_HUMI_WISH6                 (PAR_POS_PRO_HUMI_WISH5+1) 
#define PAR_POS_PRO_HUMI_WISH7                 (PAR_POS_PRO_HUMI_WISH6+1) 
#define PAR_POS_PRO_HUMI_WISH8                 (PAR_POS_PRO_HUMI_WISH7+1) 
#define PAR_POS_PRO_HUMI_WISH9                 (PAR_POS_PRO_HUMI_WISH8+1) 
#define PAR_POS_PRO_HUMI_WISH10                 (PAR_POS_PRO_HUMI_WISH9+1) 
#define PAR_POS_PRO_HUMI_WISH11                 (PAR_POS_PRO_HUMI_WISH10+1) 
#define PAR_POS_PRO_HUMI_WISH12                 (PAR_POS_PRO_HUMI_WISH11+1) //103



#define PAR_POS_MONITOR_TEMP_DISP1                 (PAR_POS_PRO_HUMI_WISH12+1)    //104
#define PAR_POS_MONITOR_TEMP_DISP2                 (PAR_POS_MONITOR_TEMP_DISP1+1) 
#define PAR_POS_MONITOR_TEMP_DISP3                 (PAR_POS_MONITOR_TEMP_DISP2+1) 
#define PAR_POS_MONITOR_TEMP_DISP4                 (PAR_POS_MONITOR_TEMP_DISP3+1) 
#define PAR_POS_MONITOR_TEMP_DISP5                 (PAR_POS_MONITOR_TEMP_DISP4+1) 
#define PAR_POS_MONITOR_TEMP_DISP6                 (PAR_POS_MONITOR_TEMP_DISP5+1) 
#define PAR_POS_MONITOR_TEMP_DISP7                 (PAR_POS_MONITOR_TEMP_DISP6+1) 
#define PAR_POS_MONITOR_TEMP_DISP8                 (PAR_POS_MONITOR_TEMP_DISP7+1) 
#define PAR_POS_MONITOR_TEMP_DISP9                 (PAR_POS_MONITOR_TEMP_DISP8+1) 
#define PAR_POS_MONITOR_TEMP_DISP10                (PAR_POS_MONITOR_TEMP_DISP9+1) 
#define PAR_POS_MONITOR_TEMP_DISP11                (PAR_POS_MONITOR_TEMP_DISP10+1) 
#define PAR_POS_MONITOR_TEMP_DISP12                (PAR_POS_MONITOR_TEMP_DISP11+1) //115

#define PAR_POS_MONITOR_TEMP_WISH1                 (PAR_POS_MONITOR_TEMP_DISP12+1) //116
#define PAR_POS_MONITOR_TEMP_WISH2                 (PAR_POS_MONITOR_TEMP_WISH1+1) 
#define PAR_POS_MONITOR_TEMP_WISH3                 (PAR_POS_MONITOR_TEMP_WISH2+1) 
#define PAR_POS_MONITOR_TEMP_WISH4                 (PAR_POS_MONITOR_TEMP_WISH3+1) 
#define PAR_POS_MONITOR_TEMP_WISH5                 (PAR_POS_MONITOR_TEMP_WISH4+1) 
#define PAR_POS_MONITOR_TEMP_WISH6                 (PAR_POS_MONITOR_TEMP_WISH5+1) 
#define PAR_POS_MONITOR_TEMP_WISH7                 (PAR_POS_MONITOR_TEMP_WISH6+1) 
#define PAR_POS_MONITOR_TEMP_WISH8                 (PAR_POS_MONITOR_TEMP_WISH7+1) 
#define PAR_POS_MONITOR_TEMP_WISH9                 (PAR_POS_MONITOR_TEMP_WISH8+1) 
#define PAR_POS_MONITOR_TEMP_WISH10                 (PAR_POS_MONITOR_TEMP_WISH9+1) 
#define PAR_POS_MONITOR_TEMP_WISH11                 (PAR_POS_MONITOR_TEMP_WISH10+1) 
#define PAR_POS_MONITOR_TEMP_WISH12                 (PAR_POS_MONITOR_TEMP_WISH11+1) //127

#define PAR_POS_MONITOR_HUMI_DISP1                 (PAR_POS_MONITOR_TEMP_WISH12+1) //128
#define PAR_POS_MONITOR_HUMI_DISP2                 (PAR_POS_MONITOR_HUMI_DISP1+1) 
#define PAR_POS_MONITOR_HUMI_DISP3                 (PAR_POS_MONITOR_HUMI_DISP2+1) 
#define PAR_POS_MONITOR_HUMI_DISP4                 (PAR_POS_MONITOR_HUMI_DISP3+1) 
#define PAR_POS_MONITOR_HUMI_DISP5                 (PAR_POS_MONITOR_HUMI_DISP4+1) 
#define PAR_POS_MONITOR_HUMI_DISP6                 (PAR_POS_MONITOR_HUMI_DISP5+1) 
#define PAR_POS_MONITOR_HUMI_DISP7                 (PAR_POS_MONITOR_HUMI_DISP6+1) 
#define PAR_POS_MONITOR_HUMI_DISP8                 (PAR_POS_MONITOR_HUMI_DISP7+1) 
#define PAR_POS_MONITOR_HUMI_DISP9                 (PAR_POS_MONITOR_HUMI_DISP8+1) 
#define PAR_POS_MONITOR_HUMI_DISP10                (PAR_POS_MONITOR_HUMI_DISP9+1) 
#define PAR_POS_MONITOR_HUMI_DISP11                (PAR_POS_MONITOR_HUMI_DISP10+1) 
#define PAR_POS_MONITOR_HUMI_DISP12                (PAR_POS_MONITOR_HUMI_DISP11+1) //139

#define PAR_POS_MONITOR_HUMI_WISH1                 (PAR_POS_MONITOR_HUMI_DISP12+1) //140
#define PAR_POS_MONITOR_HUMI_WISH2                 (PAR_POS_MONITOR_HUMI_WISH1+1) 
#define PAR_POS_MONITOR_HUMI_WISH3                 (PAR_POS_MONITOR_HUMI_WISH2+1) 
#define PAR_POS_MONITOR_HUMI_WISH4                 (PAR_POS_MONITOR_HUMI_WISH3+1) 
#define PAR_POS_MONITOR_HUMI_WISH5                 (PAR_POS_MONITOR_HUMI_WISH4+1) 
#define PAR_POS_MONITOR_HUMI_WISH6                 (PAR_POS_MONITOR_HUMI_WISH5+1) 
#define PAR_POS_MONITOR_HUMI_WISH7                 (PAR_POS_MONITOR_HUMI_WISH6+1) 
#define PAR_POS_MONITOR_HUMI_WISH8                 (PAR_POS_MONITOR_HUMI_WISH7+1) 
#define PAR_POS_MONITOR_HUMI_WISH9                 (PAR_POS_MONITOR_HUMI_WISH8+1) 
#define PAR_POS_MONITOR_HUMI_WISH10                 (PAR_POS_MONITOR_HUMI_WISH9+1) 
#define PAR_POS_MONITOR_HUMI_WISH11                 (PAR_POS_MONITOR_HUMI_WISH10+1) 
#define PAR_POS_MONITOR_HUMI_WISH12                 (PAR_POS_MONITOR_HUMI_WISH11+1) //151

#define PAR_POS_TEMP_DEATH_ZONE                (PAR_POS_MONITOR_HUMI_WISH12+4)       //155
#define PAR_POS_Humi_DEATH_ZONE                (PAR_POS_TEMP_DEATH_ZONE+1)  			      //156

#define PAR_POS_DEFROSTING_TEMP1                (PAR_POS_Humi_DEATH_ZONE+1)        //157
#define PAR_POS_DEFROSTING_INTERVAL1           (PAR_POS_DEFROSTING_TEMP1+1)        //158
#define PAR_POS_DEFROSTING_TIME1               (PAR_POS_DEFROSTING_INTERVAL1+1)    //159

#define PAR_POS_DEFROSTING_TEMP2                (PAR_POS_DEFROSTING_TIME1+1)        //160
#define PAR_POS_DEFROSTING_INTERVAL2           (PAR_POS_DEFROSTING_TEMP2+1)        //161
#define PAR_POS_DEFROSTING_TIME2               (PAR_POS_DEFROSTING_INTERVAL2+1)    //162

#define PAR_POS_DEFROSTING_TEMP3                (PAR_POS_DEFROSTING_TIME2+1)        //163
#define PAR_POS_DEFROSTING_INTERVAL3           (PAR_POS_DEFROSTING_TEMP3+1)        //164
#define PAR_POS_DEFROSTING_TIME3               (PAR_POS_DEFROSTING_INTERVAL3+1)    //165

#define PAR_POS_DEFROSTING_TEMP4                (PAR_POS_DEFROSTING_TIME3+1)        //166
#define PAR_POS_DEFROSTING_INTERVAL4           (PAR_POS_DEFROSTING_TEMP4+1)        //167
#define PAR_POS_DEFROSTING_TIME4               (PAR_POS_DEFROSTING_INTERVAL4+1)    //168

#define PAR_POS_DEFROSTING_TEMP5                (PAR_POS_DEFROSTING_TIME4+1)        //169
#define PAR_POS_DEFROSTING_INTERVAL5            (PAR_POS_DEFROSTING_TEMP5+1)        //170
#define PAR_POS_DEFROSTING_TIME5                (PAR_POS_DEFROSTING_INTERVAL5+1)    //171
#define PAR_POS_FAN_OUT_PRECENT                 (PAR_POS_DEFROSTING_TIME5+1)       //172
#define PAR_POS_MAIN_CALIB_TMEP_K               (PAR_POS_FAN_OUT_PRECENT+1)       //173
#define PAR_POS_MAIN_CALIB_TMEP_B               (PAR_POS_MAIN_CALIB_TMEP_K+1)       //174
//#define PAR_POS_PRO0_REPEAT                      (PAR_POS_TEMP_NO_CTROL_HUMI+1)       //175
//#define PAR_POS_PRO1_REPEAT                      (PAR_POS_PRO0_REPEAT+1)       //176
//#define PAR_POS_PRO2_REPEAT                      (PAR_POS_PRO1_REPEAT+1)       //177
//#define PAR_POS_PRO3_REPEAT                      (PAR_POS_PRO2_REPEAT+1)       //178

#define PAR_POS_PRO0_TEMP00                       179                      //(PAR_POS_PRO3_REPEAT+1)       //179
#define PAR_POS_PRO0_HUMI00                      (PAR_POS_PRO0_TEMP00+1)       
#define PAR_POS_PRO0_TIME00                      (PAR_POS_PRO0_HUMI00+1)       
#define PAR_POS_PRO0_TEMP01                      (PAR_POS_PRO0_TIME00+1)       
#define PAR_POS_PRO0_HUMI01                      (PAR_POS_PRO0_TEMP01+1)      
#define PAR_POS_PRO0_TIME01                      (PAR_POS_PRO0_HUMI01+1)      
#define PAR_POS_PRO0_TEMP02                      (PAR_POS_PRO0_TIME01+1)       
#define PAR_POS_PRO0_HUMI02                      (PAR_POS_PRO0_TEMP02+1)       
#define PAR_POS_PRO0_TIME02                      (PAR_POS_PRO0_HUMI02+1)       
#define PAR_POS_PRO0_TEMP03                      (PAR_POS_PRO0_TIME02+1)       
#define PAR_POS_PRO0_HUMI03                      (PAR_POS_PRO0_TEMP03+1)       
#define PAR_POS_PRO0_TIME03                      (PAR_POS_PRO0_HUMI03+1)       
#define PAR_POS_PRO0_TEMP04                      (PAR_POS_PRO0_TIME03+1)       
#define PAR_POS_PRO0_HUMI04                      (PAR_POS_PRO0_TEMP04+1)      
#define PAR_POS_PRO0_TIME04                      (PAR_POS_PRO0_HUMI04+1)       
#define PAR_POS_PRO0_TEMP05                      (PAR_POS_PRO0_TIME04+1)       
#define PAR_POS_PRO0_HUMI05                      (PAR_POS_PRO0_TEMP05+1)       
#define PAR_POS_PRO0_TIME05                      (PAR_POS_PRO0_HUMI05+1)       
#define PAR_POS_PRO0_TEMP06                      (PAR_POS_PRO0_TIME05+1)       
#define PAR_POS_PRO0_HUMI06                      (PAR_POS_PRO0_TEMP06+1)       
#define PAR_POS_PRO0_TIME06                      (PAR_POS_PRO0_HUMI06+1)       
#define PAR_POS_PRO0_TEMP07                      (PAR_POS_PRO0_TIME06+1)       
#define PAR_POS_PRO0_HUMI07                      (PAR_POS_PRO0_TEMP07+1)       
#define PAR_POS_PRO0_TIME07                      (PAR_POS_PRO0_HUMI07+1)       
#define PAR_POS_PRO0_TEMP08                      (PAR_POS_PRO0_TIME07+1)       
#define PAR_POS_PRO0_HUMI08                      (PAR_POS_PRO0_TEMP08+1)       
#define PAR_POS_PRO0_TIME08                      (PAR_POS_PRO0_HUMI08+1)      
#define PAR_POS_PRO0_TEMP09                      (PAR_POS_PRO0_TIME08+1)       
#define PAR_POS_PRO0_HUMI09                      (PAR_POS_PRO0_TEMP09+1)       
#define PAR_POS_PRO0_TIME09                      (PAR_POS_PRO0_HUMI09+1)       
#define PAR_POS_PRO0_TEMP10                      (PAR_POS_PRO0_TIME09+1)       
#define PAR_POS_PRO0_HUMI10                      (PAR_POS_PRO0_TEMP10+1)       
#define PAR_POS_PRO0_TIME10                      (PAR_POS_PRO0_HUMI10+1)       
#define PAR_POS_PRO0_TEMP11                      (PAR_POS_PRO0_TIME10+1)       
#define PAR_POS_PRO0_HUMI11                      (PAR_POS_PRO0_TEMP11+1)     
#define PAR_POS_PRO0_TIME11                      (PAR_POS_PRO0_HUMI11+1)       
#define PAR_POS_PRO0_TEMP12                      (PAR_POS_PRO0_TIME11+1)       
#define PAR_POS_PRO0_HUMI12                      (PAR_POS_PRO0_TEMP12+1)      
#define PAR_POS_PRO0_TIME12                      (PAR_POS_PRO0_HUMI12+1)       
#define PAR_POS_PRO0_TEMP13                      (PAR_POS_PRO0_TIME12+1)       
#define PAR_POS_PRO0_HUMI13                      (PAR_POS_PRO0_TEMP13+1)      
#define PAR_POS_PRO0_TIME13                      (PAR_POS_PRO0_HUMI13+1)       
#define PAR_POS_PRO0_TEMP14                      (PAR_POS_PRO0_TIME13+1)       
#define PAR_POS_PRO0_HUMI14                      (PAR_POS_PRO0_TEMP14+1)       
#define PAR_POS_PRO0_TIME14                      (PAR_POS_PRO0_HUMI14+1)       
#define PAR_POS_PRO0_TEMP15                      (PAR_POS_PRO0_TIME14+1)       
#define PAR_POS_PRO0_HUMI15                      (PAR_POS_PRO0_TEMP15+1)       
#define PAR_POS_PRO0_TIME15                      (PAR_POS_PRO0_HUMI15+1)     
#define PAR_POS_PRO0_TEMP16                      (PAR_POS_PRO0_TIME15+1)       
#define PAR_POS_PRO0_HUMI16                      (PAR_POS_PRO0_TEMP16+1)       
#define PAR_POS_PRO0_TIME16                      (PAR_POS_PRO0_HUMI16+1)       
#define PAR_POS_PRO0_TEMP17                      (PAR_POS_PRO0_TIME16+1)      
#define PAR_POS_PRO0_HUMI17                      (PAR_POS_PRO0_TEMP17+1)       
#define PAR_POS_PRO0_TIME17                      (PAR_POS_PRO0_HUMI17+1)       
#define PAR_POS_PRO0_TEMP18                      (PAR_POS_PRO0_TIME17+1)       
#define PAR_POS_PRO0_HUMI18                      (PAR_POS_PRO0_TEMP18+1)      
#define PAR_POS_PRO0_TIME18                      (PAR_POS_PRO0_HUMI18+1)       
#define PAR_POS_PRO0_TEMP19                      (PAR_POS_PRO0_TIME18+1)      
#define PAR_POS_PRO0_HUMI19                      (PAR_POS_PRO0_TEMP19+1)       
#define PAR_POS_PRO0_TIME19                      (PAR_POS_PRO0_HUMI19+1)      
#define PAR_POS_PRO0_TEMP20                      (PAR_POS_PRO0_TIME19+1)       
#define PAR_POS_PRO0_HUMI20                      (PAR_POS_PRO0_TEMP20+1)       
#define PAR_POS_PRO0_TIME20                      (PAR_POS_PRO0_HUMI20+1)       
#define PAR_POS_PRO0_TEMP21                      (PAR_POS_PRO0_TIME20+1)      
#define PAR_POS_PRO0_HUMI21                      (PAR_POS_PRO0_TEMP21+1)       
#define PAR_POS_PRO0_TIME21                      (PAR_POS_PRO0_HUMI21+1)       
#define PAR_POS_PRO0_TEMP22                      (PAR_POS_PRO0_TIME21+1)       
#define PAR_POS_PRO0_HUMI22                      (PAR_POS_PRO0_TEMP22+1)      
#define PAR_POS_PRO0_TIME22                      (PAR_POS_PRO0_HUMI22+1)       
#define PAR_POS_PRO0_TEMP23                      (PAR_POS_PRO0_TIME22+1)      
#define PAR_POS_PRO0_HUMI23                      (PAR_POS_PRO0_TEMP23+1)       
#define PAR_POS_PRO0_TIME23                      (PAR_POS_PRO0_HUMI23+1)      
#define PAR_POS_PRO0_TEMP24                      (PAR_POS_PRO0_TIME23+1)      
#define PAR_POS_PRO0_HUMI24                      (PAR_POS_PRO0_TEMP24+1)       
#define PAR_POS_PRO0_TIME24                      (PAR_POS_PRO0_HUMI24+1)       
#define PAR_POS_PRO0_TEMP25                      (PAR_POS_PRO0_TIME24+1)       
#define PAR_POS_PRO0_HUMI25                      (PAR_POS_PRO0_TEMP25+1)       
#define PAR_POS_PRO0_TIME25                      (PAR_POS_PRO0_HUMI25+1)      
#define PAR_POS_PRO0_TEMP26                      (PAR_POS_PRO0_TIME25+1)       
#define PAR_POS_PRO0_HUMI26                      (PAR_POS_PRO0_TEMP26+1)       
#define PAR_POS_PRO0_TIME26                      (PAR_POS_PRO0_HUMI26+1)       
#define PAR_POS_PRO0_TEMP27                      (PAR_POS_PRO0_TIME26+1)       
#define PAR_POS_PRO0_HUMI27                      (PAR_POS_PRO0_TEMP27+1)       
#define PAR_POS_PRO0_TIME27                      (PAR_POS_PRO0_HUMI27+1)       
#define PAR_POS_PRO0_TEMP28                      (PAR_POS_PRO0_TIME27+1)       
#define PAR_POS_PRO0_HUMI28                      (PAR_POS_PRO0_TEMP28+1)       
#define PAR_POS_PRO0_TIME28                      (PAR_POS_PRO0_HUMI28+1)      
#define PAR_POS_PRO0_TEMP29                      (PAR_POS_PRO0_TIME28+1)       
#define PAR_POS_PRO0_HUMI29                      (PAR_POS_PRO0_TEMP29+1)       
#define PAR_POS_PRO0_TIME29                      (PAR_POS_PRO0_HUMI29+1)       //268

#define PAR_POS_PRO1_TEMP00                      (PAR_POS_PRO0_TIME29+1)       //269
#define PAR_POS_PRO1_HUMI00                      (PAR_POS_PRO1_TEMP00+1)       
#define PAR_POS_PRO1_TIME00                      (PAR_POS_PRO1_HUMI00+1)       
#define PAR_POS_PRO1_TEMP01                      (PAR_POS_PRO1_TIME00+1)       
#define PAR_POS_PRO1_HUMI01                      (PAR_POS_PRO1_TEMP01+1)       
#define PAR_POS_PRO1_TIME01                      (PAR_POS_PRO1_HUMI01+1)       
#define PAR_POS_PRO1_TEMP02                      (PAR_POS_PRO1_TIME01+1)       
#define PAR_POS_PRO1_HUMI02                      (PAR_POS_PRO1_TEMP02+1)       
#define PAR_POS_PRO1_TIME02                      (PAR_POS_PRO1_HUMI02+1)       
#define PAR_POS_PRO1_TEMP03                      (PAR_POS_PRO1_TIME02+1)       
#define PAR_POS_PRO1_HUMI03                      (PAR_POS_PRO1_TEMP03+1)       
#define PAR_POS_PRO1_TIME03                      (PAR_POS_PRO1_HUMI03+1)       
#define PAR_POS_PRO1_TEMP04                      (PAR_POS_PRO1_TIME03+1)       
#define PAR_POS_PRO1_HUMI04                      (PAR_POS_PRO1_TEMP04+1)       
#define PAR_POS_PRO1_TIME04                      (PAR_POS_PRO1_HUMI04+1)       
#define PAR_POS_PRO1_TEMP05                      (PAR_POS_PRO1_TIME04+1)       
#define PAR_POS_PRO1_HUMI05                      (PAR_POS_PRO1_TEMP05+1)       
#define PAR_POS_PRO1_TIME05                      (PAR_POS_PRO1_HUMI05+1)       
#define PAR_POS_PRO1_TEMP06                      (PAR_POS_PRO1_TIME05+1)       
#define PAR_POS_PRO1_HUMI06                      (PAR_POS_PRO1_TEMP06+1)       
#define PAR_POS_PRO1_TIME06                      (PAR_POS_PRO1_HUMI06+1)       
#define PAR_POS_PRO1_TEMP07                      (PAR_POS_PRO1_TIME06+1)       
#define PAR_POS_PRO1_HUMI07                      (PAR_POS_PRO1_TEMP07+1)       
#define PAR_POS_PRO1_TIME07                      (PAR_POS_PRO1_HUMI07+1)       
#define PAR_POS_PRO1_TEMP08                      (PAR_POS_PRO1_TIME07+1)       
#define PAR_POS_PRO1_HUMI08                      (PAR_POS_PRO1_TEMP08+1)       
#define PAR_POS_PRO1_TIME08                      (PAR_POS_PRO1_HUMI08+1)       
#define PAR_POS_PRO1_TEMP09                      (PAR_POS_PRO1_TIME08+1)       
#define PAR_POS_PRO1_HUMI09                      (PAR_POS_PRO1_TEMP09+1)       
#define PAR_POS_PRO1_TIME09                      (PAR_POS_PRO1_HUMI09+1)       
#define PAR_POS_PRO1_TEMP10                      (PAR_POS_PRO1_TIME09+1)       
#define PAR_POS_PRO1_HUMI10                      (PAR_POS_PRO1_TEMP10+1)       
#define PAR_POS_PRO1_TIME10                      (PAR_POS_PRO1_HUMI10+1)       
#define PAR_POS_PRO1_TEMP11                      (PAR_POS_PRO1_TIME10+1)       
#define PAR_POS_PRO1_HUMI11                      (PAR_POS_PRO1_TEMP11+1)       
#define PAR_POS_PRO1_TIME11                      (PAR_POS_PRO1_HUMI11+1)       
#define PAR_POS_PRO1_TEMP12                      (PAR_POS_PRO1_TIME11+1)       
#define PAR_POS_PRO1_HUMI12                      (PAR_POS_PRO1_TEMP12+1)       
#define PAR_POS_PRO1_TIME12                      (PAR_POS_PRO1_HUMI12+1)       
#define PAR_POS_PRO1_TEMP13                      (PAR_POS_PRO1_TIME12+1)       
#define PAR_POS_PRO1_HUMI13                      (PAR_POS_PRO1_TEMP13+1)       
#define PAR_POS_PRO1_TIME13                      (PAR_POS_PRO1_HUMI13+1)       
#define PAR_POS_PRO1_TEMP14                      (PAR_POS_PRO1_TIME13+1)       
#define PAR_POS_PRO1_HUMI14                      (PAR_POS_PRO1_TEMP14+1)       
#define PAR_POS_PRO1_TIME14                      (PAR_POS_PRO1_HUMI14+1)       
#define PAR_POS_PRO1_TEMP15                      (PAR_POS_PRO1_TIME14+1)       
#define PAR_POS_PRO1_HUMI15                      (PAR_POS_PRO1_TEMP15+1)       
#define PAR_POS_PRO1_TIME15                      (PAR_POS_PRO1_HUMI15+1)       
#define PAR_POS_PRO1_TEMP16                      (PAR_POS_PRO1_TIME15+1)       
#define PAR_POS_PRO1_HUMI16                      (PAR_POS_PRO1_TEMP16+1)       
#define PAR_POS_PRO1_TIME16                      (PAR_POS_PRO1_HUMI16+1)       
#define PAR_POS_PRO1_TEMP17                      (PAR_POS_PRO1_TIME16+1)       
#define PAR_POS_PRO1_HUMI17                      (PAR_POS_PRO1_TEMP17+1)       
#define PAR_POS_PRO1_TIME17                      (PAR_POS_PRO1_HUMI17+1)       
#define PAR_POS_PRO1_TEMP18                      (PAR_POS_PRO1_TIME17+1)       
#define PAR_POS_PRO1_HUMI18                      (PAR_POS_PRO1_TEMP18+1)       
#define PAR_POS_PRO1_TIME18                      (PAR_POS_PRO1_HUMI18+1)       
#define PAR_POS_PRO1_TEMP19                      (PAR_POS_PRO1_TIME18+1)       
#define PAR_POS_PRO1_HUMI19                      (PAR_POS_PRO1_TEMP19+1)       
#define PAR_POS_PRO1_TIME19                      (PAR_POS_PRO1_HUMI19+1)       
#define PAR_POS_PRO1_TEMP20                      (PAR_POS_PRO1_TIME19+1)       
#define PAR_POS_PRO1_HUMI20                      (PAR_POS_PRO1_TEMP20+1)       
#define PAR_POS_PRO1_TIME20                      (PAR_POS_PRO1_HUMI20+1)       
#define PAR_POS_PRO1_TEMP21                      (PAR_POS_PRO1_TIME20+1)       
#define PAR_POS_PRO1_HUMI21                      (PAR_POS_PRO1_TEMP21+1)       
#define PAR_POS_PRO1_TIME21                      (PAR_POS_PRO1_HUMI21+1)       
#define PAR_POS_PRO1_TEMP22                      (PAR_POS_PRO1_TIME21+1)       
#define PAR_POS_PRO1_HUMI22                      (PAR_POS_PRO1_TEMP22+1)       
#define PAR_POS_PRO1_TIME22                      (PAR_POS_PRO1_HUMI22+1)       
#define PAR_POS_PRO1_TEMP23                      (PAR_POS_PRO1_TIME22+1)       
#define PAR_POS_PRO1_HUMI23                      (PAR_POS_PRO1_TEMP23+1)       
#define PAR_POS_PRO1_TIME23                      (PAR_POS_PRO1_HUMI23+1)       
#define PAR_POS_PRO1_TEMP24                      (PAR_POS_PRO1_TIME23+1)       
#define PAR_POS_PRO1_HUMI24                      (PAR_POS_PRO1_TEMP24+1)       
#define PAR_POS_PRO1_TIME24                      (PAR_POS_PRO1_HUMI24+1)       
#define PAR_POS_PRO1_TEMP25                      (PAR_POS_PRO1_TIME24+1)       
#define PAR_POS_PRO1_HUMI25                      (PAR_POS_PRO1_TEMP25+1)       
#define PAR_POS_PRO1_TIME25                      (PAR_POS_PRO1_HUMI25+1)       
#define PAR_POS_PRO1_TEMP26                      (PAR_POS_PRO1_TIME25+1)       
#define PAR_POS_PRO1_HUMI26                      (PAR_POS_PRO1_TEMP26+1)       
#define PAR_POS_PRO1_TIME26                      (PAR_POS_PRO1_HUMI26+1)       
#define PAR_POS_PRO1_TEMP27                      (PAR_POS_PRO1_TIME26+1)       
#define PAR_POS_PRO1_HUMI27                      (PAR_POS_PRO1_TEMP27+1)       
#define PAR_POS_PRO1_TIME27                      (PAR_POS_PRO1_HUMI27+1)       
#define PAR_POS_PRO1_TEMP28                      (PAR_POS_PRO1_TIME27+1)       
#define PAR_POS_PRO1_HUMI28                      (PAR_POS_PRO1_TEMP28+1)       
#define PAR_POS_PRO1_TIME28                      (PAR_POS_PRO1_HUMI28+1)       
#define PAR_POS_PRO1_TEMP29                      (PAR_POS_PRO1_TIME28+1)       
#define PAR_POS_PRO1_HUMI29                      (PAR_POS_PRO1_TEMP29+1)       
#define PAR_POS_PRO1_TIME29                      (PAR_POS_PRO1_HUMI29+1)       //358

#define PAR_POS_PRO2_TEMP00                      359//(PAR_POS_PRO1_TIME29+1)       //359   
#define PAR_POS_PRO2_HUMI00                      (PAR_POS_PRO2_TEMP00+1)       
#define PAR_POS_PRO2_TIME00                      (PAR_POS_PRO2_HUMI00+1)       
#define PAR_POS_PRO2_TEMP01                      (PAR_POS_PRO2_TIME00+1)       
#define PAR_POS_PRO2_HUMI01                      (PAR_POS_PRO2_TEMP01+1)       
#define PAR_POS_PRO2_TIME01                      (PAR_POS_PRO2_HUMI01+1)       
#define PAR_POS_PRO2_TEMP02                      (PAR_POS_PRO2_TIME01+1)       
#define PAR_POS_PRO2_HUMI02                      (PAR_POS_PRO2_TEMP02+1)       
#define PAR_POS_PRO2_TIME02                      (PAR_POS_PRO2_HUMI02+1)       
#define PAR_POS_PRO2_TEMP03                      (PAR_POS_PRO2_TIME02+1)       
#define PAR_POS_PRO2_HUMI03                      (PAR_POS_PRO2_TEMP03+1)       
#define PAR_POS_PRO2_TIME03                      (PAR_POS_PRO2_HUMI03+1)       
#define PAR_POS_PRO2_TEMP04                      (PAR_POS_PRO2_TIME03+1)       
#define PAR_POS_PRO2_HUMI04                      (PAR_POS_PRO2_TEMP04+1)       
#define PAR_POS_PRO2_TIME04                      (PAR_POS_PRO2_HUMI04+1)       
#define PAR_POS_PRO2_TEMP05                      (PAR_POS_PRO2_TIME04+1)       
#define PAR_POS_PRO2_HUMI05                      (PAR_POS_PRO2_TEMP05+1)       
#define PAR_POS_PRO2_TIME05                      (PAR_POS_PRO2_HUMI05+1)       
#define PAR_POS_PRO2_TEMP06                      (PAR_POS_PRO2_TIME05+1)       
#define PAR_POS_PRO2_HUMI06                      (PAR_POS_PRO2_TEMP06+1)       
#define PAR_POS_PRO2_TIME06                      (PAR_POS_PRO2_HUMI06+1)       
#define PAR_POS_PRO2_TEMP07                      (PAR_POS_PRO2_TIME06+1)       
#define PAR_POS_PRO2_HUMI07                      (PAR_POS_PRO2_TEMP07+1)       
#define PAR_POS_PRO2_TIME07                      (PAR_POS_PRO2_HUMI07+1)       
#define PAR_POS_PRO2_TEMP08                      (PAR_POS_PRO2_TIME07+1)       
#define PAR_POS_PRO2_HUMI08                      (PAR_POS_PRO2_TEMP08+1)       
#define PAR_POS_PRO2_TIME08                      (PAR_POS_PRO2_HUMI08+1)       
#define PAR_POS_PRO2_TEMP09                      (PAR_POS_PRO2_TIME08+1)       
#define PAR_POS_PRO2_HUMI09                      (PAR_POS_PRO2_TEMP09+1)       
#define PAR_POS_PRO2_TIME09                      (PAR_POS_PRO2_HUMI09+1)       
#define PAR_POS_PRO2_TEMP10                      (PAR_POS_PRO2_TIME09+1)       
#define PAR_POS_PRO2_HUMI10                      (PAR_POS_PRO2_TEMP10+1)       
#define PAR_POS_PRO2_TIME10                      (PAR_POS_PRO2_HUMI10+1)       
#define PAR_POS_PRO2_TEMP11                      (PAR_POS_PRO2_TIME10+1)       
#define PAR_POS_PRO2_HUMI11                      (PAR_POS_PRO2_TEMP11+1)       
#define PAR_POS_PRO2_TIME11                      (PAR_POS_PRO2_HUMI11+1)       
#define PAR_POS_PRO2_TEMP12                      (PAR_POS_PRO2_TIME11+1)       
#define PAR_POS_PRO2_HUMI12                      (PAR_POS_PRO2_TEMP12+1)       
#define PAR_POS_PRO2_TIME12                      (PAR_POS_PRO2_HUMI12+1)       
#define PAR_POS_PRO2_TEMP13                      (PAR_POS_PRO2_TIME12+1)       
#define PAR_POS_PRO2_HUMI13                      (PAR_POS_PRO2_TEMP13+1)       
#define PAR_POS_PRO2_TIME13                      (PAR_POS_PRO2_HUMI13+1)       
#define PAR_POS_PRO2_TEMP14                      (PAR_POS_PRO2_TIME13+1)       
#define PAR_POS_PRO2_HUMI14                      (PAR_POS_PRO2_TEMP14+1)       
#define PAR_POS_PRO2_TIME14                      (PAR_POS_PRO2_HUMI14+1)       
#define PAR_POS_PRO2_TEMP15                      (PAR_POS_PRO2_TIME14+1)       
#define PAR_POS_PRO2_HUMI15                      (PAR_POS_PRO2_TEMP15+1)       
#define PAR_POS_PRO2_TIME15                      (PAR_POS_PRO2_HUMI15+1)       
#define PAR_POS_PRO2_TEMP16                      (PAR_POS_PRO2_TIME15+1)       
#define PAR_POS_PRO2_HUMI16                      (PAR_POS_PRO2_TEMP16+1)       
#define PAR_POS_PRO2_TIME16                      (PAR_POS_PRO2_HUMI16+1)       
#define PAR_POS_PRO2_TEMP17                      (PAR_POS_PRO2_TIME16+1)       
#define PAR_POS_PRO2_HUMI17                      (PAR_POS_PRO2_TEMP17+1)       
#define PAR_POS_PRO2_TIME17                      (PAR_POS_PRO2_HUMI17+1)       
#define PAR_POS_PRO2_TEMP18                      (PAR_POS_PRO2_TIME17+1)       
#define PAR_POS_PRO2_HUMI18                      (PAR_POS_PRO2_TEMP18+1)       
#define PAR_POS_PRO2_TIME18                      (PAR_POS_PRO2_HUMI18+1)       
#define PAR_POS_PRO2_TEMP19                      (PAR_POS_PRO2_TIME18+1)       
#define PAR_POS_PRO2_HUMI19                      (PAR_POS_PRO2_TEMP19+1)       
#define PAR_POS_PRO2_TIME19                      (PAR_POS_PRO2_HUMI19+1)       
#define PAR_POS_PRO2_TEMP20                      (PAR_POS_PRO2_TIME19+1)       
#define PAR_POS_PRO2_HUMI20                      (PAR_POS_PRO2_TEMP20+1)       
#define PAR_POS_PRO2_TIME20                      (PAR_POS_PRO2_HUMI20+1)       
#define PAR_POS_PRO2_TEMP21                      (PAR_POS_PRO2_TIME20+1)       
#define PAR_POS_PRO2_HUMI21                      (PAR_POS_PRO2_TEMP21+1)       
#define PAR_POS_PRO2_TIME21                      (PAR_POS_PRO2_HUMI21+1)       
#define PAR_POS_PRO2_TEMP22                      (PAR_POS_PRO2_TIME21+1)       
#define PAR_POS_PRO2_HUMI22                      (PAR_POS_PRO2_TEMP22+1)       
#define PAR_POS_PRO2_TIME22                      (PAR_POS_PRO2_HUMI22+1)       
#define PAR_POS_PRO2_TEMP23                      (PAR_POS_PRO2_TIME22+1)       
#define PAR_POS_PRO2_HUMI23                      (PAR_POS_PRO2_TEMP23+1)       
#define PAR_POS_PRO2_TIME23                      (PAR_POS_PRO2_HUMI23+1)       
#define PAR_POS_PRO2_TEMP24                      (PAR_POS_PRO2_TIME23+1)       
#define PAR_POS_PRO2_HUMI24                      (PAR_POS_PRO2_TEMP24+1)       
#define PAR_POS_PRO2_TIME24                      (PAR_POS_PRO2_HUMI24+1)       
#define PAR_POS_PRO2_TEMP25                      (PAR_POS_PRO2_TIME24+1)       
#define PAR_POS_PRO2_HUMI25                      (PAR_POS_PRO2_TEMP25+1)       
#define PAR_POS_PRO2_TIME25                      (PAR_POS_PRO2_HUMI25+1)       
#define PAR_POS_PRO2_TEMP26                      (PAR_POS_PRO2_TIME25+1)       
#define PAR_POS_PRO2_HUMI26                      (PAR_POS_PRO2_TEMP26+1)       
#define PAR_POS_PRO2_TIME26                      (PAR_POS_PRO2_HUMI26+1)       
#define PAR_POS_PRO2_TEMP27                      (PAR_POS_PRO2_TIME26+1)       
#define PAR_POS_PRO2_HUMI27                      (PAR_POS_PRO2_TEMP27+1)       
#define PAR_POS_PRO2_TIME27                      (PAR_POS_PRO2_HUMI27+1)       
#define PAR_POS_PRO2_TEMP28                      (PAR_POS_PRO2_TIME27+1)       
#define PAR_POS_PRO2_HUMI28                      (PAR_POS_PRO2_TEMP28+1)       
#define PAR_POS_PRO2_TIME28                      (PAR_POS_PRO2_HUMI28+1)       
#define PAR_POS_PRO2_TEMP29                      (PAR_POS_PRO2_TIME28+1)       
#define PAR_POS_PRO2_HUMI29                      (PAR_POS_PRO2_TEMP29+1)       
#define PAR_POS_PRO2_TIME29                      (PAR_POS_PRO2_HUMI29+1)     //448  


#define PAR_POS_PRO3_HUMI00                      (PAR_POS_PRO2_TIME29+1)     //449  
#define PAR_POS_PRO3_TIME00                      (PAR_POS_PRO3_HUMI00+1)       
#define PAR_POS_PRO3_TEMP01                      (PAR_POS_PRO3_TIME00+1)       
#define PAR_POS_PRO3_HUMI01                      (PAR_POS_PRO3_TEMP01+1)       
#define PAR_POS_PRO3_TIME01                      (PAR_POS_PRO3_HUMI01+1)       
#define PAR_POS_PRO3_TEMP02                      (PAR_POS_PRO3_TIME01+1)       
#define PAR_POS_PRO3_HUMI02                      (PAR_POS_PRO3_TEMP02+1)       
#define PAR_POS_PRO3_TIME02                      (PAR_POS_PRO3_HUMI02+1)       
#define PAR_POS_PRO3_TEMP03                      (PAR_POS_PRO3_TIME02+1)       
#define PAR_POS_PRO3_HUMI03                      (PAR_POS_PRO3_TEMP03+1)       
#define PAR_POS_PRO3_TIME03                      (PAR_POS_PRO3_HUMI03+1)       
#define PAR_POS_PRO3_TEMP04                      (PAR_POS_PRO3_TIME03+1)       
#define PAR_POS_PRO3_HUMI04                      (PAR_POS_PRO3_TEMP04+1)       
#define PAR_POS_PRO3_TIME04                      (PAR_POS_PRO3_HUMI04+1)       
#define PAR_POS_PRO3_TEMP05                      (PAR_POS_PRO3_TIME04+1)       
#define PAR_POS_PRO3_HUMI05                      (PAR_POS_PRO3_TEMP05+1)       
#define PAR_POS_PRO3_TIME05                      (PAR_POS_PRO3_HUMI05+1)       
#define PAR_POS_PRO3_TEMP06                      (PAR_POS_PRO3_TIME05+1)       
#define PAR_POS_PRO3_HUMI06                      (PAR_POS_PRO3_TEMP06+1)       
#define PAR_POS_PRO3_TIME06                      (PAR_POS_PRO3_HUMI06+1)       
#define PAR_POS_PRO3_TEMP07                      (PAR_POS_PRO3_TIME06+1)       
#define PAR_POS_PRO3_HUMI07                      (PAR_POS_PRO3_TEMP07+1)       
#define PAR_POS_PRO3_TIME07                      (PAR_POS_PRO3_HUMI07+1)       
#define PAR_POS_PRO3_TEMP08                      (PAR_POS_PRO3_TIME07+1)       
#define PAR_POS_PRO3_HUMI08                      (PAR_POS_PRO3_TEMP08+1)       
#define PAR_POS_PRO3_TIME08                      (PAR_POS_PRO3_HUMI08+1)       
#define PAR_POS_PRO3_TEMP09                      (PAR_POS_PRO3_TIME08+1)       
#define PAR_POS_PRO3_HUMI09                      (PAR_POS_PRO3_TEMP09+1)       
#define PAR_POS_PRO3_TIME09                      (PAR_POS_PRO3_HUMI09+1)       
#define PAR_POS_PRO3_TEMP10                      (PAR_POS_PRO3_TIME09+1)       
#define PAR_POS_PRO3_HUMI10                      (PAR_POS_PRO3_TEMP10+1)       
#define PAR_POS_PRO3_TIME10                      (PAR_POS_PRO3_HUMI10+1)       
#define PAR_POS_PRO3_TEMP11                      (PAR_POS_PRO3_TIME10+1)       
#define PAR_POS_PRO3_HUMI11                      (PAR_POS_PRO3_TEMP11+1)       
#define PAR_POS_PRO3_TIME11                      (PAR_POS_PRO3_HUMI11+1)       
#define PAR_POS_PRO3_TEMP12                      (PAR_POS_PRO3_TIME11+1)       
#define PAR_POS_PRO3_HUMI12                      (PAR_POS_PRO3_TEMP12+1)       
#define PAR_POS_PRO3_TIME12                      (PAR_POS_PRO3_HUMI12+1)       
#define PAR_POS_PRO3_TEMP13                      (PAR_POS_PRO3_TIME12+1)       
#define PAR_POS_PRO3_HUMI13                      (PAR_POS_PRO3_TEMP13+1)       
#define PAR_POS_PRO3_TIME13                      (PAR_POS_PRO3_HUMI13+1)       
#define PAR_POS_PRO3_TEMP14                      (PAR_POS_PRO3_TIME13+1)       
#define PAR_POS_PRO3_HUMI14                      (PAR_POS_PRO3_TEMP14+1)       
#define PAR_POS_PRO3_TIME14                      (PAR_POS_PRO3_HUMI14+1)       
#define PAR_POS_PRO3_TEMP15                      (PAR_POS_PRO3_TIME14+1)       
#define PAR_POS_PRO3_HUMI15                      (PAR_POS_PRO3_TEMP15+1)       
#define PAR_POS_PRO3_TIME15                      (PAR_POS_PRO3_HUMI15+1)       
#define PAR_POS_PRO3_TEMP16                      (PAR_POS_PRO3_TIME15+1)       
#define PAR_POS_PRO3_HUMI16                      (PAR_POS_PRO3_TEMP16+1)       
#define PAR_POS_PRO3_TIME16                      (PAR_POS_PRO3_HUMI16+1)       
#define PAR_POS_PRO3_TEMP17                      (PAR_POS_PRO3_TIME16+1)       
#define PAR_POS_PRO3_HUMI17                      (PAR_POS_PRO3_TEMP17+1)       
#define PAR_POS_PRO3_TIME17                      (PAR_POS_PRO3_HUMI17+1)       
#define PAR_POS_PRO3_TEMP18                      (PAR_POS_PRO3_TIME17+1)       
#define PAR_POS_PRO3_HUMI18                      (PAR_POS_PRO3_TEMP18+1)       
#define PAR_POS_PRO3_TIME18                      (PAR_POS_PRO3_HUMI18+1)       
#define PAR_POS_PRO3_TEMP19                      (PAR_POS_PRO3_TIME18+1)       
#define PAR_POS_PRO3_HUMI19                      (PAR_POS_PRO3_TEMP19+1)       
#define PAR_POS_PRO3_TIME19                      (PAR_POS_PRO3_HUMI19+1)       
#define PAR_POS_PRO3_TEMP20                      (PAR_POS_PRO3_TIME19+1)       
#define PAR_POS_PRO3_HUMI20                      (PAR_POS_PRO3_TEMP20+1)       
#define PAR_POS_PRO3_TIME20                      (PAR_POS_PRO3_HUMI20+1)       
#define PAR_POS_PRO3_TEMP21                      (PAR_POS_PRO3_TIME20+1)       
#define PAR_POS_PRO3_HUMI21                      (PAR_POS_PRO3_TEMP21+1)       
#define PAR_POS_PRO3_TIME21                      (PAR_POS_PRO3_HUMI21+1)       
#define PAR_POS_PRO3_TEMP22                      (PAR_POS_PRO3_TIME21+1)       
#define PAR_POS_PRO3_HUMI22                      (PAR_POS_PRO3_TEMP22+1)       
#define PAR_POS_PRO3_TIME22                      (PAR_POS_PRO3_HUMI22+1)       
#define PAR_POS_PRO3_TEMP23                      (PAR_POS_PRO3_TIME22+1)       
#define PAR_POS_PRO3_HUMI23                      (PAR_POS_PRO3_TEMP23+1)       
#define PAR_POS_PRO3_TIME23                      (PAR_POS_PRO3_HUMI23+1)       
#define PAR_POS_PRO3_TEMP24                      (PAR_POS_PRO3_TIME23+1)       
#define PAR_POS_PRO3_HUMI24                      (PAR_POS_PRO3_TEMP24+1)       
#define PAR_POS_PRO3_TIME24                      (PAR_POS_PRO3_HUMI24+1)       
#define PAR_POS_PRO3_TEMP25                      (PAR_POS_PRO3_TIME24+1)       
#define PAR_POS_PRO3_HUMI25                      (PAR_POS_PRO3_TEMP25+1)       
#define PAR_POS_PRO3_TIME25                      (PAR_POS_PRO3_HUMI25+1)       
#define PAR_POS_PRO3_TEMP26                      (PAR_POS_PRO3_TIME25+1)       
#define PAR_POS_PRO3_HUMI26                      (PAR_POS_PRO3_TEMP26+1)       
#define PAR_POS_PRO3_TIME26                      (PAR_POS_PRO3_HUMI26+1)       
#define PAR_POS_PRO3_TEMP27                      (PAR_POS_PRO3_TIME26+1)       
#define PAR_POS_PRO3_HUMI27                      (PAR_POS_PRO3_TEMP27+1)       
#define PAR_POS_PRO3_TIME27                      (PAR_POS_PRO3_HUMI27+1)       
#define PAR_POS_PRO3_TEMP28                      (PAR_POS_PRO3_TIME27+1)       
#define PAR_POS_PRO3_HUMI28                      (PAR_POS_PRO3_TEMP28+1)       
#define PAR_POS_PRO3_TIME28                      (PAR_POS_PRO3_HUMI28+1)       
#define PAR_POS_PRO3_TEMP29                      (PAR_POS_PRO3_TIME28+1)       
#define PAR_POS_PRO3_HUMI29                      (PAR_POS_PRO3_TEMP29+1)       
#define PAR_POS_PRO3_TIME29                      (PAR_POS_PRO3_HUMI29+1)    //538   

#define PAR_POS_PRO4_TEMP00                      539//(PAR_POS_PRO3_TIME29+1)    //539   
#define PAR_POS_PRO4_HUMI00                      (PAR_POS_PRO4_TEMP00+1)       
#define PAR_POS_PRO4_TIME00                      (PAR_POS_PRO4_HUMI00+1)       
#define PAR_POS_PRO4_TEMP01                      (PAR_POS_PRO4_TIME00+1)       
#define PAR_POS_PRO4_HUMI01                      (PAR_POS_PRO4_TEMP01+1)       
#define PAR_POS_PRO4_TIME01                      (PAR_POS_PRO4_HUMI01+1)       
#define PAR_POS_PRO4_TEMP02                      (PAR_POS_PRO4_TIME01+1)       
#define PAR_POS_PRO4_HUMI02                      (PAR_POS_PRO4_TEMP02+1)       
#define PAR_POS_PRO4_TIME02                      (PAR_POS_PRO4_HUMI02+1)       
#define PAR_POS_PRO4_TEMP03                      (PAR_POS_PRO4_TIME02+1)       
#define PAR_POS_PRO4_HUMI03                      (PAR_POS_PRO4_TEMP03+1)       
#define PAR_POS_PRO4_TIME03                      (PAR_POS_PRO4_HUMI03+1)       
#define PAR_POS_PRO4_TEMP04                      (PAR_POS_PRO4_TIME03+1)       
#define PAR_POS_PRO4_HUMI04                      (PAR_POS_PRO4_TEMP04+1)       
#define PAR_POS_PRO4_TIME04                      (PAR_POS_PRO4_HUMI04+1)       
#define PAR_POS_PRO4_TEMP05                      (PAR_POS_PRO4_TIME04+1)       
#define PAR_POS_PRO4_HUMI05                      (PAR_POS_PRO4_TEMP05+1)       
#define PAR_POS_PRO4_TIME05                      (PAR_POS_PRO4_HUMI05+1)       
#define PAR_POS_PRO4_TEMP06                      (PAR_POS_PRO4_TIME05+1)       
#define PAR_POS_PRO4_HUMI06                      (PAR_POS_PRO4_TEMP06+1)       
#define PAR_POS_PRO4_TIME06                      (PAR_POS_PRO4_HUMI06+1)       
#define PAR_POS_PRO4_TEMP07                      (PAR_POS_PRO4_TIME06+1)       
#define PAR_POS_PRO4_HUMI07                      (PAR_POS_PRO4_TEMP07+1)       
#define PAR_POS_PRO4_TIME07                      (PAR_POS_PRO4_HUMI07+1)       
#define PAR_POS_PRO4_TEMP08                      (PAR_POS_PRO4_TIME07+1)       
#define PAR_POS_PRO4_HUMI08                      (PAR_POS_PRO4_TEMP08+1)       
#define PAR_POS_PRO4_TIME08                      (PAR_POS_PRO4_HUMI08+1)       
#define PAR_POS_PRO4_TEMP09                      (PAR_POS_PRO4_TIME08+1)       
#define PAR_POS_PRO4_HUMI09                      (PAR_POS_PRO4_TEMP09+1)       
#define PAR_POS_PRO4_TIME09                      (PAR_POS_PRO4_HUMI09+1)       
#define PAR_POS_PRO4_TEMP10                      (PAR_POS_PRO4_TIME09+1)       
#define PAR_POS_PRO4_HUMI10                      (PAR_POS_PRO4_TEMP10+1)       
#define PAR_POS_PRO4_TIME10                      (PAR_POS_PRO4_HUMI10+1)       
#define PAR_POS_PRO4_TEMP11                      (PAR_POS_PRO4_TIME10+1)       
#define PAR_POS_PRO4_HUMI11                      (PAR_POS_PRO4_TEMP11+1)       
#define PAR_POS_PRO4_TIME11                      (PAR_POS_PRO4_HUMI11+1)       
#define PAR_POS_PRO4_TEMP12                      (PAR_POS_PRO4_TIME11+1)       
#define PAR_POS_PRO4_HUMI12                      (PAR_POS_PRO4_TEMP12+1)       
#define PAR_POS_PRO4_TIME12                      (PAR_POS_PRO4_HUMI12+1)       
#define PAR_POS_PRO4_TEMP13                      (PAR_POS_PRO4_TIME12+1)       
#define PAR_POS_PRO4_HUMI13                      (PAR_POS_PRO4_TEMP13+1)       
#define PAR_POS_PRO4_TIME13                      (PAR_POS_PRO4_HUMI13+1)       
#define PAR_POS_PRO4_TEMP14                      (PAR_POS_PRO4_TIME13+1)       
#define PAR_POS_PRO4_HUMI14                      (PAR_POS_PRO4_TEMP14+1)       
#define PAR_POS_PRO4_TIME14                      (PAR_POS_PRO4_HUMI14+1)       
#define PAR_POS_PRO4_TEMP15                      (PAR_POS_PRO4_TIME14+1)       
#define PAR_POS_PRO4_HUMI15                      (PAR_POS_PRO4_TEMP15+1)       
#define PAR_POS_PRO4_TIME15                      (PAR_POS_PRO4_HUMI15+1)       
#define PAR_POS_PRO4_TEMP16                      (PAR_POS_PRO4_TIME15+1)       
#define PAR_POS_PRO4_HUMI16                      (PAR_POS_PRO4_TEMP16+1)       
#define PAR_POS_PRO4_TIME16                      (PAR_POS_PRO4_HUMI16+1)       
#define PAR_POS_PRO4_TEMP17                      (PAR_POS_PRO4_TIME16+1)       
#define PAR_POS_PRO4_HUMI17                      (PAR_POS_PRO4_TEMP17+1)       
#define PAR_POS_PRO4_TIME17                      (PAR_POS_PRO4_HUMI17+1)       
#define PAR_POS_PRO4_TEMP18                      (PAR_POS_PRO4_TIME17+1)       
#define PAR_POS_PRO4_HUMI18                      (PAR_POS_PRO4_TEMP18+1)       
#define PAR_POS_PRO4_TIME18                      (PAR_POS_PRO4_HUMI18+1)       
#define PAR_POS_PRO4_TEMP19                      (PAR_POS_PRO4_TIME18+1)       
#define PAR_POS_PRO4_HUMI19                      (PAR_POS_PRO4_TEMP19+1)       
#define PAR_POS_PRO4_TIME19                      (PAR_POS_PRO4_HUMI19+1)       
#define PAR_POS_PRO4_TEMP20                      (PAR_POS_PRO4_TIME19+1)       
#define PAR_POS_PRO4_HUMI20                      (PAR_POS_PRO4_TEMP20+1)       
#define PAR_POS_PRO4_TIME20                      (PAR_POS_PRO4_HUMI20+1)       
#define PAR_POS_PRO4_TEMP21                      (PAR_POS_PRO4_TIME20+1)       
#define PAR_POS_PRO4_HUMI21                      (PAR_POS_PRO4_TEMP21+1)       
#define PAR_POS_PRO4_TIME21                      (PAR_POS_PRO4_HUMI21+1)       
#define PAR_POS_PRO4_TEMP22                      (PAR_POS_PRO4_TIME21+1)       
#define PAR_POS_PRO4_HUMI22                      (PAR_POS_PRO4_TEMP22+1)       
#define PAR_POS_PRO4_TIME22                      (PAR_POS_PRO4_HUMI22+1)       
#define PAR_POS_PRO4_TEMP23                      (PAR_POS_PRO4_TIME22+1)       
#define PAR_POS_PRO4_HUMI23                      (PAR_POS_PRO4_TEMP23+1)       
#define PAR_POS_PRO4_TIME23                      (PAR_POS_PRO4_HUMI23+1)       
#define PAR_POS_PRO4_TEMP24                      (PAR_POS_PRO4_TIME23+1)       
#define PAR_POS_PRO4_HUMI24                      (PAR_POS_PRO4_TEMP24+1)       
#define PAR_POS_PRO4_TIME24                      (PAR_POS_PRO4_HUMI24+1)       
#define PAR_POS_PRO4_TEMP25                      (PAR_POS_PRO4_TIME24+1)       
#define PAR_POS_PRO4_HUMI25                      (PAR_POS_PRO4_TEMP25+1)       
#define PAR_POS_PRO4_TIME25                      (PAR_POS_PRO4_HUMI25+1)       
#define PAR_POS_PRO4_TEMP26                      (PAR_POS_PRO4_TIME25+1)       
#define PAR_POS_PRO4_HUMI26                      (PAR_POS_PRO4_TEMP26+1)       
#define PAR_POS_PRO4_TIME26                      (PAR_POS_PRO4_HUMI26+1)       
#define PAR_POS_PRO4_TEMP27                      (PAR_POS_PRO4_TIME26+1)       
#define PAR_POS_PRO4_HUMI27                      (PAR_POS_PRO4_TEMP27+1)       
#define PAR_POS_PRO4_TIME27                      (PAR_POS_PRO4_HUMI27+1)       
#define PAR_POS_PRO4_TEMP28                      (PAR_POS_PRO4_TIME27+1)       
#define PAR_POS_PRO4_HUMI28                      (PAR_POS_PRO4_TEMP28+1)       
#define PAR_POS_PRO4_TIME28                      (PAR_POS_PRO4_HUMI28+1)       
#define PAR_POS_PRO4_TEMP29                      (PAR_POS_PRO4_TIME28+1)       
#define PAR_POS_PRO4_HUMI29                      (PAR_POS_PRO4_TEMP29+1)       
#define PAR_POS_PRO4_TIME29                      (PAR_POS_PRO4_HUMI29+1)  //628     

#define PAR_POS_PRO5_TEMP00                      (PAR_POS_PRO4_TIME29+1)  //629     
#define PAR_POS_PRO5_HUMI00                      (PAR_POS_PRO5_TEMP00+1)       
#define PAR_POS_PRO5_TIME00                      (PAR_POS_PRO5_HUMI00+1)       
#define PAR_POS_PRO5_TEMP01                      (PAR_POS_PRO5_TIME00+1)       
#define PAR_POS_PRO5_HUMI01                      (PAR_POS_PRO5_TEMP01+1)       
#define PAR_POS_PRO5_TIME01                      (PAR_POS_PRO5_HUMI01+1)       
#define PAR_POS_PRO5_TEMP02                      (PAR_POS_PRO5_TIME01+1)       
#define PAR_POS_PRO5_HUMI02                      (PAR_POS_PRO5_TEMP02+1)       
#define PAR_POS_PRO5_TIME02                      (PAR_POS_PRO5_HUMI02+1)       
#define PAR_POS_PRO5_TEMP03                      (PAR_POS_PRO5_TIME02+1)       
#define PAR_POS_PRO5_HUMI03                      (PAR_POS_PRO5_TEMP03+1)       
#define PAR_POS_PRO5_TIME03                      (PAR_POS_PRO5_HUMI03+1)       
#define PAR_POS_PRO5_TEMP04                      (PAR_POS_PRO5_TIME03+1)       
#define PAR_POS_PRO5_HUMI04                      (PAR_POS_PRO5_TEMP04+1)       
#define PAR_POS_PRO5_TIME04                      (PAR_POS_PRO5_HUMI04+1)       
#define PAR_POS_PRO5_TEMP05                      (PAR_POS_PRO5_TIME04+1)       
#define PAR_POS_PRO5_HUMI05                      (PAR_POS_PRO5_TEMP05+1)       
#define PAR_POS_PRO5_TIME05                      (PAR_POS_PRO5_HUMI05+1)       
#define PAR_POS_PRO5_TEMP06                      (PAR_POS_PRO5_TIME05+1)       
#define PAR_POS_PRO5_HUMI06                      (PAR_POS_PRO5_TEMP06+1)       
#define PAR_POS_PRO5_TIME06                      (PAR_POS_PRO5_HUMI06+1)       
#define PAR_POS_PRO5_TEMP07                      (PAR_POS_PRO5_TIME06+1)       
#define PAR_POS_PRO5_HUMI07                      (PAR_POS_PRO5_TEMP07+1)       
#define PAR_POS_PRO5_TIME07                      (PAR_POS_PRO5_HUMI07+1)       
#define PAR_POS_PRO5_TEMP08                      (PAR_POS_PRO5_TIME07+1)       
#define PAR_POS_PRO5_HUMI08                      (PAR_POS_PRO5_TEMP08+1)       
#define PAR_POS_PRO5_TIME08                      (PAR_POS_PRO5_HUMI08+1)       
#define PAR_POS_PRO5_TEMP09                      (PAR_POS_PRO5_TIME08+1)       
#define PAR_POS_PRO5_HUMI09                      (PAR_POS_PRO5_TEMP09+1)       
#define PAR_POS_PRO5_TIME09                      (PAR_POS_PRO5_HUMI09+1)       
#define PAR_POS_PRO5_TEMP10                      (PAR_POS_PRO5_TIME09+1)       
#define PAR_POS_PRO5_HUMI10                      (PAR_POS_PRO5_TEMP10+1)       
#define PAR_POS_PRO5_TIME10                      (PAR_POS_PRO5_HUMI10+1)       
#define PAR_POS_PRO5_TEMP11                      (PAR_POS_PRO5_TIME10+1)       
#define PAR_POS_PRO5_HUMI11                      (PAR_POS_PRO5_TEMP11+1)       
#define PAR_POS_PRO5_TIME11                      (PAR_POS_PRO5_HUMI11+1)       
#define PAR_POS_PRO5_TEMP12                      (PAR_POS_PRO5_TIME11+1)       
#define PAR_POS_PRO5_HUMI12                      (PAR_POS_PRO5_TEMP12+1)       
#define PAR_POS_PRO5_TIME12                      (PAR_POS_PRO5_HUMI12+1)       
#define PAR_POS_PRO5_TEMP13                      (PAR_POS_PRO5_TIME12+1)       
#define PAR_POS_PRO5_HUMI13                      (PAR_POS_PRO5_TEMP13+1)       
#define PAR_POS_PRO5_TIME13                      (PAR_POS_PRO5_HUMI13+1)       
#define PAR_POS_PRO5_TEMP14                      (PAR_POS_PRO5_TIME13+1)       
#define PAR_POS_PRO5_HUMI14                      (PAR_POS_PRO5_TEMP14+1)       
#define PAR_POS_PRO5_TIME14                      (PAR_POS_PRO5_HUMI14+1)       
#define PAR_POS_PRO5_TEMP15                      (PAR_POS_PRO5_TIME14+1)       
#define PAR_POS_PRO5_HUMI15                      (PAR_POS_PRO5_TEMP15+1)       
#define PAR_POS_PRO5_TIME15                      (PAR_POS_PRO5_HUMI15+1)       
#define PAR_POS_PRO5_TEMP16                      (PAR_POS_PRO5_TIME15+1)       
#define PAR_POS_PRO5_HUMI16                      (PAR_POS_PRO5_TEMP16+1)       
#define PAR_POS_PRO5_TIME16                      (PAR_POS_PRO5_HUMI16+1)       
#define PAR_POS_PRO5_TEMP17                      (PAR_POS_PRO5_TIME16+1)       
#define PAR_POS_PRO5_HUMI17                      (PAR_POS_PRO5_TEMP17+1)       
#define PAR_POS_PRO5_TIME17                      (PAR_POS_PRO5_HUMI17+1)       
#define PAR_POS_PRO5_TEMP18                      (PAR_POS_PRO5_TIME17+1)       
#define PAR_POS_PRO5_HUMI18                      (PAR_POS_PRO5_TEMP18+1)       
#define PAR_POS_PRO5_TIME18                      (PAR_POS_PRO5_HUMI18+1)       
#define PAR_POS_PRO5_TEMP19                      (PAR_POS_PRO5_TIME18+1)       
#define PAR_POS_PRO5_HUMI19                      (PAR_POS_PRO5_TEMP19+1)       
#define PAR_POS_PRO5_TIME19                      (PAR_POS_PRO5_HUMI19+1)       
#define PAR_POS_PRO5_TEMP20                      (PAR_POS_PRO5_TIME19+1)       
#define PAR_POS_PRO5_HUMI20                      (PAR_POS_PRO5_TEMP20+1)       
#define PAR_POS_PRO5_TIME20                      (PAR_POS_PRO5_HUMI20+1)       
#define PAR_POS_PRO5_TEMP21                      (PAR_POS_PRO5_TIME20+1)       
#define PAR_POS_PRO5_HUMI21                      (PAR_POS_PRO5_TEMP21+1)       
#define PAR_POS_PRO5_TIME21                      (PAR_POS_PRO5_HUMI21+1)       
#define PAR_POS_PRO5_TEMP22                      (PAR_POS_PRO5_TIME21+1)       
#define PAR_POS_PRO5_HUMI22                      (PAR_POS_PRO5_TEMP22+1)       
#define PAR_POS_PRO5_TIME22                      (PAR_POS_PRO5_HUMI22+1)       
#define PAR_POS_PRO5_TEMP23                      (PAR_POS_PRO5_TIME22+1)       
#define PAR_POS_PRO5_HUMI23                      (PAR_POS_PRO5_TEMP23+1)       
#define PAR_POS_PRO5_TIME23                      (PAR_POS_PRO5_HUMI23+1)       
#define PAR_POS_PRO5_TEMP24                      (PAR_POS_PRO5_TIME23+1)       
#define PAR_POS_PRO5_HUMI24                      (PAR_POS_PRO5_TEMP24+1)       
#define PAR_POS_PRO5_TIME24                      (PAR_POS_PRO5_HUMI24+1)       
#define PAR_POS_PRO5_TEMP25                      (PAR_POS_PRO5_TIME24+1)       
#define PAR_POS_PRO5_HUMI25                      (PAR_POS_PRO5_TEMP25+1)       
#define PAR_POS_PRO5_TIME25                      (PAR_POS_PRO5_HUMI25+1)       
#define PAR_POS_PRO5_TEMP26                      (PAR_POS_PRO5_TIME25+1)       
#define PAR_POS_PRO5_HUMI26                      (PAR_POS_PRO5_TEMP26+1)       
#define PAR_POS_PRO5_TIME26                      (PAR_POS_PRO5_HUMI26+1)       
#define PAR_POS_PRO5_TEMP27                      (PAR_POS_PRO5_TIME26+1)       
#define PAR_POS_PRO5_HUMI27                      (PAR_POS_PRO5_TEMP27+1)       
#define PAR_POS_PRO5_TIME27                      (PAR_POS_PRO5_HUMI27+1)       
#define PAR_POS_PRO5_TEMP28                      (PAR_POS_PRO5_TIME27+1)       
#define PAR_POS_PRO5_HUMI28                      (PAR_POS_PRO5_TEMP28+1)       
#define PAR_POS_PRO5_TIME28                      (PAR_POS_PRO5_HUMI28+1)       
#define PAR_POS_PRO5_TEMP29                      (PAR_POS_PRO5_TIME28+1)       
#define PAR_POS_PRO5_HUMI29                      (PAR_POS_PRO5_TEMP29+1)       
#define PAR_POS_PRO5_TIME29                      (PAR_POS_PRO5_HUMI29+1)   //718    

#define PAR_POS_PRO6_TEMP00                      719//(PAR_POS_PRO5_TIME29+1)       
#define PAR_POS_PRO6_HUMI00                      (PAR_POS_PRO6_TEMP00+1)       
#define PAR_POS_PRO6_TIME00                      (PAR_POS_PRO6_HUMI00+1)       
#define PAR_POS_PRO6_TEMP01                      (PAR_POS_PRO6_TIME00+1)       
#define PAR_POS_PRO6_HUMI01                      (PAR_POS_PRO6_TEMP01+1)       
#define PAR_POS_PRO6_TIME01                      (PAR_POS_PRO6_HUMI01+1)       
#define PAR_POS_PRO6_TEMP02                      (PAR_POS_PRO6_TIME01+1)       
#define PAR_POS_PRO6_HUMI02                      (PAR_POS_PRO6_TEMP02+1)       
#define PAR_POS_PRO6_TIME02                      (PAR_POS_PRO6_HUMI02+1)       
#define PAR_POS_PRO6_TEMP03                      (PAR_POS_PRO6_TIME02+1)       
#define PAR_POS_PRO6_HUMI03                      (PAR_POS_PRO6_TEMP03+1)       
#define PAR_POS_PRO6_TIME03                      (PAR_POS_PRO6_HUMI03+1)       
#define PAR_POS_PRO6_TEMP04                      (PAR_POS_PRO6_TIME03+1)       
#define PAR_POS_PRO6_HUMI04                      (PAR_POS_PRO6_TEMP04+1)       
#define PAR_POS_PRO6_TIME04                      (PAR_POS_PRO6_HUMI04+1)       
#define PAR_POS_PRO6_TEMP05                      (PAR_POS_PRO6_TIME04+1)       
#define PAR_POS_PRO6_HUMI05                      (PAR_POS_PRO6_TEMP05+1)       
#define PAR_POS_PRO6_TIME05                      (PAR_POS_PRO6_HUMI05+1)       
#define PAR_POS_PRO6_TEMP06                      (PAR_POS_PRO6_TIME05+1)       
#define PAR_POS_PRO6_HUMI06                      (PAR_POS_PRO6_TEMP06+1)       
#define PAR_POS_PRO6_TIME06                      (PAR_POS_PRO6_HUMI06+1)       
#define PAR_POS_PRO6_TEMP07                      (PAR_POS_PRO6_TIME06+1)       
#define PAR_POS_PRO6_HUMI07                      (PAR_POS_PRO6_TEMP07+1)       
#define PAR_POS_PRO6_TIME07                      (PAR_POS_PRO6_HUMI07+1)       
#define PAR_POS_PRO6_TEMP08                      (PAR_POS_PRO6_TIME07+1)       
#define PAR_POS_PRO6_HUMI08                      (PAR_POS_PRO6_TEMP08+1)       
#define PAR_POS_PRO6_TIME08                      (PAR_POS_PRO6_HUMI08+1)       
#define PAR_POS_PRO6_TEMP09                      (PAR_POS_PRO6_TIME08+1)       
#define PAR_POS_PRO6_HUMI09                      (PAR_POS_PRO6_TEMP09+1)       
#define PAR_POS_PRO6_TIME09                      (PAR_POS_PRO6_HUMI09+1)       
#define PAR_POS_PRO6_TEMP10                      (PAR_POS_PRO6_TIME09+1)       
#define PAR_POS_PRO6_HUMI10                      (PAR_POS_PRO6_TEMP10+1)       
#define PAR_POS_PRO6_TIME10                      (PAR_POS_PRO6_HUMI10+1)       
#define PAR_POS_PRO6_TEMP11                      (PAR_POS_PRO6_TIME10+1)       
#define PAR_POS_PRO6_HUMI11                      (PAR_POS_PRO6_TEMP11+1)       
#define PAR_POS_PRO6_TIME11                      (PAR_POS_PRO6_HUMI11+1)       
#define PAR_POS_PRO6_TEMP12                      (PAR_POS_PRO6_TIME11+1)       
#define PAR_POS_PRO6_HUMI12                      (PAR_POS_PRO6_TEMP12+1)       
#define PAR_POS_PRO6_TIME12                      (PAR_POS_PRO6_HUMI12+1)       
#define PAR_POS_PRO6_TEMP13                      (PAR_POS_PRO6_TIME12+1)       
#define PAR_POS_PRO6_HUMI13                      (PAR_POS_PRO6_TEMP13+1)       
#define PAR_POS_PRO6_TIME13                      (PAR_POS_PRO6_HUMI13+1)       
#define PAR_POS_PRO6_TEMP14                      (PAR_POS_PRO6_TIME13+1)       
#define PAR_POS_PRO6_HUMI14                      (PAR_POS_PRO6_TEMP14+1)       
#define PAR_POS_PRO6_TIME14                      (PAR_POS_PRO6_HUMI14+1)       
#define PAR_POS_PRO6_TEMP15                      (PAR_POS_PRO6_TIME14+1)       
#define PAR_POS_PRO6_HUMI15                      (PAR_POS_PRO6_TEMP15+1)       
#define PAR_POS_PRO6_TIME15                      (PAR_POS_PRO6_HUMI15+1)       
#define PAR_POS_PRO6_TEMP16                      (PAR_POS_PRO6_TIME15+1)       
#define PAR_POS_PRO6_HUMI16                      (PAR_POS_PRO6_TEMP16+1)       
#define PAR_POS_PRO6_TIME16                      (PAR_POS_PRO6_HUMI16+1)       
#define PAR_POS_PRO6_TEMP17                      (PAR_POS_PRO6_TIME16+1)       
#define PAR_POS_PRO6_HUMI17                      (PAR_POS_PRO6_TEMP17+1)       
#define PAR_POS_PRO6_TIME17                      (PAR_POS_PRO6_HUMI17+1)       
#define PAR_POS_PRO6_TEMP18                      (PAR_POS_PRO6_TIME17+1)       
#define PAR_POS_PRO6_HUMI18                      (PAR_POS_PRO6_TEMP18+1)       
#define PAR_POS_PRO6_TIME18                      (PAR_POS_PRO6_HUMI18+1)       
#define PAR_POS_PRO6_TEMP19                      (PAR_POS_PRO6_TIME18+1)       
#define PAR_POS_PRO6_HUMI19                      (PAR_POS_PRO6_TEMP19+1)       
#define PAR_POS_PRO6_TIME19                      (PAR_POS_PRO6_HUMI19+1)       
#define PAR_POS_PRO6_TEMP20                      (PAR_POS_PRO6_TIME19+1)       
#define PAR_POS_PRO6_HUMI20                      (PAR_POS_PRO6_TEMP20+1)       
#define PAR_POS_PRO6_TIME20                      (PAR_POS_PRO6_HUMI20+1)       
#define PAR_POS_PRO6_TEMP21                      (PAR_POS_PRO6_TIME20+1)       
#define PAR_POS_PRO6_HUMI21                      (PAR_POS_PRO6_TEMP21+1)       
#define PAR_POS_PRO6_TIME21                      (PAR_POS_PRO6_HUMI21+1)       
#define PAR_POS_PRO6_TEMP22                      (PAR_POS_PRO6_TIME21+1)       
#define PAR_POS_PRO6_HUMI22                      (PAR_POS_PRO6_TEMP22+1)       
#define PAR_POS_PRO6_TIME22                      (PAR_POS_PRO6_HUMI22+1)       
#define PAR_POS_PRO6_TEMP23                      (PAR_POS_PRO6_TIME22+1)       
#define PAR_POS_PRO6_HUMI23                      (PAR_POS_PRO6_TEMP23+1)       
#define PAR_POS_PRO6_TIME23                      (PAR_POS_PRO6_HUMI23+1)       
#define PAR_POS_PRO6_TEMP24                      (PAR_POS_PRO6_TIME23+1)       
#define PAR_POS_PRO6_HUMI24                      (PAR_POS_PRO6_TEMP24+1)       
#define PAR_POS_PRO6_TIME24                      (PAR_POS_PRO6_HUMI24+1)       
#define PAR_POS_PRO6_TEMP25                      (PAR_POS_PRO6_TIME24+1)       
#define PAR_POS_PRO6_HUMI25                      (PAR_POS_PRO6_TEMP25+1)       
#define PAR_POS_PRO6_TIME25                      (PAR_POS_PRO6_HUMI25+1)       
#define PAR_POS_PRO6_TEMP26                      (PAR_POS_PRO6_TIME25+1)       
#define PAR_POS_PRO6_HUMI26                      (PAR_POS_PRO6_TEMP26+1)       
#define PAR_POS_PRO6_TIME26                      (PAR_POS_PRO6_HUMI26+1)       
#define PAR_POS_PRO6_TEMP27                      (PAR_POS_PRO6_TIME26+1)       
#define PAR_POS_PRO6_HUMI27                      (PAR_POS_PRO6_TEMP27+1)       
#define PAR_POS_PRO6_TIME27                      (PAR_POS_PRO6_HUMI27+1)       
#define PAR_POS_PRO6_TEMP28                      (PAR_POS_PRO6_TIME27+1)       
#define PAR_POS_PRO6_HUMI28                      (PAR_POS_PRO6_TEMP28+1)       
#define PAR_POS_PRO6_TIME28                      (PAR_POS_PRO6_HUMI28+1)       
#define PAR_POS_PRO6_TEMP29                      (PAR_POS_PRO6_TIME28+1)       
#define PAR_POS_PRO6_HUMI29                      (PAR_POS_PRO6_TEMP29+1)       
#define PAR_POS_PRO6_TIME29                      (PAR_POS_PRO6_HUMI29+1)    //808   

#define PAR_POS_PRO7_TEMP00                      (PAR_POS_PRO6_TIME29+1)       
#define PAR_POS_PRO7_HUMI00                      (PAR_POS_PRO7_TEMP00+1)       
#define PAR_POS_PRO7_TIME00                      (PAR_POS_PRO7_HUMI00+1)       
#define PAR_POS_PRO7_TEMP01                      (PAR_POS_PRO7_TIME00+1)       
#define PAR_POS_PRO7_HUMI01                      (PAR_POS_PRO7_TEMP01+1)       
#define PAR_POS_PRO7_TIME01                      (PAR_POS_PRO7_HUMI01+1)       
#define PAR_POS_PRO7_TEMP02                      (PAR_POS_PRO7_TIME01+1)       
#define PAR_POS_PRO7_HUMI02                      (PAR_POS_PRO7_TEMP02+1)       
#define PAR_POS_PRO7_TIME02                      (PAR_POS_PRO7_HUMI02+1)       
#define PAR_POS_PRO7_TEMP03                      (PAR_POS_PRO7_TIME02+1)       
#define PAR_POS_PRO7_HUMI03                      (PAR_POS_PRO7_TEMP03+1)       
#define PAR_POS_PRO7_TIME03                      (PAR_POS_PRO7_HUMI03+1)       
#define PAR_POS_PRO7_TEMP04                      (PAR_POS_PRO7_TIME03+1)       
#define PAR_POS_PRO7_HUMI04                      (PAR_POS_PRO7_TEMP04+1)       
#define PAR_POS_PRO7_TIME04                      (PAR_POS_PRO7_HUMI04+1)       
#define PAR_POS_PRO7_TEMP05                      (PAR_POS_PRO7_TIME04+1)       
#define PAR_POS_PRO7_HUMI05                      (PAR_POS_PRO7_TEMP05+1)       
#define PAR_POS_PRO7_TIME05                      (PAR_POS_PRO7_HUMI05+1)       
#define PAR_POS_PRO7_TEMP06                      (PAR_POS_PRO7_TIME05+1)       
#define PAR_POS_PRO7_HUMI06                      (PAR_POS_PRO7_TEMP06+1)       
#define PAR_POS_PRO7_TIME06                      (PAR_POS_PRO7_HUMI06+1)       
#define PAR_POS_PRO7_TEMP07                      (PAR_POS_PRO7_TIME06+1)       
#define PAR_POS_PRO7_HUMI07                      (PAR_POS_PRO7_TEMP07+1)       
#define PAR_POS_PRO7_TIME07                      (PAR_POS_PRO7_HUMI07+1)       
#define PAR_POS_PRO7_TEMP08                      (PAR_POS_PRO7_TIME07+1)       
#define PAR_POS_PRO7_HUMI08                      (PAR_POS_PRO7_TEMP08+1)       
#define PAR_POS_PRO7_TIME08                      (PAR_POS_PRO7_HUMI08+1)       
#define PAR_POS_PRO7_TEMP09                      (PAR_POS_PRO7_TIME08+1)       
#define PAR_POS_PRO7_HUMI09                      (PAR_POS_PRO7_TEMP09+1)       
#define PAR_POS_PRO7_TIME09                      (PAR_POS_PRO7_HUMI09+1)       
#define PAR_POS_PRO7_TEMP10                      (PAR_POS_PRO7_TIME09+1)       
#define PAR_POS_PRO7_HUMI10                      (PAR_POS_PRO7_TEMP10+1)       
#define PAR_POS_PRO7_TIME10                      (PAR_POS_PRO7_HUMI10+1)       
#define PAR_POS_PRO7_TEMP11                      (PAR_POS_PRO7_TIME10+1)       
#define PAR_POS_PRO7_HUMI11                      (PAR_POS_PRO7_TEMP11+1)       
#define PAR_POS_PRO7_TIME11                      (PAR_POS_PRO7_HUMI11+1)       
#define PAR_POS_PRO7_TEMP12                      (PAR_POS_PRO7_TIME11+1)       
#define PAR_POS_PRO7_HUMI12                      (PAR_POS_PRO7_TEMP12+1)       
#define PAR_POS_PRO7_TIME12                      (PAR_POS_PRO7_HUMI12+1)       
#define PAR_POS_PRO7_TEMP13                      (PAR_POS_PRO7_TIME12+1)       
#define PAR_POS_PRO7_HUMI13                      (PAR_POS_PRO7_TEMP13+1)       
#define PAR_POS_PRO7_TIME13                      (PAR_POS_PRO7_HUMI13+1)       
#define PAR_POS_PRO7_TEMP14                      (PAR_POS_PRO7_TIME13+1)       
#define PAR_POS_PRO7_HUMI14                      (PAR_POS_PRO7_TEMP14+1)       
#define PAR_POS_PRO7_TIME14                      (PAR_POS_PRO7_HUMI14+1)       
#define PAR_POS_PRO7_TEMP15                      (PAR_POS_PRO7_TIME14+1)       
#define PAR_POS_PRO7_HUMI15                      (PAR_POS_PRO7_TEMP15+1)       
#define PAR_POS_PRO7_TIME15                      (PAR_POS_PRO7_HUMI15+1)       
#define PAR_POS_PRO7_TEMP16                      (PAR_POS_PRO7_TIME15+1)       
#define PAR_POS_PRO7_HUMI16                      (PAR_POS_PRO7_TEMP16+1)       
#define PAR_POS_PRO7_TIME16                      (PAR_POS_PRO7_HUMI16+1)       
#define PAR_POS_PRO7_TEMP17                      (PAR_POS_PRO7_TIME16+1)       
#define PAR_POS_PRO7_HUMI17                      (PAR_POS_PRO7_TEMP17+1)       
#define PAR_POS_PRO7_TIME17                      (PAR_POS_PRO7_HUMI17+1)       
#define PAR_POS_PRO7_TEMP18                      (PAR_POS_PRO7_TIME17+1)       
#define PAR_POS_PRO7_HUMI18                      (PAR_POS_PRO7_TEMP18+1)       
#define PAR_POS_PRO7_TIME18                      (PAR_POS_PRO7_HUMI18+1)       
#define PAR_POS_PRO7_TEMP19                      (PAR_POS_PRO7_TIME18+1)       
#define PAR_POS_PRO7_HUMI19                      (PAR_POS_PRO7_TEMP19+1)       
#define PAR_POS_PRO7_TIME19                      (PAR_POS_PRO7_HUMI19+1)       
#define PAR_POS_PRO7_TEMP20                      (PAR_POS_PRO7_TIME19+1)       
#define PAR_POS_PRO7_HUMI20                      (PAR_POS_PRO7_TEMP20+1)       
#define PAR_POS_PRO7_TIME20                      (PAR_POS_PRO7_HUMI20+1)       
#define PAR_POS_PRO7_TEMP21                      (PAR_POS_PRO7_TIME20+1)       
#define PAR_POS_PRO7_HUMI21                      (PAR_POS_PRO7_TEMP21+1)       
#define PAR_POS_PRO7_TIME21                      (PAR_POS_PRO7_HUMI21+1)       
#define PAR_POS_PRO7_TEMP22                      (PAR_POS_PRO7_TIME21+1)       
#define PAR_POS_PRO7_HUMI22                      (PAR_POS_PRO7_TEMP22+1)       
#define PAR_POS_PRO7_TIME22                      (PAR_POS_PRO7_HUMI22+1)       
#define PAR_POS_PRO7_TEMP23                      (PAR_POS_PRO7_TIME22+1)       
#define PAR_POS_PRO7_HUMI23                      (PAR_POS_PRO7_TEMP23+1)       
#define PAR_POS_PRO7_TIME23                      (PAR_POS_PRO7_HUMI23+1)       
#define PAR_POS_PRO7_TEMP24                      (PAR_POS_PRO7_TIME23+1)       
#define PAR_POS_PRO7_HUMI24                      (PAR_POS_PRO7_TEMP24+1)       
#define PAR_POS_PRO7_TIME24                      (PAR_POS_PRO7_HUMI24+1)       
#define PAR_POS_PRO7_TEMP25                      (PAR_POS_PRO7_TIME24+1)       
#define PAR_POS_PRO7_HUMI25                      (PAR_POS_PRO7_TEMP25+1)       
#define PAR_POS_PRO7_TIME25                      (PAR_POS_PRO7_HUMI25+1)       
#define PAR_POS_PRO7_TEMP26                      (PAR_POS_PRO7_TIME25+1)       
#define PAR_POS_PRO7_HUMI26                      (PAR_POS_PRO7_TEMP26+1)       
#define PAR_POS_PRO7_TIME26                      (PAR_POS_PRO7_HUMI26+1)       
#define PAR_POS_PRO7_TEMP27                      (PAR_POS_PRO7_TIME26+1)       
#define PAR_POS_PRO7_HUMI27                      (PAR_POS_PRO7_TEMP27+1)       
#define PAR_POS_PRO7_TIME27                      (PAR_POS_PRO7_HUMI27+1)       
#define PAR_POS_PRO7_TEMP28                      (PAR_POS_PRO7_TIME27+1)       
#define PAR_POS_PRO7_HUMI28                      (PAR_POS_PRO7_TEMP28+1)       
#define PAR_POS_PRO7_TIME28                      (PAR_POS_PRO7_HUMI28+1)       
#define PAR_POS_PRO7_TEMP29                      (PAR_POS_PRO7_TIME28+1)       
#define PAR_POS_PRO7_HUMI29                      (PAR_POS_PRO7_TEMP29+1)       
#define PAR_POS_PRO7_TIME29                      (PAR_POS_PRO7_HUMI29+1)      //898 

#define PAR_POS_PRO8_TEMP00                      890//(PAR_POS_PRO7_TIME29+1)       
#define PAR_POS_PRO8_HUMI00                      (PAR_POS_PRO8_TEMP00+1)       
#define PAR_POS_PRO8_TIME00                      (PAR_POS_PRO8_HUMI00+1)       
#define PAR_POS_PRO8_TEMP01                      (PAR_POS_PRO8_TIME00+1)       
#define PAR_POS_PRO8_HUMI01                      (PAR_POS_PRO8_TEMP01+1)       
#define PAR_POS_PRO8_TIME01                      (PAR_POS_PRO8_HUMI01+1)       
#define PAR_POS_PRO8_TEMP02                      (PAR_POS_PRO8_TIME01+1)       
#define PAR_POS_PRO8_HUMI02                      (PAR_POS_PRO8_TEMP02+1)       
#define PAR_POS_PRO8_TIME02                      (PAR_POS_PRO8_HUMI02+1)       
#define PAR_POS_PRO8_TEMP03                      (PAR_POS_PRO8_TIME02+1)       
#define PAR_POS_PRO8_HUMI03                      (PAR_POS_PRO8_TEMP03+1)       
#define PAR_POS_PRO8_TIME03                      (PAR_POS_PRO8_HUMI03+1)       
#define PAR_POS_PRO8_TEMP04                      (PAR_POS_PRO8_TIME03+1)       
#define PAR_POS_PRO8_HUMI04                      (PAR_POS_PRO8_TEMP04+1)       
#define PAR_POS_PRO8_TIME04                      (PAR_POS_PRO8_HUMI04+1)       
#define PAR_POS_PRO8_TEMP05                      (PAR_POS_PRO8_TIME04+1)       
#define PAR_POS_PRO8_HUMI05                      (PAR_POS_PRO8_TEMP05+1)       
#define PAR_POS_PRO8_TIME05                      (PAR_POS_PRO8_HUMI05+1)       
#define PAR_POS_PRO8_TEMP06                      (PAR_POS_PRO8_TIME05+1)       
#define PAR_POS_PRO8_HUMI06                      (PAR_POS_PRO8_TEMP06+1)       
#define PAR_POS_PRO8_TIME06                      (PAR_POS_PRO8_HUMI06+1)       
#define PAR_POS_PRO8_TEMP07                      (PAR_POS_PRO8_TIME06+1)       
#define PAR_POS_PRO8_HUMI07                      (PAR_POS_PRO8_TEMP07+1)       
#define PAR_POS_PRO8_TIME07                      (PAR_POS_PRO8_HUMI07+1)       
#define PAR_POS_PRO8_TEMP08                      (PAR_POS_PRO8_TIME07+1)       
#define PAR_POS_PRO8_HUMI08                      (PAR_POS_PRO8_TEMP08+1)       
#define PAR_POS_PRO8_TIME08                      (PAR_POS_PRO8_HUMI08+1)       
#define PAR_POS_PRO8_TEMP09                      (PAR_POS_PRO8_TIME08+1)       
#define PAR_POS_PRO8_HUMI09                      (PAR_POS_PRO8_TEMP09+1)       
#define PAR_POS_PRO8_TIME09                      (PAR_POS_PRO8_HUMI09+1)       
#define PAR_POS_PRO8_TEMP10                      (PAR_POS_PRO8_TIME09+1)       
#define PAR_POS_PRO8_HUMI10                      (PAR_POS_PRO8_TEMP10+1)       
#define PAR_POS_PRO8_TIME10                      (PAR_POS_PRO8_HUMI10+1)       
#define PAR_POS_PRO8_TEMP11                      (PAR_POS_PRO8_TIME10+1)       
#define PAR_POS_PRO8_HUMI11                      (PAR_POS_PRO8_TEMP11+1)       
#define PAR_POS_PRO8_TIME11                      (PAR_POS_PRO8_HUMI11+1)       
#define PAR_POS_PRO8_TEMP12                      (PAR_POS_PRO8_TIME11+1)       
#define PAR_POS_PRO8_HUMI12                      (PAR_POS_PRO8_TEMP12+1)       
#define PAR_POS_PRO8_TIME12                      (PAR_POS_PRO8_HUMI12+1)       
#define PAR_POS_PRO8_TEMP13                      (PAR_POS_PRO8_TIME12+1)       
#define PAR_POS_PRO8_HUMI13                      (PAR_POS_PRO8_TEMP13+1)       
#define PAR_POS_PRO8_TIME13                      (PAR_POS_PRO8_HUMI13+1)       
#define PAR_POS_PRO8_TEMP14                      (PAR_POS_PRO8_TIME13+1)       
#define PAR_POS_PRO8_HUMI14                      (PAR_POS_PRO8_TEMP14+1)       
#define PAR_POS_PRO8_TIME14                      (PAR_POS_PRO8_HUMI14+1)       
#define PAR_POS_PRO8_TEMP15                      (PAR_POS_PRO8_TIME14+1)       
#define PAR_POS_PRO8_HUMI15                      (PAR_POS_PRO8_TEMP15+1)       
#define PAR_POS_PRO8_TIME15                      (PAR_POS_PRO8_HUMI15+1)       
#define PAR_POS_PRO8_TEMP16                      (PAR_POS_PRO8_TIME15+1)       
#define PAR_POS_PRO8_HUMI16                      (PAR_POS_PRO8_TEMP16+1)       
#define PAR_POS_PRO8_TIME16                      (PAR_POS_PRO8_HUMI16+1)       
#define PAR_POS_PRO8_TEMP17                      (PAR_POS_PRO8_TIME16+1)       
#define PAR_POS_PRO8_HUMI17                      (PAR_POS_PRO8_TEMP17+1)       
#define PAR_POS_PRO8_TIME17                      (PAR_POS_PRO8_HUMI17+1)       
#define PAR_POS_PRO8_TEMP18                      (PAR_POS_PRO8_TIME17+1)       
#define PAR_POS_PRO8_HUMI18                      (PAR_POS_PRO8_TEMP18+1)       
#define PAR_POS_PRO8_TIME18                      (PAR_POS_PRO8_HUMI18+1)       
#define PAR_POS_PRO8_TEMP19                      (PAR_POS_PRO8_TIME18+1)       
#define PAR_POS_PRO8_HUMI19                      (PAR_POS_PRO8_TEMP19+1)       
#define PAR_POS_PRO8_TIME19                      (PAR_POS_PRO8_HUMI19+1)       
#define PAR_POS_PRO8_TEMP20                      (PAR_POS_PRO8_TIME19+1)       
#define PAR_POS_PRO8_HUMI20                      (PAR_POS_PRO8_TEMP20+1)       
#define PAR_POS_PRO8_TIME20                      (PAR_POS_PRO8_HUMI20+1)       
#define PAR_POS_PRO8_TEMP21                      (PAR_POS_PRO8_TIME20+1)       
#define PAR_POS_PRO8_HUMI21                      (PAR_POS_PRO8_TEMP21+1)       
#define PAR_POS_PRO8_TIME21                      (PAR_POS_PRO8_HUMI21+1)       
#define PAR_POS_PRO8_TEMP22                      (PAR_POS_PRO8_TIME21+1)       
#define PAR_POS_PRO8_HUMI22                      (PAR_POS_PRO8_TEMP22+1)       
#define PAR_POS_PRO8_TIME22                      (PAR_POS_PRO8_HUMI22+1)       
#define PAR_POS_PRO8_TEMP23                      (PAR_POS_PRO8_TIME22+1)       
#define PAR_POS_PRO8_HUMI23                      (PAR_POS_PRO8_TEMP23+1)       
#define PAR_POS_PRO8_TIME23                      (PAR_POS_PRO8_HUMI23+1)       
#define PAR_POS_PRO8_TEMP24                      (PAR_POS_PRO8_TIME23+1)       
#define PAR_POS_PRO8_HUMI24                      (PAR_POS_PRO8_TEMP24+1)       
#define PAR_POS_PRO8_TIME24                      (PAR_POS_PRO8_HUMI24+1)       
#define PAR_POS_PRO8_TEMP25                      (PAR_POS_PRO8_TIME24+1)       
#define PAR_POS_PRO8_HUMI25                      (PAR_POS_PRO8_TEMP25+1)       
#define PAR_POS_PRO8_TIME25                      (PAR_POS_PRO8_HUMI25+1)       
#define PAR_POS_PRO8_TEMP26                      (PAR_POS_PRO8_TIME25+1)       
#define PAR_POS_PRO8_HUMI26                      (PAR_POS_PRO8_TEMP26+1)       
#define PAR_POS_PRO8_TIME26                      (PAR_POS_PRO8_HUMI26+1)       
#define PAR_POS_PRO8_TEMP27                      (PAR_POS_PRO8_TIME26+1)       
#define PAR_POS_PRO8_HUMI27                      (PAR_POS_PRO8_TEMP27+1)       
#define PAR_POS_PRO8_TIME27                      (PAR_POS_PRO8_HUMI27+1)       
#define PAR_POS_PRO8_TEMP28                      (PAR_POS_PRO8_TIME27+1)       
#define PAR_POS_PRO8_HUMI28                      (PAR_POS_PRO8_TEMP28+1)       
#define PAR_POS_PRO8_TIME28                      (PAR_POS_PRO8_HUMI28+1)       
#define PAR_POS_PRO8_TEMP29                      (PAR_POS_PRO8_TIME28+1)       
#define PAR_POS_PRO8_HUMI29                      (PAR_POS_PRO8_TEMP29+1)       
#define PAR_POS_PRO8_TIME29                      (PAR_POS_PRO8_HUMI29+1)    //988   

#define PAR_POS_PRO9_TEMP00                      (PAR_POS_PRO8_TIME29+1)       
#define PAR_POS_PRO9_HUMI00                      (PAR_POS_PRO9_TEMP00+1)       
#define PAR_POS_PRO9_TIME00                      (PAR_POS_PRO9_HUMI00+1)       
#define PAR_POS_PRO9_TEMP01                      (PAR_POS_PRO9_TIME00+1)       
#define PAR_POS_PRO9_HUMI01                      (PAR_POS_PRO9_TEMP01+1)       
#define PAR_POS_PRO9_TIME01                      (PAR_POS_PRO9_HUMI01+1)       
#define PAR_POS_PRO9_TEMP02                      (PAR_POS_PRO9_TIME01+1)       
#define PAR_POS_PRO9_HUMI02                      (PAR_POS_PRO9_TEMP02+1)       
#define PAR_POS_PRO9_TIME02                      (PAR_POS_PRO9_HUMI02+1)       
#define PAR_POS_PRO9_TEMP03                      (PAR_POS_PRO9_TIME02+1)       
#define PAR_POS_PRO9_HUMI03                      (PAR_POS_PRO9_TEMP03+1)       
#define PAR_POS_PRO9_TIME03                      (PAR_POS_PRO9_HUMI03+1)       
#define PAR_POS_PRO9_TEMP04                      (PAR_POS_PRO9_TIME03+1)       
#define PAR_POS_PRO9_HUMI04                      (PAR_POS_PRO9_TEMP04+1)       
#define PAR_POS_PRO9_TIME04                      (PAR_POS_PRO9_HUMI04+1)       
#define PAR_POS_PRO9_TEMP05                      (PAR_POS_PRO9_TIME04+1)       
#define PAR_POS_PRO9_HUMI05                      (PAR_POS_PRO9_TEMP05+1)       
#define PAR_POS_PRO9_TIME05                      (PAR_POS_PRO9_HUMI05+1)       
#define PAR_POS_PRO9_TEMP06                      (PAR_POS_PRO9_TIME05+1)       
#define PAR_POS_PRO9_HUMI06                      (PAR_POS_PRO9_TEMP06+1)       
#define PAR_POS_PRO9_TIME06                      (PAR_POS_PRO9_HUMI06+1)       
#define PAR_POS_PRO9_TEMP07                      (PAR_POS_PRO9_TIME06+1)       
#define PAR_POS_PRO9_HUMI07                      (PAR_POS_PRO9_TEMP07+1)       
#define PAR_POS_PRO9_TIME07                      (PAR_POS_PRO9_HUMI07+1)       
#define PAR_POS_PRO9_TEMP08                      (PAR_POS_PRO9_TIME07+1)       
#define PAR_POS_PRO9_HUMI08                      (PAR_POS_PRO9_TEMP08+1)       
#define PAR_POS_PRO9_TIME08                      (PAR_POS_PRO9_HUMI08+1)       
#define PAR_POS_PRO9_TEMP09                      (PAR_POS_PRO9_TIME08+1)       
#define PAR_POS_PRO9_HUMI09                      (PAR_POS_PRO9_TEMP09+1)       
#define PAR_POS_PRO9_TIME09                      (PAR_POS_PRO9_HUMI09+1)       
#define PAR_POS_PRO9_TEMP10                      (PAR_POS_PRO9_TIME09+1)       
#define PAR_POS_PRO9_HUMI10                      (PAR_POS_PRO9_TEMP10+1)       
#define PAR_POS_PRO9_TIME10                      (PAR_POS_PRO9_HUMI10+1)       
#define PAR_POS_PRO9_TEMP11                      (PAR_POS_PRO9_TIME10+1)       
#define PAR_POS_PRO9_HUMI11                      (PAR_POS_PRO9_TEMP11+1)       
#define PAR_POS_PRO9_TIME11                      (PAR_POS_PRO9_HUMI11+1)       
#define PAR_POS_PRO9_TEMP12                      (PAR_POS_PRO9_TIME11+1)       
#define PAR_POS_PRO9_HUMI12                      (PAR_POS_PRO9_TEMP12+1)       
#define PAR_POS_PRO9_TIME12                      (PAR_POS_PRO9_HUMI12+1)       
#define PAR_POS_PRO9_TEMP13                      (PAR_POS_PRO9_TIME12+1)       
#define PAR_POS_PRO9_HUMI13                      (PAR_POS_PRO9_TEMP13+1)       
#define PAR_POS_PRO9_TIME13                      (PAR_POS_PRO9_HUMI13+1)       
#define PAR_POS_PRO9_TEMP14                      (PAR_POS_PRO9_TIME13+1)       
#define PAR_POS_PRO9_HUMI14                      (PAR_POS_PRO9_TEMP14+1)       
#define PAR_POS_PRO9_TIME14                      (PAR_POS_PRO9_HUMI14+1)       
#define PAR_POS_PRO9_TEMP15                      (PAR_POS_PRO9_TIME14+1)       
#define PAR_POS_PRO9_HUMI15                      (PAR_POS_PRO9_TEMP15+1)       
#define PAR_POS_PRO9_TIME15                      (PAR_POS_PRO9_HUMI15+1)       
#define PAR_POS_PRO9_TEMP16                      (PAR_POS_PRO9_TIME15+1)       
#define PAR_POS_PRO9_HUMI16                      (PAR_POS_PRO9_TEMP16+1)       
#define PAR_POS_PRO9_TIME16                      (PAR_POS_PRO9_HUMI16+1)       
#define PAR_POS_PRO9_TEMP17                      (PAR_POS_PRO9_TIME16+1)       
#define PAR_POS_PRO9_HUMI17                      (PAR_POS_PRO9_TEMP17+1)       
#define PAR_POS_PRO9_TIME17                      (PAR_POS_PRO9_HUMI17+1)       
#define PAR_POS_PRO9_TEMP18                      (PAR_POS_PRO9_TIME17+1)       
#define PAR_POS_PRO9_HUMI18                      (PAR_POS_PRO9_TEMP18+1)       
#define PAR_POS_PRO9_TIME18                      (PAR_POS_PRO9_HUMI18+1)       
#define PAR_POS_PRO9_TEMP19                      (PAR_POS_PRO9_TIME18+1)       
#define PAR_POS_PRO9_HUMI19                      (PAR_POS_PRO9_TEMP19+1)       
#define PAR_POS_PRO9_TIME19                      (PAR_POS_PRO9_HUMI19+1)       
#define PAR_POS_PRO9_TEMP20                      (PAR_POS_PRO9_TIME19+1)       
#define PAR_POS_PRO9_HUMI20                      (PAR_POS_PRO9_TEMP20+1)       
#define PAR_POS_PRO9_TIME20                      (PAR_POS_PRO9_HUMI20+1)       
#define PAR_POS_PRO9_TEMP21                      (PAR_POS_PRO9_TIME20+1)       
#define PAR_POS_PRO9_HUMI21                      (PAR_POS_PRO9_TEMP21+1)       
#define PAR_POS_PRO9_TIME21                      (PAR_POS_PRO9_HUMI21+1)       
#define PAR_POS_PRO9_TEMP22                      (PAR_POS_PRO9_TIME21+1)       
#define PAR_POS_PRO9_HUMI22                      (PAR_POS_PRO9_TEMP22+1)       
#define PAR_POS_PRO9_TIME22                      (PAR_POS_PRO9_HUMI22+1)       
#define PAR_POS_PRO9_TEMP23                      (PAR_POS_PRO9_TIME22+1)       
#define PAR_POS_PRO9_HUMI23                      (PAR_POS_PRO9_TEMP23+1)       
#define PAR_POS_PRO9_TIME23                      (PAR_POS_PRO9_HUMI23+1)       
#define PAR_POS_PRO9_TEMP24                      (PAR_POS_PRO9_TIME23+1)       
#define PAR_POS_PRO9_HUMI24                      (PAR_POS_PRO9_TEMP24+1)       
#define PAR_POS_PRO9_TIME24                      (PAR_POS_PRO9_HUMI24+1)       
#define PAR_POS_PRO9_TEMP25                      (PAR_POS_PRO9_TIME24+1)       
#define PAR_POS_PRO9_HUMI25                      (PAR_POS_PRO9_TEMP25+1)       
#define PAR_POS_PRO9_TIME25                      (PAR_POS_PRO9_HUMI25+1)       
#define PAR_POS_PRO9_TEMP26                      (PAR_POS_PRO9_TIME25+1)       
#define PAR_POS_PRO9_HUMI26                      (PAR_POS_PRO9_TEMP26+1)       
#define PAR_POS_PRO9_TIME26                      (PAR_POS_PRO9_HUMI26+1)       
#define PAR_POS_PRO9_TEMP27                      (PAR_POS_PRO9_TIME26+1)       
#define PAR_POS_PRO9_HUMI27                      (PAR_POS_PRO9_TEMP27+1)       
#define PAR_POS_PRO9_TIME27                      (PAR_POS_PRO9_HUMI27+1)       
#define PAR_POS_PRO9_TEMP28                      (PAR_POS_PRO9_TIME27+1)       
#define PAR_POS_PRO9_HUMI28                      (PAR_POS_PRO9_TEMP28+1)       
#define PAR_POS_PRO9_TIME28                      (PAR_POS_PRO9_HUMI28+1)       
#define PAR_POS_PRO9_TEMP29                      (PAR_POS_PRO9_TIME28+1)       
#define PAR_POS_PRO9_HUMI29                      (PAR_POS_PRO9_TEMP29+1)       
#define PAR_POS_PRO9_TIME29                      (PAR_POS_PRO9_HUMI29+1)    //1078   

#define PAR_POS_PRO0_REPEAT                      1079                      //(PAR_POS_PRO9_TIME29+1)       
#define PAR_POS_PRO1_REPEAT                      (PAR_POS_PRO0_REPEAT+1)      
#define PAR_POS_PRO2_REPEAT                      (PAR_POS_PRO1_REPEAT+1)       
#define PAR_POS_PRO3_REPEAT                      (PAR_POS_PRO2_REPEAT+1)      
#define PAR_POS_PRO4_REPEAT                      (PAR_POS_PRO3_REPEAT+1)   
#define PAR_POS_PRO5_REPEAT                      (PAR_POS_PRO4_REPEAT+1)     
#define PAR_POS_PRO6_REPEAT                      (PAR_POS_PRO5_REPEAT+1)      
#define PAR_POS_PRO7_REPEAT                      (PAR_POS_PRO6_REPEAT+1)     
#define PAR_POS_PRO8_REPEAT                      (PAR_POS_PRO7_REPEAT+1)      
#define PAR_POS_PRO9_REPEAT                      (PAR_POS_PRO8_REPEAT+1)   //1088          
#define PAR_POS_END                              (PAR_POS_PRO9_REPEAT+2)   


#define PAR_REDA_ADDR                        0X500
#define PAR_POS_CURRENT_YEAR				         PAR_REDA_ADDR+0    									//0
#define PAR_POS_CURRENT_MONTH               (PAR_POS_CURRENT_YEAR+1)              //1
#define PAR_POS_CURRENT_DAY                 (PAR_POS_CURRENT_MONTH+1)             //2
#define PAR_POS_CURRENT_HOUR                (PAR_POS_CURRENT_DAY+1)               //3
#define PAR_POS_CURRENT_MINUTE              (PAR_POS_CURRENT_HOUR+1)              //4
#define PAR_POS_TEMP_MONITOR                (PAR_POS_CURRENT_MINUTE+1)            //5 
#define PAR_POS_HUMI_MONITOR                (PAR_POS_TEMP_MONITOR+1)        		//6 




//以下为只读程序

#define PAR_POS_TEMP_DISPLAY                (PAR_POS_HUMI_MONITOR+1)                   //7
#define PAR_POS_HUMI_DISPLAY                (PAR_POS_TEMP_DISPLAY+1)              //8
#define PAR_POS_TEMP_POWER                  (PAR_POS_HUMI_DISPLAY+1)              //9
#define PAR_POS_HUMI_POWER                  (PAR_POS_TEMP_POWER+1)                //10
#define PAR_POS_WATER_LEVEL                 (PAR_POS_HUMI_POWER+1)               //11
#define PAR_POS_DOOR_STATE                  (PAR_POS_WATER_LEVEL+1)             //12
#define PAR_POS_RUN_COUNT_TIME              (PAR_POS_DOOR_STATE+1)             //13
#define PAR_POS_LEFT_TIME                   (PAR_POS_RUN_COUNT_TIME+1)             //14

#define PAR_POS_CURRENT_DUAN                (PAR_POS_LEFT_TIME+1)             //15
#define PAR_POS_PRO_CIRCLE_COUNT            (PAR_POS_CURRENT_DUAN+1)             //16


#define PAR_POS_PRO_ALARM_CODE              (PAR_POS_PRO_CIRCLE_COUNT+1)             //17
//#define PAR_POS_PRO_READ_END_DUAN           (PAR_POS_PRO_ALARM_CODE+1)             //18
//#define PAR_POS_PRO_READ_Start_LINE          (PAR_POS_PRO_ALARM_CODE+1)             //19


/* USER CODE BEGIN Prototypes */

#define  DoorOpen_State      0
#define  DoorClose_State     1
#define  Unlock_State        1
#define  Lock_State          0
#define  High_Water_Level_State  1

#define  Pump_Power_ON     HAL_GPIO_WritePin(Water_Input_Ctrl_GPIO_Port,Water_Input_Ctrl_Pin,GPIO_PIN_SET)
#define  Pump_Power_OFF    HAL_GPIO_WritePin(Water_Input_Ctrl_GPIO_Port,Water_Input_Ctrl_Pin,GPIO_PIN_RESET)

#define Hum_HEAT_OFF 		 	 HAL_GPIO_WritePin(Hum_Ctrl_GPIO_Port, Hum_Ctrl_Pin, GPIO_PIN_RESET)
#define Hum_HEAT_ON 	  	 HAL_GPIO_WritePin(Hum_Ctrl_GPIO_Port, Hum_Ctrl_Pin, GPIO_PIN_SET)
#define HEAT_OFF 		  		 HAL_GPIO_WritePin(GPIOC, Heat_Ctrl_Pin, GPIO_PIN_RESET)
#define HEAT_ON 	       	 HAL_GPIO_WritePin(GPIOC, Heat_Ctrl_Pin, GPIO_PIN_SET)
#define Water_Out_Delay_Pwr_ON         	 HAL_GPIO_WritePin(Water_Output_Ctrl_GPIO_Port, Water_Output_Ctrl_Pin, GPIO_PIN_RESET)
#define Water_Out_Delay_Pwr_OFF          HAL_GPIO_WritePin(Water_Output_Ctrl_GPIO_Port, Water_Output_Ctrl_Pin, GPIO_PIN_SET)


#define Ligt_OFF         	 HAL_GPIO_WritePin(Ctr_Light_GPIO_Port, Ctr_Light_Pin, GPIO_PIN_RESET)
#define Ligt_ON            HAL_GPIO_WritePin(Ctr_Light_GPIO_Port, Ctr_Light_Pin, GPIO_PIN_SET)

#define Defrost_ON        HAL_GPIO_WritePin(Ctr_Defrost_GPIO_Port,Ctr_Defrost_Pin, GPIO_PIN_SET)
#define Defrost_OFF       HAL_GPIO_WritePin(Ctr_Defrost_GPIO_Port,Ctr_Defrost_Pin, GPIO_PIN_RESET)

#define High_Level_Check    HAL_GPIO_ReadPin(Water_High_Level_Check_GPIO_Port,Water_High_Level_Check_Pin)
#define Low_Level_Check     HAL_GPIO_ReadPin(Water_Low_Level_Check_GPIO_Port,Water_Low_Level_Check_Pin)

#define Lock_State_Check    HAL_GPIO_ReadPin(Lock_State_GPIO_Port,Lock_State_Pin)
#define Door_State_Check    HAL_GPIO_ReadPin(Door_State_GPIO_Port,Door_State_Pin)
#define Dry_Protect_Check   HAL_GPIO_ReadPin(Dry_Protect_State_GPIO_Port,Dry_Protect_State_Pin)

#define Fan_ON              HAL_GPIO_WritePin(Ctr_FAN_GPIO_Port, Ctr_FAN_Pin, GPIO_PIN_SET)
#define Fan_OFF             HAL_GPIO_WritePin(Ctr_FAN_GPIO_Port, Ctr_FAN_Pin, GPIO_PIN_RESET)

#define ALM_ON      			  HAL_GPIO_WritePin(ALARM_GPIO_Port, ALARM_Pin, GPIO_PIN_RESET)
#define ALM_OFF     			  HAL_GPIO_WritePin(ALARM_GPIO_Port, ALARM_Pin, GPIO_PIN_SET)

#define  Cmpr_ON            HAL_GPIO_WritePin(Cmpr_Ctrl_GPIO_Port, Cmpr_Ctrl_Pin, GPIO_PIN_SET)
#define  Cmpr_OFF           HAL_GPIO_WritePin(Cmpr_Ctrl_GPIO_Port, Cmpr_Ctrl_Pin, GPIO_PIN_RESET)




typedef union {
    uint16_t uVal;
    int16_t iVal;
    uint8_t  uBytes[2];
    int8_t  iBytes[2];
} partab_type;
extern uint16_t CurrentDuan;
extern uint16_t ProCircleNum;

extern partab_type ParTab[PAR_POS_END + 2]; //工作参数
extern partab_type ParReceive[PAR_POS_END + 2]; //接收参数
extern partab_type ParSaved[PAR_POS_END + 2]; //接收已存储的参数
extern partab_type ParUnSaved[25];
//extern  uint8_t    ParReadSt[PAR_POS_END + 2]; //指示上位机已读该参数


typedef struct {
    uint16_t attr;
    int16_t  min;
    int16_t max;
    int16_t init;
    uint8_t code[20];
} partab_attr_type;




/* USER CODE END Prototypes */

#ifdef __cplusplus
}

#endif

#endif /* __ADC_H__ */

