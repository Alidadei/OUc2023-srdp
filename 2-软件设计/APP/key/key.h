/*
���ߣ���С�ף�QQ��3443792007��
��Ȩ�����̳̰�Ȩ����С�ף�QQ��3443792007�����У�δ����Ȩ����ת�ء�ժ�������������ҵ��;������
	  һ�����֣�����Ը�������
*/

#ifndef _key_H
#define _key_H


#include "system.h"
 
#define KEY1_PIN   			GPIO_Pin_6    	
#define KEY2_PIN    		GPIO_Pin_7    	
#define KEY3_PIN    		GPIO_Pin_8    	
#define KEY4_PIN  			GPIO_Pin_9 

#define KEY_PORT 			GPIOB 			

//ʹ��λ��������
#define KEY1 	PBin(6)
#define KEY2 	PBin(7)
#define KEY3 	PBin(8)
#define KEY4 	PBin(9)

//�����������ֵ  
#define KEY1_PRESS 		1
#define KEY2_PRESS		2
#define KEY3_PRESS		3
#define KEY4_PRESS		4
 
void KEY_Init(void);
u8 KEY_Scan(u8 mode);

#endif
