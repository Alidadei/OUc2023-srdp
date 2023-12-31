#include "led.h" 
#include "bh1750.h"
#include "timer.h"

//初始化PC6和PC7为输出口.并使能这两个口的时钟		    
/*LED IO初始化
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

  //GPIOC6,C7初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
}*/

//亮灯
void Light_Open(void)
  {
   uint32_t pulse;  
   pulse=99;        
   TIM_SetCompare1(TIM4,pulse); 
	}
//灭灯
void Light_Shut(void){
	uint32_t pulse;  
   pulse=0;        
   TIM_SetCompare1(TIM4,pulse);}


	
void Light_Up(void)//调亮 
{
uint32_t pulse;  
pulse=TIM_GetCapture1(TIM4); 
   pulse+=9;        
   TIM_SetCompare1(TIM4,pulse);  
}



void Light_Down(void)//调暗 
{
uint32_t pulse;  
pulse=TIM_GetCapture1(TIM4);        
   pulse-=1;        
   TIM_SetCompare1(TIM4,pulse);  
}



void Auto_light(void)//自动调光			
{
	float lux;
uint16_t pulse;
lux=read_BH1750(); 

if(lux<70){        
   pulse+=10;       
   TIM_SetCompare1(TIM4,pulse); 
}else if(lux>75){  
   pulse-=10;
   TIM_SetCompare1(TIM4,pulse);   
}  
}


