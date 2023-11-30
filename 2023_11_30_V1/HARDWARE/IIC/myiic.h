#ifndef __IIC_H
#define __IIC_H




#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"





/*********************************
**	����ѡ��					**
**			SCL ---> PXx		**
**			SDA ---> PXx		**
**********************************/





/*******************************���ݱ���ʱ��***********************************/  
#define  IIC_Delay()		{	delay_us(20);	}


/************************* IO��������(GPIOx * 2) ****************************/

/**		PXx����ģʽ		**/
#define SDA_IN()  {GPIOB -> MODER &=~(3 << (13*2));GPIOB -> MODER |= 0 << 13*2;}
/**		PXx���ģʽ		**/
#define SDA_OUT() {GPIOB -> MODER &=~(3 << (13*2));GPIOB -> MODER |= 1 << 13*2;}




/*************************** IO�������� *************************/
#define 	IIC_SCL    			PBout(2) 			//SCL ���
#define 	IIC_SDA    			PBout(3) 			//SDA ���	 
#define 	Read_SDA   			PBin (3)  			//SDA ����





/***************************** IIC���в������� **********************************/

void 				IIC_Init				(void);             //��ʼ��IIC��IO��
void 				IIC_Start				(void);				//����IIC��ʼ�ź�
void 				IIC_Stop				(void);	  			//����IICֹͣ�ź�
void 				IIC_Send_Byte			(uint8_t Tx_Data);		//IIC����һ���ֽ�
uint8_t 			IIC_Read_Byte			(uint8_t ACK);		//IIC��ȡһ���ֽ�
uint8_t 			IIC_Wait_Ack			(void); 			//IIC�ȴ�ACK�ź�
void 				IIC_Ack					(void);				//IIC����ACK�ź�
void 				IIC_NAck				(void);				//IIC������ACK�ź�


u16 Get_Adc3(u8 ch);
void Adc3_Init(void);

#endif
















