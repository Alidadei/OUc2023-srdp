#include "stm32f4xx.h"                
#include "BH1750.h"
#include "myiic.h"
#include "delay.h"
 
void LightSensor_Init(void)    //��ʼ������������
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);  //��ʼ��GPIOFʱ��
	
	//��ʼ��GPIOF PF7ͨ��
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AN;  //ģʽΪģ������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  //����������
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	Adc3_Init(); //��ʼ��ADC3
}
//��ȡ��������Light Sensor��ֵ
//0~100 0���100����
u16 LightSensor_Get_Val(void)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<LSENS_READ_TIMES;t++)
	{
		temp_val=Get_Adc3(ADC_Channel_5)+temp_val;// ȡ��ζ�ȡ���ܺͼ���һ��
		delay_ms(5);
	}
	temp_val=temp_val/LSENS_READ_TIMES;  //�õ�ƽ��ֵ
	if(temp_val>1600)
		temp_val=1600;
	return (u16)((100-(temp_val/16))/0.04);
	
}
 
