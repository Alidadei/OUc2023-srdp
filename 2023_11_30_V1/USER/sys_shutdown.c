#include "led.h"
#include "fan.h"
#include "delay.h"
#include "sys_shutdown.h"
//�����ӦO������PF0��V����5V��G�Žӵ�
//�ֻ������߽�PF1�����߽ӵ�

//PF1 PHONE_LK���ų�ʼ��
void LK_Init(void)
{

  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //Pin1��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ������PF1
  GPIO_ResetBits(GPIOF,GPIO_Pin_1);//��ʼ���ֻ���Ϊ����״̬
}
//PF0 ��ʼ��
void WKUP_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //Pin0��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//������������ȫ���ⲿ�������
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��PF0


}
//���WKUP�ŵ��ź�
//����ֵ0:PF0���ż�⵽�͵�ƽ�����ˣ�һС��ʱ�䣬����롰˯�ߡ�
//      1:����,PF0���ż�⵽�ߵ�ƽ
u8 Check_WKUP(void)
{
  u8 t=0;
  u8 tx=0;//��¼PF0��״̬

  //��ʱ�������
  while(t<2)
    {
      t++;
      if( WKUP_PIN==1)
        {
          tx=1;
        }
      else
        {
          tx=0;
        }
      //delay_ms(330);//��ʱ������������Լ��1s
    }
  return tx;
}
//���ֻ���
void Lock_open(void)
{
	GPIO_SetBits(GPIOF,GPIO_Pin_1);//���ߵ�ƽ����
	delay_ms(500);
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);//�������󣬳�ʼ���ֻ���Ϊ����״̬;
}
//ϵͳ�������ģʽ
void sys_shut(void)
{
	Light_Shut();//�ص�
	FAN_Shut();//�ط���
  //Lock_open();//���ֻ���
}
