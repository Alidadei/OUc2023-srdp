#include "dht11.h"
#include "delay.h"
#include "stm32f4xx.h"



/*******************************************************************************
 * ��������DHT11_GPIO_Config
 * ����  ������DHT11�õ���I/O��
 * ����  ����
 * ���  ����
 *******************************************************************************/
void DHT11_GPIO_Config(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure; 
  RCC_AHB1PeriphClockCmd(RCC_DHT11, ENABLE);       /*����DS18B20��Ӧ��GPIO������ʱ��*/ 
  GPIO_InitStructure.GPIO_Pin = DHT11_DQ_GPIO_PIN; /*ѡ��Ҫ���Ƶ�DS18B20����*/ 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      /*��������ģʽ���ģʽ*/       
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     /*�������ŵ��������Ϊ�������*/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  /*������������Ϊ50MHz */ 
  GPIO_Init(DHT11_DQ_GPIO_PORT, &GPIO_InitStructure); /*���ÿ⺯������ʼ����ӦGPIO*/
}
/*******************************************************************************
 * ��������DHT11_Mode_Out
 * ����  ��ʹDHT11-DATA���ű�Ϊ���ģʽ
 * ����  ����
 * ���  ����
 *******************************************************************************/
static void DHT11_Mode_Out(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	 	/*ѡ��Ҫ���Ƶ�DHT11_DQ_GPIO_PORT����*/															   
  	GPIO_InitStructure.GPIO_Pin = DHT11_DQ_GPIO_PIN;	
	/*��������ģʽ���ģʽ*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
  /*�������ŵ��������Ϊ�������*/     
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*���ÿ⺯������ʼ��DHT11_DQ_GPIO_PORT*/
  	GPIO_Init(DHT11_DQ_GPIO_PORT, &GPIO_InitStructure);
}
/*******************************************************************************
 * ��������DHT11_Mode_IN
 * ����  ��ʹDHT11-DATA���ű�Ϊ����ģʽ
 * ����  ����
 * ���  ����
 *******************************************************************************/
static void DHT11_Mode_IN(void)
{
 	  GPIO_InitTypeDef GPIO_InitStructure;

	  	/*ѡ��Ҫ���Ƶ�DS18B20_DQ_GPIO_PORT����*/	
	  GPIO_InitStructure.GPIO_Pin = DHT11_DQ_GPIO_PIN;

	   /*��������ģʽΪ��������ģʽ*/ 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	

	  /*���ÿ⺯������ʼ��DS18B20_DQ_GPIO_PORT*/
	  GPIO_Init(DHT11_DQ_GPIO_PORT, &GPIO_InitStructure);
}
/*******************************************************************************
 * ��������DHT11_Reset
 * ����  ����ʼ��DHT11
 * ����  ����
 * ���  ����
 * ����ֵ����ʼ���ɹ�Ϊ0�����ɹ�Ϊ1
 *******************************************************************************/
void DHT11_Reset() 
{      
	  DHT11_Mode_Out(); //SET OUTPUT
    DHT11_DQ_L; //DQ=0
    delay_ms(20);    //��������18ms
    DHT11_DQ_H; //DQ=1 
    delay_us(30);     //��������20~40us
}
u8 DHT11_Check(void)    
{   
	u8 retry=0;//������ʱ����
	DHT11_Mode_IN();//SET INPUT 
	while ((GPIO_ReadInputDataBit(DHT11_DQ_GPIO_PORT,DHT11_DQ_GPIO_PIN)==1)&&retry<100)//DHT11������40~80us
	 {
	   retry++;
	   delay_us(1);
	 }; 
	if(retry>=100)return 1;
	else retry=0;
  while ((GPIO_ReadInputDataBit(DHT11_DQ_GPIO_PORT,DHT11_DQ_GPIO_PIN)==0)&&retry<100)//DHT11���ͺ���ٴ�����40~80us
	 {
	   retry++;
	   delay_us(1);
	 };
	if(retry>=100)return 1;    
	return 0;
}
//��DHT11��ȡһ��λ
//����ֵ��1/0
u8 DHT11_Read_Bit(void)  
{
	 u8 retry=0;
	while((GPIO_ReadInputDataBit(DHT11_DQ_GPIO_PORT,DHT11_DQ_GPIO_PIN)==1)&&retry<100)//�ȴ���Ϊ�͵�ƽ
	{
	 retry++;
	 delay_us(1);
	}
	retry=0;
	while((GPIO_ReadInputDataBit(DHT11_DQ_GPIO_PORT,DHT11_DQ_GPIO_PIN)==0)&&retry<100)//�ȴ���ߵ�ƽ
	{
	 retry++;
	 delay_us(1);
	}
	delay_us(40);//�ȴ�40us
	if(GPIO_ReadInputDataBit(DHT11_DQ_GPIO_PORT,DHT11_DQ_GPIO_PIN)==1)
	return 1;
	else 
	return 0;   
}
//��DHT11��ȡһ���ֽ�
//����ֵ������������
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
//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
u8 DHT11_Read_Data(u8 *temp,u8 *humi)    
{        
	u8 buf[5];
	u8 i;
	DHT11_Reset();
	if(DHT11_Check()==0)
	{
	 for(i=0;i<5;i++)//��ȡ40λ����
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
//��ʼ��DHT11��IO�� DQ ͬʱ���DHT11�Ĵ���
//����1:������
//����0:����     
void DHT11_Init(void)
{     
	DHT11_GPIO_Config();
	DHT11_Reset();  //��λDHT11
	DHT11_Check();//�ȴ�DHT11�Ļ�Ӧ
}
