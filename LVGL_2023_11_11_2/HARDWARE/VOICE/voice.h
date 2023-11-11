#ifndef _VOICE_H
#define _VOICE_H
#include "stdio.h"
#include "stm32f4xx_conf.h"
#include "sys.h"
void Remind_pose(u8 mode);//提醒坐姿（用串口发送消息的形式）
void Voice_ctrol(void);//语音控制函数，cmd 为语音模块发过来的串口消息编号


#endif


