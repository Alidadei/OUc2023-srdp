#ifndef _app_demo_H
#define _app_demo_H
/*
作者：嗨小易（QQ：3443792007）
版权：本教程版权归嗨小易（QQ：3443792007）所有，未经授权不得转载、摘编或用于其他商业用途！！！
	  一经发现，后果自负！！！
*/

#include "system.h"


//定义结构体
typedef struct
{
	u8 mode;//模式
	u16 temp;//温度
	int temph;//温度上下限
	int templ;
	u8 auto_flag;//自动手动标志，0自动，1手动
	int gear;//档位
	u8 man;//感应有无人，0无人，1有人
	u8 sound;//感应有无声音，0无声，1有声
}_sys_ctrl;
extern _sys_ctrl sys_ctrl;


//函数声明
void appdemo_show(void);

#endif
