#include "key.h"
#include "delay.h" 
#include "sys.h"
#ifndef __BH1750_H__
#define __BH1750_H__



#define uchar unsigned char 
#define uint  unsigned int

#define SDA   PAout(3)   
#define SCL   PAout(2)    
#define sda   GPIO_Pin_3
#define scl   GPIO_Pin_2
#define bh1750_PORT GPIOA

#define	  SlaveAddress   0x46   //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
                              //ALT  ADDRESS���Žӵ�ʱ��ַΪ0x46���ӵ�Դʱ��ַΪ0xB8
															
extern uchar    BUF[8];                         //�������ݻ�����      	
extern int     dis_data;                       //����		
extern int   mcy;              //��ʾ��λ��־λ

void Init_BH1750(void);
void conversion(uint temp_data);
void  Single_Write_BH1750(uchar REG_Address);//����д������
uchar Single_Read_BH1750(uchar REG_Address);   //������ȡ�ڲ��Ĵ�������
void  mread(void);         //�����Ķ�ȡ�ڲ��Ĵ�������
float read_BH1750(void);
#endif


