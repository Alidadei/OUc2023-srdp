#include "sys.h"
#include "usart3.h"
//********************************************************************************//
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif

//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η���0x3FFF��
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART3_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
//********************************************************************************//
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
  int handle;
};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
  x = x;
}
//�ض���fputc����
int fputc(int ch, FILE *f)
{
  while((USART3->SR&0X40)==0);//ѭ������,ֱ���������
  USART3->DR = (u8) ch;
  return ch;
}
#endif

#if EN_USART3_RX   //���ʹ���˽���

//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���

u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.

//USART_RX_STA��ʾ����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x55
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���

//��ʼ��IO ����1
//bound:������
void uart_init1(u32 bound)
{
  //GPIOB�˿�����,PB10/PB11
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��

  //����3��Ӧ PB10/PB11���Ÿ���ӳ��
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11����ΪUSART3
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOA10����ΪUSART3

  //USART3�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10; //GPIOA9��GPIOA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
  GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PB11��PA10

  //USART3 ��ʼ������
  USART_InitStructure.USART_BaudRate = bound;//����������
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure); //��ʼ������1

  USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���3

  //USART_ClearFlag(USART3, USART_FLAG_TC);

#if EN_USART3_RX
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����������͵��ж�

  //USART3 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����1�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
  NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif

}
//���ڷ����ֽ�
void Serial_SendByte(uint8_t Byte)
{
  USART_SendData(USART3, Byte);
  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
}

//���ڷ�������
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
  uint16_t i;
  for (i = 0; i < Length; i ++)
    {
      Serial_SendByte(Array[i]);
    }
}

//����3���յ��жϷ������һ�����յ����ݾͻ�ִ���������
//����ģ�������ͨѶ��ʱ���ԡ�0x55 0xAA"��β
void USART3_IRQHandler(void)
{
  u8 Res;//�ݴ�һ���ֽڵ�������
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//������ռĴ�������(�����ݣ�
    {
      Res =USART_ReceiveData(USART3);//(USART3->DR����ȡ���յ�������

      if((USART_RX_STA&0x8000)==0)//����δ���
        {
          if(USART_RX_STA&0x4000)//��һ�ν��յ���0x55
            {
              if(Res!=0xAA)USART_RX_STA=0;//���մ���,���¿�ʼ
              else USART_RX_STA|=0x8000;	//��������ˣ������յ�0x55,0xAA��
            }
          else//��û�յ�0X55��bit14!=0
            {
              if(Res==0x55)USART_RX_STA|=0x4000;//�˴��յ���0x55������bit14Ϊ1

              //���˴ν��յ�������������Ϣ��ţ������������Ӧ�Ľ��ջ�����
              else if(Res>0&&Res<9)
                {
                  USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
                  USART_RX_STA++;
                  //if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����
                }
            }
        }
    }
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
  OSIntExit();
#endif
}
#endif





