#include "dht11.h"
#include "delay.h"

void DHT11_IO_OUT (void){ //�ݤf???�X
	GPIO_InitTypeDef  GPIO_InitStructure; 	
    GPIO_InitStructure.GPIO_Pin = DHT11_IO; //??�ݤf?�]0~15��all�^                        
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_OUT; //??IO���f�u�@�覡       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //?�mIO���f�t�ס]2/10/50MHz�^    
	GPIO_Init(DHT11PORT, &GPIO_InitStructure);
}

void DHT11_IO_IN (void){ //�ݤf???�J
	GPIO_InitTypeDef  GPIO_InitStructure; 	
    GPIO_InitStructure.GPIO_Pin = DHT11_IO; //??�ݤf?�]0~15��all�^                        
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN; //??IO���f�u�@�覡       
	GPIO_Init(DHT11PORT, &GPIO_InitStructure);
}



void DHT11_RST (void){ //DHT11�ݤf�`��A?�X�_�l�H?�]IO?�e�^
	DHT11_IO_OUT();
	GPIO_ResetBits(DHT11PORT,DHT11_IO); //	
	delay_ms(20); //�ԧC�ܤ�18ms						
	GPIO_SetBits(DHT11PORT,DHT11_IO); //							
	delay_us(30); //�D��԰�20~40us
}



u8 Dht11_Check(void){ //����DHT11�^?�A��^1:��??��DHT11�A��^0:���\�]IO�����^	   
    u8 retry=0;
    DHT11_IO_IN();//IO��?�J??	 
    while (GPIO_ReadInputDataBit(DHT11PORT,DHT11_IO)&&retry<100){//DHT11?�ԧC40~80us
        retry++;
        delay_us(1);
    }	 
    if(retry>=100)return 1; else retry=0;
    while (!GPIO_ReadInputDataBit(DHT11PORT,DHT11_IO)&&retry<100){//DHT11�ԧC�Z?�A���԰�40~80us
        retry++;
        delay_us(1);
    }
    if(retry>=100)return 1;	    
    return 0;
}



u8 DHT11_Init (void){	//DHT11��l��
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOA, ENABLE); //PA15�~???�ϯ�      
	DHT11_RST();
	return Dht11_Check(); //����DHT11�^?
}

u8 Dht11_ReadByte(void){  //?DHT11?���@?�r?  ��^�ȡG?�쪺?�u
    u8 i,dat;
    dat=0;
    for (i=0;i<8;i++){ 
        dat<<=1; 
        dat|=Dht11_ReadBit();
    }						    
    return dat;
}

u8 Dht11_ReadBit(void){ //?DHT11?���@?�� ��^�ȡG1/0
    u8 retry=0;
    while(GPIO_ReadInputDataBit(DHT11PORT,DHT11_IO)&&retry<100){//����??�C?��
        retry++;
        delay_us(1);
    }
    retry=0;
    while(!GPIO_ReadInputDataBit(DHT11PORT,DHT11_IO)&&retry<100){//����?��?��
        retry++;
        delay_us(1);
    }
    delay_us(40);//����40us	//�Τ_�P?���C?���A�Y?�u1��0
    if(GPIO_ReadInputDataBit(DHT11PORT,DHT11_IO))return 1; else return 0;		   
}

u8 DHT11_ReadData(u8 *h){ //?���@��?�u//?�׭�(�Q?��A�S?:20%~90%) �A?�׭�(�Q?��A�S?:0~50�X)�A��^�ȡG0,���`;1,��? 
    u8 buf[5];
    u8 i;
    DHT11_RST();//DHT11�ݤf�`��A?�X�_�l�H?
    if(Dht11_Check()==1){ //����DHT11�^?
        for(i=0;i<5;i++){//?��5��?�u
            buf[i]=Dht11_ReadByte(); //?�X?�u
        }
        if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4]){	//?�u��?
            *h=buf[0]; //??�׭ȩ�J��?1
			h++;
            *h=buf[1]; //??�׭ȩ�J��?2
			h++;
			*h=buf[2]; //??�׭ȩ�J��?1
			h++;
            *h=buf[3]; //??�׭ȩ�J��?2
			h++;
			*h=buf[4]; //??�׭ȩ�J��?1
		
        }
    }else return 1;
    return 0;	    
}
