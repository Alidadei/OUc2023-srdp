#include "sys.h"
#include "usart3.h"
//********************************************************************************//
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方（0x3FFF）
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART3_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
//********************************************************************************//
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
  int handle;
};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
  x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
  while((USART3->SR&0X40)==0);//循环发送,直到发送完毕
  USART3->DR = (u8) ch;
  return ch;
}
#endif

#if EN_USART3_RX   //如果使能了接收

//注意,读取USARTx->SR能避免莫名其妙的错误

u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

//USART_RX_STA表示接收状态
//bit15，	接收完成标志
//bit14，	接收到0x55
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记

//初始化IO 串口1
//bound:波特率
void uart_init1(u32 bound)
{
  //GPIOB端口设置,PB10/PB11
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOB时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟

  //串口3对应 PB10/PB11引脚复用映射
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11复用为USART3
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOA10复用为USART3

  //USART3端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10; //GPIOA9与GPIOA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
  GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PB11，PA10

  //USART3 初始化设置
  USART_InitStructure.USART_BaudRate = bound;//波特率设置
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure); //初始化串口1

  USART_Cmd(USART3, ENABLE);  //使能串口3

  //USART_ClearFlag(USART3, USART_FLAG_TC);

#if EN_USART3_RX
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启具体类型的中断

  //USART3 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口1中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
  NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif

}
//串口发送字节
void Serial_SendByte(uint8_t Byte)
{
  USART_SendData(USART3, Byte);
  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
}

//串口发送数组
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
  uint16_t i;
  for (i = 0; i < Length; i ++)
    {
      Serial_SendByte(Array[i]);
    }
}

//串口3接收的中断服务程序，一旦接收到数据就会执行这个函数
//语音模块和主控通讯的时候以“0x55 0xAA"结尾
void USART3_IRQHandler(void)
{
  u8 Res;//暂存一个字节的数据上
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//如果接收寄存器不空(有数据）
    {
      Res =USART_ReceiveData(USART3);//(USART3->DR，读取接收到的数据

      if((USART_RX_STA&0x8000)==0)//接收未完成
        {
          if(USART_RX_STA&0x4000)//上一次接收到了0x55
            {
              if(Res!=0xAA)USART_RX_STA=0;//接收错误,重新开始
              else USART_RX_STA|=0x8000;	//接收完成了（接连收到0x55,0xAA）
            }
          else//还没收到0X55，bit14!=0
            {
              if(Res==0x55)USART_RX_STA|=0x4000;//此次收到了0x55，设置bit14为1

              //若此次接收到的是正常的消息编号，则将数据送入对应的接收缓存区
              else if(Res>0&&Res<9)
                {
                  USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
                  USART_RX_STA++;
                  //if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收
                }
            }
        }
    }
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
  OSIntExit();
#endif
}
#endif





