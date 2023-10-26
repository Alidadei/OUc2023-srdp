#include "fan.h"
#include "dht11.h"
#include "sys.h"
uint32_t pulse=0; 




void FAN_Init(void){   
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOA, ENABLE);	 //A8
    					 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 		 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  		 	
	GPIO_Init(GPIOA, &GPIO_InitStructure);	    	     
}  

void FAN_Open(void){  //开风扇
    GPIO_SetBits(GPIOA,GPIO_Pin_8);
	 pulse=500; 
	TIM_SetCompare1(TIM3,pulse);
}

void FAN_Shut(void){   //关风扇
    GPIO_ResetBits(GPIOA,GPIO_Pin_8); 
}
 
void FAN_Up(void)  
{   
    if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_8)==0)
    {
        FAN_Open();
        delay_ms(500);
        if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_8)==1)
        {
        }
        else   
        {
            FAN_Up(); 
        }
    }
    else
    {
        pulse+=10;     
        TIM_SetCompare1(TIM3,pulse);  
    }
} 


 void FAN_Down(void){   
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
       
       pulse-=10;     
        TIM_SetCompare1(TIM3,pulse);  
    }
}

void Auto_FAN(void)
	{   
float temp,hum;   
u8 DHT11_Data[5]; 
DHT11_Init();     
DHT11_ReadData(DHT11_Data); 
temp=(DHT11_Data[2]*10+DHT11_Data[3])/10.0;
hum=(DHT11_Data[0]*10+DHT11_Data[1])/10.0;  

if(temp>=30 || hum>60){ 
FAN_Open();}
else{        
FAN_Shut(); } 

if(temp>30 && hum<=60)   
{FAN_Up(); }
if(temp<30 && hum<=60) 
{FAN_Down();
}
}