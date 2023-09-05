#ifndef _FAN_H
#define _FAN_H
#include "sys.h"

//风扇端口定义(自己选）
#define FAN_PIN 
void FAN_Init(void);//初始化

void FAN_Open(void);//开风扇
void FAN_Shut(void);//关风扇
void FAN_Up(void);//开大一档
void FAN_Down(void);//开小一档
void Auto_FAN(void);//自动风扇调节
	


#endif

