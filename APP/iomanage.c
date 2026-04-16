#include "main.h"
#include "cmsis_os.h"


sys_run_st_type SysRunSt;


void io_manage(void)
{
    sys_in_type InStLast;
    sys_in_type InSt;
    uint32_t Cnt;
    uint32_t u32t;
    
    //记录上次值用于边沿判定
    InStLast.St = SysRunSt.Io.InPut.St;
        //状态获取
    InSt.St = 0;
    if(IN_EN_ST==IN_YL)
        InSt.bits.in_YL = 1;
    
    if(IN_EN_ST == IN_RHW_SW)
        InSt.bits.in_RHW_SW = 1;
    
    if(IN_EN_ST == IN_DOOR_SW)
        InSt.bits.in_DOOR_SW = 1;
    
    if(IN_EN_ST == IN_CO2TANK1)
        InSt.bits.in_CO2TANK1 = 1;
    
    if(IN_EN_ST == IN_CO2TANK2)
        InSt.bits.in_CO2TANK2 = 1;
    
    SysRunSt.Io.InPutList[3].St = SysRunSt.Io.InPutList[2].St;
    SysRunSt.Io.InPutList[2].St = SysRunSt.Io.InPutList[1].St;
    SysRunSt.Io.InPutList[1].St = SysRunSt.Io.InPutList[0].St;
    SysRunSt.Io.InPutList[0].St = InSt.St;
    
    //输入  状态  确认
    for(Cnt=0;Cnt<sizeof(sys_in_type);Cnt++) {
        if((SysRunSt.Io.InPutList[0].Stv[Cnt] == SysRunSt.Io.InPutList[1].Stv[Cnt])
        && (SysRunSt.Io.InPutList[0].Stv[Cnt] == SysRunSt.Io.InPutList[2].Stv[Cnt])
        && (SysRunSt.Io.InPutList[0].Stv[Cnt] == SysRunSt.Io.InPutList[3].Stv[Cnt])
        ){
            SysRunSt.Io.InPut.Stv[Cnt] = SysRunSt.Io.InPutList[0].Stv[Cnt];
        } 
    }
    //边沿
    u32t = InStLast.St ^ SysRunSt.Io.InPut.St; //边沿存在
    SysRunSt.Io.FoundEdge.St = u32t & SysRunSt.Io.InPut.St; //开关闭合边沿
    SysRunSt.Io.LostEdge.St = u32t & InStLast.St; //开关断开边沿
    
    
//    //输出映射
//    if(SysRunSt.Io.OutPut.bits.out_COOL_PUMP)
//        OUT_COOL_PUMP = JK_OUT_ENABLE;
//    else
//        OUT_COOL_PUMP = JK_OUT_DISABLE;
    
    
//    if(SysRunSt.Io.OutPut.bits.out_WATER_PUMP)
//        OUT_WATER_PUMP = JK_OUT_ENABLE;
//    else
//        OUT_WATER_PUMP = JK_OUT_DISABLE;
    
    //200V供电
//    JK_OUT_AC220_en();
    
//    //蜂鸣器
//    if(IsSysErrFound())
//    {
//        if(bSysWave_500ms)
//            BuzzerOff();
//        else
//            BuzzerOn();
//    }
//    else
//    {
//        BuzzerOff();
//    }
}







