#ifndef _app_demo_H
#define _app_demo_H
/*
���ߣ���С�ף�QQ��3443792007��
��Ȩ�����̳̰�Ȩ����С�ף�QQ��3443792007�����У�δ����Ȩ����ת�ء�ժ�������������ҵ��;������
	  һ�����֣�����Ը�������
*/

#include "system.h"


//����ṹ��
typedef struct
{
	u8 mode;//ģʽ
	u16 temp;//�¶�
	int temph;//�¶�������
	int templ;
	u8 auto_flag;//�Զ��ֶ���־��0�Զ���1�ֶ�
	int gear;//��λ
	u8 man;//��Ӧ�����ˣ�0���ˣ�1����
	u8 sound;//��Ӧ����������0������1����
}_sys_ctrl;
extern _sys_ctrl sys_ctrl;


//��������
void appdemo_show(void);

#endif
