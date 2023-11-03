#ifndef __SYS_SHUTDOWN_H
#define __SYS_SHUTDOWN_H
#include "sys.h"
#define WKUP_PIN PFin(0)//检测外部PF0的电平，0：无人，1：有人

u8 Check_WKUP(void);  			//检测WKUP脚的信号
void WKUP_Init(void); 			//PF0 WKUP唤醒初始化
void Sys_shutdown(void);	//系统进入待机模式

//测试函数
void test(void);
#endif