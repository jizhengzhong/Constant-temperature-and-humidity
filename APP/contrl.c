#include "contrl.h"
#include "display.h"
#include "ADCSample.h"
#include "cmsis_os.h"
#include "math.h"
#include "W25Qxx.h"
#include "appmange.h"

typedef struct {
    uint16_t Temperature;
    uint16_t Humidity;
    int16_t SetTime;
} ProModeStruct;

uint16_t ProCircleNum;
ProModeStruct *ProModeStructment = NULL;
	uint16_t CurrentDuan=0;
void ProgramAPP(partab_type *pTab, TypeCtrl *pSensorValue)
{   
    float TempValue, HumidityValue;
	  uint16_t SetProCircleTimes;
    ProModeStructment = (ProModeStruct *)pTab+CurrentDuan;
	  SetProCircleTimes = ParTab[PAR_POS_PRO0_REPEAT+ParTab[PAR_POS_PRO_INDEX].uVal].uVal;
    if ( ParTab[PAR_POS_Run_Stop_Command].uVal && ProCircleNum < SetProCircleTimes) {
           if ( ProModeStructment->SetTime != 0)
					{
						  ParTab[PAR_POS_HUMI_SV].uVal = ProModeStructment->Humidity;
						  ParTab[PAR_POS_TEMP_SV].uVal = ProModeStructment->Temperature;
							pSensorValue->SetRunTime = ProModeStructment->SetTime;
							HumidityValue =10 *(pSensorValue->Parameter[HumidityID].Measure - ProModeStructment->Humidity);
							TempValue =10*(pSensorValue->Parameter[TemperatureID].Measure - ProModeStructment->Temperature);

            if ((fabs(HumidityValue) <= ParTab[PAR_POS_HUMI_DIFF_COUNT].uVal &&
                    fabs(TempValue) <= ParTab[PAR_POS_TEMP_DIFF_COUNT].uVal) || pSensorValue->Flag.TimeMode == 0) {
                pSensorValue->Flag.EnTime = 1;
                if (pSensorValue->RunTime  >= ProModeStructment->SetTime * 60) {
                    pSensorValue->RunTime = 0;
									  CurrentDuan++;
								   	ProModeStructment++;
					          pSensorValue-> Parameter[TemperatureID].Bit.bStable = 0;
										pSensorValue-> Parameter[HumidityID].Bit.bStable = 0;
									  pSensorValue->Parameter[TemperatureID].Time_5min = 0;
                    if (ProModeStructment->SetTime == 0 || CurrentDuan > 29 ) {
										     	 ProCircleNum ++ ;
                        if ( ProCircleNum  >= SetProCircleTimes) {
                           	pSensorValue->Flag.EnTime =   pSensorValue->Flag.TimeMode ?      0  : 1 ;
													  ParTab[PAR_POS_Run_Stop_Command].uVal = 0;
                        } else
												{
												 CurrentDuan=0;
												}
                    }

                }
            } else {
               pSensorValue->Flag.EnTime =   pSensorValue->Flag.TimeMode ?      0  : 1 ;
            }
        } 
					else  {
						 ParTab[PAR_POS_Run_Stop_Command].uVal = 0;
             pSensorValue->Flag.EnTime =   pSensorValue->Flag.TimeMode ?      0  : 1 ;
						
        }
    } 
		else {
           CurrentDuan = 0;
           ProCircleNum=0;
    }
}

void FixedMode( TypeCtrl *pSensorValue )
{
    if (pSensorValue->Flag.CurrentModeState ==  FixedModeID)
		{
				  SensorType.SetRunTime = ParTab[PAR_POS_RUN_TIME_SET].uVal;
				 if ( pSensorValue->SetRunTime >0 && pSensorValue->RunTime  >= pSensorValue->SetRunTime * 60 )
				 {
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

    /* Infinite loop */
    for (;;) {

        if (SensorType.Flag.CurrentModeState ==  ProgramModeID) {
					  ProIndex = 90*ParTab[PAR_POS_PRO_INDEX].uVal;
					  ProIndex = ProIndex + PAR_POS_PRO0_TEMP00;
            ProgramAPP(&ParTab[ProIndex], &SensorType);
        }
				else 
				{
					FixedMode( &SensorType );
        }

        osDelay(10);
    }

    /* USER CODE END ProgramemodeTask */
}






