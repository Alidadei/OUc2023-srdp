#include "fan.h"
#include "dht11.h"
#include "sys.h"
uint32_t pulse=0; 


void FAN_Init(void){    
    GPIO_InitTypeDef  GPIO_InitStructure;  

    RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOA, ENABLE); //ʹ��GPIOAʱ��
    					  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;			//ѡ��GPIOA Pin8	 		 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		//����Ϊ���ģʽ 		 			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�������50MHz		  		 	
	GPIO_Init(GPIOA, &GPIO_InitStructure);	    	  //���ÿ⺯����ʼ��GPIOA
}  



void FAN_Open(void)  //������
	{ 
    GPIO_SetBits(GPIOA,GPIO_Pin_8);      //GPIOA Pin8����ߵ�ƽ
	 pulse=50;					//���÷��ȳ��ٶ�PWMֵΪ50
	 TIM_SetCompare2(TIM4,pulse);  	//����TIM4ͨ��4�Ƚ�ֵ,����PWM
}


void FAN_Shut(void)//�ط���  
	{   
    GPIO_ResetBits(GPIOA,GPIO_Pin_8);  //GPIOA Pin8����͵�ƽ,�رշ���
}
 
void FAN_Up(void)  //���ӷ���
{   
    //�жϷ����Ƿ��ѿ���,���δ�������
    //����������PWMֵ���ӷ���
    if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_8)==0)  //�ж�Pin8�Ƿ�����ߵ�ƽ,�Ƿ���
    {
        FAN_Open();							
        delay_ms(500);						//��ʱ�ȴ����ȴ�		
        if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_8)==1)//���ж��Ƿ�ɹ���
        {                                   
        }
        else   							//�����ʧ��,�ݹ�������ӷ��� 
        {
            FAN_Up(); 
        }
    }
    else											//��������ѿ���
    {
        pulse+=10;  	            //����PWMֵ10	    
        TIM_SetCompare1(TIM4,pulse); 		//������PWMֵ,�������  
    }
} 


void FAN_Down(void)//��С����
	 {   
    //�жϷ����Ƿ��ѿ���,���δ�������
    //�������СPWMֵ��С����
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
       
        pulse-=10;  	       			//��СPWMֵ10	
        TIM_SetCompare2(TIM4,pulse);  //������PWMֵ,��С����  
    }
}

void Auto_FAN(void)   //�Զ����Ʒ���
	{   
u8 wd=0, sd=0,temp;
u8 tab_wd[5] ;
    DHT11_Init();         
	DHT11_Read_Data(&wd,&sd);
		temp=wd;

if(temp>26){     //�¶ȸ���26,�򿪷���
FAN_Open();}
else{        				 //����رշ���
FAN_Shut(); }
}
