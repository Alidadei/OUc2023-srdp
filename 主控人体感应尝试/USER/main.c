#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "wkup.h"
int main(void)
{

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
  delay_init(168);    //��ʼ����ʱ����
  uart_init(256000);	//��ʼ�����ڲ�����
  LED_Init();				//��ʼ��LED,DS0����DS1��
  //WKUP_Init();				//�������ѳ�ʼ��
  LCD_Init();         //Һ����ʼ��
  POINT_COLOR=RED;
  LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");
  LCD_ShowString(30,70,200,16,16,"WKUP TEST");
  LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
  LCD_ShowString(30,110,200,16,16,"2014/5/6");
  LCD_ShowString(30,130,200,16,16,"WK_UP:Stanby/WK_UP");
  while(1)
    {


      //����Ƿ�����������(���˿������˹أ�
      if(Check_WKUP()==0)
        {
          GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);//GPIOF9,F10���øߣ�����
          //Sys_Enter_STOP();	//���ˣ����ǿ���,�������ģʽ
        }
      else
        {
          delay_ms(250);//��ʱ250ms
          LED1=!LED1;
        }

    }

}

