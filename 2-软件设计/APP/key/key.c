/*
���ߣ���С�ף�QQ��3443792007��
��Ȩ�����̳̰�Ȩ����С�ף�QQ��3443792007�����У�δ����Ȩ����ת�ء�ժ�������������ҵ��;������
	  һ�����֣�����Ը�������
*/

#include "key.h"
#include "SysTick.h"

/*******************************************************************************
* �� �� ��         : KEY_Init
* ��������		   : ������ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ�����	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=KEY1_PIN|KEY2_PIN|KEY3_PIN|KEY4_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//��������
	GPIO_Init(KEY_PORT,&GPIO_InitStructure);
}

/*******************************************************************************
* �� �� ��         : KEY_Scan
* ��������		   : ����ɨ����
* ��    ��         : mode=0:���ΰ��°���
					 mode=1���������°���
* ��    ��         : 0��δ�а�������
					 KEY_UP_PRESS��KEY_UP������
					 KEY0_PRESS��KEY0������
					 KEY1_PRESS��KEY1������
					 KEY2_PRESS��KEY2������
*******************************************************************************/
u8 KEY_Scan(u8 mode)
{
	static u8 key=1;
	
	if(mode==1) //������������
		key=1;
	if(key==1&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0)) //����һ����������
	{
		delay_ms(1);  //����
		key=0;
		if(KEY1==0)
			return KEY1_PRESS; 
		else if(KEY2==0)
			return KEY2_PRESS; 
		else if(KEY3==0)
			return KEY3_PRESS; 
		else if(KEY4==0)
			return KEY4_PRESS;		
	}
	else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)    //�ް�������
		key=1;
	return 0;
}

