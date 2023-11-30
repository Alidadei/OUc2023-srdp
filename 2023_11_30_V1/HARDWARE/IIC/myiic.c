#include "myiic.h"
#include "stm32f4xx.h"  

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//IIC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//��ʼ��IIC
void IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��

  //GPIOA2,3��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	IIC_SCL=1;
	IIC_SDA=1;
}
/**
*	����˵��������IIC��ʼ�ź�
*
*	ԭ��˵����	�� SCL �Ǹߵ�ƽ״̬ʱ,
*				������ SDA �ɸ�����;
**/
void IIC_Start(void)
{
    SDA_OUT();		/* SDA �����ģʽ */
    IIC_SDA = 1;	/* ������ SDA ״̬ */
    IIC_SCL = 1;	/* ������ SCL ״̬ */
    IIC_Delay();	/* ��ʱ���ֵ�ƽ״̬ */
    IIC_SDA = 0;	/* ������ SDA ״̬ */
    IIC_Delay();	/* ��ʱ���ֵ�ƽ״̬ */
    IIC_SCL = 0;	/* ǯסI2C,���ͻ���� */
}
/**
*	����˵��������IICֹͣ�ź�
*
*	ԭ��˵����	SCL��Ϊ�ߵ�ƽ״̬ʱ, SDA ��
*				�ɵ͵�ƽ��ߵ�ƽ����(������)
**/
void IIC_Stop(void)
{
    SDA_OUT();		/* SDA �����ģʽ */
    IIC_SCL = 0;	/* ������ SCL ״̬ */
    IIC_SDA = 0;	/* ������ SDA ״̬ */
    IIC_Delay();	/* ��ʱ���ֵ�ƽ״̬ */
    IIC_SCL = 1;	/* ������ SCL ״̬ */
    IIC_SDA = 1;	/* ������ SDA ״̬ */
    IIC_Delay();	/* ��ʱ���ֵ�ƽ״̬ */
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t IIC_Wait_Ack(void)
{
    uint16_t ErrorTime;
	
    SDA_IN();      			/** SDA ������ģʽ **/
    IIC_SDA = 1;			/* ������ SDA ״̬ */
    IIC_Delay();			/* ��ʱ���ֵ�ƽ״̬ */
    IIC_SCL = 1;			/* ������ SCL ״̬ */
    IIC_Delay();			/* ��ʱ���ֵ�ƽ״̬ */
	
    while(Read_SDA)			/* ���� SDA ��ƽ״̬ */
    {
        ErrorTime ++;
		
        if(ErrorTime > 250)
        {
            IIC_Stop();		/* ��Ӧ��,����ͨ�� */
            return 0;
        }
    }
    IIC_SCL = 0;			/** ǯס,�ȴ������� **/
	
    return 1;				/** ���յ�Ӧ���ź� **/
}
//����ACKӦ��
void IIC_Ack(void)
{	
    IIC_SCL = 0;				/* ʱ���� SCL ���� */
	SDA_OUT();					/** SDA �����ģʽ **/
    IIC_SDA = 0;				/* ������ SDA ���� */
    IIC_Delay();				/* ��ʱ���ֵ�ƽ״̬ */
    IIC_SCL = 1;				/* ʱ���� SCL ���� */
    IIC_Delay();				/* ��ʱ���ֵ�ƽ״̬ */
    IIC_SCL = 0;				/* ʱ���� SCL ���� */
}

//������ACKӦ��		    
void IIC_NAck(void)
{
    IIC_SCL = 0;				/* ������ SCL ���� */
	SDA_OUT();					/** SDA �����ģʽ **/
    IIC_SDA = 1;				/* ������ SDA ���� */
    IIC_Delay();				/* ��ʱ���ֵ�ƽ״̬ */
    IIC_SCL = 1;				/* ʱ���� SCL ���� */
    IIC_Delay();				/* ��ʱ���ֵ�ƽ״̬ */
    IIC_SCL = 0;				/* ʱ���� SCL ���� */
}

//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(uint8_t Txd)
{
    uint8_t T;
	
    SDA_OUT();						/** ������SDA���̬ **/
    IIC_SCL = 0;					/** ʱ���� SCL ���� **/
	
    for(T = 0; T < 8; T++)			/** �ɸߵ��ͷ������� */
    {
        IIC_SDA = (Txd & 0x80) >> 7;/** ÿ�η������λ **/
        Txd <<= 1;					/* �������ݵ����λ */
        IIC_Delay();				/* ��ʱ���ֵ�ƽ״̬ */
        IIC_SCL = 1;				/* ʱ���� SCL ���� */
        IIC_Delay();				/* ��ʱ���ֵ�ƽ״̬ */
        IIC_SCL = 0;				/* ʱ���� SCL ���� */
        IIC_Delay();				/* ��ʱ���ֵ�ƽ״̬ */
    }
}

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t IIC_Read_Byte(uint8_t ACK)
{
	uint8_t	T;
	uint8_t	Receive;

    SDA_IN();				/** SDA ������Ϊ���� **/
	
    for(T = 0; T < 8; T++ )	/** �ɸߵ��ͽ�������**/
    {
        IIC_SCL = 0;		/* ʱ�������͵�ʱ��,SDA������仯 */
        IIC_Delay();		/* ��ʱ���ֵ�ƽ״̬ */
        IIC_SCL = 1;		/* ����ʱ����,������SDA�仯,���Զ�ȡ SDA */
        Receive <<= 1;		/** ���յ�����λ���� **/
        if(Read_SDA)
        {
            Receive++;		/** �ߵ�ƽ,�����λΪ1 **/
        }
        IIC_Delay();		/* ��ʱ���ֵ�ƽ״̬ */
    }
	
    if ( !ACK )	
		IIC_NAck();			/** ���� NACK **/
    else		
		IIC_Ack(); 			/** ����  ACK **/
    return Receive;
}

void Adc3_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE); //ʹ��ADC3ʱ��
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);  //ADC3��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);   //��λ����
	
	//��ʼ��CCR�Ĵ���
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_CommonInitStructure.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled; //DMA��ʹ�ܣ�DMAͨ�����ڶ�ͨ����ת��
	ADC_CommonInitStructure.ADC_Mode=ADC_Mode_Independent;  //����ģʽ
	ADC_CommonInitStructure.ADC_Prescaler=ADC_Prescaler_Div4;  //Ԥ��Ƶ4��Ƶ
	ADC_CommonInitStructure.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;  //���������׶�֮���ӳ�5��ʱ��
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	//��ʼ��ADC
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;  //�ر�����ת��
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;  //�Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_NbrOfConversion=1; //1��ת���ڹ���������
	ADC_InitStructure.ADC_Resolution=ADC_Resolution_12b;  //12λģʽ�ֱ���
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;  //��ɨ��ģʽ
	ADC_Init(ADC3,&ADC_InitStructure);
	
	ADC_Cmd(ADC3,ENABLE);  //����ADת����
}
//���ADC��ֵ
//ch��ͨ��ֵ0~16  ADC_Channel_0~ADC_Channel_16 
//����ֵ��ת���Ľ��
u16 Get_Adc3(u8 ch)
{
	ADC_RegularChannelConfig(ADC3,ch,1,ADC_SampleTime_480Cycles); //����ADC������ͨ����1������ ����ʱ��
	ADC_SoftwareStartConv(ADC3);//ʹ��ָ����ADC3�����ת����������
	while(!ADC_GetFlagStatus(ADC3,ADC_FLAG_EOC));//�ȴ�״̬�Ĵ���ת����־λ����
	return ADC_GetConversionValue(ADC3);   //����ת���Ľ��
}
 






