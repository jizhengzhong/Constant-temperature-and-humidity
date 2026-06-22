
#include "main.h"
#include "cmsis_os.h"

#define fliter_adv  0.5

float Temp_Set;      //�¶��趨ֵ
s16 Temp_Display;  //�¶���ʾֵ

int32_t DataFlt(float f_real, int32_t i_now);



float ADC2temp(unsigned short int *pBuff, char ch)
{
    unsigned char i, j;
    unsigned  short int  temp, val_list[MAX_ANALOG_SAMPLING_NUMBER] = {0}, val_cur;
    static unsigned short int Value_Index[3] = {0}, Value_List[3][MAX_ANALOG_SAMPLING_NUMBER] = {0};
    static unsigned int AdSum[3] = {0};
    static unsigned char TempCnt[3] = {0}, Collection_Count = 0;
    static float temp_real[3], temp_ctr[3];
    float temp_cur;
    unsigned char cnt;
    Value_Index[ch]++;

    if (Value_Index[ch] > MAX_ANALOG_SAMPLING_NUMBER) {
        Value_Index[ch] = 0;
    }

    Value_List[ch][Value_Index[ch]] =  *pBuff;
    cnt =  MAX_ANALOG_SAMPLING_NUMBER;

    while (cnt--) {
        val_list[cnt] = Value_List[ch][cnt];
    }

    for (j = 0; j < MAX_ANALOG_SAMPLING_NUMBER - 1; j++) {
        for (i = 0 ; i < MAX_ANALOG_SAMPLING_NUMBER - j - 1; i++) {
            if (val_list[i] > val_list[i + 1]) {
                temp = val_list[i];
                val_list[i] = val_list[i + 1];
                val_list[i + 1] = temp;
            }
        }
    }

    val_cur = val_list[MAX_ANALOG_SAMPLING_NUMBER / 2];
    AdSum[ch] +=  val_cur;
    TempCnt[ch]++;

    if (TempCnt[ch] >= 8) {
        TempCnt[ch] = 0;
        val_cur =  AdSum[ch] >> 3;
        AdSum[ch] = 0;
        temp_cur  				 =	(0.062 * val_cur + 1.5922) ;
        Collection_Count ++;

        if (Collection_Count > 50) {
            Collection_Count = 60;
            temp_real[ch]            =    fliter_adv * temp_real[ch]   + (1 - fliter_adv) * temp_cur;
            temp_ctr[ch]  = temp_real[ch] * (1 + (float)Temperature.PK1 * 0.0001) 	+ Temperature.Pb1;
        } else {
            temp_real[ch] = temp_cur;
            temp_ctr[ch] = temp_real[ch] ;
        }
    }

    return    temp_ctr[ch] ;
}

int32_t DataFlt(float f_real, int32_t i_now)
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


unsigned short int   ADCBUFFER[5] = {0};
void TempTask(void *argument)
{
    /* USER CODE BEGIN TempTask */
    /* Infinite loop */
    char i ;
    float Temp, LastTemp[3], Pressure, LastPressure;
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&ADCBUFFER, sizeof(ADCBUFFER) / sizeof(ADCBUFFER[0]));

    for (i = 0 ; i < PT_B3A3 ; i++) {
        LastTemp[i] = (0.062 * ADCBUFFER[i] + 1.6611) ;
    }

    for (;;) {
        Temp = ADC2temp(&ADCBUFFER[PT_B1A1],
                        PT_B1A1) +0.1 * (int16_t)ParTab[PAR_POS_STEAM_SENSOR_CALIBRATION].iVal;
        RunFlow .DisplaySteam = LastTemp[PT_B1A1] * ParTab[PAR_POS_FILTER].uVal * 0.01 + Temp *
                                (1 - ParTab[PAR_POS_FILTER].uVal * 0.01);
        LastTemp[PT_B1A1] = RunFlow .DisplaySteam;
        Temp = ADC2temp(&ADCBUFFER[PT_B2A2],
                        PT_B2A2) +0.1 * (int16_t)ParTab[PAR_POS_WATER_SENSOR_CALIBRATION].iVal;
        RunFlow.DisplayWater = LastTemp[PT_B2A2] * ParTab[PAR_POS_FILTER].uVal * 0.01 + Temp *
                               (1 - ParTab[PAR_POS_FILTER].uVal * 0.01);
        LastTemp[PT_B2A2] = RunFlow.DisplayWater;
        Temp = ADC2temp(&ADCBUFFER[PT_B3A3],
                        PT_B3A3) +0.1 * (int16_t)ParTab[PAR_POS_OBJECT_SENSOR_CALIBRATION].iVal;
        RunFlow.DisplayObject = LastTemp[PT_B3A3] * ParTab[PAR_POS_FILTER].uVal * 0.01 + Temp *
                                (1 - ParTab[PAR_POS_FILTER].uVal * 0.01);
        LastTemp[PT_B3A3] = RunFlow .DisplayObject;
        RunFlow .DisplaySteam = ParTab[PAR_POS_FUNCTION_SET].uVal & (1 << OBJECT_FUNCTION_BIT1) ? RunFlow.DisplayObject :
                                RunFlow .DisplaySteam;
        Temperature.Measure = RunFlow.StateParameter.DryStartFlag ? RunFlow.DisplayObject : RunFlow .DisplaySteam ;
        Pressure = 3.3 * ADCBUFFER[Pressure_ADC] / 4096;
        Pressure = ((Pressure / 5 - 0.04) / 0.0012858) + (int16_t)ParTab[PAR_POS_PRESSURE_SENSOR_CALIBRATION].iVal -
                   90;
        RunFlow.Pressure = Pressure * (1 - ParTab[PAR_POS_FILTER].uVal * 0.01) + LastPressure *
                           ParTab[PAR_POS_FILTER].uVal * 0.01;
        LastPressure = RunFlow.Pressure;

        if (RunFlow.Err.ErrorFlag != 0) {
            ALM_ON;
            osDelay(500);
            ALM_OFF;
            osDelay(500);
        }

        //		osMessageQueuePut ( FlowQueueHandle, &RunFlow, NULL, NULL) ;
        HeatControl(&Temperature, &Temp_Ctrl, &TEMP);
        osDelay(100);
    }

    /* USER CODE END TempTask */
}
