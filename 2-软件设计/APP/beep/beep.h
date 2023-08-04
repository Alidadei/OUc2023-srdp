#ifndef _beep_H
#define _beep_H

#include "system.h"

/*  蜂鸣器时钟端口、引脚定义 */
#define BEEP_PORT 			GPIOB   
#define BEEP_PIN 			GPIO_Pin_12
#define BEEP_PORT_RCC		RCC_APB2Periph_GPIOB

#define BEEP PBout(12)

void BEEP_Init(void);
void beep_alarm(u16 time,u16 fre);

#endif
