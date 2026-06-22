/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */

 #include "stdio.h"
 
UCB_CB U1_Control;            //控制串口1的结构体

uint8_t  U1_TxBuff[U1_TXBUFF_SIZE];         //定义一个数组缓冲区，串口1发送缓冲区 
uint8_t  U1_RxBuff[U1_RXBUFF_SIZE];         //定义一个数组缓冲区，串口1发接收冲区 
uint8_t  U1Copy_RxBuff[U1_COPYBUFF_SIZE];   //定义一个数组缓冲区，串口1处理接收数据的副本缓冲区


/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
//DMA_HandleTypeDef hdma_usart1_rx;
//DMA_HandleTypeDef hdma_usart1_tx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  U1_Control.UART_Handler.Instance = USART1;
  U1_Control.UART_Handler.Init.BaudRate = 115200;
  U1_Control.UART_Handler.Init.WordLength = UART_WORDLENGTH_8B;
  U1_Control.UART_Handler.Init.StopBits = UART_STOPBITS_1;
  U1_Control.UART_Handler.Init.Parity = UART_PARITY_NONE;
  U1_Control.UART_Handler.Init.Mode = UART_MODE_TX_RX;
  U1_Control.UART_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  U1_Control.UART_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&U1_Control.UART_Handler) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  __HAL_UART_CLEAR_IDLEFLAG(&U1_Control.UART_Handler);                //清除空闲中断标志 
	__HAL_UART_ENABLE_IT(&U1_Control.UART_Handler,UART_IT_IDLE);        //开启空闲中断	
	U1Rx_Buff_Init();                                                   //初始化串口1处理接收数据的各个变量
	HAL_UART_Receive_DMA(&U1_Control.UART_Handler, U1_RxBuff, U1_DMARX_SIZE);  //启动DMA接收
  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
   U1_Control.hdma_rx.Instance = DMA1_Channel5;
   U1_Control.hdma_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
   U1_Control.hdma_rx.Init.PeriphInc = DMA_PINC_DISABLE;
   U1_Control.hdma_rx.Init.MemInc = DMA_MINC_ENABLE;
   U1_Control.hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
   U1_Control.hdma_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
   U1_Control.hdma_rx.Init.Mode = DMA_NORMAL;
   U1_Control.hdma_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&U1_Control.hdma_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,U1_Control.hdma_rx);
    /* USART1_TX Init */
   U1_Control.hdma_tx.Instance = DMA1_Channel4;
   U1_Control.hdma_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
   U1_Control.hdma_tx.Init.PeriphInc = DMA_PINC_DISABLE;
   U1_Control.hdma_tx.Init.MemInc = DMA_MINC_ENABLE;
   U1_Control.hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
   U1_Control.hdma_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
   U1_Control.hdma_tx.Init.Mode = DMA_NORMAL;
   U1_Control.hdma_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&U1_Control.hdma_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,U1_Control.hdma_tx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PD5     ------> USART2_TX
    PD6     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_USART2_ENABLE();

  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = Screen_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(Screen_RX_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = Screen_Tx_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(Screen_Tx_GPIO_Port, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_USART3_ENABLE();

  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PD5     ------> USART2_TX
    PD6     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_5|GPIO_PIN_6);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOD, Screen_RX_Pin|Screen_Tx_Pin);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint32_t time_out=0;
  /* Prevent unused argument(s) compilation warning */
 if( huart->Instance == USART3 )
	 {
		 while( HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY )
		 {
			 time_out++;
				if(time_out>0xffffffffU)
					break;
		 }
		  __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);

	 }
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_RxCpltCallback could be implemented in the user file
   */
}


/*****************  发送字符串 **********************/
void Usart_SendString(uint8_t *str)
{
	unsigned int k=0;
  do 
  {
      HAL_UART_Transmit(&huart2,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
  
}
//重定向c库函数printf到串口DEBUG_USART，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口DEBUG_USART */
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 1000);	
	
	return (ch);
}

//重定向c库函数scanf到串口DEBUG_USART，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{		
	int ch;
	HAL_UART_Receive(&huart2, (uint8_t *)&ch, 1, 1000);	
	return (ch);
}

/*----------------------------------------------------------*/
/*函数名：初始化处理接收数据的结构体数组                    */
/*参  数：无                                                */
/*返回值：无                                                */
/*----------------------------------------------------------*/
void U1Rx_Buff_Init(void)
{	
	U1_Control.URxDataInPtr  = &U1_Control.UCB_RxBuffPtrCB[0];           //指向标记接收数据起始位置，截止位置的结构体的数组的插入数据的位置的指针，在最开始的时候指向该数组的0号成员
	U1_Control.URxDataOutPtr =  U1_Control.URxDataInPtr;                 //指向标记接收数据起始位置，截止位置的结构体的数组的提取数据的位置的指针，在最开始的时候指向该数组的0号成员
  U1_Control.URxDataEndPtr = &U1_Control.UCB_RxBuffPtrCB[NUM-1];       //指向标记接收数据起始位置，截止位置的结构体的数组的结尾数据的位置的指针，在最开始的时候指向该数组的最后一个成员
	U1_Control.URxDataInPtr->StartPtr = U1_RxBuff;                       //起始位置
	U1_Control.URxDataInPtr->EndPtr = U1_RxBuff;                         //起始位置
	U1_Control.U_RxCounter = 0;                                          //串口1累计接收位置变量，设置为0
}
/* USER CODE END 1 */
