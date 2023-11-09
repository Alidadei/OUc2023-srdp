#include "key.h"
#include "delay.h" 
#ifndef __DHT11_H
#define __DHT11_H 

#define DHT11PORT	GPIOA	//定?IO接口
#define DHT11_IO	GPIO_Pin_15	//定?IO接口
void DHT11_IO_OUT (void); // ?置IO口??出模式
void DHT11_IO_IN (void);  // ?置IO口??入模式
void DHT11_RST (void);   // 复位DHT11
u8 Dht11_Check(void); 	 // ?查DHT11是否正常
u8 Dht11_ReadBit(void);  // ?取一位?据
u8 Dht11_ReadByte(void);  // ?取一?字?
u8 DHT11_Init (void);   // DHT11 初始化
u8 DHT11_ReadData(u8 *h); // DHT11?取?据

#endif















