#ifndef __LED_H
#define __LED_H
#include "sys.h"
//LED端口定义
#define LED0 PFout(9)	// DS0
#define LED1 PFout(10)	// DS1	
//void LED_Init(void);//初始化	

void Light_Open(void);//开灯
void Light_Shut(void);//关灯
void Light_Up(void);//调亮一点
void Light_Down(void);//调暗一点
void Auto_light(void);//自动调光			    
#endif
