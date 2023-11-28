#include "led.h"
#include "fan.h"
#include "delay.h"
#include "sys_shutdown.h"
//人体感应O脚连接PF0，V脚连5V，G脚接地
//手机锁红线接PF1，黑线接地

//PF1 PHONE_LK引脚初始化
void LK_Init(void)
{

  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //Pin1脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化配置PF1
  GPIO_ResetBits(GPIOF,GPIO_Pin_1);//初始化手机锁为上锁状态
}
//PF0 初始化
void WKUP_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //Pin0脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉，完全由外部输入决定
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化PF0


}
//检测WKUP脚的信号
//返回值0:PF0引脚检测到低电平（无人）一小段时间，则进入“睡眠”
//      1:有人,PF0引脚检测到高电平
u8 Check_WKUP(void)
{
  u8 t=0;
  u8 tx=0;//记录PF0的状态

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
      //delay_ms(330);//延时，三次下来大约是1s
    }
  return tx;
}
//打开手机锁
void Lock_open(void)
{
	GPIO_SetBits(GPIOF,GPIO_Pin_1);//给高电平开锁
	delay_ms(500);
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);//开完锁后，初始化手机锁为上锁状态;
}
//系统进入待机模式
void sys_shut(void)
{
	Light_Shut();//关灯
	FAN_Shut();//关风扇
  //Lock_open();//打开手机锁
}
