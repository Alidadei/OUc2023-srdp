#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "wkup.h"
int main(void)
{

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
  delay_init(168);    //初始化延时函数
  uart_init(256000);	//初始化串口波特率
  LED_Init();				//初始化LED,DS0亮，DS1灭
  //WKUP_Init();				//待机唤醒初始化
  LCD_Init();         //液晶初始化
  POINT_COLOR=RED;
  LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");
  LCD_ShowString(30,70,200,16,16,"WKUP TEST");
  LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
  LCD_ShowString(30,110,200,16,16,"2014/5/6");
  LCD_ShowString(30,130,200,16,16,"WK_UP:Stanby/WK_UP");
  while(1)
    {


      //检查是否是正常开机(有人开，无人关）
      if(Check_WKUP()==0)
        {
          GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);//GPIOF9,F10设置高，灯灭
          //Sys_Enter_STOP();	//无人，不是开机,进入待机模式
        }
      else
        {
          delay_ms(250);//延时250ms
          LED1=!LED1;
        }

    }

}

