#ifndef __WKUP_H
#define __WKUP_H
#include "sys.h"
#define WKUP_PIN PBin(15)//����ⲿPB15�ĵ�ƽ��0�����ˣ�1������

u8 Check_WKUP(void);  			//���WKUP�ŵ��ź�
void WKUP_Init(void); 			//PB15 WKUP���ѳ�ʼ��
void Sys_Enter_STOP(void);	//ϵͳ�������ģʽ
#endif


