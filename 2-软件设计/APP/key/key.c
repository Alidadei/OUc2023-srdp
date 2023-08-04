/*
作者：嗨小易（QQ：3443792007）
版权：本教程版权归嗨小易（QQ：3443792007）所有，未经授权不得转载、摘编或用于其他商业用途！！！
	  一经发现，后果自负！！！
*/

#include "key.h"
#include "SysTick.h"

/*******************************************************************************
* 函 数 名         : KEY_Init
* 函数功能		   : 按键初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=KEY1_PIN|KEY2_PIN|KEY3_PIN|KEY4_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//上拉输入
	GPIO_Init(KEY_PORT,&GPIO_InitStructure);
}

/*******************************************************************************
* 函 数 名         : KEY_Scan
* 函数功能		   : 按键扫描检测
* 输    入         : mode=0:单次按下按键
					 mode=1：连续按下按键
* 输    出         : 0：未有按键按下
					 KEY_UP_PRESS：KEY_UP键按下
					 KEY0_PRESS：KEY0键按下
					 KEY1_PRESS：KEY1键按下
					 KEY2_PRESS：KEY2键按下
*******************************************************************************/
u8 KEY_Scan(u8 mode)
{
	static u8 key=1;
	
	if(mode==1) //连续按键按下
		key=1;
	if(key==1&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0)) //任意一个按键按下
	{
		delay_ms(1);  //消抖
		key=0;
		if(KEY1==0)
			return KEY1_PRESS; 
		else if(KEY2==0)
			return KEY2_PRESS; 
		else if(KEY3==0)
			return KEY3_PRESS; 
		else if(KEY4==0)
			return KEY4_PRESS;		
	}
	else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)    //无按键按下
		key=1;
	return 0;
}

