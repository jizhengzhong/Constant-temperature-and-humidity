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
int modbuswaterlevel ,appwaterlevel,rtcwaterlevel,defaultwaterlever,prograwatrlevel,Parawaterlevel;

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint32_t osTimerTest;

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
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for tid_modbus */
osThreadId_t tid_modbusHandle;
const osThreadAttr_t tid_modbus_attributes = {
  .name = "tid_modbus",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for tid_APP */
osThreadId_t tid_APPHandle;
const osThreadAttr_t tid_APP_attributes = {
  .name = "tid_APP",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for tid_PromodeTask */
osThreadId_t tid_PromodeTaskHandle;
const osThreadAttr_t tid_PromodeTask_attributes = {
  .name = "tid_PromodeTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for tid_ParaSave */
osThreadId_t tid_ParaSaveHandle;
const osThreadAttr_t tid_ParaSave_attributes = {
  .name = "tid_ParaSave",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for tid_CheckIn */
osThreadId_t tid_CheckInHandle;
const osThreadAttr_t tid_CheckIn_attributes = {
  .name = "tid_CheckIn",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for ReadonlyQueue0 */
osMessageQueueId_t ReadonlyQueue0Handle;
const osMessageQueueAttr_t ReadonlyQueue0_attributes = {
  .name = "ReadonlyQueue0"
};
/* Definitions for ADC_Timer */
osTimerId_t ADC_TimerHandle;
const osTimerAttr_t ADC_Timer_attributes = {
  .name = "ADC_Timer"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void RtcTask(void *argument);
void mbTask(void *argument);
void APPTask(void *argument);
void ProgramemodeTask(void *argument);
void ParaTask(void *argument);
void CheckInTask(void *argument);
void ADC_Callback(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
    osStatus_t  status; 

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of ADC_Timer */
  ADC_TimerHandle = osTimerNew(ADC_Callback, osTimerPeriodic, NULL, &ADC_Timer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
	if( ADC_TimerHandle != NULL )
	{
	  status = osTimerStart	(	ADC_TimerHandle,1000);	
		   if( status != osOK)
				 printf(" osTimerStart failed!");
	}
	
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of ReadonlyQueue0 */
  ReadonlyQueue0Handle = osMessageQueueNew (4, sizeof(uint16_t), &ReadonlyQueue0_attributes);

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

  /* creation of tid_APP */
  tid_APPHandle = osThreadNew(APPTask, NULL, &tid_APP_attributes);

  /* creation of tid_PromodeTask */
  tid_PromodeTaskHandle = osThreadNew(ProgramemodeTask, NULL, &tid_PromodeTask_attributes);

  /* creation of tid_ParaSave */
  tid_ParaSaveHandle = osThreadNew(ParaTask, NULL, &tid_ParaSave_attributes);

  /* creation of tid_CheckIn */
  tid_CheckInHandle = osThreadNew(CheckInTask, NULL, &tid_CheckIn_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
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

void FunStartStopCheck(void)
{
    static char LastPAR_POS_Run_Stop_Command;

    if (ParTab[PAR_POS_Run_Stop_Command].uVal != LastPAR_POS_Run_Stop_Command) {
        ParTab[PAR_POS_UPDATE_TIME].uVal = 0;
        if (ParTab[PAR_POS_Run_Stop_Command].uVal == 1) {
				    	ProCircleNum = ParTab[PAR_POS_PRO_Set_Circle_Count].uVal;
 					  TimeUpdate();
            PrintStart();
	          printf(" Temperature SV: %0.2f¡æ\r\n",SensorType.Parameter[TemperatureID].Target);
					  printf(" Humidity SV: %0.2f%%rh\r\n",SensorType.Parameter[HumidityID].Target);
					  printf("------------------------\r\n");
					  SensorType.PrintmilliSecond = 0;
					  SensorType.PrintLastSecond = sTime.Seconds;
        } else {
            ParTab[PAR_POS_UPDATE_TIME].uVal = 1;
        }

        LastPAR_POS_Run_Stop_Command =  ParTab[PAR_POS_Run_Stop_Command].uVal;
    }
}
void PrintData(partab_type * pTab,TypeCtrl *pSensorValue)
{
	 uint32_t MillSecond;
   if(  pTab[PAR_POS_PRINT_INTERVAL].uVal>0 && pTab[PAR_POS_Run_Stop_Command].uVal == 1 )
	 {
		  MillSecond = pTab[PAR_POS_PRINT_INTERVAL].uVal*60*1000-50*1000;
		  if( SensorType.PrintmilliSecond >= MillSecond  && sTime.Seconds == SensorType.PrintLastSecond)
			{
				printf("20%d/%02d/%02d %02d:%02d:%02d, %0.1f¡æ,%0.1f%%rh\r\n", DateToUpdate.Year, DateToUpdate.Month, DateToUpdate.Date,  \
				sTime.Hours, sTime.Minutes, sTime.Seconds,pSensorValue->Parameter[TemperatureID].Display*0.1,pSensorValue->Parameter[HumidityID].Display*0.1);
				SensorType.PrintmilliSecond = 0;
			}
	 }

}
void RtcTask(void *argument)
{
  /* USER CODE BEGIN RtcTask */
	  MX_RTC_Init();
  /* Infinite loop */
  for(;;)
  {
		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN);
		FunStartStopCheck();
		PrintData(&ParTab[0], &SensorType);
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
		eMBPoll();
    osDelay(5);
  }
  /* USER CODE END mbTask */
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

/* USER CODE BEGIN Header_ProgramemodeTask */
/**
* @brief Function implementing the tid_PromodeTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ProgramemodeTask */
__weak void ProgramemodeTask(void *argument)
{
  /* USER CODE BEGIN ProgramemodeTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ProgramemodeTask */
}

/* USER CODE BEGIN Header_ParaTask */
/**
* @brief Function implementing the tid_ParaSave thread.
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

/* USER CODE BEGIN Header_CheckInTask */
/**
* @brief Function implementing the tid_CheckIn thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CheckInTask */
__weak void CheckInTask(void *argument)
{
  /* USER CODE BEGIN CheckInTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END CheckInTask */
}

/* ADC_Callback function */
void ADC_Callback(void *argument)
{
  /* USER CODE BEGIN ADC_Callback */
	
//	int modbuswaterlevel ,appwaterlevel,rtcwaterlevel,defaultwaterlever;
	modbuswaterlevel=	uxTaskGetStackHighWaterMark(tid_modbusHandle);
	appwaterlevel=	uxTaskGetStackHighWaterMark(tid_APPHandle);
	Parawaterlevel = uxTaskGetStackHighWaterMark(tid_ParaSaveHandle);
	prograwatrlevel=	uxTaskGetStackHighWaterMark(tid_PromodeTaskHandle);
	rtcwaterlevel=	uxTaskGetStackHighWaterMark(tid_RtcHandle);
	osTimerTest++;
  /* USER CODE END ADC_Callback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */



/* USER CODE END Application */

