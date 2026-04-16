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

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

#include "stm32f1xx_hal.h"
#include "tim.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
 void prvvTIMERExpiredISR( void );

/* ----------------------- Start implementation -----------------------------*/

extern TIM_HandleTypeDef htim4;


BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{

  /* USER CODE BEGIN TIM4_Init 0 */
	uint16_t PrescalerValue = 0;
  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */
	PrescalerValue = (uint16_t)((SystemCoreClock/20000)-1);
  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = PrescalerValue;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = usTim1Timerout50us-1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  sClockSourceConfig.ClockPolarity = TIM_CLOCKPOLARITY_NONINVERTED;
  sClockSourceConfig.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
  sClockSourceConfig.ClockFilter = 0;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_DISABLE;
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
  if (HAL_TIM_SlaveConfigSynchro(&htim4, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
	
//	 __HAL_TIM_CLEAR_FLAG(&htim4, TIM_FLAG_UPDATE);      // 清除一下定时器的中断标记,防止使能中断后直接触发中断
   __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);	      // 使能定时器更新中断


  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

    return TRUE;
}


inline void
vMBPortTimersEnable(  )
{
	
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
	__HAL_TIM_CLEAR_IT(&htim4,TIM_IT_UPDATE);
	__HAL_TIM_ENABLE_IT(&htim4,TIM_IT_UPDATE);
	__HAL_TIM_SET_COUNTER(&htim4,0);
	__HAL_TIM_ENABLE(&htim4);


    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
}

inline void
vMBPortTimersDisable(  )
{
	__HAL_TIM_DISABLE(&htim4);
	__HAL_TIM_SET_COUNTER(&htim4,0);
	__HAL_TIM_DISABLE_IT(&htim4,TIM_IT_UPDATE);
	__HAL_TIM_CLEAR_IT(&htim4,TIM_IT_UPDATE);

    /* Disable any pending timers. */
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
void prvvTIMERExpiredISR( void )
{

    ( void )pxMBPortCBTimerExpired(  );
}


