/*
作者：嗨小易（QQ：3443792007）
版权：本教程版权归嗨小易（QQ：3443792007）所有，未经授权不得转载、摘编或用于其他商业用途！！！
	  一经发现，后果自负！！！
*/

#include "dc_motor.h"

//直流电机初始化
void dc_motor_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
	
	RCC_APB2PeriphClockCmd(DC_MOTOR_IN1_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(DC_MOTOR_IN2_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(DC_MOTOR_ENA_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=DC_MOTOR_IN1_PIN;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(DC_MOTOR_IN1_PORT,&GPIO_InitStructure); 	   /* 初始化GPIO */
	
	GPIO_InitStructure.GPIO_Pin=DC_MOTOR_IN2_PIN;  //选择你要设置的IO口
	GPIO_Init(DC_MOTOR_IN2_PORT,&GPIO_InitStructure); 	   /* 初始化GPIO */
	
	GPIO_InitStructure.GPIO_Pin=DC_MOTOR_ENA_PIN;  //选择你要设置的IO口
	GPIO_Init(DC_MOTOR_ENA_PORT,&GPIO_InitStructure); 	   /* 初始化GPIO */
	
	DC_MOTOR_ENA=0;
	
	dc_motor_sta(0);
}

//直流电机正转
void _dc_motor_zturn(void)
{
	DC_MOTOR_IN1=1;
	DC_MOTOR_IN2=0;
}
//直流电机反转
void _dc_motor_fturn(void)
{
	DC_MOTOR_IN1=0;
	DC_MOTOR_IN2=1;
}
//直流电机方向控制
//dir：0正转，1反转
void dc_motor_dir_ctrl(u8 dir)
{
	if(dir==0)
		_dc_motor_zturn();
	else 
		_dc_motor_fturn();	
}

//直流电机工作状态
//sta：0关闭，1开启
void dc_motor_sta(u8 sta)
{
	if(sta==0)
	{
		DC_MOTOR_IN1=0;DC_MOTOR_IN2=0;		
	}	
}
