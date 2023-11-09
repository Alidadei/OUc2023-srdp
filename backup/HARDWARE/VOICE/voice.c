//***************************和语音模块通讯的文件，使用USART1串口！**********************************************/
//！注意：通讯的时候两个设备之间要共地（比如都用USB供电然后都连在电脑上）；* B5接PB11，B6接PB10
#include "sys.h"
#include "voice.h"
#include "usart3                                          .h"
#include <stdio.h>
#include "led.h"
#include "fan.h"
#include "delay.h"
///********************************语音模块与主控通讯消息合集↓↓↓↓**********************************************/

//*********（32主控->语音）:*****************************************

//疲劳提醒消息
u8 RemindTired1[]= {  0xAA, 0x55, 0x03,0x03, 0x55, 0xAA};//注意！第一位为停止位！

//提醒成年人坐姿的消息数组
u8 Remind_Pos_Adt[] = {00,0xAA, 0x55, 0x02,0x02, 0x55, 0xAA};

//自动调光通知（当亮度偏低的时候）
u8 Auto_Light_info[]= {0xAA, 0x55, 0x00,0x00, 0x55, 0xAA};

//*********（语音->32主控）:*****************************************
typedef enum VOICE_MSG { light_open=0x01, light_shut, light_up,light_down,
                         fan_open, fan_shut, fan_up, fan_down=0x08
                       } voice_cmd;
/****************************************************************************************/

//*********（32主控->语音）:*****************************************
//坐姿提醒（用串口发送消息的形式）
//mode模式不同，发送不同消息
void Remind_pose(u8 mode)
{
  if(mode)
    {

      Serial_SendArray(Remind_Pos_Adt,7); //提醒成年人坐姿
    }
  else {} //提醒儿童坐姿

}

//*********（语音->32主控）:*****************************************
//以下为外设控制函数的指针数组，相当于一个哈希表(在哪个文件里调用就在哪里定义！)
void(*Ctr_func[8])(void)= { Light_Open, Light_Shut, Light_Up, Light_Down,
                            FAN_Open,     FAN_Shut,    FAN_Up,FAN_Down

                          };

//外设的语音控制，根据从语音串口发来的消息编号进行
void Voice_ctrol(void)
{
  u16 times=0;//辅助变量
  u8 cmd;//语音命令消息编号
  if(USART_RX_STA&0x8000)//如果接收完成了
    {
      /*u8 len;//数据长度
      len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度 */
			
      //把接收到的第七个字节作为消息编号
      cmd=USART_RX_BUF[6];
      Ctr_func[cmd-1]();//直接通过函数指针数组来调用对应的控制函数，相当于哈希查找！
      USART_RX_STA=0;//标志还原
    }
  else //如果尚未接收完成
    {
      times++;
      if(times%30==0)LED1=!LED1;//闪烁LED,提示系统正在运行.
      delay_ms(10);
    }
}










