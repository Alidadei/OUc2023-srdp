/*
作者：嗨小易（QQ：3443792007）
版权：本教程版权归嗨小易（QQ：3443792007）所有，未经授权不得转载、摘编或用于其他商业用途！！！
	  一经发现，后果自负！！！
*/

#ifndef _key_H
#define _key_H


#include "system.h"
 
#define KEY1_PIN   			GPIO_Pin_6    	
#define KEY2_PIN    		GPIO_Pin_7    	
#define KEY3_PIN    		GPIO_Pin_8    	
#define KEY4_PIN  			GPIO_Pin_9 

#define KEY_PORT 			GPIOB 			

//使用位操作定义
#define KEY1 	PBin(6)
#define KEY2 	PBin(7)
#define KEY3 	PBin(8)
#define KEY4 	PBin(9)

//定义各个按键值  
#define KEY1_PRESS 		1
#define KEY2_PRESS		2
#define KEY3_PRESS		3
#define KEY4_PRESS		4
 
void KEY_Init(void);
u8 KEY_Scan(u8 mode);

#endif
