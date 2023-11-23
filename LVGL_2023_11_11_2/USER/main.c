#include "sys.h"
#include "delay.h"
#include "usart3.h"
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

//*ע��F407��ƵΪ84MHz*//


int main(void)
{

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
  delay_init(168);  			//��ʼ����ʱ����
  uart_init1(9600);			//��ʼ�����ڲ�����Ϊ115200
  LED_Init();					//��ʼ��LED
  KEY_Init(); 				//������ʼ��
  My_RTC_Init();				//RTCʱ�ӳ�ʼ��
  WKUP_Init(); 			//PF0 WKUP���ų�ʼ��
  LK_Init();//PF1 PHONE_LK���ų�ʼ��

  //LVGL��س�ʼ��
  TIM3_Int_Init(100-1,840-1);//��ʱ��3��ʼ������ʱ1ms����1000Hz����lvglʱ�����õ�Ƶ�ʱ���һ��
  TIM4_Int_Init(500-1,84-1);//��ʱ��4��ʼ��,2000Hz
  lv_init();//lvgl��ʼ��
  lv_port_disp_init();//��ʾ�豸�ӿڳ�ʼ��
  lv_port_indev_init();//�����豸�ӿڳ�ʼ��
  lv_mainstart();//����lvglҳ��

  uint8_t TilePageSet_Enable = 0;//ƽ����ͼҳ����µ���ҳ��ʹ�ܱ���
  while(1)
    {
      if(Check_WKUP())
        {
          delay_ms(5);
		  TilePageSet_Enable = 0;//����ƽ����ͼҳ���л�ʹ��
          lv_timer_handler();//��Լ5msһ�ε��øú���
          Voice_ctrol();//��������
          RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);//��ȡʱ��
          RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);//��ȡ����
          date_time_update();
//		Temp_Bright_Update(DHT11_Data[2],DHT11_Data[4]);

        }
      else//����ʹ��ʱ
        {
          sys_shut();
	      lv_timer_handler();//��Լ5msһ�ε��øú���
		  RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);//��ȡʱ��
          RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);//��ȡ����
		  date_time_update();//��Ȼ����ʱ��
		  if(!TilePageSet_Enable)
		  {
			  tile_page_set();//����Ϊ��ҳ��
			  TilePageSet_Enable = 1;
		  }
        }
    }
}