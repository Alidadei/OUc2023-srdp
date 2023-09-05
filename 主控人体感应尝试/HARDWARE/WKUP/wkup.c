#include "wkup.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
//系统进入待机模式
void Sys_Enter_STOP(void)
{
//	while(WKUP_KD);			//等待WK_UP按键松开(在有RTC中断时,必须等WK_UP松开再进入待机)

  RCC_AHB1PeriphResetCmd(0X04FF,ENABLE);//复位所有IO口
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能PWR时钟
	
	//这里我们就直接关闭相关RTC中断
	RTC_ITConfig(RTC_IT_TS|RTC_IT_WUT|RTC_IT_ALRB|RTC_IT_ALRA,DISABLE);//关闭RTC相关中断，可能在RTC实验打开了。
	RTC_ClearITPendingBit(RTC_IT_TS|RTC_IT_WUT|RTC_IT_ALRB|RTC_IT_ALRA);//清除RTC相关中断标志位。
	

	//进入STOP模式，任意中断可以唤醒
	PWR_EnterSTOPMode(PWR_LowPowerRegulator_ON,  PWR_STOPEntry_WFI);
}
//检测WKUP脚的信号
//返回值0:PB15引脚检测到低电平（无人）一小段时间，则进入“睡眠”
//      1:有人,PB15引脚检测到高电平
u8 Check_WKUP(void)
{
  u8 t=0;
  u8 tx=0;//记录PB15的状态

  //延时检测三次
  while(t<3)
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
//中断15操作,检测到PB15脚的一个上升沿则说明此时有人
//中断线15上的中断检测
void EXTI15_10_IRQHandler(void)
{
  EXTI_ClearITPendingBit(EXTI_Line15); // 清除LINE15上的中断标志位
  if(WKUP_PIN==0)
    {
      Sys_Enter_STOP();
    }
  else
    {
			/*SystemInit();
      LED0=0;//点亮DS0
  */  }


}

//PB15 WKUP唤醒初始化
void WKUP_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;
  EXTI_InitTypeDef   EXTI_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //Pin15脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉，完全由外部输入决定
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化PB15


  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource15);//PB15必须连接到中断线15


  EXTI_InitStructure.EXTI_Line = EXTI_Line15;//LINE15
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断（立即触发）
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //高：5v!!/低边沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE15
  EXTI_Init(&EXTI_InitStructure);//配置EXTI


  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断15(中断线5-9共用一个中断函数）
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置NVIC


}
















