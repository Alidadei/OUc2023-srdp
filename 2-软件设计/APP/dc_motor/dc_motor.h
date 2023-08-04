/*
���ߣ���С�ף�QQ��3443792007��
��Ȩ�����̳̰�Ȩ����С�ף�QQ��3443792007�����У�δ����Ȩ����ת�ء�ժ�������������ҵ��;������
	  һ�����֣�����Ը�������
*/

#ifndef _dc_motor_H
#define _dc_motor_H

#include "system.h"


//�ܽŶ���
#define DC_MOTOR_IN1_PORT 			GPIOB   
#define DC_MOTOR_IN1_PIN 			GPIO_Pin_10
#define DC_MOTOR_IN1_PORT_RCC		RCC_APB2Periph_GPIOB

#define DC_MOTOR_IN2_PORT 			GPIOB   
#define DC_MOTOR_IN2_PIN 			GPIO_Pin_11
#define DC_MOTOR_IN2_PORT_RCC		RCC_APB2Periph_GPIOB

#define DC_MOTOR_ENA_PORT 			GPIOA   
#define DC_MOTOR_ENA_PIN 			GPIO_Pin_7
#define DC_MOTOR_ENA_PORT_RCC		RCC_APB2Periph_GPIOA

#define DC_MOTOR_IN1				PBout(10) 
#define DC_MOTOR_IN2				PBout(11) 
#define DC_MOTOR_ENA				PAout(7) 

//��������
void dc_motor_init(void);
void dc_motor_dir_ctrl(u8 dir);
void dc_motor_sta(u8 sta);

#endif
