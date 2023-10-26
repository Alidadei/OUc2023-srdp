#include "led.h" 
#include "bh1750.h"

//��ʼ��PF9��PF10Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOFʱ��

  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
	
  //GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);//GPIOF9,F10���øߣ�����

}
//������
void Light_Open(void){
  {
    GPIO_ResetBits(GPIOF,  GPIO_Pin_10|GPIO_Pin_9);//PF9&PF10��͵�ƽ������
  }}

void Light_Shut(void){
	{
		GPIO_SetBits(GPIOF,  GPIO_Pin_10|GPIO_Pin_9);//���
	}}

	
void Light_Up(void)
{
uint32_t pulse;  
pulse=TIM_GetCapture2(TIM3); 

if(pulse<500){       
   pulse+=10;        
   TIM_SetCompare2(TIM3,pulse);  
}else if(300<pulse && pulse<=500){
   pulse+=5;
   TIM_SetCompare2(TIM3,pulse); 
}else{                  
   pulse=500;        
   TIM_SetCompare2(TIM3,pulse);   
} 
}//����һ��
void Light_Down(void)
{
uint32_t pulse;  

pulse=TIM_GetCapture2(TIM3);  
if(pulse>300){        
   pulse-=10;        
   TIM_SetCompare2(TIM3,pulse);  
}else if(150<pulse && pulse<=300){ 
   pulse-=5;
   TIM_SetCompare2(TIM3,pulse);  
}else if(pulse<=150){  
   pulse=150;
   TIM_SetCompare2(TIM3,pulse); 
}
}//����һ��
void Auto_light(void)
{
	float lux;
uint16_t pulse;
lux=read_BH1750(); 

if(lux<70){        
   pulse+=10;       
   TIM_SetCompare2(TIM3,pulse); 
}else if(lux>70){  
   pulse-=10;
   TIM_SetCompare2(TIM3,pulse);   
}  

if(50<lux && lux<=70){  
   pulse=300;
   TIM_SetCompare2(TIM3,pulse);
}else if(20<lux && lux<=50){  
   pulse=200;
   TIM_SetCompare2(TIM3,pulse);  
}else if(lux<=20){      
   pulse=100;
   TIM_SetCompare2(TIM3,pulse);   
}
}//�Զ�����			



