#include "sys.h"
#include "delay.h"
#include "usart1.h"
#include "led.h"
#include "key.h"
#include "voice.h"
#include "fan.h"

//STM32F407开发板与语音模块串口通讯，控制风扇&小灯
//技术支持：www.openedv.com
//作者：yuhl

int main(void)
{
  //u8 mode=1;//模式选择：1（默认）成人普通，2 儿童
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
  delay_init(168);		//延时初始化
  uart_init(9600);	//串口初始化波特率为9600
  LED_Init();		  		//初始化与LED连接的硬件接口

  //提醒坐姿
  /* delay_ms(5000);
   Remind_pose(mode);*/


  //和主控通讯
  while(1)
    {
      Voice_ctrol();//语音控制

    }
}

