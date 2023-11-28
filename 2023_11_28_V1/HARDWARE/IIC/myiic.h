#ifndef __IIC_H
#define __IIC_H




#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"





/*********************************
**	引脚选择：					**
**			SCL ---> PXx		**
**			SDA ---> PXx		**
**********************************/





/*******************************数据保持时间***********************************/  
#define  IIC_Delay()		{	delay_us(20);	}


/************************* IO方向设置(GPIOx * 2) ****************************/

/**		PXx输入模式		**/
#define SDA_IN()  {GPIOB -> MODER &=~(3 << (13*2));GPIOB -> MODER |= 0 << 13*2;}
/**		PXx输出模式		**/
#define SDA_OUT() {GPIOB -> MODER &=~(3 << (13*2));GPIOB -> MODER |= 1 << 13*2;}




/*************************** IO操作函数 *************************/
#define 	IIC_SCL    			PBout(2) 			//SCL 输出
#define 	IIC_SDA    			PBout(3) 			//SDA 输出	 
#define 	Read_SDA   			PBin (3)  			//SDA 输入





/***************************** IIC所有操作函数 **********************************/

void 				IIC_Init				(void);             //初始化IIC的IO口
void 				IIC_Start				(void);				//发送IIC开始信号
void 				IIC_Stop				(void);	  			//发送IIC停止信号
void 				IIC_Send_Byte			(uint8_t Tx_Data);		//IIC发送一个字节
uint8_t 			IIC_Read_Byte			(uint8_t ACK);		//IIC读取一个字节
uint8_t 			IIC_Wait_Ack			(void); 			//IIC等待ACK信号
void 				IIC_Ack					(void);				//IIC发送ACK信号
void 				IIC_NAck				(void);				//IIC不发送ACK信号


u16 Get_Adc3(u8 ch);
void Adc3_Init(void);

#endif
















