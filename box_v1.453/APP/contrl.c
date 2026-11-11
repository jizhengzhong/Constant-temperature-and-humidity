#include "contrl.h"
#include "display.h"
#include "ADCSample.h"
#include "cmsis_os.h"
#include "math.h"
#include "W25Qxx.h"
#include "appmange.h"

typedef struct {
    int16_t Temperature;
    uint16_t Humidity;
    int16_t SetTime;
} ProModeStruct;

uint16_t ProCircleNum;
ProModeStruct *ProModeStructment = NULL;
uint16_t CurrentDuan = 0;
void ProgramAPP(partab_type *pTab, TypeCtrl *pSensorValue)
{
    float TempValue, HumidityValue;
    uint16_t SetProCircleTimes;
    uint16_t pro = ParTab[PAR_POS_PRO_INDEX].uVal;
    ProModeStructment = (ProModeStruct *)pTab + CurrentDuan;
    if (pro > (PAR_POS_PRO9_REPEAT - PAR_POS_PRO0_REPEAT)) {
        pro = 0;
    }
    SetProCircleTimes = ParTab[PAR_POS_PRO0_REPEAT + pro].uVal;

    if (ParTab[PAR_POS_Run_Stop_Command].uVal && ProCircleNum < SetProCircleTimes) {
        if (ProModeStructment->SetTime != 0) {
            pSensorValue->Parameter[HumidityID].Target = ParTab[PAR_POS_HUMI_SV].uVal = ProModeStructment->Humidity;
            pSensorValue->Parameter[TemperatureID].Target = ParTab[PAR_POS_TEMP_SV].iVal = ProModeStructment->Temperature;
            pSensorValue->SetRunTime = ProModeStructment->SetTime;
            HumidityValue = 10 * (pSensorValue->Parameter[HumidityID].Measure - ProModeStructment->Humidity);
            TempValue = 10 * (pSensorValue->Parameter[TemperatureID].Measure - ProModeStructment->Temperature);

            if ((fabs(HumidityValue) <= ParTab[PAR_POS_HUMI_DIFF_COUNT].uVal &&
                    fabs(TempValue) <= ParTab[PAR_POS_TEMP_DIFF_COUNT].uVal) || pSensorValue->Flag.TimeMode == 0) {
                pSensorValue->Flag.EnTime = 1;

                if (pSensorValue->RunTime  >= ProModeStructment->SetTime * 60) {
                    pSensorValue->RunTime = 0;
                    CurrentDuan++;
                    ProModeStructment++;
                    pSensorValue-> Parameter[TemperatureID].Bit.bStable = 0;
                    pSensorValue-> Parameter[HumidityID].Bit.bStable = 0;
                    pSensorValue->Parameter[TemperatureID].DiffStableSecond = 0;
                    pSensorValue->Parameter[HumidityID].DiffStableSecond = 0;

                    if (ProModeStructment->SetTime == 0 || CurrentDuan > 29) {
                        ProCircleNum ++ ;

                        if (ProCircleNum  >= SetProCircleTimes) {
                            pSensorValue->Flag.EnTime =   pSensorValue->Flag.TimeMode ?      0  : 1 ;
                            ParTab[PAR_POS_Run_Stop_Command].uVal = 0;
                        } else {
                            CurrentDuan = 0;
                        }
                    }
                }
            } else {
                pSensorValue->Flag.EnTime =   pSensorValue->Flag.TimeMode ?      0  : 1 ;
            }
        } else  {
            ParTab[PAR_POS_Run_Stop_Command].uVal = 0;
            pSensorValue->Flag.EnTime =   pSensorValue->Flag.TimeMode ?      0  : 1 ;
        }
    } else {
        CurrentDuan = 0;
        ProCircleNum = 0;
    }
}

void FixedMode(TypeCtrl *pSensorValue)
{
    if (pSensorValue->Flag.CurrentModeState ==  FixedModeID) {
        SensorType.SetRunTime = ParTab[PAR_POS_RUN_TIME_SET].uVal;

        if (pSensorValue->SetRunTime > 0 && pSensorValue->RunTime  >= pSensorValue->SetRunTime * 60) {
            pSensorValue->RunTime = 0;
            ParTab[PAR_POS_Run_Stop_Command].uVal = 0;
        }
    }
}



void Temp_Hea_State(bool state)
{
    state == Heat_OFF  ? HEAT_OFF : HEAT_ON;
}

void Hum_Hea_State(bool state)
{
    state == Heat_OFF ? Hum_HEAT_OFF : Hum_HEAT_ON;
}

float dgdsaf;
void StableCheck(partab_type *pBuff, TypeCtrl*pSensorValue)
{
   volatile float DiffTempValue, DiffHumiValue;
  dgdsaf=  DiffTempValue = fabs(pSensorValue->Parameter[TemperatureID].Measure - pSensorValue->Parameter[TemperatureID].Target);

    if (DiffTempValue <= pBuff[PAR_POS_TEMP_DIFF_COUNT].uVal * 0.1) {
        if (pSensorValue->Parameter[TemperatureID].DiffStableSecond > pBuff[PAR_POS_TEMP_STA_SECOND].uVal) {
            pSensorValue->Parameter[TemperatureID].DiffStableSecond = pBuff[PAR_POS_TEMP_STA_SECOND].uVal + 5;
            pSensorValue->Parameter[TemperatureID].Bit.bStable = 1;
                pSensorValue->Flag.EnTime = 1;
        }
    } else {
        pSensorValue->Flag.EnTime =   pSensorValue->Flag.TimeMode ?      0  : 1 ;
        pSensorValue->Parameter[TemperatureID].DiffStableSecond = 0;
    }

    DiffHumiValue = fabs(pSensorValue->Parameter[HumidityID].Measure - pSensorValue->Parameter[HumidityID].Target);
    if ( DiffHumiValue <= pBuff[PAR_POS_HUMI_DIFF_COUNT].uVal * 0.1) {
        if (pSensorValue->Parameter[HumidityID].DiffStableSecond > pBuff[PAR_POS_HUMI_STA_SECOND].uVal) {
            pSensorValue->Parameter[HumidityID].DiffStableSecond = pBuff[PAR_POS_HUMI_STA_SECOND].uVal + 5;
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

        if (DiffTempValue >= (float) pBuff[PAR_POS_TEMP_AH].uVal * 0.1) {
            ParUnSaved[PAR_POS_PRO_ALARM_CODE - PAR_REDA_ADDR].uVal |=	bStateTempH;
        } else if (DiffTempValue <= (float) pBuff[PAR_POS_TEMP_AH].uVal * 0.05
                   && DiffTempValue >= (float) pBuff[PAR_POS_TEMP_AL].uVal * (-0.05)) {
            ParUnSaved[PAR_POS_PRO_ALARM_CODE - PAR_REDA_ADDR].uVal &=	~(bStateTempH | bStateTempL);
        } else if (DiffTempValue <= (float) pBuff[PAR_POS_TEMP_AL].uVal * (-0.1)) {
            ParUnSaved[PAR_POS_PRO_ALARM_CODE - PAR_REDA_ADDR].uVal |=	bStateTempL;
        }

        if (DiffHumiValue >=  pBuff[PAR_POS_HUMI_AH].uVal * 0.1) {
            ParUnSaved[PAR_POS_PRO_ALARM_CODE - PAR_REDA_ADDR].uVal |=	bStateHumiH;
        } else if (DiffHumiValue <=  pBuff[PAR_POS_HUMI_AH].uVal * 0.05
                   && DiffHumiValue >=  pBuff[PAR_POS_HUMI_AL].uVal* (-0.05)) {
            ParUnSaved[PAR_POS_PRO_ALARM_CODE - PAR_REDA_ADDR].uVal &=	~(bStateHumiH | bStateHumiL);
        } else if (DiffHumiValue <=  pBuff[PAR_POS_HUMI_AL].uVal * (-0.1)) {
            ParUnSaved[PAR_POS_PRO_ALARM_CODE - PAR_REDA_ADDR].uVal |=	bStateHumiL;
        }
    }  else {
        ParUnSaved[PAR_POS_PRO_ALARM_CODE - PAR_REDA_ADDR].uVal &=	~(bStateHumiH | bStateHumiL | bStateTempH | bStateTempL);
    }

    WaterLevelState = High_Level_Check << 1 | Low_Level_Check;
    WaterLevelState == High_Water_Level_State ? Pump_Power_OFF : Pump_Power_ON;
    (!Low_Level_Check) ? (ParUnSaved[PAR_POS_PRO_ALARM_CODE - PAR_REDA_ADDR].uVal |=	bStateWaterL) :
    (ParUnSaved[PAR_POS_PRO_ALARM_CODE - PAR_REDA_ADDR].uVal &=	~bStateWaterL);
    ParTab[PAR_POS_ALMAb0].uVal&(1<<0) ?  ParUnSaved[PAR_POS_PRO_ALARM_CODE - PAR_REDA_ADDR].uVal ?  ALM_ON : ALM_OFF  : ALM_OFF;
}

/* USER CODE BEGIN Header_ProgramemodeTask */
/**
* @brief Function implementing the tid_PromodeTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ProgramemodeTask */
void ProgramemodeTask(void *argument)
{
    /* USER CODE BEGIN ProgramemodeTask */
    uint16_t ProIndex;
    SensorType.Parameter[TemperatureID].HeatCallback = Temp_Hea_State;
    SensorType.Parameter[HumidityID].HeatCallback = Hum_Hea_State;
		ParTab[PAR_POS_Run_Stop_Command].uVal = ParReceive[PAR_POS_Run_Stop_Command].uVal = 0;
		SensorType.Flag.PowerOnRun = ParTab[PAR_POS_bCTROL].uVal >> bCtrol_Power_On_Run_Mode ;

    /* Infinite loop */
    for (;;) {
			      if( SensorType.Flag.PowerOnRun )
						{
				     ParTab[PAR_POS_Run_Stop_Command].uVal = 1;
							SensorType.Flag.PowerOnRun =0;
						}
			
        if (SensorType.Flag.CurrentModeState ==  ProgramModeID) {
            ProIndex = ParTab[PAR_POS_PRO_INDEX].uVal;
            if (ProIndex > (PAR_POS_PRO9_REPEAT - PAR_POS_PRO0_REPEAT)) {
                ProIndex = 0;
            }
            ProIndex = 90 * ProIndex;
            ProIndex = ProIndex + PAR_POS_PRO0_TEMP00;
            ProgramAPP(&ParTab[ProIndex], &SensorType);
        } else {
            FixedMode(&SensorType);
        }

        if (ParTab[PAR_POS_Run_Stop_Command].uVal == 1) {
            StableCheck(&ParTab[0], &SensorType);
            AlarmCode(&ParTab[0], &SensorType);
            SensorType.Parameter[TemperatureID].Target =    ParTab[PAR_POS_TEMP_SV].iVal;
            SensorType.Parameter[HumidityID].Target =    ParTab[PAR_POS_HUMI_SV].uVal;
        }

        osDelay(10);
    }

    /* USER CODE END ProgramemodeTask */
}






