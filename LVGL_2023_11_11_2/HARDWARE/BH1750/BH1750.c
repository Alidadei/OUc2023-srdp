/**************************************************************************
 * �ļ���  ��bh1750.c
 * ����    ����ǿ����ģ��     
****************************************************************************/

#include "BH1750.h"
#include "delay.h"


uchar    BUF[8];               //�������ݻ�����  
int   mcy;     //��λ��־





/***��ʼ�ź�***/
void BH1750_Start()
{
  SDA=1;                    //����������
  SCL=1;                   //����ʱ����
  delay_us(5);                 //��ʱ
  GPIO_ResetBits(bh1750_PORT, sda);                    //�����½���
  delay_us(5);                 //��ʱ
  GPIO_ResetBits(bh1750_PORT, scl);                    //����ʱ����
}

/*****ֹͣ�ź�******/
void BH1750_Stop()
{
    SDA=0;                   //����������
    SCL=1;                      //����ʱ����
    delay_us(5);                 //��ʱ
    GPIO_SetBits(bh1750_PORT, sda);                    //����������
    delay_us(5);                 //��ʱ
}

/**************************************
����Ӧ���ź�
��ڲ���:ack (0:ACK 1:NAK)
**************************************/
void BH1750_SendACK(int ack)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
  GPIO_InitStruct.GPIO_Mode =GPIO_Mode_OUT;  
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Pin = sda;
  GPIO_Init(bh1750_PORT, &GPIO_InitStruct);  
	
	if(ack == 1)   //дӦ���ź�
		SDA=1; 
	else if(ack == 0)
		SDA=0; 
	else
		return;			
  SCL=1;     //����ʱ����
  delay_us(5);                 //��ʱ
  SCL=0;      //����ʱ����
  delay_us(5);                //��ʱ
}

/**************************************
����Ӧ���ź�
**************************************/
int BH1750_RecvACK()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;  
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;/*����һ��Ҫ������������������ܶ�������*/
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin=sda;
	GPIO_Init(bh1750_PORT,&GPIO_InitStruct);
	
  SCL=1;            //����ʱ����
  delay_us(5);                 //��ʱ	
	if(GPIO_ReadInputDataBit(GPIOA,sda)==1)//��Ӧ���ź�
    mcy = 1 ;  
  else
    mcy = 0 ;				
  SCL=0;                    //����ʱ����
  delay_us(5);                 //��ʱ
  GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
  GPIO_Init(bh1750_PORT,&GPIO_InitStruct);
	return mcy;
}

/**************************************
��IIC���߷���һ���ֽ�����
**************************************/
void BH1750_SendByte(uchar dat)
{
  uchar i;
  for (i=0; i<8; i++)         //8λ������
  {
		if( 0X80 & dat )
      GPIO_SetBits(bh1750_PORT,sda);
    else
      GPIO_ResetBits(bh1750_PORT,sda);
		dat <<= 1;
    SCL=1;               //����ʱ����
    delay_us(5);             //��ʱ
    SCL=0;                //����ʱ����
    delay_us(5);            //��ʱ
  }
  BH1750_RecvACK();
}

uchar BH1750_RecvByte()
{
  uchar i;
  uchar dat = 0;
	uchar bit;
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;  
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;   /*����һ��Ҫ������������������ܶ�������*/
  GPIO_InitStruct.GPIO_Pin = sda;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(bh1750_PORT,&GPIO_InitStruct );
	
  GPIO_SetBits(bh1750_PORT,sda);          //ʹ���ڲ�����,׼����ȡ����,
  for (i=0; i<8; i++)         //8λ������
  {
    dat <<= 1;
    SCL=1;               //����ʱ����
    delay_us(5);             //��ʱ
			
		if( SET == GPIO_ReadInputDataBit(bh1750_PORT,sda))
      bit = 0X01;
    else
      bit = 0x00;  
		dat |= bit;             //������    
		SCL=0;                //����ʱ����
    delay_us(5);            //��ʱ
  }		
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(bh1750_PORT, &GPIO_InitStruct );
  return dat;
}

void Single_Write_BH1750(uchar REG_Address)
{
  BH1750_Start();                  //��ʼ�ź�
  BH1750_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
  BH1750_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ����ο�����pdf22ҳ 
//  BH1750_SendByte(REG_data);       //�ڲ��Ĵ������ݣ���ο�����pdf22ҳ 
  BH1750_Stop();                   //����ֹͣ�ź�
}

//��ʼ��BH1750��������Ҫ��ο�pdf�����޸�****
void Init_BH1750()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	 /*����GPIOB������ʱ��*/ 
	RCC_AHB1PeriphResetCmd( RCC_AHB1Periph_GPIOA, ENABLE); 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;  
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = sda | scl ;
	GPIO_Init(bh1750_PORT,&GPIO_InitStruct); 
	
	Single_Write_BH1750(0x01);  
	delay_ms(180);            //��ʱ180ms
}

//��������BH1750�ڲ�����
void mread(void)
{   
	uchar i;	
	BH1750_Start();                          //��ʼ�ź�
	BH1750_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
	
	for (i=0; i<3; i++)                      //������ȡ6����ַ���ݣ��洢��BUF
  {
    BUF[i] = BH1750_RecvByte();          //BUF[0]�洢0x32��ַ�е�����
    if (i == 3)
    {
      BH1750_SendACK(1);                //���һ��������Ҫ��NOACK
    }
    else
    {		
      BH1750_SendACK(0);                //��ӦACK
    }
  }
  BH1750_Stop();                          //ֹͣ�ź�
  delay_ms(5);
}

float read_BH1750(void)
{
  int dis_data;                       //����	
	float temp1;
	float temp2;
	Single_Write_BH1750(0x01);   // power on
	Single_Write_BH1750(0x10);   // H- resolution mode
	delay_ms(180);            //��ʱ180ms
	mread();       //�����������ݣ��洢��BUF��
	dis_data=BUF[0];
	dis_data=(dis_data<<8)+BUF[1]; //�ϳ����� 
	temp1=dis_data/1.2;
	temp2=10*dis_data/1.2;	
	temp2=(int)temp2%10;
//	OLED_ShowString(87,2,".",12); 
//	OLED_ShowNum(94,2,temp2,1,12);	
	return temp1;
}
