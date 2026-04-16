
#include "eeprom.h"
#include "iic.h"
#include "main.h"
#include "common.h"
#include "cmsis_os.h"
#include "math.h"

typedef  void (*pCallback)(partab_type *ParTab, TypeCtrl* pType);
typedef  void (*pEventFunEntry)(EVIdEnum evenId, TypeCtrl *pTypeCtrl);
typedef struct {
    EVIdEnum  EventId;				/* 功能ID */
    pEventFunEntry ActFun;		/* 功能函数 */
} ACT_FUN;

typedef struct {
    EVEnum  EventId;				/* 功能ID */
    partab_type *ParTab;
    pCallback pFun;		      /* 功能函数 */
} pCallbackFun;


static void CtrlFun(EVIdEnum evenId, TypeCtrl *pTypeCtrl);


ACT_FUN EvFunTab[] = {
    {TemperatureID,  CtrlFun},
    {HumidityID,     CtrlFun},
};

static int32_t DataFlt(float f_real, int32_t i_now)
{
    int32_t i_real;
    int32_t i_real_add2;
    int32_t i_real_sub2;

    if (f_real < 0) {
        i_real = (int32_t)f_real;
        i_real_add2 = (int32_t)(f_real - 0.2f);

        if (f_real <= -0.2f) {
            i_real_sub2 = (int32_t)(f_real + 0.2f);
        } else {
            i_real_sub2 = 1;
        }

        if (i_real_add2 < i_real) {
            i_now = i_real_add2;
        } else if (i_real_sub2 > i_real) {
            i_now = i_real;
        } else {
            if (i_now > i_real) {
                i_now = i_real;
            } else if (i_now < (i_real - 1)) {
                i_now = i_real - 1;
            }
        }
    } else {
        i_real = (int32_t)f_real;
        i_real_add2 = (int32_t)(f_real + 0.2f);

        if (f_real >= 0.2f) {
            i_real_sub2 = (int32_t)(f_real - 0.2f);
        } else {
            i_real_sub2 = -1;
        }

        if (i_real_add2 > i_real) {
            i_now = i_real_add2;
        } else if (i_real_sub2 < i_real) {
            i_now = i_real;
        } else {
            if (i_now < i_real) {
                i_now = i_real;
            } else if (i_now > (i_real + 1)) {
                i_now = i_real + 1;
            }
        }
    }

    return i_now;
}




static unsigned short int ADC_Filter(unsigned  short int *pBuff, char ch)
{
    unsigned char i, j;
    unsigned  short int  temp;
    static unsigned short int Value_Index[Filter_Channel] = {0},  \
        Value_List[Filter_Channel][MAX_ANALOG_SAMPLING_NUMBER] = {0}, val_cur[Filter_Channel] = {0};
    static unsigned short int val_list[Filter_Channel][MAX_ANALOG_SAMPLING_NUMBER] = {0};
    static unsigned int AdSum[Filter_Channel] = {0};
    static char TempCnt[4] = {0};
    unsigned char cnt;
    Value_Index[ch]++;

    if (Value_Index[ch] > MAX_ANALOG_SAMPLING_NUMBER) {
        Value_Index[ch] = 0;
    }

    Value_List[ch][Value_Index[ch]] =  *pBuff;
    cnt =  MAX_ANALOG_SAMPLING_NUMBER;

    while (cnt--) {
        val_list[ch][cnt] = Value_List[ch][cnt];
    }

    for (j = 0; j < MAX_ANALOG_SAMPLING_NUMBER - 1; j++) {
        for (i = 0 ; i < MAX_ANALOG_SAMPLING_NUMBER - j - 1; i++) {
            if (val_list[ch][i] > val_list[ch][i + 1]) {
                temp = val_list[ch][i];
                val_list[ch][i] = val_list[ch][i + 1];
                val_list[ch][i + 1] = temp;
            }
        }
    }

    val_cur[ch] = val_list[ch][MAX_ANALOG_SAMPLING_NUMBER / 2];
    AdSum[ch] +=  val_cur[ch];
    TempCnt[ch]++;

    if (TempCnt[ch] >= 8) {
        val_cur[ch] =  AdSum[ch] >> 3;
        TempCnt[ch] = 0;
        AdSum[ch] = 0;
    }

    return val_cur[ch];
}

static void Temp_Hea_State(bool state)
{
    state == Heat_OFF  ? HEAT_OFF : HEAT_ON;
}

static void Hum_Hea_State(bool state)
{
    state == Heat_OFF ? Hum_HEAT_OFF : Hum_HEAT_ON;
}


static void CtrlFun(EVIdEnum evenId, TypeCtrl *pTypeCtrl)
{
    if (evenId == TemperatureID) {
        pTypeCtrl->Parameter[evenId].HeatCallback = Temp_Hea_State;
    } else {
        pTypeCtrl->Parameter[evenId].HeatCallback = Hum_Hea_State;
    }

    Control(&pTypeCtrl->Parameter[evenId], &pTypeCtrl->Flag);
}


static void PID_Calculate(PidParam *vPID, TypeCommon *ParaCtrl)
{
    float temp_a, temp_b;
    float temp_c, temp_d;
    s16 PVauleRun, DVauleRun;

		PVauleRun = vPID->P; //得到积分的真实运行控制值
		DVauleRun = vPID->d; //得到微分的真实运行控制值

    if (fabs(vPID->Target - vPID->Measure) > PVauleRun) { //禁止微分
        vPID->DFVaule    = vPID->Measure;      //微分先行后微分值
        vPID->LastMeasure = vPID->Measure; //上一次温度测量值
    } else {
        temp_c = (vPID->Measure - vPID->LastMeasure) * DVauleRun * vPID->gama;
        temp_c = temp_c + DVauleRun * vPID->DFVaule;
        temp_c = temp_c + vPID->Measure * vPID->T * vPID->gama;
        temp_d = temp_c / (DVauleRun + vPID->T * vPID->gama);

        if (temp_d >= 110) {
            vPID->DFVaule = 110;
        } else if (temp_d <= -50) {
            vPID->DFVaule = -50;
        } else {
            vPID->DFVaule = temp_d;
        }
        vPID->LastMeasure = vPID->Measure;
    }

    /* 积分处理 */
    if ((vPID->Target - vPID->DFVaule) >= PVauleRun) { //下比例带外
        vPID->OutValue = 1000; //全功率输出
        vPID->ISumVaule = 0; //积分累积清零
    } else {
        if ((vPID->DFVaule - vPID->Target) >= PVauleRun) { //上比例带外
            vPID->OutValue = 0; //关闭输出
            vPID->ISumVaule = 0; //积分累积清零
        } else { //变速积分处理
            if (vPID->Measure < vPID->Target) {
                temp_a = PVauleRun / 5;
                if (vPID->Target - vPID->Measure > temp_a) {
                    temp_b = PVauleRun;
                    temp_c = vPID->Target - vPID->DFVaule;
                    temp_b = temp_c * (temp_b - (vPID->Target - vPID->Measure)) / (temp_b - temp_a);
                } else {
                    temp_b = vPID->Target -  vPID->DFVaule;
                }
            } else {
                temp_b = vPID->Target - vPID->DFVaule;
            }
                if (!vPID->Bit.bDisIsum) {
                    temp_b = 0;    //禁止积分
      
            }

            temp_c = temp_b * 1000;
            temp_c = (temp_c * vPID->T) / PVauleRun;
            vPID->ISumVaule = vPID->ISumVaule + temp_c;

            if (vPID->ISumVaule < 0) {
                vPID->ISumVaule = 0;
            }

            temp_a = vPID->ISumVaule / vPID->I;
//            temp_i_display =	 temp_a;

            if (temp_a >= 1000) {
                temp_a = 1000;
                vPID->ISumVaule = (s32)vPID->I * 1000;
            }

            temp_c = (vPID->Target - vPID->DFVaule) * 1000;
            temp_b = temp_c / PVauleRun;
//            temp_p_display = 	temp_b;
////            Time_ADCSwitch++;
//            printf("%d,%.1f,%.1f ,%.1f ,%.1f\r\n", Time_ADCSwitch, vPID->Measure, vPID->DFVaule, temp_p_display, temp_i_display);
            temp_b = temp_a + temp_b;

            if (temp_b < 0) {
                vPID->OutValue = 0;
            } else if (temp_b > 1000) {
                vPID->OutValue = 1000;
            } else {
                vPID->OutValue = (s16)temp_b;
            }
        }
    }
}


void Control(PidParam* PID_Parameter, TypeCommon *pTypeCommon)
{
    short int temp_a;
    int temp_b;

    if (PID_Parameter->Bit.bOver || PID_Parameter->Bit.bProtect) { //温度溢出标志，ji
        PID_Parameter->DFVaule = PID_Parameter->Measure; //微分先行后微分值
        PID_Parameter->LastMeasure = PID_Parameter->Measure; //上一次温度测量值
        PID_Parameter-> HeatCallback(Heat_OFF);
        return;
    }

    if (pTypeCommon->Flag_EnRun) { ////控制器运行


            if (PID_Parameter->Display <= PID_Parameter->Target) {
                PID_Parameter->Bit.bDowm = 0;
            }

            if (PID_Parameter->Bit.bRun == 0) { //对应标志位判断
                PID_Parameter->Time_5min = 0;
                PID_Parameter->Bit.bDisIsum = 0;//禁止积分
            }

    }

    //禁止积分处理
    if (PID_Parameter->Time_5min > 300) {
        PID_Parameter->Time_5min = 310;
        PID_Parameter->Bit.bDisIsum  = 1;  //禁止加热积分累积
    }

    //溢出、定时结束后停止温控、禁止温度控制时关断加热  //自整定标志位
    if (PID_Parameter->Bit.bOver || ((PID_Parameter->Bit.bRun == 0))) { //温度溢出或停止温控
        PID_Parameter->HeatCallback(Heat_OFF);
        PID_Parameter->Bit.bHeatOut = 0;
        //PID控制赋初值
        PID_Parameter->SetDiff = 0;
        PID_Parameter->Time_5min = 0; //禁止积分计时清零
        PID_Parameter->Bit.bDisIsum = 0;
        PID_Parameter->ISumVaule = 0;
        PID_Parameter->DFVaule = PID_Parameter->Measure;
        PID_Parameter->LastMeasure = PID_Parameter->Measure;
        return;
    }

    if (PID_Parameter->Time_HeatCycle >= PID_Parameter->T) { //加热一个周期   ParaTempPID.T = 5;   //控制周期
        PID_Parameter->Time_HeatCycle = 0; //Time_HeatCycle  加热控制周期计时
        PID_Parameter->Time_HeatCounter = 0;	//Time_HeatCounter 加热控制时间计时
        //输出功率控制
        PID_Calculate(PID_Parameter, pTypeCommon);

        if (PID_Parameter->SetDiff >= 5 || PID_Parameter->SetDiff <= -5) {
            PID_Parameter->Time_5min = 0;
            PID_Parameter->Bit.bDisIsum = 0;
            PID_Parameter->SetDiff = 0;
        }//设定值变化超5，禁止积分

        temp_a = PID_Parameter->OutValue * PID_Parameter->T; //PID输出值*控制周期
        temp_b = temp_a * PID_Parameter->AR * 0.01; //乘积*功率
        PID_Parameter->OutValue = (s16)temp_b;
    } else {
        if (PID_Parameter->Bit.bAlm) {
            PID_Parameter-> HeatCallback(Heat_OFF);
            PID_Parameter->Bit.bHeatOut = 0;
        } else {
            if (PID_Parameter->Time_HeatCounter < PID_Parameter->OutValue) {
                PID_Parameter->	HeatCallback(Heat_ON);
                PID_Parameter->Bit.bHeatOut = 1;
            } else {
                PID_Parameter-> HeatCallback(Heat_OFF);
                PID_Parameter->Bit.bHeatOut = 0;
            }
        }
    }
}




static void TempHumiCalibration(partab_type *pTemp, partab_type *pWishTemp, float *temp, char *pIndex)
{
    char i ;
    float temperature;
    float K = 0.000001, b = 0.000001;
    temperature = *temp * 10;

    for (i = 0; i < 11; i++) {
        if (pTemp[i + 1].iVal == pTemp[i].iVal) {
            return;
        }
    }

    if (temperature >= pTemp[*pIndex].iVal && temperature <= pTemp[*pIndex + 1].iVal) {
        K =  1.0 * (pWishTemp[*pIndex + 1].iVal - pWishTemp[*pIndex].iVal) / (pTemp[*pIndex + 1].iVal - pTemp[*pIndex].iVal);
        b =   pWishTemp[*pIndex].iVal - K * pTemp[*pIndex].iVal;
        *temp = (temperature * K + b) * 0.1;
    } else if (temperature > pTemp[11].iVal) {
        K =  1.0 * (pWishTemp[11].iVal - pWishTemp[10].iVal) / (pTemp[11].iVal - pTemp[10].iVal);
        b =   pWishTemp[10].iVal - K * pTemp[10].iVal;
        *temp = (temperature * K + b) * 0.1;
    } else if (temperature < pTemp[0].iVal) {
        K =  1.0 * (pWishTemp[1].iVal - pWishTemp[0].iVal) / (pTemp[1].iVal - pTemp[0].iVal);
        b =   pWishTemp[0].iVal - K * pTemp[0].iVal;
    } else {
        if (*pIndex < 11) {
            (*pIndex)++;
        } else {
            *pIndex = 0;
        }
    }
}



static void CurrentSampleValue(partab_type *pBuff, TypeCtrl*pSensorValue)
{
	 volatile short int ADC_Channel[4] = {0};
    static char MainTempIndex = 0, MonitorTempIndex = 0, MainHumiIndex = 0, MonitorHumiIndex = 0;
    static  SensorADC LastSenorValue;
    float Temperature, SubTemperature, Humidity, SubHumidity;
    ADC_Channel[PT_B1A1]			= 	ADC_Filter((unsigned short *)&pBuff[PT_B1A1], PT_B1A1);
    ADC_Channel[PT_B2A2]			= 	ADC_Filter((unsigned short *)&pBuff[PT_B2A2], PT_B2A2);
    ADC_Channel[Hum_ADC]			= 	ADC_Filter((unsigned short *)&pBuff[Hum_ADC], Hum_ADC);
    ADC_Channel[ADC_Sense]		= ADC_Filter((unsigned short *)&pBuff[ADC_Sense], ADC_Sense);
    Temperature = (0.062 * ADC_Channel[PT_B1A1] + 1.5922) ;  //暂时注释掉
    SubTemperature = (0.062 * ADC_Channel[PT_B2A2] + 1.5922) ;
    //		修改为
    Temperature = Temperature * (1 + ParTab[PAR_POS_TEMP_Pk].iVal * 0.0001) + 0.01 * ParTab[PAR_POS_TEMP_Pb].iVal;
    TempHumiCalibration((partab_type *) &ParTab[PAR_POS_PRO_TEMP_DISP1].iVal,
                        (partab_type *)&ParTab[PAR_POS_PRO_TEMP_WISH1].iVal, &Temperature, &MainTempIndex);
    SubTemperature = SubTemperature * (1 + ParTab[PAR_POS_TEMP_Pk1].iVal * 0.0001) + 0.01 * ParTab[PAR_POS_TEMP_Pb1].iVal;
    TempHumiCalibration((partab_type *) &ParTab[PAR_POS_MONITOR_TEMP_DISP1].iVal,
                        (partab_type *)&ParTab[PAR_POS_MONITOR_TEMP_WISH1].iVal, &SubTemperature, &MonitorTempIndex);
    pSensorValue->Parameter[TemperatureID].Measure	 =  Temperature * (1 - pSensorValue->Parameter[TemperatureID].Fliter *
        0.01) + \
        pSensorValue->Parameter[TemperatureID].Fliter * 0.01 * LastSenorValue.Temperature;
    pSensorValue->SubTemperature	 =  SubTemperature * (1 - pSensorValue->Parameter[TemperatureID].Fliter * 0.01) + \
                                      pSensorValue->Parameter[TemperatureID].Fliter * 0.01 * pSensorValue->LstSubTemperature;
    Humidity		= 1000.0 / (float)(ParTab[PAR_POS_HUMI_SENSOR_HIGH_VOLTAGE].iVal -
                                  ParTab[PAR_POS_HUMI_SENSOR_LOW_VOLTAGE].iVal) ;
    Humidity = Humidity * ((3.3 * ADC_Channel[Hum_ADC] / 4096 / 0.6) - ParTab[PAR_POS_HUMI_SENSOR_LOW_VOLTAGE].iVal * 0.1);
    SubHumidity		= 1000.0 / (float)(ParTab[PAR_POS_HUMI_SENSOR_HIGH_VOLTAGE].iVal -     \
                                     ParTab[PAR_POS_HUMI_SENSOR_LOW_VOLTAGE].iVal) ;
    SubHumidity = SubHumidity * ((3.3 * ADC_Channel[ADC_Sense] / 4096 / 0.6) - ParTab[PAR_POS_HUMI_SENSOR_LOW_VOLTAGE].iVal
                                 \
                                 * 0.1);
    Humidity  =  Humidity * (1 + ParTab[PAR_POS_HUMI_Pk].iVal * 0.0001) +0.1 * ParTab[PAR_POS_HUMI_Pb].iVal;
    TempHumiCalibration((partab_type *) &ParTab[PAR_POS_PRO_HUMI_DISP1].iVal,
                        (partab_type *)&ParTab[PAR_POS_PRO_HUMI_WISH1].iVal, &Humidity, &MainHumiIndex);
    SubHumidity  =  SubHumidity * (1 + ParTab[PAR_POS_HUMI_Pk1].iVal * 0.0001) +0.1 * ParTab[PAR_POS_HUMI_Pb1].iVal;
    TempHumiCalibration((partab_type *) &ParTab[PAR_POS_MONITOR_HUMI_DISP1].iVal,
                        (partab_type *)&ParTab[PAR_POS_MONITOR_HUMI_WISH1].iVal, &SubHumidity, &MonitorHumiIndex);
    pSensorValue->Parameter[HumidityID].Measure		 =  Humidity  *
        (1 - pSensorValue->Parameter[HumidityID].Fliter * 0.01) +  \
        pSensorValue->Parameter[HumidityID].Fliter * 0.01 * LastSenorValue.Humidity;
    pSensorValue->SubHumidity		 =  SubHumidity  *                  \
                                    (1 - pSensorValue->Parameter[HumidityID].Fliter * 0.01) +  \
                                    pSensorValue->Parameter[HumidityID].Fliter * 0.01 * pSensorValue->LstSubHumidity;
    LastSenorValue.Temperature = pSensorValue->Parameter[TemperatureID].Measure;
    LastSenorValue.Humidity    = pSensorValue->Parameter[HumidityID].Measure	;
    pSensorValue->LstSubTemperature = pSensorValue->SubTemperature;
    pSensorValue->LstSubHumidity =  pSensorValue->SubHumidity;
}


void ADCSample(partab_type *SensorDisplay, TypeCtrl *pSensorValue)
{
    pSensorValue->Parameter[TemperatureID].Display	=	DataFlt(pSensorValue->Parameter[TemperatureID].Measure * 10,
        pSensorValue->Parameter[TemperatureID].Display);
    pSensorValue->Parameter[HumidityID].Display   =   DataFlt(pSensorValue->Parameter[HumidityID].Measure	* 10,
        pSensorValue->Parameter[HumidityID].Display);
}

void TempParaUpdata(partab_type * pTab, TypeCtrl* pType)
{
    pType->Parameter[TemperatureID].Fliter = pTab[PAR_POS_TEMP_FILTER].uVal;
    pType->Parameter[TemperatureID].P = pTab[PAR_POS_TEMP_P_SET].uVal;
    pType->Parameter[TemperatureID].I = pTab[PAR_POS_TEMP_I_SET].uVal;
    pType->Parameter[TemperatureID].d = pTab[PAR_POS_TEMP_D_SET].uVal;
    pType->Parameter[TemperatureID].T = pTab[PAR_POS_TEMP_T_SET].uVal;
    pType->Parameter[TemperatureID].gama = pTab[PAR_POS_TEMP_GAMA_SET].uVal;
    pType->Parameter[TemperatureID].AR = pTab[PAR_POS_TEMP_AR_SET].uVal;
    pType->Parameter[TemperatureID].Target =    ParTab[PAR_POS_TEMP_SV].uVal ;
    pType->Parameter[TemperatureID].Bit.bRun = pTab[PAR_POS_Run_Stop_Command].uVal;
}

void HumiParaUpdata(partab_type * pTab, TypeCtrl* pType)
{
    pType->Parameter[HumidityID].Fliter = pTab[PAR_POS_HUMI_FILTER].uVal;
    pType->Parameter[HumidityID].P = pTab[PAR_POS_HUMI_P].uVal;
    pType->Parameter[HumidityID].I = pTab[PAR_POS_HUMI_I].uVal;
    pType->Parameter[HumidityID].d = pTab[PAR_POS_HUMI_D].uVal;
    pType->Parameter[HumidityID].T = pTab[PAR_POS_HUMI_T].uVal;
    pType->Parameter[HumidityID].gama = pTab[PAR_POS_HUMI_GAMA].uVal;
    pType->Parameter[HumidityID].AR = pTab[PAR_POS_HUMI_AR].uVal;
    pType->Parameter[HumidityID].Target =  ParTab[PAR_POS_HUMI_SV].uVal ; 
    pType->Parameter[HumidityID].Bit.bRun = pTab[PAR_POS_Run_Stop_Command].uVal;
}


void PrintString(char *pString)
{
    printf("%s\r\n", pString);
    printf("20%d/%02d/%02d %02d:%02d:%02d\r\n", DateToUpdate.Year, DateToUpdate.Month, DateToUpdate.Date,  \
           sTime.Hours, sTime.Minutes, sTime.Seconds);
    printf("------------------------\r\n");
}


void PrintStart(void)
{
	  printf("------------------------\r\n");
    printf("       恒温恒湿箱       \r\n");
    printf("------------------------\r\n");
    printf("      Start  Point      \r\n");
    printf("------------------------\r\n");
    PrintString("Start Time");
    printf("      parameter     \r\n");
    printf("------------------------\r\n");
}

void StableCheck(partab_type *pBuff, TypeCtrl*pSensorValue)
{
    float DiffTempValue, DiffHumiValue;

    DiffTempValue = pSensorValue->Parameter[TemperatureID].Measure - pSensorValue->Parameter[TemperatureID].Target;
 
    if (fabs(DiffTempValue) <= *pBuff[PAR_POS_TEMP_DIFF_COUNT].uBytes * 0.1) {
        if (pSensorValue->Parameter[TemperatureID].DiffStableSecond > *pBuff[PAR_POS_TEMP_STA_SECOND].uBytes) {
            pSensorValue->Parameter[TemperatureID].DiffStableSecond = *pBuff[PAR_POS_TEMP_STA_SECOND].uBytes+5;
            pSensorValue->Parameter[TemperatureID].Bit.bStable = 1;
            pSensorValue->Flag.EnTime = 1; //使能运行计时
        }
    } else {
		  	pSensorValue->Flag.EnTime =   pSensorValue->Flag.TimeMode ?      0  : 1 ;
        pSensorValue->Parameter[TemperatureID].DiffStableSecond = 0;
    }

    DiffHumiValue = pSensorValue->Parameter[HumidityID].Measure - pSensorValue->Parameter[HumidityID].Target;

    if (fabs(DiffHumiValue) <= *pBuff[PAR_POS_HUMI_DIFF_COUNT].uBytes * 0.1) {
        if (pSensorValue->Parameter[HumidityID].DiffStableSecond > *pBuff[PAR_POS_HUMI_STA_SECOND].uBytes) {
            pSensorValue->Parameter[HumidityID].DiffStableSecond = *pBuff[PAR_POS_HUMI_STA_SECOND].uBytes+5;
            pSensorValue->Parameter[HumidityID].Bit.bStable = 1;
        }
    } else {
		  	pSensorValue->Flag.EnTime =   pSensorValue->Flag.TimeMode ?      0  : 1 ;
        pSensorValue->Parameter[HumidityID].DiffStableSecond = 0;
    }
}


void AlarmCode(partab_type *pBuff, TypeCtrl*pSensorValue)
{
    char WaterLevelState;
    float DiffTempValue, DiffHumiValue;


    if (pSensorValue->Parameter[TemperatureID].Bit.bStable == 1 &&
        pSensorValue->Parameter[HumidityID].Bit.bStable == 1) {
        DiffTempValue = pSensorValue->Parameter[TemperatureID].Measure -  \
                        pSensorValue->Parameter[TemperatureID].Target;
        DiffHumiValue = (pSensorValue->Parameter[HumidityID].Measure -  \
                         pSensorValue->Parameter[HumidityID].Target);
				if (DiffTempValue >= (float) * pBuff[PAR_POS_TEMP_AH].uBytes * 0.1) {
						ParUnSaved[PAR_POS_PRO_ALARM_CODE - PAR_REDA_ADDR].uVal |=	bStateTempH;
				} else if (DiffTempValue <= (float) * pBuff[PAR_POS_TEMP_AH].uBytes * 0.05 && DiffTempValue >= (float) * pBuff[PAR_POS_TEMP_AL].uBytes * (-0.05)) {
						ParUnSaved[PAR_POS_PRO_ALARM_CODE - PAR_REDA_ADDR].uVal &=	~(bStateTempH|bStateTempL);
				} else if (DiffTempValue <= (float) * pBuff[PAR_POS_TEMP_AL].uBytes * (-0.1)) {
						ParUnSaved[PAR_POS_PRO_ALARM_CODE - PAR_REDA_ADDR].uVal |=	bStateTempL;
				}

				if (DiffHumiValue >=  *pBuff[PAR_POS_HUMI_AH].uBytes * 0.1) {
						ParUnSaved[PAR_POS_PRO_ALARM_CODE - PAR_REDA_ADDR].uVal |=	bStateHumiH;
				} else if (DiffHumiValue <=  *pBuff[PAR_POS_HUMI_AH].uBytes * 0.05 && DiffHumiValue >=  *pBuff[PAR_POS_HUMI_AL].uBytes * (-0.05) ) {
						ParUnSaved[PAR_POS_PRO_ALARM_CODE - PAR_REDA_ADDR].uVal &=	~(bStateHumiH|bStateHumiL);
				} else if (DiffHumiValue <=  *pBuff[PAR_POS_HUMI_AL].uBytes * (-0.1)) {
						ParUnSaved[PAR_POS_PRO_ALARM_CODE - PAR_REDA_ADDR].uVal |=	bStateHumiL;
				}
    }  else {
             ParUnSaved[PAR_POS_PRO_ALARM_CODE - PAR_REDA_ADDR].uVal &=	~(bStateHumiH | bStateHumiL | bStateTempH | bStateTempL);
    }


    WaterLevelState = High_Level_Check << 1 | Low_Level_Check;
    WaterLevelState == High_Water_Level_State ? Pump_Power_OFF : Pump_Power_ON;
    (!Low_Level_Check) ? (ParUnSaved[PAR_POS_PRO_ALARM_CODE - PAR_REDA_ADDR].uVal |=	bStateWaterL) :
    (ParUnSaved[PAR_POS_PRO_ALARM_CODE - PAR_REDA_ADDR].uVal &=	~bStateWaterL);
		
		 ParTab[PAR_POS_ALMAb0].uVal?  ParUnSaved[PAR_POS_PRO_ALARM_CODE - PAR_REDA_ADDR].uVal ?  ALM_ON : ALM_OFF  : ALM_OFF;
}



TypeCtrl SensorType;
void APPTask(void *argument)
{
    /* USER CODE BEGIN TempTask */
    /* Infinite loop */
    //    osStatus_t status;
    SensorType.Parameter[TemperatureID].Bit.bStable = 0;
    SensorType.Parameter[HumidityID].Bit.bStable = 0;
    unsigned short int   ADCBUFFER[4] = {0};
    pCallbackFun CallBackFunTab[] = {
        {GetSensorValueID,	(partab_type *) &ADCBUFFER, 	CurrentSampleValue},
        {DisplaySensorValueID,	(partab_type *)0,							ADCSample},
        {TempParaUpdataID,        &ParTab[0], 								TempParaUpdata },
        {TempStableID,        &ParTab[0],								     HumiParaUpdata },

    };
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&ADCBUFFER, sizeof(ADCBUFFER) / sizeof(ADCBUFFER[0]));

    for (;;) {
			 if( SensorType.Parameter[TemperatureID].Time_5min >10 )
			 {
        StableCheck(&ParTab[0], &SensorType);
        AlarmCode(&ParTab[0], &SensorType);
			 }

        for (char i = 0; i < EVEnumCount; i++) {
            CallBackFunTab[i].pFun(CallBackFunTab[i].ParTab, &SensorType);
        }

        EvFunTab[TemperatureID].ActFun(TemperatureID, &SensorType);

        if (SensorType.Parameter[TemperatureID].Bit.bStable == 0||!Low_Level_Check ) {
            SensorType.Parameter[HumidityID].OutValue = 0;
            SensorType.Parameter[HumidityID].Bit.bOver = 1;
        } else {
            SensorType.Parameter[HumidityID].Bit.bOver = 0;
        }

        EvFunTab[HumidityID].ActFun(HumidityID, &SensorType);
        osDelay(10);
    }

    /* USER CODE END TempTask */
}


