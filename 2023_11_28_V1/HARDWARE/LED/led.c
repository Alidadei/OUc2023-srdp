#include "led.h"
#include "bh1750.h"

//��ʼ��PC6��PC7Ϊ�����.��ʹ���������ڵ�ʱ��
//LED IO��ʼ��
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOCʱ��

  //GPIOC6,C7��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
	LightSensor_Init();
}
//����
void Light_Open(void)
{
	TIM_SetCompare1(TIM4,33);
}

//���
void Light_Shut(void)
{
	TIM_SetCompare1(TIM4,0);
}



void Light_Up(void)//����
{
	uint16_t pulse;
	pulse= TIM_GetCapture1(TIM4);
	pulse+=9;
	TIM_SetCompare1(TIM4,pulse);
}



void Light_Down(void)//����
{
	uint16_t pulse;
	pulse=TIM_GetCapture1(TIM4);
	pulse-=9;
	TIM_SetCompare1(TIM4,pulse);
}



void Auto_light(void)//�Զ�����
{
	uint16_t lux;
	uint16_t pulse;
	pulse= TIM_GetCapture1(TIM4);
	lux=LightSensor_Get_Val();

	if(lux<825)
    {
		pulse+=10;
		TIM_SetCompare1(TIM4,pulse);
    }
	else if(lux>1875)
    {
		pulse-=10;
		TIM_SetCompare1(TIM4,pulse);
	}
}


