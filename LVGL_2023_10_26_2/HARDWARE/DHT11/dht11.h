#include "key.h"
#include "delay.h" 
#ifndef __DHT11_H
#define __DHT11_H 

#define DHT11PORT	GPIOA	//�w?IO���f
#define DHT11_IO	GPIO_Pin_15	//�w?IO���f
void DHT11_IO_OUT (void); // ?�mIO�f??�X�Ҧ�
void DHT11_IO_IN (void);  // ?�mIO�f??�J�Ҧ�
void DHT11_RST (void);   // �`��DHT11
u8 Dht11_Check(void); 	 // ?�dDHT11�O�_���`
u8 Dht11_ReadBit(void);  // ?���@��?�u
u8 Dht11_ReadByte(void);  // ?���@?�r?
u8 DHT11_Init (void);   // DHT11 ��l��
u8 DHT11_ReadData(u8 *h); // DHT11?��?�u

#endif















