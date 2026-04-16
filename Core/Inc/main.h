/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
	
	  #include "mb.h"
	  #include "stdio.h"
    #include "rtc.h"
	  #include "display.h"
	  #include <stdint.h>
	  #include "adc.h"
  	#include "ADCSample.h"
		#include "FreeRTOS.h"
		#include "contrl.h"

	  #include "malloc.h"	
    #include "flow.h"  
    #include "iomanage.h" 
		#include "eeprom.h"
		#include "paratask.h"
    #include "cmsis_os.h"
		#include "stdbool.h"
	  #include "appmange.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

    /* 7.18.1.1 */

    /* exact-width signed integer types */
typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;
typedef   int8_t           s8;
typedef   int16_t          s16;
typedef   int32_t          s32;


    /* exact-width unsigned integer types */
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;

typedef   uint8_t           u8;
typedef   uint16_t          u16;
typedef   uint32_t          u32;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern   RTC_TimeTypeDef sTime;
extern   RTC_DateTypeDef DateToUpdate ;  
extern   RTC_HandleTypeDef hrtc;



//extern osMessageQueueId_t FlowQueueHandle;
//extern osEventFlagsId_t MyStartStopEventHandle;


/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */


/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern osMessageQueueId_t ReadonlyQueue0Handle;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Ctr_Defrost_Pin GPIO_PIN_3
#define Ctr_Defrost_GPIO_Port GPIOE
#define Ctr_FAN_Pin GPIO_PIN_4
#define Ctr_FAN_GPIO_Port GPIOE
#define Cmpr_Ctrl_Pin GPIO_PIN_6
#define Cmpr_Ctrl_GPIO_Port GPIOE
#define Suction_Ctrl_Pin GPIO_PIN_9
#define Suction_Ctrl_GPIO_Port GPIOE
#define PTA1_Pin GPIO_PIN_0
#define PTA1_GPIO_Port GPIOA
#define PTA2_Pin GPIO_PIN_1
#define PTA2_GPIO_Port GPIOA
#define PTA3_Pin GPIO_PIN_2
#define PTA3_GPIO_Port GPIOA
#define ALARM_Pin GPIO_PIN_10
#define ALARM_GPIO_Port GPIOE
#define Ctr_Light_Pin GPIO_PIN_2
#define Ctr_Light_GPIO_Port GPIOE
#define SP2_CS_Pin GPIO_PIN_12
#define SP2_CS_GPIO_Port GPIOB
#define Screen_RX_Pin GPIO_PIN_8
#define Screen_RX_GPIO_Port GPIOD
#define Screen_Tx_Pin GPIO_PIN_9
#define Screen_Tx_GPIO_Port GPIOD
#define Door_State_Pin GPIO_PIN_10
#define Door_State_GPIO_Port GPIOD
#define Heat_Ctrl_Pin GPIO_PIN_6
#define Heat_Ctrl_GPIO_Port GPIOC
#define Hum_Ctrl_Pin GPIO_PIN_7
#define Hum_Ctrl_GPIO_Port GPIOC
#define Water_High_Level_Check_Pin GPIO_PIN_4
#define Water_High_Level_Check_GPIO_Port GPIOD
#define Water_Low_Level_Check_Pin GPIO_PIN_7
#define Water_Low_Level_Check_GPIO_Port GPIOD
#define Water_Input_Ctrl_Pin GPIO_PIN_0
#define Water_Input_Ctrl_GPIO_Port GPIOE
#define Water_Output_Ctrl_Pin GPIO_PIN_1
#define Water_Output_Ctrl_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
