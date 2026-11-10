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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
#include "usart.h"
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
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htim1;

/* USER CODE BEGIN EV */

extern  void prvvTIMERExpiredISR(void);
extern  void prvvUARTTxReadyISR(void);
extern  void prvvUARTRxISR(void);

/* ͣ�� HardFault ���⼸�hf_pc �ǳ���������ָ���ַ�� */
volatile uint32_t hf_r0;
volatile uint32_t hf_r1;
volatile uint32_t hf_r2;
volatile uint32_t hf_r3;
volatile uint32_t hf_r12;
volatile uint32_t hf_lr;
volatile uint32_t hf_pc;
volatile uint32_t hf_psr;
volatile uint32_t hf_cfsr;
volatile uint32_t hf_hfsr;
volatile uint32_t hf_bfar;
volatile uint32_t hf_mmfar;

void HardFault_Dump(uint32_t *stack)
{
    hf_r0  = stack[0];
    hf_r1  = stack[1];
    hf_r2  = stack[2];
    hf_r3  = stack[3];
    hf_r12 = stack[4];
    hf_lr  = stack[5];
    hf_pc  = stack[6];
    hf_psr = stack[7];
    hf_cfsr = SCB->CFSR;
    hf_hfsr = SCB->HFSR;
    hf_bfar = SCB->BFAR;
    hf_mmfar = SCB->MMFAR;
    while (1) {
    }
}
/* eMBInit() ֮ǰ��Щָ���� NULL���ж���ֱ�ӵ��û� HardFault */
extern unsigned char (*pxMBFrameCBByteReceived)(void);
extern unsigned char (*pxMBFrameCBTransmitterEmpty)(void);
extern unsigned char (*pxMBPortCBTimerExpired)(void);
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
    while (1) {
    }

    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
/* ������ C ��������ջ֮֡ǰȡ�� MSP/PSP�������õ��Ĳ���Ӳ��ѹջ�� */
__asm void HardFault_Handler(void)
{
    IMPORT  HardFault_Dump
    TST     LR, #4
    ITE     EQ
    MRSEQ   R0, MSP
    MRSNE   R0, PSP
    B       HardFault_Dump
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
    /* USER CODE BEGIN MemoryManagement_IRQn 0 */

    /* USER CODE END MemoryManagement_IRQn 0 */
    while (1) {
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
    while (1) {
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
    while (1) {
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
    /* USER CODE BEGIN TIM4_IRQn 1 */
    /* ����ͬʱ�������±�־��ʹ��λ��ֻ��ʹ��λʱ����ʼ�����µ� UIF ���ڻص���δ��ֵʱ���ÿ�ָ�롣 */
    if ((__HAL_TIM_GET_FLAG(&htim4, TIM_FLAG_UPDATE) != RESET) &&
        (__HAL_TIM_GET_IT_SOURCE(&htim4, TIM_IT_UPDATE) != RESET)) {
        __HAL_TIM_CLEAR_IT(&htim4, TIM_IT_UPDATE);
        if (pxMBPortCBTimerExpired != NULL) {
            prvvTIMERExpiredISR();
        }
    }
    /* USER CODE END TIM4_IRQn 1 */
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
    uint32_t sr;
    uint32_t cr1;
    /* USER CODE BEGIN USART3_IRQn 0 */
    /* USER CODE END USART3_IRQn 0 */
    /* Modbus ��ռ USART3��HAL_UART_IRQHandler �ڳ���ʱ���� ErrorCallback��
       �� ErrorCallback ԭ�Ȼ��� eMBInit() ֮ǰ�� RXNE���Ӷ����ÿյ� pxMBFrameCBByteReceived�� */
    /* USER CODE BEGIN USART3_IRQn 1 */
    sr = USART3->SR;
    cr1 = USART3->CR1;

    if (((sr & USART_SR_RXNE) != 0U) && ((cr1 & USART_CR1_RXNEIE) != 0U)) {
        if (pxMBFrameCBByteReceived != NULL) {
            prvvUARTRxISR();
        } else {
            (void)USART3->DR;
        }
    } else if ((sr & USART_SR_ORE) != 0U) {
        (void)USART3->DR;
    }

    if (((sr & USART_SR_TXE) != 0U) && ((cr1 & USART_CR1_TXEIE) != 0U)) {
        if (pxMBFrameCBTransmitterEmpty != NULL) {
            prvvUARTTxReadyISR();
        } else {
            USART3->CR1 &= ~USART_CR1_TXEIE;
        }
    }
    /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief This function handles TIM5 global interrupt.
  */
void TIM5_IRQHandler(void)
{
    /* USER CODE BEGIN TIM5_IRQn 0 */
    static uint16_t MS = 0;
    /* USER CODE END TIM5_IRQn 0 */
    HAL_TIM_IRQHandler(&htim5);
    /* USER CODE BEGIN TIM5_IRQn 1 */
    MS++;
    if (MS >= 1000) {
        MS = 0;
			  Cloudy.Second++ ;
			  SensorType.Parameter[HumidityID].DiffStableSecond++;
			  SensorType.Parameter[TemperatureID].DiffStableSecond++;
        SensorType.Parameter[HumidityID].Time_5min++;
        SensorType.Parameter[TemperatureID].Time_5min++;
        SensorType.Parameter[HumidityID].Time_HeatCycle++;
        SensorType.Parameter[TemperatureID].Time_HeatCycle++;
		  	SensorType.Compressor_Delay_time++;
			  SensorType.DeforstingIntervalSecond++;
			  if(SensorType.Flag.EnTime == 1 )
		  	  SensorType.RunTime ++;
    }
				SensorType.DeforstingmilliSecond++;
    if (SensorType.FanOutTime++ >= 100) {
        SensorType.FanOutTime  =  0;
    }
		ParTab[PAR_POS_Run_Stop_Command].uVal ? 
      SensorType.FanOutTime <  ParTab[PAR_POS_FAN_OUT_PRECENT].uVal ?  Fan_ON : Fan_OFF
		: Fan_OFF ;
    SensorType.Parameter[HumidityID].Time_HeatCounter++;
    SensorType.Parameter[TemperatureID].Time_HeatCounter++;
    /* USER CODE END TIM5_IRQn 1 */
}

/* USER CODE BEGIN 1 */

static void USART1_RestartDmaRx(void)
{
    U1_Control.U_RxCounter = 0;
    U1_Control.UART_Handler.ErrorCode = HAL_UART_ERROR_NONE;
    if (U1_Control.URxDataInPtr != NULL) {
        U1_Control.URxDataInPtr->StartPtr = U1_RxBuff;
    }
    if (HAL_UART_Receive_DMA(&U1_Control.UART_Handler, U1_RxBuff, U1_DMARX_SIZE) == HAL_OK) {
        __HAL_UART_ENABLE_IT(&U1_Control.UART_Handler, UART_IT_IDLE);
    }
}

void USART1_IRQHandler(void)
{
    uint16_t dma_left;
    uint16_t received;
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&U1_Control.UART_Handler);
  /* USER CODE BEGIN USART1_IRQn 1 */
    if (U1_Control.URxDataInPtr == NULL) {
        return;
    }

	if(__HAL_UART_GET_FLAG(&U1_Control.UART_Handler,UART_FLAG_IDLE) != RESET){
		__HAL_UART_CLEAR_IDLEFLAG(&U1_Control.UART_Handler);
		HAL_UART_DMAStop(&U1_Control.UART_Handler);
        dma_left = (uint16_t)__HAL_DMA_GET_COUNTER(&U1_Control.hdma_rx);
        received = (dma_left <= U1_DMARX_SIZE) ? (uint16_t)(U1_DMARX_SIZE - dma_left) : 0U;
        if ((uint32_t)U1_Control.U_RxCounter + received > U1_RXBUFF_SIZE) {
            U1_Control.U_RxCounter = 0;
        } else {
            U1_Control.U_RxCounter += received;
        }
		U1_Control.URxDataInPtr->EndPtr = &U1_RxBuff[U1_Control.U_RxCounter];

		U1_Control.URxDataInPtr++;
		if(U1_Control.URxDataInPtr==U1_Control.URxDataEndPtr)
			U1_Control.URxDataInPtr = &U1_Control.UCB_RxBuffPtrCB[0];
		U1_Control.URxDataInPtr->StartPtr = &U1_RxBuff[U1_Control.U_RxCounter];

		if(U1_RXBUFF_SIZE - U1_Control.U_RxCounter >= U1_DMARX_SIZE){
			HAL_UART_Receive_DMA(&U1_Control.UART_Handler, &U1_RxBuff[U1_Control.U_RxCounter], U1_DMARX_SIZE);
            U1_Control.URxDataInPtr->StartPtr = &U1_RxBuff[U1_Control.U_RxCounter];
		}else{
			U1_Control.U_RxCounter = 0;
			HAL_UART_Receive_DMA(&U1_Control.UART_Handler, U1_RxBuff, U1_DMARX_SIZE);
			U1_Control.URxDataInPtr->StartPtr = U1_RxBuff;
		}
        __HAL_UART_ENABLE_IT(&U1_Control.UART_Handler, UART_IT_IDLE);
	} else if ((U1_Control.UART_Handler.RxState == HAL_UART_STATE_READY) &&
               (U1_Control.UART_Handler.ErrorCode != HAL_UART_ERROR_NONE)) {
        USART1_RestartDmaRx();
    }
  /* USER CODE END USART1_IRQn 1 */
}
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3) {
        __HAL_UART_CLEAR_OREFLAG(huart);
        if (pxMBFrameCBByteReceived != NULL) {
            __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
        }
    }
}



/* USER CODE END 1 */

