#include "myiic.h"
#include "stm32f4xx.h"  

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//IIC 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//初始化IIC
void IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟

  //GPIOA2,3初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	IIC_SCL=1;
	IIC_SDA=1;
}
/**
*	函数说明：产生IIC起始信号
*
*	原理说明：	当 SCL 是高电平状态时,
*				数据线 SDA 由高拉低;
**/
void IIC_Start(void)
{
    SDA_OUT();		/* SDA 线输出模式 */
    IIC_SDA = 1;	/* 先拉高 SDA 状态 */
    IIC_SCL = 1;	/* 再拉高 SCL 状态 */
    IIC_Delay();	/* 延时保持电平状态 */
    IIC_SDA = 0;	/* 后拉低 SDA 状态 */
    IIC_Delay();	/* 延时保持电平状态 */
    IIC_SCL = 0;	/* 钳住I2C,发送或接收 */
}
/**
*	函数说明：产生IIC停止信号
*
*	原理说明：	SCL线为高电平状态时, SDA 线
*				由低电平向高电平跳变(上升沿)
**/
void IIC_Stop(void)
{
    SDA_OUT();		/* SDA 线输出模式 */
    IIC_SCL = 0;	/* 先拉低 SCL 状态 */
    IIC_SDA = 0;	/* 先拉低 SDA 状态 */
    IIC_Delay();	/* 延时保持电平状态 */
    IIC_SCL = 1;	/* 后拉高 SCL 状态 */
    IIC_SDA = 1;	/* 后拉高 SDA 状态 */
    IIC_Delay();	/* 延时保持电平状态 */
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
    uint16_t ErrorTime;
	
    SDA_IN();      			/** SDA 线输入模式 **/
    IIC_SDA = 1;			/* 先拉高 SDA 状态 */
    IIC_Delay();			/* 延时保持电平状态 */
    IIC_SCL = 1;			/* 后拉高 SCL 状态 */
    IIC_Delay();			/* 延时保持电平状态 */
	
    while(Read_SDA)			/* 采样 SDA 电平状态 */
    {
        ErrorTime ++;
		
        if(ErrorTime > 250)
        {
            IIC_Stop();		/* 非应答,结束通信 */
            return 0;
        }
    }
    IIC_SCL = 0;			/** 钳住,等待发或收 **/
	
    return 1;				/** 接收到应答信号 **/
}
//产生ACK应答
void IIC_Ack(void)
{	
    IIC_SCL = 0;				/* 时钟线 SCL 拉低 */
	SDA_OUT();					/** SDA 线输出模式 **/
    IIC_SDA = 0;				/* 数据线 SDA 拉低 */
    IIC_Delay();				/* 延时保持电平状态 */
    IIC_SCL = 1;				/* 时钟线 SCL 拉高 */
    IIC_Delay();				/* 延时保持电平状态 */
    IIC_SCL = 0;				/* 时钟线 SCL 拉低 */
}

//不产生ACK应答		    
void IIC_NAck(void)
{
    IIC_SCL = 0;				/* 数据线 SCL 拉低 */
	SDA_OUT();					/** SDA 线输出模式 **/
    IIC_SDA = 1;				/* 数据线 SDA 拉高 */
    IIC_Delay();				/* 延时保持电平状态 */
    IIC_SCL = 1;				/* 时钟线 SCL 拉高 */
    IIC_Delay();				/* 延时保持电平状态 */
    IIC_SCL = 0;				/* 时钟线 SCL 拉低 */
}

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(uint8_t Txd)
{
    uint8_t T;
	
    SDA_OUT();						/** 数据线SDA输出态 **/
    IIC_SCL = 0;					/** 时钟线 SCL 拉低 **/
	
    for(T = 0; T < 8; T++)			/** 由高到低发送数据 */
    {
        IIC_SDA = (Txd & 0x80) >> 7;/** 每次发送最高位 **/
        Txd <<= 1;					/* 更新数据的最高位 */
        IIC_Delay();				/* 延时保持电平状态 */
        IIC_SCL = 1;				/* 时钟线 SCL 拉高 */
        IIC_Delay();				/* 延时保持电平状态 */
        IIC_SCL = 0;				/* 时钟线 SCL 拉低 */
        IIC_Delay();				/* 延时保持电平状态 */
    }
}

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t IIC_Read_Byte(uint8_t ACK)
{
	uint8_t	T;
	uint8_t	Receive;

    SDA_IN();				/** SDA 线设置为输入 **/
	
    for(T = 0; T < 8; T++ )	/** 由高到低接收数据**/
    {
        IIC_SCL = 0;		/* 时钟线拉低的时候,SDA才允许变化 */
        IIC_Delay();		/* 延时保持电平状态 */
        IIC_SCL = 1;		/* 拉高时钟线,不允许SDA变化,可以读取 SDA */
        Receive <<= 1;		/** 接收到数据位左移 **/
        if(Read_SDA)
        {
            Receive++;		/** 高电平,则最低位为1 **/
        }
        IIC_Delay();		/* 延时保持电平状态 */
    }
	
    if ( !ACK )	
		IIC_NAck();			/** 发送 NACK **/
    else		
		IIC_Ack(); 			/** 发送  ACK **/
    return Receive;
}

void Adc3_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE); //使能ADC3时钟
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);  //ADC3复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);   //复位结束
	
	//初始化CCR寄存器
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_CommonInitStructure.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled; //DMA不使能，DMA通常用于多通道的转移
	ADC_CommonInitStructure.ADC_Mode=ADC_Mode_Independent;  //独立模式
	ADC_CommonInitStructure.ADC_Prescaler=ADC_Prescaler_Div4;  //预分频4分频
	ADC_CommonInitStructure.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;  //两个采样阶段之间延迟5个时钟
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	//初始化ADC
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;  //关闭连续转换
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;  //右对齐
	ADC_InitStructure.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_NbrOfConversion=1; //1个转换在规则序列中
	ADC_InitStructure.ADC_Resolution=ADC_Resolution_12b;  //12位模式分辨率
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;  //非扫描模式
	ADC_Init(ADC3,&ADC_InitStructure);
	
	ADC_Cmd(ADC3,ENABLE);  //开启AD转换器
}
//获得ADC的值
//ch：通道值0~16  ADC_Channel_0~ADC_Channel_16 
//返回值：转换的结果
u16 Get_Adc3(u8 ch)
{
	ADC_RegularChannelConfig(ADC3,ch,1,ADC_SampleTime_480Cycles); //设置ADC规则组通道，1个序列 采样时间
	ADC_SoftwareStartConv(ADC3);//使能指定的ADC3的软件转换启动功能
	while(!ADC_GetFlagStatus(ADC3,ADC_FLAG_EOC));//等待状态寄存器转换标志位结束
	return ADC_GetConversionValue(ADC3);   //返回转换的结果
}
 






