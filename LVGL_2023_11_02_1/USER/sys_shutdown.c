#include "sys_shutdown.h"
#include "led.h"
#include "fan.h"
#include "sys.h"
#include "delay.h"
//PF0初始化
void WKUP_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //pin0脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉，完全由外部输入决定
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化PF0

}
//检测WKUP脚的信号
//返回值0:PF0脚检测到低电平（无人）一小段时间，则进入“睡眠”
//      1:有人,PF0脚检测到高电平
u8 Check_WKUP(void)
{
  u8 t=0;
  u8 tx=0;//记录PF0状态

  //延时检测三次
  while(t<2)
    {
      t++;
      if( WKUP_PIN==1)
        {
          tx=1;
        }
      else
        {
          tx=0;
        }
      delay_ms(330);//延时，三次下来大约是1s
    }
  return tx;
}
//关闭风扇和LED，系统进入“待机”模式
void Sys_shutdown()
{

  Light_Shut();
  FAN_Shut();
}
//测试函数
void test(void)
{
	u8 i=18;
	/*Light_Open();
  
  while(i>0)
    {
      Light_Down();
      delay_ms(1000);
    }*/
	
		while(i>0)
    {
      Light_Up();
      delay_ms(1000);
			i--;
    }


}