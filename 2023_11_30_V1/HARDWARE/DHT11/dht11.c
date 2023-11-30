#include "dht11.h"
#include "delay.h"
#include "stm32f4xx.h"



/*******************************************************************************
 * 函数名：DHT11_GPIO_Config
 * 描述  ：配置DHT11用到的I/O口
 * 输入  ：无
 * 输出  ：无
 *******************************************************************************/
void DHT11_GPIO_Config(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure; 
  RCC_AHB1PeriphClockCmd(RCC_DHT11, ENABLE);       /*开启DS18B20对应的GPIO的外设时钟*/ 
  GPIO_InitStructure.GPIO_Pin = DHT11_DQ_GPIO_PIN; /*选择要控制的DS18B20引脚*/ 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      /*设置引脚模式输出模式*/       
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     /*设置引脚的输出类型为推挽输出*/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  /*设置引脚速率为50MHz */ 
  GPIO_Init(DHT11_DQ_GPIO_PORT, &GPIO_InitStructure); /*调用库函数，初始化相应GPIO*/
}
/*******************************************************************************
 * 函数名：DHT11_Mode_Out
 * 描述  ：使DHT11-DATA引脚变为输出模式
 * 输入  ：无
 * 输出  ：无
 *******************************************************************************/
static void DHT11_Mode_Out(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	 	/*选择要控制的DHT11_DQ_GPIO_PORT引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = DHT11_DQ_GPIO_PIN;	
	/*设置引脚模式输出模式*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
  /*设置引脚的输出类型为推挽输出*/     
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*调用库函数，初始化DHT11_DQ_GPIO_PORT*/
  	GPIO_Init(DHT11_DQ_GPIO_PORT, &GPIO_InitStructure);
}
/*******************************************************************************
 * 函数名：DHT11_Mode_IN
 * 描述  ：使DHT11-DATA引脚变为输入模式
 * 输入  ：无
 * 输出  ：无
 *******************************************************************************/
static void DHT11_Mode_IN(void)
{
 	  GPIO_InitTypeDef GPIO_InitStructure;

	  	/*选择要控制的DS18B20_DQ_GPIO_PORT引脚*/	
	  GPIO_InitStructure.GPIO_Pin = DHT11_DQ_GPIO_PIN;

	   /*设置引脚模式为浮空输入模式*/ 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	

	  /*调用库函数，初始化DS18B20_DQ_GPIO_PORT*/
	  GPIO_Init(DHT11_DQ_GPIO_PORT, &GPIO_InitStructure);
}
/*******************************************************************************
 * 函数名：DHT11_Reset
 * 描述  ：初始化DHT11
 * 输入  ：无
 * 输出  ：无
 * 返回值：初始化成功为0，不成功为1
 *******************************************************************************/
void DHT11_Reset() 
{      
	  DHT11_Mode_Out(); //SET OUTPUT
    DHT11_DQ_L; //DQ=0
    delay_ms(20);    //拉低至少18ms
    DHT11_DQ_H; //DQ=1 
    delay_us(30);     //主机拉高20~40us
}
u8 DHT11_Check(void)    
{   
	u8 retry=0;//定义临时变量
	DHT11_Mode_IN();//SET INPUT 
	while ((GPIO_ReadInputDataBit(DHT11_DQ_GPIO_PORT,DHT11_DQ_GPIO_PIN)==1)&&retry<100)//DHT11会拉低40~80us
	 {
	   retry++;
	   delay_us(1);
	 }; 
	if(retry>=100)return 1;
	else retry=0;
  while ((GPIO_ReadInputDataBit(DHT11_DQ_GPIO_PORT,DHT11_DQ_GPIO_PIN)==0)&&retry<100)//DHT11拉低后会再次拉高40~80us
	 {
	   retry++;
	   delay_us(1);
	 };
	if(retry>=100)return 1;    
	return 0;
}
//从DHT11读取一个位
//返回值：1/0
u8 DHT11_Read_Bit(void)  
{
	 u8 retry=0;
	while((GPIO_ReadInputDataBit(DHT11_DQ_GPIO_PORT,DHT11_DQ_GPIO_PIN)==1)&&retry<100)//等待变为低电平
	{
	 retry++;
	 delay_us(1);
	}
	retry=0;
	while((GPIO_ReadInputDataBit(DHT11_DQ_GPIO_PORT,DHT11_DQ_GPIO_PIN)==0)&&retry<100)//等待变高电平
	{
	 retry++;
	 delay_us(1);
	}
	delay_us(40);//等待40us
	if(GPIO_ReadInputDataBit(DHT11_DQ_GPIO_PORT,DHT11_DQ_GPIO_PIN)==1)
	return 1;
	else 
	return 0;   
}
//从DHT11读取一个字节
//返回值：读到的数据
u8 DHT11_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
for (i=0;i<8;i++) 
{
   dat<<=1; 
    dat|=DHT11_Read_Bit();
    }    
    return dat;
}
//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
u8 DHT11_Read_Data(u8 *temp,u8 *humi)    
{        
	u8 buf[5];
	u8 i;
	DHT11_Reset();
	if(DHT11_Check()==0)
	{
	 for(i=0;i<5;i++)//读取40位数据
	 {
	  buf[i]=DHT11_Read_Byte();
	 }
	 if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
	 {
	  *humi=buf[0];
	  *temp=buf[2];
	 }
	}
	else return 1;
return 0;    
}
//初始化DHT11的IO口 DQ 同时检测DHT11的存在
//返回1:不存在
//返回0:存在     
void DHT11_Init(void)
{     
	DHT11_GPIO_Config();
	DHT11_Reset();  //复位DHT11
	DHT11_Check();//等待DHT11的回应
}
