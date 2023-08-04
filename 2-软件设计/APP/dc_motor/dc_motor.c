/*
���ߣ���С�ף�QQ��3443792007��
��Ȩ�����̳̰�Ȩ����С�ף�QQ��3443792007�����У�δ����Ȩ����ת�ء�ժ�������������ҵ��;������
	  һ�����֣�����Ը�������
*/

#include "dc_motor.h"

//ֱ�������ʼ��
void dc_motor_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����ṹ�����
	
	RCC_APB2PeriphClockCmd(DC_MOTOR_IN1_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(DC_MOTOR_IN2_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(DC_MOTOR_ENA_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=DC_MOTOR_IN1_PIN;  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(DC_MOTOR_IN1_PORT,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	
	GPIO_InitStructure.GPIO_Pin=DC_MOTOR_IN2_PIN;  //ѡ����Ҫ���õ�IO��
	GPIO_Init(DC_MOTOR_IN2_PORT,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	
	GPIO_InitStructure.GPIO_Pin=DC_MOTOR_ENA_PIN;  //ѡ����Ҫ���õ�IO��
	GPIO_Init(DC_MOTOR_ENA_PORT,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	
	DC_MOTOR_ENA=0;
	
	dc_motor_sta(0);
}

//ֱ�������ת
void _dc_motor_zturn(void)
{
	DC_MOTOR_IN1=1;
	DC_MOTOR_IN2=0;
}
//ֱ�������ת
void _dc_motor_fturn(void)
{
	DC_MOTOR_IN1=0;
	DC_MOTOR_IN2=1;
}
//ֱ������������
//dir��0��ת��1��ת
void dc_motor_dir_ctrl(u8 dir)
{
	if(dir==0)
		_dc_motor_zturn();
	else 
		_dc_motor_fturn();	
}

//ֱ���������״̬
//sta��0�رգ�1����
void dc_motor_sta(u8 sta)
{
	if(sta==0)
	{
		DC_MOTOR_IN1=0;DC_MOTOR_IN2=0;		
	}	
}
