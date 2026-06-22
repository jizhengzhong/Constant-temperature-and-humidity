 
 #include "display.h"
 #include "stm32f1xx_hal.h"

/* 上电状态初始化 */
void PowerUpState(void)
{
//    if(Time_SpeakInital<=300) 
//			SPEAK_ON; //上电时蜂鸣器"滴"鸣叫
//    else
//    {
//        Time_SpeakInital=320;
//        SPEAK_OFF; 
//    }
//    IWDG->KR=0xaa;
    
    //UART2Error();
    //UART2Switch(); 
    //温度采样处理
//    if(SAMPLE.Flag_KtOK1&&SAMPLE.Flag_KtOK2&&SAMPLE.Flag_EnvOK)
//    {
//        SAMPLE.Flag_KtOK1=0;
//        SAMPLE.Flag_KtOK2=0;
//        SAMPLE.Flag_EnvOK=0;
//        DataFilter(Temp_KtSample1,Temp_KtSample2,Temp_EnvSample); 
//        CalculateTemp();//计算温度
//    }
    //上电时PID赋初值
    Temp_ISumVaule=0;  // //积分项累加值
    Temp_DFVaule=Temp_Measure; //Temp_Measure 温度测量值设为  微分现行赋值
    Temp_LastMeasure=Temp_Measure;
    SP_Buffer[0]=Temp_Display;
//    OTHER();
}