#include "SysTick.h"


void delay_us(u32 nus)
{		
	while(nus--);
}

void delay_10us(u32 nus)
{    
   u8 i=0;  
   while(nus--)
   {
      i=12;  //�Լ�����
      while(i--) ;    
   }
}

void delay_ms(u16 nms)
{    
   u16 i=0;  
   while(nms--)
   {
      i=1200;  //�Լ�����
      while(i--) ;    
   }
}


