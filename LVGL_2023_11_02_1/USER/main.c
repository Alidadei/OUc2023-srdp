#include "sys.h"
#include "delay.h"
#include "usart1.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "voice.h"
#include "fan.h"
#include "touch.h"
#include "timer.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "LV_GUITASK.h"
#include "rtc.h"
#include "sys_shutdown.h"

//*注意F407主频为84MHz*//


int main(void)
{

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
  delay_init(168);  			//初始化延时函数
  uart_init1(9600);			//初始化串口波特率为115200
  //LED_Init();					//初始化LED
  KEY_Init(); 				//按键初始化
  My_RTC_Init();				//RTC时钟初始化

  //LVGL相关初始化
  TIM3_Int_Init(100-1,840-1);//定时器3初始化，计时1ms，即1000Hz，与lvgl时基配置的频率保持一致
  TIM4_Int_Init(100-1,420-1);//定时器4初始化,2000Hz
  lv_init();//lvgl初始化
  lv_port_disp_init();//显示设备接口初始化
  lv_port_indev_init();//输入设备接口初始化
  WKUP_Init();//初始化PF0，人体检测口
  lv_mainstart();


delay_ms(1000);
  while(1)
    {
      //Light_Down();
      delay_ms(5);
      lv_timer_handler();//大约5ms一次调用该函数
      Voice_ctrol();//语音控制
      RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);//获取时间
      RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);//获取日期
      date_time_update();
      /*若PF0脚检测到有人
      if(Check_WKUP())
        {

        }
      //无人
      else
        {

          Sys_shutdown();

        }*/


    }
}
