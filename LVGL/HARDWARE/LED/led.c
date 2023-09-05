#include "led.h" 


//初始化PF9和PF10为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟

  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化
	
	GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);//GPIOF9,F10设置高，灯灭

}
//测试用
void Light_Open(void){
  {
    GPIO_ResetBits(GPIOF,  GPIO_Pin_10|GPIO_Pin_9);//PF9&PF10设低电平，点亮
  }}

void Light_Shut(void){
	{
		GPIO_SetBits(GPIOF,  GPIO_Pin_10|GPIO_Pin_9);//灭灯
	}}

	
void Light_Up(void){}//调亮一点
void Light_Down(void){}//调暗一点
void Auto_light(void){}//自动调光			



