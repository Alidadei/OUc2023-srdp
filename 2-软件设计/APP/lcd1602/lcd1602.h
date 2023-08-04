/*
���ߣ���С�ף�QQ��3443792007��
��Ȩ�����̳̰�Ȩ����С�ף�QQ��3443792007�����У�δ����Ȩ����ת�ء�ժ�������������ҵ��;������
	  һ�����֣�����Ը�������
*/

#ifndef _lcd1602_H
#define _lcd1602_H

#include "system.h"


//�ܽŶ���
#define LCD1602_RS_PORT 			GPIOB   
#define LCD1602_RS_PIN 				GPIO_Pin_0
#define LCD1602_RS_PORT_RCC			RCC_APB2Periph_GPIOB

#define LCD1602_RW_PORT 			GPIOB   
#define LCD1602_RW_PIN 				GPIO_Pin_1
#define LCD1602_RW_PORT_RCC			RCC_APB2Periph_GPIOB

#define LCD1602_E_PORT 				GPIOB   
#define LCD1602_E_PIN 				GPIO_Pin_2
#define LCD1602_E_PORT_RCC			RCC_APB2Periph_GPIOB


#define LCD1602_DATAPORT_PORT		GPIOC
#define LCD1602_DATAPORT_PIN		(GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7)
#define LCD1602_DATAPORT_PORT_RCC	RCC_APB2Periph_GPIOC
#define LCD1602_DATAPORT(dat) 		GPIO_Write(LCD1602_DATAPORT_PORT,dat)	

#define LCD1602_RS 	PBout(0)
#define LCD1602_RW 	PBout(1) 
#define LCD1602_E 	PBout(2) 


//��������
void lcd1602_init(void);
void lcd1602_clear(void);
void lcd1602_show_string(u8 x,u8 y,u8 *str);
void lcd1602_show_num(u8 x,u8 y,u8 num,u8 mode);
void lcd1602_show_nums(u8 x,u8 y,u32 num,u8 len,u8 mode);

#endif
