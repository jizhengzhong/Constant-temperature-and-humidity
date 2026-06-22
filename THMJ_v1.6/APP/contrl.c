#include "contrl.h"
#include "display.h"
#include "ADCSample.h"
#include "cmsis_os.h"

PID_Para Temperature = {

    0,//s16 AL;//超温偏差报警
    0,//s16 FL;//电磁阀偏差报警
    3,//s16 T; //控制周期
    10,//s16 P; //比例带
    200,//s16 I; //积分时间
    300,//s16 d; //微分时间
    0,//;s16 Pb1;//第一路热敏电阻零位调整
    0,//s16 PK1;//满度调整
    0,//s16 OutValue;
    0,//s32 ISumVaule;
    0,//float Target;
    0,//float Measure;
    0,//float LastMeasure;
    0,//float DFVaule;
    6,//u8 gama;
    70,//u8    AR;
    0,//u8 Flag_DisIsum:1;  //禁止加热积分累积
    0,//u8 Time_HeatCycle	;
    0,//u8 Time_HeatCounter;
    Temp_Hea_State//  void (*HeatCallback)( HeatState  );
};


InterParaTwo  Temp_Ctrl = {0};

Para_Flag_STRUCT TEMP = {0, 0, 0, 0, 0, 0}; //温度位段变量

CONTROL_STRUCT CONTROL = {0, 0, 0, 0, 0, 0}; //控制器位段变量


u16 SP_Buffer[11];  //分段温控温度参数
s16 ST_Buffer[12];  //分段温控时间参数

//时间变量定义

u16 TimMs_ModbusWait = 0; //通讯间隔时间
u16 Time_Base1ms     = 0; //1毫秒时基
u16 Time_Base1min    = 0; //1分钟时基
u16 Time_5min        = 0; //5分钟计时
u8  Time_PowerInital = 0; //上电初始化计时
u16 Time_SpeakInital = 0; //上电蜂鸣器初始化计时

u16 Time_Out = 0;        //通讯超时计时
u32 Time_runsec     = 0; //编程模式运行时间秒
u16 Time_runmin     = 0; //编程模式运行时间分
u16 Time_ADCSwitch   = 0; //ADC转换延时计时


s16 Temp_SetDiff;//与上一次设定值差值
float temp_i_display;
float temp_p_display;
void PID_Calculate(PID_Para *vPID, InterParaTwo *ParaCtrl)
{
    float temp_a, temp_b;
    float temp_c, temp_d;
    s16 PVauleRun, DVauleRun;

        PVauleRun = vPID->P; //得到积分的真实运行控制值
        DVauleRun = vPID->d; //得到微分的真实运行控制值


    if ((vPID->Target - vPID->Measure) > PVauleRun) { //禁止微分
        vPID->DFVaule    = vPID->Measure;      //微分先行后微分值
        vPID->LastMeasure = vPID->Measure; //上一次温度测量值
    } else {
        temp_c = (vPID->Measure - vPID->LastMeasure) * DVauleRun * vPID->gama;
        temp_c = temp_c + DVauleRun * vPID->DFVaule;
        temp_c = temp_c + vPID->Measure * vPID->T * vPID->gama;
        temp_d = temp_c / (DVauleRun + vPID->T * vPID->gama);

        if (temp_d >= 7000) {
            vPID->DFVaule = 7000;
        } else if (temp_d <= -500) {
            vPID->DFVaule = -500;
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
                if (!vPID->Flag_DisIsum) {
                    temp_b = 0;    //禁止积分
                }
            temp_c = temp_b * 1000;
            temp_c = (temp_c * vPID->T) / PVauleRun;
            vPID->ISumVaule = vPID->ISumVaule + temp_c;

            if (vPID->ISumVaule < 0) {
                vPID->ISumVaule = 0;
            }

            temp_a = vPID->ISumVaule / vPID->I;
            temp_i_display =	 temp_a;

            if (temp_a >= 1000) {
                temp_a = 1000;
                vPID->ISumVaule = (s32)vPID->I * 1000;
            }

            temp_c = (vPID->Target - vPID->DFVaule) * 1000;
            temp_b = temp_c / PVauleRun;
            temp_p_display = 	temp_b;
            Time_ADCSwitch++;
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


void Temp_Hea_State(u8 state)
	{   
		RunFlow.StateParameter.DryStartFlag ?  (state == Heat_OFF ?  Dry_OFF : Dry_ON ) : (state == Heat_OFF ?  HEAT_OFF : HEAT_ON) ;
}

void HeatControl(PID_Para *PID_Parameter, InterParaTwo *ParaCtrl,
                 Para_Flag_STRUCT *Para_Flag_Structment)
{
    s16 temp_a;
    s32 temp_b;

    if (Para_Flag_Structment->Flag_Over || Para_Flag_Structment->Flag_Protect) //温度溢出标志，ji
        //温度保护标志，1:停止加热 0:允许加热ji
    {
        PID_Parameter->DFVaule = PID_Parameter->Measure; //微分先行后微分值
        PID_Parameter->LastMeasure = PID_Parameter->Measure; //上一次温度测量值
        return;
    }
    if (CONTROL.Flag_TempRun) { ////控制器停止温度控制标志位
            if (CONTROL.Flag_EnRun == 0) {
                Time_5min = 0;
                PID_Parameter->Flag_DisIsum = 0;//禁止积分
            }


            Time_runsec = 0;//编程模式运行时间秒
            Time_runmin = 0;//编程模式运行时间分
  
    }

    //禁止积分处理
    if (Time_5min > 180) {
        Time_5min = 400;
        PID_Parameter->Flag_DisIsum = 1;  //禁止加热积分累积
    }

    //溢出、定时结束后停止温控、禁止温度控制时关断加热  //自整定标志位
    if (Para_Flag_Structment->Flag_Over || ((CONTROL.Flag_TempRun == 0))) { //温度溢出或停止温控
        PID_Parameter->HeatCallback(Heat_OFF);
        Para_Flag_Structment->Flag_HeatOut = 0;
        //PID控制赋初值
        Temp_SetDiff = 0;
        Time_5min = 0; //禁止积分计时清零
        PID_Parameter->Flag_DisIsum = 0;
        PID_Parameter->ISumVaule = 0;
        PID_Parameter->DFVaule = PID_Parameter->Measure;
        PID_Parameter->LastMeasure = PID_Parameter->Measure;
        return;
    }

    if (PID_Parameter->Time_HeatCycle >= PID_Parameter->T) { //加热一个周期   ParaTempPID.T = 5;   //控制周期
        PID_Parameter->Time_HeatCycle = 0; //Time_HeatCycle  加热控制周期计时
        PID_Parameter->Time_HeatCounter = 0;	//Time_HeatCounter 加热控制时间计时
        //输出功率控制
        PID_Calculate(PID_Parameter, ParaCtrl);

        if (Temp_SetDiff >= 5 || Temp_SetDiff <= -5) {
            Time_5min = 0;
            PID_Parameter->Flag_DisIsum = 0;
            Temp_SetDiff = 0;
        }//设定值变化超5，禁止积分

        temp_a = PID_Parameter->OutValue * PID_Parameter->T; //PID输出值*控制周期
        temp_b = temp_a * PID_Parameter->AR * 0.01; //乘积*功率
        PID_Parameter->OutValue = (s16)temp_b;
    } else {
        if (Para_Flag_Structment->Flag_Alm) {
            PID_Parameter-> HeatCallback(Heat_OFF);
            Para_Flag_Structment->Flag_HeatOut = 0;
        } else {
            if (PID_Parameter->Time_HeatCounter < PID_Parameter->OutValue) {
                PID_Parameter->	HeatCallback(Heat_ON);
                Para_Flag_Structment->Flag_HeatOut = 1;
            } else {
                PID_Parameter-> HeatCallback(Heat_OFF);
                Para_Flag_Structment->Flag_HeatOut = 0;
            }
        }
    }
}




