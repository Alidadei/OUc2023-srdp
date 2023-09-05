#ifndef __WKUP_H
#define __WKUP_H
#include "sys.h"
#define WKUP_PIN PBin(15)//检测外部PB15的电平，0：无人，1：有人

u8 Check_WKUP(void);  			//检测WKUP脚的信号
void WKUP_Init(void); 			//PB15 WKUP唤醒初始化
void Sys_Enter_STOP(void);	//系统进入待机模式
#endif


