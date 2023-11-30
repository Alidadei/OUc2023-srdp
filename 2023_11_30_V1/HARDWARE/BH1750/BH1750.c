#include "stm32f4xx.h"                
#include "BH1750.h"
#include "myiic.h"
#include "delay.h"
 
void LightSensor_Init(void)    //初始化光敏传感器
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);  //初始化GPIOF时钟
	
	//初始化GPIOF PF7通道
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AN;  //模式为模拟输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  //不带上下拉
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	Adc3_Init(); //初始化ADC3
}
//读取光敏电阻Light Sensor的值
//0~100 0最暗，100最亮
u16 LightSensor_Get_Val(void)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<LSENS_READ_TIMES;t++)
	{
		temp_val=Get_Adc3(ADC_Channel_5)+temp_val;// 取多次读取的总和加在一起
		delay_ms(5);
	}
	temp_val=temp_val/LSENS_READ_TIMES;  //得到平均值
	if(temp_val>1600)
		temp_val=1600;
	return (u16)((100-(temp_val/16))/0.04);
	
}
 
