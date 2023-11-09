#include "dht11.h"
#include "delay.h"

void DHT11_IO_OUT (void){ //端口???出
	GPIO_InitTypeDef  GPIO_InitStructure; 	
    GPIO_InitStructure.GPIO_Pin = DHT11_IO; //??端口?（0~15或all）                        
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_OUT; //??IO接口工作方式       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //?置IO接口速度（2/10/50MHz）    
	GPIO_Init(DHT11PORT, &GPIO_InitStructure);
}

void DHT11_IO_IN (void){ //端口???入
	GPIO_InitTypeDef  GPIO_InitStructure; 	
    GPIO_InitStructure.GPIO_Pin = DHT11_IO; //??端口?（0~15或all）                        
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN; //??IO接口工作方式       
	GPIO_Init(DHT11PORT, &GPIO_InitStructure);
}



void DHT11_RST (void){ //DHT11端口复位，?出起始信?（IO?送）
	DHT11_IO_OUT();
	GPIO_ResetBits(DHT11PORT,DHT11_IO); //	
	delay_ms(20); //拉低至少18ms						
	GPIO_SetBits(DHT11PORT,DHT11_IO); //							
	delay_us(30); //主机拉高20~40us
}



u8 Dht11_Check(void){ //等待DHT11回?，返回1:未??到DHT11，返回0:成功（IO接收）	   
    u8 retry=0;
    DHT11_IO_IN();//IO到?入??	 
    while (GPIO_ReadInputDataBit(DHT11PORT,DHT11_IO)&&retry<100){//DHT11?拉低40~80us
        retry++;
        delay_us(1);
    }	 
    if(retry>=100)return 1; else retry=0;
    while (!GPIO_ReadInputDataBit(DHT11PORT,DHT11_IO)&&retry<100){//DHT11拉低后?再次拉高40~80us
        retry++;
        delay_us(1);
    }
    if(retry>=100)return 1;	    
    return 0;
}



u8 DHT11_Init (void){	//DHT11初始化
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOA, ENABLE); //PA15外???使能      
	DHT11_RST();
	return Dht11_Check(); //等待DHT11回?
}

u8 Dht11_ReadByte(void){  //?DHT11?取一?字?  返回值：?到的?据
    u8 i,dat;
    dat=0;
    for (i=0;i<8;i++){ 
        dat<<=1; 
        dat|=Dht11_ReadBit();
    }						    
    return dat;
}

u8 Dht11_ReadBit(void){ //?DHT11?取一?位 返回值：1/0
    u8 retry=0;
    while(GPIO_ReadInputDataBit(DHT11PORT,DHT11_IO)&&retry<100){//等待??低?平
        retry++;
        delay_us(1);
    }
    retry=0;
    while(!GPIO_ReadInputDataBit(DHT11PORT,DHT11_IO)&&retry<100){//等待?高?平
        retry++;
        delay_us(1);
    }
    delay_us(40);//等待40us	//用于判?高低?平，即?据1或0
    if(GPIO_ReadInputDataBit(DHT11PORT,DHT11_IO))return 1; else return 0;		   
}

u8 DHT11_ReadData(u8 *h){ //?取一次?据//?度值(十?制，范?:20%~90%) ，?度值(十?制，范?:0~50°)，返回值：0,正常;1,失? 
    u8 buf[5];
    u8 i;
    DHT11_RST();//DHT11端口复位，?出起始信?
    if(Dht11_Check()==1){ //等待DHT11回?
        for(i=0;i<5;i++){//?取5位?据
            buf[i]=Dht11_ReadByte(); //?出?据
        }
        if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4]){	//?据校?
            *h=buf[0]; //??度值放入指?1
			h++;
            *h=buf[1]; //??度值放入指?2
			h++;
			*h=buf[2]; //??度值放入指?1
			h++;
            *h=buf[3]; //??度值放入指?2
			h++;
			*h=buf[4]; //??度值放入指?1
		
        }
    }else return 1;
    return 0;	    
}
