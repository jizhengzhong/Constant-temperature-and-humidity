/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//  extern  uint8_t Rebuff[5] ;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
//extern DMA_HandleTypeDef hdma_usart1_rx;
//extern DMA_HandleTypeDef hdma_usart1_tx;
//extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htim1;

/* USER CODE BEGIN EV */

 extern  void prvvTIMERExpiredISR( void );
 extern  void prvvUARTTxReadyISR( void );
 extern  void prvvUARTRxISR( void );
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel1 global interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel4 global interrupt.
  */
void DMA1_Channel4_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel4_IRQn 0 */

  /* USER CODE END DMA1_Channel4_IRQn 0 */
  HAL_DMA_IRQHandler(&U1_Control.hdma_tx);
  /* USER CODE BEGIN DMA1_Channel4_IRQn 1 */

  /* USER CODE END DMA1_Channel4_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel5 global interrupt.
  */
void DMA1_Channel5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel5_IRQn 0 */

  /* USER CODE END DMA1_Channel5_IRQn 0 */
  HAL_DMA_IRQHandler(&U1_Control.hdma_rx);
  /* USER CODE BEGIN DMA1_Channel5_IRQn 1 */

  /* USER CODE END DMA1_Channel5_IRQn 1 */
}

/**
  * @brief This function handles ADC1 and ADC2 global interrupts.
  */
void ADC1_2_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_2_IRQn 0 */

  /* USER CODE END ADC1_2_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  /* USER CODE BEGIN ADC1_2_IRQn 1 */

  /* USER CODE END ADC1_2_IRQn 1 */
}

/**
  * @brief This function handles TIM1 update interrupt.
  */
void TIM1_UP_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_IRQn 0 */

  /* USER CODE END TIM1_UP_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_IRQn 1 */

  /* USER CODE END TIM1_UP_IRQn 1 */
}

/**
  * @brief This function handles TIM4 global interrupt.
  */
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */

  /* USER CODE END TIM4_IRQn 0 */
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM4_IRQn 1 */
	  HAL_TIM_IRQHandler(&htim4);
	
	 if(__HAL_TIM_GET_IT_SOURCE(&htim4, TIM_FLAG_UPDATE))	       // 更新中断标记被置位
    {
        __HAL_TIM_CLEAR_IT(&htim4, TIM_FLAG_UPDATE);		// 清除中断标记
        prvvTIMERExpiredISR();			               // 通知modbus3.5个字符等待时间到
    }

  /* USER CODE END TIM4_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&U1_Control.UART_Handler);
  /* USER CODE BEGIN USART1_IRQn 1 */
	if(__HAL_UART_GET_FLAG(&U1_Control.UART_Handler,UART_FLAG_IDLE) != RESET){	                                 //如果发生空闲中断      	
		__HAL_UART_CLEAR_IDLEFLAG(&U1_Control.UART_Handler);                                                     //清除空闲中断标志 				
		HAL_UART_DMAStop(&U1_Control.UART_Handler);		                                                         //停止DMA
		U1_Control.U_RxCounter += (U1_DMARX_SIZE - __HAL_DMA_GET_COUNTER(&U1_Control.hdma_rx));                  //计算本次的接收数据量，定位结束位置
		U1_Control.URxDataInPtr->EndPtr = &U1_RxBuff[U1_Control.U_RxCounter];                                    //记录本次接收的结束位置		
				
		U1_Control.URxDataInPtr++;                                                                               //数据IN指针下移
		if(U1_Control.URxDataInPtr==U1_Control.URxDataEndPtr)                                                    //如果指针到处理接收数据的结构体数组尾部了
			U1_Control.URxDataInPtr = &U1_Control.UCB_RxBuffPtrCB[0];                                            //指针归位到处理接收数据的结构体数组的起始位置，也就是0号成员
		U1_Control.URxDataInPtr->StartPtr = &U1_RxBuff[U1_Control.U_RxCounter];                                  //记录下次接收的起始位置
		
		if(U1_RXBUFF_SIZE - U1_Control.U_RxCounter >= U1_DMARX_SIZE){                                            //如果剩余空间大于 DMA接收的单次数据量 进入if
			HAL_UART_Receive_DMA(&U1_Control.UART_Handler, &U1_RxBuff[U1_Control.U_RxCounter], U1_DMARX_SIZE);   //启动DMA接收
            U1_Control.URxDataInPtr->StartPtr = &U1_RxBuff[U1_Control.U_RxCounter];                              //记录下次接收的起始位置	
		}else{                                                                                                   //如果剩余空间不够单次接收量 进入else
			U1_Control.U_RxCounter = 0;                                                                          //U1_Control.U_RxCounter 清零
			HAL_UART_Receive_DMA(&U1_Control.UART_Handler, U1_RxBuff, U1_DMARX_SIZE);                            //启动DMA接收
			U1_Control.URxDataInPtr->StartPtr = U1_RxBuff;                                                       //记录下次接收的起始位置
		}
	}
  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */

  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */

	if(__HAL_UART_GET_IT_SOURCE(&huart3,UART_IT_RXNE)  )
	{
		__HAL_UART_CLEAR_FLAG(&huart3, UART_FLAG_RXNE);			// 清除中断标记
		prvvUARTRxISR();
		
	}
	if(__HAL_UART_GET_IT_SOURCE(&huart3,UART_IT_TXE))
	{
    __HAL_UART_CLEAR_FLAG(&huart3, UART_FLAG_TXE);			// 清除中断标记
		prvvUARTTxReadyISR();
		
	}

  /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief This function handles TIM5 global interrupt.
  */
void TIM5_IRQHandler(void)
{
  /* USER CODE BEGIN TIM5_IRQn 0 */

  /* USER CODE END TIM5_IRQn 0 */
  HAL_TIM_IRQHandler(&htim5);
  /* USER CODE BEGIN TIM5_IRQn 1 */
	Time_Base1ms++;//1ms累加
  TimMs_ModbusWait++;
 	RunFlow.FanOutMs++;
	if(RunFlow.FanOutMs>=100)
		 RunFlow.FanOutMs=0;
    if(Time_Base1ms >= 1000)
    {
        RunFlow.TenSecond++;
			  if(RunFlow.TenSecond>=10)
					RunFlow.StateParameter.TenSecondFlag=1;
        RunFlow.SteriSecond++;
        Time_Base1ms=0;//清零
        Time_PowerInital++;//上电计时 
        Temperature.Time_HeatCycle++;//加热控制周期计时
        if(CONTROL.Flag_EnRun)
				{
					Time_runsec++;   //编程模式运行时间秒
					RunFlow.RunTime++;
				}
        else Time_runsec=0;
        Time_5min++;//5分钟计时
        Time_Base1min++;
        if(Time_Base1min >= 60)
        {
						RunFlow.StateParameter.MinuteFlag = 1;
						RunFlow.SteriSecond = 0;
			      RunFlow.SteriMinute++;
            Time_Base1min = 0;
            if(CONTROL.Flag_EnRun)Time_runmin++;//编程模式运行时间分
            else Time_runmin = 0;
        }
    }
		


    Temperature.Time_HeatCounter++;//加热控制时间计时


//    if(COM_flag==1)//通讯未完成开始计时
//        Time_Out++;//通讯超时时间

  /* USER CODE END TIM5_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
//  UNUSED(huart);
//	 HAL_UART_Receive_IT();
	  __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);  
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_ErrorCallback could be implemented in the user file
   */
}



/* USER CODE END 1 */

