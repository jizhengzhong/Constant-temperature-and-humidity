#include "appmange.h"
#include "eeprom.h"
#include "iic.h"
#include "main.h"
#include "common.h"
#include "cmsis_os.h"
#include "math.h"


void PrintString(char *pString)
{
	osThreadFlagsSet (tid_RtcHandle, PR_BIT_1);
	printf("%s\r\n",pString);
	printf("%d/%d/%d %d:%d:%d\r\n",DateToUpdate.Year,DateToUpdate.Month,DateToUpdate.Date,  \
	sTime.Hours,sTime.Minutes,sTime.Seconds);
  printf("------------------------\r\n");

}

void PrintStart(void)
{
  char *pType[6]={"自定义灭菌","液体灭菌","溶解保温","固体灭菌","织物灭菌","预热程序",};
	printf("     STIK MJ Series     \r\n");
	printf("------------------------\r\n");
	printf("      Start  Point      \r\n");
	printf("------------------------\r\n");
	printf("Type:%s\r\n",(char *)pType[Procedure.type]);
	printf("Cycle No: %d\r\n",ParTab[PAR_POS_STERI_COUNT_CORRECT].uVal);
  PrintString("Start Time");
	printf("     Ster parameter     \r\n");
	printf("------------------------\r\n");

}
void F0_Calculation(void)
{
   if(Temperature.Measure>110 && RunFlow.StateParameter.MinuteFlag == 1 )
	 {
     RunFlow.f0_value +=  pow(  10,((float)(Temperature.Measure-121)/10) );
		 RunFlow.StateParameter.MinuteFlag=0;
	 }
}



