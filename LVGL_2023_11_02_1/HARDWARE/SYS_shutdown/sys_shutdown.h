#ifndef __SYS_SHUTDOWN_H
#define __SYS_SHUTDOWN_H
#include "sys.h"
#define WKUP_PIN PFin(0)//����ⲿPF0�ĵ�ƽ��0�����ˣ�1������

u8 Check_WKUP(void);  			//���WKUP�ŵ��ź�
void WKUP_Init(void); 			//PF0 WKUP���ѳ�ʼ��
void Sys_shutdown(void);	//ϵͳ�������ģʽ

//���Ժ���
void test(void);
#endif