/*
 * FreeModbus Libary: BARE Demo Application
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
//#include <sys.h>
#include "math.h"
#include "main.h"
#include "contrl.h"

partab_type ParTab[PAR_POS_END + 2]; //工作参数
partab_type ParReceive[PAR_POS_END + 2]; //接收参数
partab_type ParSaved[PAR_POS_END + 2]; //接收已存储的参数
partab_type ParUnSaved[25];

// uint8_t    ParReadSt[PAR_POS_END + 2]; //指示上位机已读该参数


#define REG_INPUT_START     0x0001U //寻址地址是从1开始的
#define REG_INPUT_NREGS 4

#define REG_HOLDING_START               ( 1 )
#define REG_HOLDING_NREGS               ( 32 )
/* ----------------------- Static variables ---------------------------------*/
static uint16_t   usRegInputStart = REG_INPUT_START;
static uint16_t   usRegInputBuf[REG_INPUT_NREGS] = {0x01, 0x02, 0x03, 0x04}; //为了验证使用的初始化值

static USHORT   usRegHoldingStart = REG_HOLDING_START;

extern struct key_mgr key_manage;
//static USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];

//extern struct SetFlow Set_Flow_Function[Func_Count];

typedef union {
    uint16_t Val;
    uint16_t uVal;
    int16_t iVal;
    struct {
        uint8_t LB;
        uint8_t HB;
    } byte;
} mbU16Type;


uint16_t RegHoldRead(uint16_t Index);
uint16_t RegHoldWrite(uint16_t Index, uint16_t Val);

eMBErrorCode
eMBRegInputCB(UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs)
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if ((usAddress >= REG_INPUT_START)
            && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS)) {
        iRegIndex = (int)(usAddress - usRegInputStart);

        while (usNRegs > 0) {
            *pucRegBuffer++ =
                (unsigned char)(usRegInputBuf[iRegIndex] >> 8);
            *pucRegBuffer++ =
                (unsigned char)(usRegInputBuf[iRegIndex] & 0xFF);
            iRegIndex++;
            usNRegs--;
        }
    } else {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

USHORT usAddr_value;
UCHAR  *buff = NULL;
mbU16Type   Data;
eMBErrorCode
eMBRegHoldingCB(UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                eMBRegisterMode eMode)
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
    usAddr_value  = usAddress;
    //    if( ( usAddress >= REG_HOLDING_START ) && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    //    {
    iRegIndex = (int)(usAddress - usRegHoldingStart);

    switch (eMode) {
    case MB_REG_READ:
        while (usNRegs > 0) {
            //                *pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] >> 8 );
            //                *pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] & 0xFF );
            Data.Val = RegHoldRead(iRegIndex);
            *pucRegBuffer++ = Data.byte.HB;
            *pucRegBuffer++ = Data.byte.LB;
            iRegIndex++;
            usNRegs--;
        }

        break;

    case MB_REG_WRITE:
        while (usNRegs > 0) {
            //                usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
            //                usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
            /////////////////////////////////JI_ADD////////////////////////
            buff =  pucRegBuffer;
            Data.byte.HB = *pucRegBuffer++;
            Data.byte.LB = *pucRegBuffer++;
            RegHoldWrite(iRegIndex, Data.Val);
            /////////////////////////////////JI_END////////////////////////
            iRegIndex++;
            usNRegs--;
        }
    }

    //    }
    //    else
    //    {
    //        eStatus = MB_ENOREG;
    //    }
    return eStatus;
}

/*****************************************************************************//*!
* @brief   hold read.
*
* @param   index: 数据地址
*
* @return  data:数据
* 地址区间划分：
* 0x100 获取系统参数
* 0x200 获取系统参数
* 0x280 命令/运行状态
* 0x400 调试
* @ Pass/ Fail criteria: none
*****************************************************************************/

uint16_t RegHoldRead(uint16_t Index)
{
    mbU16Type Data;
    Data.uVal = 0;
//	  int16_t LeftTimeValue;
    //	 osStatus_t status;
    //   uint16_t msg1[4]={100};

    //	 status = osMessageQueueGet(ReadonlyQueue0Handle, &msg1, NULL, 50);   // wait for message
    //    if (status == osOK)
    //		{

    //获取参数
    if ((Index >= PAR_VERSION_ID) && (Index <= (PAR_VERSION_ID + PAR_POS_END))) {
        Data.iVal = ParTab[Index - PAR_VERSION_ID].uVal;
    } else 
		{
        switch (Index) {
					case	PAR_POS_CURRENT_YEAR:
						 Data.iVal = ParUnSaved[PAR_POS_CURRENT_YEAR-PAR_REDA_ADDR].uVal;
					break;
					
				  case	PAR_POS_CURRENT_MONTH:
						 Data.iVal = ParUnSaved[PAR_POS_CURRENT_MONTH-PAR_REDA_ADDR].uVal;
					break;
					
					case	PAR_POS_CURRENT_DAY:
						 Data.iVal = ParUnSaved[PAR_POS_CURRENT_DAY-PAR_REDA_ADDR].uVal;
					break;
					
					case	PAR_POS_CURRENT_HOUR:
						   Data.iVal = ParUnSaved[PAR_POS_CURRENT_HOUR-PAR_REDA_ADDR].uVal;
					break;
					
				  case	PAR_POS_CURRENT_MINUTE:
						    Data.iVal = ParUnSaved[PAR_POS_CURRENT_MINUTE-PAR_REDA_ADDR].uVal;
					break;
								
        case PAR_POS_TEMP_MONITOR:
            Data.iVal = SensorType.SubTemperature * 10 ;   //监控温度
            break;

        case PAR_POS_HUMI_MONITOR:
            Data.iVal = SensorType.SubHumidity * 10; //监控湿度
            break;

        case PAR_POS_TEMP_DISPLAY:
            fabs(SensorType.Parameter[TemperatureID].Target * 10 - SensorType.Parameter[TemperatureID].Display) <=
            ParTab[PAR_POS_TEMP_DEATH_ZONE].uVal ? (Data.iVal = SensorType.Parameter[TemperatureID].Target * 10) : (Data.iVal =
                    SensorType.Parameter[TemperatureID].Display) ;
            break;

        case PAR_POS_HUMI_DISPLAY:
            fabs(SensorType.Parameter[HumidityID].Target * 10 - SensorType.Parameter[HumidityID].Display) <=
            ParTab[PAR_POS_Humi_DEATH_ZONE].uVal ?
            (Data.iVal = SensorType.Parameter[HumidityID].Target * 10) : (Data.iVal = SensorType.Parameter[HumidityID].Display) ;
            break;

        case PAR_POS_TEMP_POWER:
            Data.iVal = SensorType.Parameter[TemperatureID].OutValue * 0.1 / SensorType.Parameter[TemperatureID].T; //当前水温
            break;

        case PAR_POS_HUMI_POWER:
            Data.iVal = SensorType.Parameter[HumidityID].OutValue * 0.1 / SensorType.Parameter[HumidityID].T; //出力
            break;

        case PAR_POS_WATER_LEVEL:
            Data.iVal = High_Level_Check << 1 | Low_Level_Check; //水位检测
            break;

        case PAR_POS_DOOR_STATE:
            SensorType.bDoorState == DoorOpen_State ? (Data.iVal = 0) : (Data.iVal = 1);
            break;

        case PAR_POS_RUN_COUNT_TIME:                                            //当前时间
            Data.iVal = SensorType.RunTime / 60;
            break;

        case PAR_POS_LEFT_TIME:                                           //剩余时间

					Data.iVal=	SensorType.SetRunTime == 0?  0:( SensorType.SetRunTime - SensorType.RunTime / 60 );

            break;

        case PAR_POS_CURRENT_DUAN:                                           
            Data.iVal  =CurrentDuan;
            break;

        case PAR_POS_PRO_CIRCLE_COUNT: 
            Data.uVal =  ParTab[PAR_POS_PRO0_REPEAT+ParTab[PAR_POS_PRO_INDEX].uVal].uVal-ProCircleNum;
            break;
				
				case	PAR_POS_PRO_ALARM_CODE:
					  Data.uVal = ParUnSaved[PAR_POS_PRO_ALARM_CODE-PAR_REDA_ADDR].uVal;
				    break;
//			   case	PAR_POS_PRO_READ_END_DUAN:
//					    Data.uVal = ParTab[PAR_POS_TEMP_PROGRAM_END_COUNT].uVal;
//				 break;
//				 case PAR_POS_PRO_READ_Start_LINE:
//					 Data.uVal = ProStartRun%30;
//				    break;

        default:
            break;
        }
    }

    //					}
    return Data.uVal;
}


/*****************************************************************************//*
* @brief   hold read.
*
* @param   index: 数据地址
*
* @return  data:数据
* 地址区间划分：
* 0x100 写入系统参数
* 0x280 命令/运行状态
* 0x400 调试
* @ Pass/ Fail criteria: none
*****************************************************************************/
uint16_t positon;
uint16_t RegHoldWrite(uint16_t Index, uint16_t Val)
{
    mbU16Type   Data;
    uint16_t pos;
    Data.uVal = Val;
	  pos = PAR_POS_END+PAR_VERSION_ID;
   if ((Index >= PAR_VERSION_ID) && (Index < pos)) 
	     {
        pos = Index - PAR_VERSION_ID;
        positon   = pos;
        if (ParCheck(pos, Data.iVal)) { //暂将数据范围限制去掉
            //参数检测成功后更新数据
            ParTab[pos].uVal = ParReceive[pos].uVal = Val;
        }
    } else {
        pos = Index - PAR_REDA_ADDR;
        positon = pos;
			if(pos<25)
        ParUnSaved[pos].uVal = Val;
    }

    return Val;
}


eMBErrorCode
eMBRegCoilsCB(UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
              eMBRegisterMode eMode)
{
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB(UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete)
{
    return MB_ENOREG;
}
