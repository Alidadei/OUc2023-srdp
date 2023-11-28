#include "fan.h"
#include "dht11.h"
#include "sys.h"
uint32_t pulse=0; 


void FAN_Init(void){    
    GPIO_InitTypeDef  GPIO_InitStructure;  

    RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能GPIOA时钟
    					  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;			//选择GPIOA Pin8	 		 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		//设置为输出模式 		 			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//输出速率50MHz		  		 	
	GPIO_Init(GPIOA, &GPIO_InitStructure);	    	  //调用库函数初始化GPIOA
}  



void FAN_Open(void)  //开风扇
	{ 
    GPIO_SetBits(GPIOA,GPIO_Pin_8);      //GPIOA Pin8输出高电平
	 pulse=50;					//设置风扇初速度PWM值为50
	 TIM_SetCompare2(TIM4,pulse);  	//设置TIM4通道4比较值,控制PWM
}


void FAN_Shut(void)//关风扇  
	{   
    GPIO_ResetBits(GPIOA,GPIO_Pin_8);  //GPIOA Pin8输出低电平,关闭风扇
}
 
void FAN_Up(void)  //增加风量
{   
    //判断风扇是否已开启,如果未开启则打开
    //开启后增加PWM值增加风量
    if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_8)==0)  //判断Pin8是否输出高电平,是否开启
    {
        FAN_Open();							
        delay_ms(500);						//延时等待风扇打开		
        if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_8)==1)//再判断是否成功打开
        {                                   
        }
        else   							//如果打开失败,递归调用增加风量 
        {
            FAN_Up(); 
        }
    }
    else											//如果风扇已开启
    {
        pulse+=10;  	            //增加PWM值10	    
        TIM_SetCompare1(TIM4,pulse); 		//设置新PWM值,增大风量  
    }
} 


void FAN_Down(void)//减小风量
	 {   
    //判断风扇是否已开启,如果未开启则打开
    //开启后减小PWM值减小风量
    if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_8)==0)  
    {
        FAN_Open();
        delay_ms(500);
        if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_8)==1)
        {			
        }
        else  
        {
            FAN_Down(); 
        }
    }
    else									 
    {
       
        pulse-=10;  	       			//减小PWM值10	
        TIM_SetCompare2(TIM4,pulse);  //设置新PWM值,减小风量  
    }
}

void Auto_FAN(void)   //自动控制风扇
	{   
u8 wd=0, sd=0,temp;
u8 tab_wd[5] ;
    DHT11_Init();         
	DHT11_Read_Data(&wd,&sd);
		temp=wd;

if(temp>26){     //温度高于26,打开风扇
FAN_Open();}
else{        				 //否则关闭风扇
FAN_Shut(); }
}
