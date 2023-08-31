#include "sys.h"
#include "delay.h"
#include "usart1.h"
#include "led.h"
#include "key.h"
#include "voice.h"
#include "fan.h"

//STM32F407������������ģ�鴮��ͨѶ�����Ʒ���&С��
//����֧�֣�www.openedv.com
//���ߣ�yuhl

int main(void)
{
  //u8 mode=1;//ģʽѡ��1��Ĭ�ϣ�������ͨ��2 ��ͯ
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
  delay_init(168);		//��ʱ��ʼ��
  uart_init(9600);	//���ڳ�ʼ��������Ϊ9600
  LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�

  //��������
  /* delay_ms(5000);
   Remind_pose(mode);*/


  //������ͨѶ
  while(1)
    {
      Voice_ctrol();//��������

    }
}

