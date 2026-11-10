
 #include "main.h"
 #include "cmsis_os.h"

//#define fliter_adv  0.5

//float Temp_Set;      //温度设定值
//s16 Temp_Display;  //温度显示值

//int32_t DataFlt(float f_real, int32_t i_now);





//int32_t DataFlt(float f_real, int32_t i_now)
//{
//    int32_t i_real;
//    int32_t i_real_add2;
//    int32_t i_real_sub2; 
//    
//    if(f_real < 0)
//    {
//        i_real = (int32_t)f_real;
//        i_real_add2 = (int32_t)(f_real - 0.2f);
//        if(f_real <= -0.2f)
//            i_real_sub2 = (int32_t)(f_real + 0.2f);
//        else
//            i_real_sub2 = 1;
//        
//        if(i_real_add2 < i_real)  
//        {
//            i_now = i_real_add2;
//        } else if( i_real_sub2 > i_real)
//        {
//            i_now = i_real;
//        } else { 
//            if(i_now > i_real)
//                i_now = i_real;
//            else if(i_now < (i_real-1))
//                i_now = i_real-1;
//        }  
//    } else {
//        i_real = (int32_t)f_real;
//        i_real_add2 = (int32_t)(f_real + 0.2f); 
//        if(f_real >= 0.2f)
//            i_real_sub2 = (int32_t)(f_real - 0.2f);
//        else
//            i_real_sub2 = -1;

//        if(i_real_add2 > i_real) 
//        {
//            i_now = i_real_add2;
//        } else if( i_real_sub2 < i_real)
//        {
//            i_now = i_real;
//        } else { 
//            if(i_now < i_real) 
//                i_now = i_real;
//            else if(i_now > (i_real+1))
//                i_now = i_real+1;
//        }        
//    }

//    return i_now;
//}




