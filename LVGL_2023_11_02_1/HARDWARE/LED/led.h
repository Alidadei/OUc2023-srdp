#ifndef __LED_H
#define __LED_H
#include "sys.h"
//LED�˿ڶ���
#define LED0 PFout(9)	// DS0
#define LED1 PFout(10)	// DS1	
//void LED_Init(void);//��ʼ��	

void Light_Open(void);//����
void Light_Shut(void);//�ص�
void Light_Up(void);//����һ��
void Light_Down(void);//����һ��
void Auto_light(void);//�Զ�����			    
#endif
