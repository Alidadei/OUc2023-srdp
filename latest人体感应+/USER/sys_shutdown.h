#ifndef __WKUP_H   //�����ӦO������PF0��V����5V��G�Žӵ�
#define __WKUP_H
#include "sys.h"
#define WKUP_PIN PFin(0)//����ⲿPF0��ƽ��0�����ˣ�1������

u8 Check_WKUP(void);  			//���WKUP�ŵ��ź�
void WKUP_Init(void); 			//PF0 WKUP���ų�ʼ��
void sys_shut(void);	//ϵͳ�������ģʽ
#endif
