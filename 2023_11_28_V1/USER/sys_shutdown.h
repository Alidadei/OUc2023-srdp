#ifndef __WKUP_H   //人体感应O脚连接PF0，V脚连5V，G脚接地
#define __WKUP_H	//手机锁红线接PF1，黑线接地
#include "sys.h"
#define WKUP_PIN PFin(0)//检测外部PF0电平，0：无人，1：有人
#define PHONE_LK PFout(1)//手机锁控制引脚，0：上锁，1：开锁

u8 Check_WKUP(void);  			//检测WKUP脚的信号
void LK_Init(void);//PF1 PHONE_LK引脚初始化
void WKUP_Init(void); 			//PF0 WKUP引脚初始化
void Lock_open(void);//打开手机锁
void sys_shut(void);	//系统进入待机模式
#endif
