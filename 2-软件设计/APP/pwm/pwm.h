/*
作者：嗨小易（QQ：3443792007）
版权：本教程版权归嗨小易（QQ：3443792007）所有，未经授权不得转载、摘编或用于其他商业用途！！！
	  一经发现，后果自负！！！
*/

#ifndef _pwm_H
#define _pwm_H

#include "system.h"

void pwm_init(u16 per,u16 psc);
void pwm_set_duty(u16 duty);
void pwm_out_sta(u8 sta);
#endif
