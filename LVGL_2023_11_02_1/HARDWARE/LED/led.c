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
}
//������
void Light_Open(void){
  {
   GPIO_SetBits(GPIOC,  GPIO_Pin_6|GPIO_Pin_7);//PC6&PC7��ߵ�ƽ������
  }}

void Light_Shut(void){
	{
		 GPIO_ResetBits(GPIOC,  GPIO_Pin_6|GPIO_Pin_7);//���
	}}


	
	void Light_Up(void)//���� 
{
uint32_t pulse;  
pulse=TIM_GetCapture3(TIM4); 
   pulse+=9;        
   TIM_SetCompare3(TIM4,pulse);  
}



void Light_Down(void)//���� 
{
uint32_t pulse;  
pulse=TIM_GetCapture3(TIM4);        
   pulse-=99;        
   TIM_SetCompare3(TIM4,pulse);  
}



void Auto_light(void)//�Զ�����			
{
	float lux;
uint16_t pulse;
lux=read_BH1750(); 

if(lux<70){        
   pulse+=10;       
   TIM_SetCompare3(TIM4,pulse); 
}else if(lux>75){  
   pulse-=10;
   TIM_SetCompare3(TIM4,pulse);   
}  
}


