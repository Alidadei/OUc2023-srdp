#ifndef __WKUP_H   //人体感应O脚连接PF0，V脚连5V，G脚接地
#define __WKUP_H
#include "sys.h"
#define WKUP_PIN PFin(0)//检测外部PF0电平，0：无人，1：有人

u8 Check_WKUP(void);  			//检测WKUP脚的信号
void WKUP_Init(void); 			//PF0 WKUP引脚初始化
void sys_shut(void);	//系统进入待机模式
#endif
