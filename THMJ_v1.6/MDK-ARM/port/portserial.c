/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

#include "stm32f1xx_hal.h"
#include "usart.h"

/* ----------------------- static functions ---------------------------------*/
 void prvvUARTTxReadyISR( void );
 void prvvUARTRxISR( void );

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
	if( xRxEnable )
	{
	  __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);
	}
	else 
	{
		__HAL_UART_DISABLE_IT(&huart3, UART_IT_RXNE);
	}
	
	if( xTxEnable )
	{
	  __HAL_UART_ENABLE_IT(&huart3, UART_IT_TXE);
	}else 
	{
		__HAL_UART_DISABLE_IT(&huart3, UART_IT_TXE);
	}
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
	  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = ulBaudRate;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	
	
	ATOMIC_CLEAR_BIT( huart3.Instance->CR1, USART_CR1_TE);

  /* USER CODE BEGIN USART3_Init 2 */
	
	  switch(eParity)
    {
    // 奇校验
    case MB_PAR_ODD:
        huart3.Init.Parity = UART_PARITY_ODD;
        huart3.Init.WordLength = UART_WORDLENGTH_9B;			// 带奇偶校验数据位为9bits
        break;

    // 偶校验
    case MB_PAR_EVEN:
        huart3.Init.Parity = UART_PARITY_EVEN;
        huart3.Init.WordLength = UART_WORDLENGTH_9B;			// 带奇偶校验数据位为9bits
        break;

    // 无校验
    default:
        huart3.Init.Parity = UART_PARITY_NONE;
		    huart3.Init.StopBits = UART_STOPBITS_2;
        huart3.Init.WordLength = UART_WORDLENGTH_8B;			// 无奇偶校验数据位为8bits
        break;
    }
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
		
		return FALSE;
//    Error_Handler();
  }

  /* USER CODE END USART3_Init 2 */
    return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
	 if( HAL_UART_Transmit(&huart3,(uint8_t *)&ucByte, 1,1) != HAL_OK )
			return FALSE ;
	 else 
			return TRUE;
}


BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
//	uint32_t time_out;
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
        *pucByte = (USART3->DR & (uint16_t)0x01FF);
	      __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);
			return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
 void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}



