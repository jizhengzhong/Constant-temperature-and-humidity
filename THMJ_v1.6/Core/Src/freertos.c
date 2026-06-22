/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */


/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for tid_Rtc */
osThreadId_t tid_RtcHandle;
const osThreadAttr_t tid_Rtc_attributes = {
  .name = "tid_Rtc",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for tid_modbus */
osThreadId_t tid_modbusHandle;
const osThreadAttr_t tid_modbus_attributes = {
  .name = "tid_modbus",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for tid_tempHandle */
osThreadId_t tid_tempHandleHandle;
const osThreadAttr_t tid_tempHandle_attributes = {
  .name = "tid_tempHandle",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};
/* Definitions for tid_Para */
osThreadId_t tid_ParaHandle;
const osThreadAttr_t tid_Para_attributes = {
  .name = "tid_Para",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow4,
};
/* Definitions for tid_APP */
osThreadId_t tid_APPHandle;
const osThreadAttr_t tid_APP_attributes = {
  .name = "tid_APP",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for tid_Ctrl */
osMessageQueueId_t FlowQueueHandle;
const osMessageQueueAttr_t FlowQueue_attributes = {
  .name = "FlowQueue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* Definitions for tid_Cloudy */
osThreadId_t tid_CloudyHandle;
const osThreadAttr_t tid_Cloudy_attributes = {
  .name = "tid_Cloudy",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
//EventGroupHandle_t xEventGroup;
float ADC_2_Temp(unsigned short int *pBuff,char counter);
unsigned int ADC_Filter( unsigned short int *pBuff ,char counter ) ;
float ADC_2_hum(unsigned short int *pBuff,char counter) ;
void CloudyTask(void *argument);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void RtcTask(void *argument);
void mbTask(void *argument);
void TempTask(void *argument);
void ParaTask(void *argument);
void APPTask(void *argument);
void CtrlTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of tid_Rtc */
  tid_RtcHandle = osThreadNew(RtcTask, NULL, &tid_Rtc_attributes);

  /* creation of tid_modbus */
  tid_modbusHandle = osThreadNew(mbTask, NULL, &tid_modbus_attributes);

  /* creation of tid_tempHandle */
  tid_tempHandleHandle = osThreadNew(TempTask, NULL, &tid_tempHandle_attributes);

  /* creation of tid_Para */
  tid_ParaHandle = osThreadNew(ParaTask, NULL, &tid_Para_attributes);

  /* creation of tid_APP */
  tid_APPHandle = osThreadNew(APPTask, NULL, &tid_APP_attributes);

  /* creation of tid_Ctrl */
  FlowQueueHandle = osMessageQueueNew (16, sizeof(uint16_t), &FlowQueue_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	  tid_CloudyHandle = osThreadNew(CloudyTask, NULL, &tid_Cloudy_attributes);
		
//		xEventGroup = xEventGroupCreate();
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_RtcTask */
/**
* @brief Function implementing the tid_Rtc thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_RtcTask */
void RtcTask(void *argument)
{
  /* USER CODE BEGIN RtcTask */
	  MX_RTC_Init();
  /* Infinite loop */
  for(;;)
  {
		
	  osThreadFlagsWait (PR_BIT_1, PR_BIT_1 , osWaitForever);	
		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN);
      osDelay(5);

  }
  /* USER CODE END RtcTask */
}

/* USER CODE BEGIN Header_mbTask */
/**
* @brief Function implementing the tid_modbus thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_mbTask */
__weak void mbTask(void *argument)
{
  /* USER CODE BEGIN mbTask */
	eMBErrorCode    eStatus;
	
	eStatus = eMBInit(MB_RTU,2,0,19200,MB_PAR_EVEN);
 if( eStatus != MB_ENOERR )
	{
	  printf("eMBInit_eStatus=%d",eStatus);	
	}
	
	/* Enable the Modbus Protocol Stack. */
	eStatus = eMBEnable(  );
	 if( eStatus != MB_ENOERR )
	{
	  printf("eMBEnable eStatus=%d",eStatus);	
	}
  /* Infinite loop */
  for(;;)
  {
		eMBPoll(  );
//				HAL_IWDG_Refresh(&hiwdg);
    osDelay(5);
  }
  /* USER CODE END mbTask */
}

/* USER CODE BEGIN Header_TempTask */
/**
* @brief Function implementing the tid_tempHandle thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_TempTask */
__weak void TempTask(void *argument)
{
  /* USER CODE BEGIN TempTask */
  /* Infinite loop */
  for(;;)
  {	
    osDelay(10);
  }
  /* USER CODE END TempTask */
}

/* USER CODE BEGIN Header_ParaTask */
/**
* @brief Function implementing the tid_Para thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ParaTask */
__weak void ParaTask(void *argument)
{
  /* USER CODE BEGIN ParaTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ParaTask */
}

/* USER CODE BEGIN Header_APPTask */
/**
* @brief Function implementing the tid_APP thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_APPTask */
__weak void APPTask(void *argument)
{
  /* USER CODE BEGIN APPTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END APPTask */
}

/* USER CODE BEGIN Header_CtrlTask */
/**
* @brief Function implementing the tid_Ctrl thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CtrlTask */
__weak void CtrlTask(void *argument)
{
  /* USER CODE BEGIN CtrlTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END CtrlTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

__weak void CloudyTask(void *argument)
{
  /* USER CODE BEGIN CloudyTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END CloudyTask */
}

/* USER CODE END Application */

