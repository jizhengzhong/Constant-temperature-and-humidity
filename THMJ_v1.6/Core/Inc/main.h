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

	  #include <stdint.h>
	  #include "adc.h"
  	#include "ADCSample.h"
		#include "FreeRTOS.h"
		#include "contrl.h"
	  #include "appmange.h"
	  #include "malloc.h"	
    #include "flow.h"  
    #include "iomanage.h" 
		#include "eeprom.h"
		#include "paratask.h"
    #include "cmsis_os.h"
		#include "iwdg.h"
		#include "event_groups.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern   RTC_TimeTypeDef sTime;
extern   RTC_DateTypeDef DateToUpdate ;  
extern   RTC_HandleTypeDef hrtc;
extern   osThreadId_t tid_tempHandleHandle;
extern   osThreadId_t tid_ParaHandle;
extern   osThreadId_t tid_RtcHandle;
//extern osMessageQueueId_t FlowQueueHandle;
//extern osEventFlagsId_t MyStartStopEventHandle;


/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Ctr_CKF_Pin GPIO_PIN_3
#define Ctr_CKF_GPIO_Port GPIOE
#define Ctr_FAN_Pin GPIO_PIN_4
#define Ctr_FAN_GPIO_Port GPIOE
#define PTA1_Pin GPIO_PIN_0
#define PTA1_GPIO_Port GPIOA
#define PTA2_Pin GPIO_PIN_1
#define PTA2_GPIO_Port GPIOA
#define PTA3_Pin GPIO_PIN_2
#define PTA3_GPIO_Port GPIOA
#define YL_ADC_Pin GPIO_PIN_7
#define YL_ADC_GPIO_Port GPIOA

#define WL_ADC_Pin GPIO_PIN_5
#define WL_ADC_GPIO_Port GPIOA

#define Ctr_Lock_Pin GPIO_PIN_9
#define Ctr_Lock_GPIO_Port GPIOE
#define Ctr_Beep_Pin GPIO_PIN_10
#define Ctr_Beep_GPIO_Port GPIOE
#define SP2_CS_Pin GPIO_PIN_12
#define SP2_CS_GPIO_Port GPIOB
#define Screen_RX_Pin GPIO_PIN_8
#define Screen_RX_GPIO_Port GPIOD
#define Screen_Tx_Pin GPIO_PIN_9
#define Screen_Tx_GPIO_Port GPIOD
#define Lock_State_Pin GPIO_PIN_11
#define Lock_State_GPIO_Port GPIOD
#define Door_State_Pin GPIO_PIN_10
#define Door_State_GPIO_Port GPIOD
#define Heat_Ctrl_Pin GPIO_PIN_6
#define Heat_Ctrl_GPIO_Port GPIOC
#define Dry_Ctrl_Pin GPIO_PIN_7
#define Dry_Ctrl_GPIO_Port GPIOC
#define Hum_Ctrl_Pin GPIO_PIN_8
#define Hum_Ctrl_GPIO_Port GPIOC
#define Water_High_Level_Check_Pin GPIO_PIN_3
#define Water_High_Level_Check_GPIO_Port GPIOD
#define Water_Low_Level_Check_Pin GPIO_PIN_4
#define Water_Low_Level_Check_GPIO_Port GPIOD
#define Dry_Protect_State_Pin GPIO_PIN_7
#define Dry_Protect_State_GPIO_Port GPIOD
#define AirAdmissionCtrl_Pin GPIO_PIN_2
#define AirAdmissionCtrl_GPIO_Port GPIOE
#define Water_Output_Ctrl_Pin GPIO_PIN_1
#define Water_Output_Ctrl_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */
// 定义事件位掩码
#define TP_BIT_0 (1 << 0)
#define PR_BIT_1 (1 << 1)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
