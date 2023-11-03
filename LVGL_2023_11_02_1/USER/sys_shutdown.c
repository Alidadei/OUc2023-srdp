#include "sys_shutdown.h"
#include "led.h"
#include "fan.h"
#include "sys.h"
#include "delay.h"
//PF0��ʼ��
void WKUP_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //pin0��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//������������ȫ���ⲿ�������
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��PF0

}
//���WKUP�ŵ��ź�
//����ֵ0:PF0�ż�⵽�͵�ƽ�����ˣ�һС��ʱ�䣬����롰˯�ߡ�
//      1:����,PF0�ż�⵽�ߵ�ƽ
u8 Check_WKUP(void)
{
  u8 t=0;
  u8 tx=0;//��¼PF0״̬

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
      delay_ms(330);//��ʱ������������Լ��1s
    }
  return tx;
}
//�رշ��Ⱥ�LED��ϵͳ���롰������ģʽ
void Sys_shutdown()
{

  Light_Shut();
  FAN_Shut();
}
//���Ժ���
void test(void)
{
	u8 i=18;
	/*Light_Open();
  
  while(i>0)
    {
      Light_Down();
      delay_ms(1000);
    }*/
	
		while(i>0)
    {
      Light_Up();
      delay_ms(1000);
			i--;
    }


}