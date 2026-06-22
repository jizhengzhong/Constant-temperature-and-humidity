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
//#include "pid.h"
#include "main.h"

partab_type ParTab[PAR_POS_END + 2]; //工作参数
partab_type ParReceive[PAR_POS_END + 2]; //接收参数
partab_type ParSaved[PAR_POS_END + 2]; //接收已存储的参数
partab_type ParUnSaved[PAR_POS_FAN_AR+2];

// uint8_t    ParReadSt[PAR_POS_END + 2]; //指示上位机已读该参数


#define REG_INPUT_START     0x0001U //寻址地址是从1开始的
#define REG_INPUT_NREGS 4

#define REG_HOLDING_START               ( 1 )
#define REG_HOLDING_NREGS               ( 32 )
/* ----------------------- Static variables ---------------------------------*/
static uint16_t   usRegInputStart = REG_INPUT_START;
static uint16_t   usRegInputBuf[REG_INPUT_NREGS]={0x01,0x02,0x03,0x04};//为了验证使用的初始化值

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
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

  USHORT usAddr_value;
UCHAR  *buff=NULL;
 mbU16Type   Data;
eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{
	
	eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
	

	
	usAddr_value  = usAddress;


//    if( ( usAddress >= REG_HOLDING_START ) && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
//    {
        iRegIndex = ( int )( usAddress - usRegHoldingStart );
        switch ( eMode )
        {
        case MB_REG_READ:
            while( usNRegs > 0 )
            {

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
            while( usNRegs > 0 )
            {
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
 
//extern s16 Temp_Measure;  //温度测量值
//extern InterParaOne ParaTempPID;
//extern s16 Temp_Set;      //温度设定值
    
uint16_t RegHoldRead(uint16_t Index)
{
    mbU16Type Data;
    Data.uVal = 0;

    //获取参数
    if ((Index >= 0x1000) && (Index <= (0x1000 + PAR_POS_END)))
    {
        Data.uVal = ParTab[Index - 0x1000].uVal;
    }
    else if( (Index >= 4499) && (Index <= (4499 + PAR_POS_FAN_AR) ))
			{
			  Data.uVal=ParUnSaved[Index - 0x1193].uVal;
			
			}
			else
		{

       switch (Index)
        {

            case 0x401: //杀菌状态
                Data.uVal = ParTab[PAR_POS_RUN_PROGRAM_TYPE].uVal;
                break;
            case 0x40F:
                Data.iVal = Temperature.Measure*10;   //测量温度
                break;
            case 0x410:
							Data.iVal = RunFlow.SteriCount; //灭菌次数 
                break;
						 case 0x411:
					      Data.iVal =  RunFlow.DisplayWater*10;   //当前水温
                break;
            case 0x412:
                Data.iVal = RunFlow.CurveState;  //运行状态
                break;
					  case 0x413:
                Data.iVal = RunFlow.Pressure; //测量压力
                break;
					 	case 0x414:
                Data.iVal = Procedure.dry_kwarm_time.dry_temp;  //系统版本
                break;
            case 0x415:
                Data.iVal = Procedure.steri;         //设定温度
                break;
            case 0x416:
                Data.iVal = Procedure.steri_time;    //设定时间
                break;
            case 0x417:
                  Data.iVal =  Procedure.mode.dry_temp;   //干燥温度                 //SysErrGet(ERR_TEMP_AH); //
                break;
            case 0x418:
                  Data.iVal = Procedure.dry_kwarm_time.dry_temp;  //干燥时间
                break;
            case 0x419:
                 Data.iVal = Procedure.mode.keepwarm;         //保温温度
                break;
						case 0x41a:
								 Data.iVal = Procedure.dry_kwarm_time.keepwarm;   //保温时间
						break;
            case 0x41b:
						  Door_State_Check == DoorOpen_State ?      ( Data.iVal = 0) : (Data.iVal = 1 );
						
						   if(Data.iVal == 1 )
						      Lock_State_Check == Unlock_State   ?      ( Data.iVal = 1) : (Data.iVal = 2 );
						
//									Door_State_Check == DoorOpen_State ?      ( Data.iVal = 0) : (Data.iVal = 1 );
//						   if( Data.iVal == 1 && ParTab[PAR_POS_Run_Stop_Command].uVal ==1 )
//						      Lock_State_Check == Unlock_State   ?      ( Data.iVal = 3) : (Data.iVal = 2 );

                break;
            case 0x41c:                                           //灭菌剩余时间
							   ( RunFlow.flow_state == WORK_PROGRAM_FLOW_STATE_STERI || RunFlow.flow_state == WORK_PROGRAM_FLOW_STATE_DRY)?                     \
						     ( Data.iVal = Procedure.steri_time - RunFlow.SteriMinute ): ( Data.iVal = 0 ); 
                break;
						case 0x41d:                                           //加热状态
							   TEMP.Flag_HeatOut  ?         ( Data.iVal = 1 ): ( Data.iVal = 0 ); 
                break;
						case 0x41e:                                           //加热状态
							   Data.iVal = RunFlow.Err.ErrorFlag; 
                break;
					  case 0x41f: //时间更新请求
                Data.uVal = ParTab[PAR_POS_UPDATE_TIME].uVal;
                break;
            default:
 
                break;
        }
			}

			
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
//       uint16_t positon;
uint16_t RegHoldWrite(uint16_t Index, uint16_t Val)
{
		mbU16Type           Data;
	  uint16_t pos;

    Data.uVal = Val;

    if ((Index >= 0x1000) && (Index < (0x1000 + PAR_POS_END)))
    {
        pos = Index - 0x1000;
//		  	positon   =pos;
        if (ParCheck(pos, Data.iVal))   //暂将数据范围限制去掉
        {                    //参数检测成功后更新数据
            ParTab[pos].uVal = ParReceive[pos].uVal = Val;
					  osThreadFlagsSet (tid_ParaHandle, TP_BIT_0);
        }
    }
    else
    {
			pos = Index - 4499;
//			positon = pos;
			ParUnSaved[pos].uVal = Val;
    }

    return Val;   
}


eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    return MB_ENOREG;
}
