#ifndef __WKUP_H   //�����ӦO������PF0��V����5V��G�Žӵ�
#define __WKUP_H	//�ֻ������߽�PF1�����߽ӵ�
#include "sys.h"
#define WKUP_PIN PFin(0)//����ⲿPF0��ƽ��0�����ˣ�1������
#define PHONE_LK PFout(1)//�ֻ����������ţ�0��������1������

u8 Check_WKUP(void);  			//���WKUP�ŵ��ź�
void LK_Init(void);//PF1 PHONE_LK���ų�ʼ��
void WKUP_Init(void); 			//PF0 WKUP���ų�ʼ��
void Lock_open(void);//���ֻ���
void sys_shut(void);	//ϵͳ�������ģʽ
#endif
