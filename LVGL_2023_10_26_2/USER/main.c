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
//ALIENTEK 探索者STM32F407开发板 实验28
//LVGL移植实验   --库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK


	
int main(void)
{ 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  			//初始化延时函数
	uart_init1(9600);			//初始化串口波特率为115200
	LED_Init();					//初始化LED 
	KEY_Init(); 				//按键初始化  
	My_RTC_Init();				//RTC时钟初始化
	
	//LVGL相关初始化
	TIM3_Int_Init(10-1,8400-1);//定时器3初始化，计时1ms，即1000Hz，与lvgl时基配置的频率保持一致
	lv_init();//lvgl初始化
	lv_port_disp_init();//显示设备接口初始化
	lv_port_indev_init();//输入设备接口初始化
	
	lv_mainstart();
	
	while(1)
	{
		delay_ms(5);
		lv_timer_handler();//大约5ms一次调用该函数
		Voice_ctrol();//语音控制
		RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);//获取时间
		RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);//获取日期
		date_time_update();
	}
}
