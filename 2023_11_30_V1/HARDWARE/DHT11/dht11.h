#include "key.h"
#include "delay.h" 
#include "sys.h"
#include "stm32f4xx.h"
#ifndef __DHT11_H
#define __DHT11_H 






 
/************************** DS18B20 连接引脚定义********************************/
#define    RCC_DHT11                          RCC_AHB1Periph_GPIOA
#define    DHT11_DQ_GPIO_PORT                 GPIOA
#define    DHT11_DQ_GPIO_PIN                  GPIO_Pin_15

/************************** DS18B20 函数宏定义********************************/
#define    DHT11_DQ_L	            GPIO_ResetBits ( DHT11_DQ_GPIO_PORT, DHT11_DQ_GPIO_PIN ) 
#define    DHT11_DQ_H	            GPIO_SetBits ( DHT11_DQ_GPIO_PORT, DHT11_DQ_GPIO_PIN ) 


/************************** DS18B20 函数声明 ********************************/

void DHT11_Init(void);
u8 DHT11_Read_Data(u8 *temp,u8 *humi);

#endif /* __DS18B20_H */
















