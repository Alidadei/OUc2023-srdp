#include "beep.h"
#include "SysTick.h"

/*******************************************************************************
* �� �� ��         : BEEP_Init
* ��������		   : ��������ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void BEEP_Init(void)	  //�˿ڳ�ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;	//����һ���ṹ�������������ʼ��GPIO

	RCC_APB2PeriphClockCmd(BEEP_PORT_RCC,ENABLE);   /* ����GPIOʱ�� */

	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=BEEP_PIN;		//ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;		  //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(BEEP_PORT,&GPIO_InitStructure); 	 /* ��ʼ��GPIO */
	
	GPIO_SetBits(BEEP_PORT,BEEP_PIN);
}

/*******************************************************************************
* �� �� ��       : beep_alarm
* ��������		 : ��������������
* ��    ��       : time����������ʱ��
				   fre������Ƶ��
* ��    ��    	 : ��
*******************************************************************************/
void beep_alarm(u16 time,u16 fre)
{
	while(time--)
	{
		BEEP=!BEEP;
		delay_10us(fre);	
	}		
}


