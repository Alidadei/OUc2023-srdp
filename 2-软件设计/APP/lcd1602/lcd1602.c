/*
作者：嗨小易（QQ：3443792007）
版权：本教程版权归嗨小易（QQ：3443792007）所有，未经授权不得转载、摘编或用于其他商业用途！！！
	  一经发现，后果自负！！！
*/

#include "lcd1602.h"
#include "SysTick.h"

/*******************************************************************************
* 函 数 名       : lcd1602_write_cmd
* 函数功能		 : LCD1602写命令
* 输    入       : cmd：指令
* 输    出    	 : 无
*******************************************************************************/
void lcd1602_write_cmd(u8 cmd)
{
	LCD1602_RS=0;//选择命令
	LCD1602_RW=0;//选择写
	LCD1602_E=0;
	LCD1602_DATAPORT(cmd);//准备命令
	delay_10us(100);
	LCD1602_E=1;//使能脚E先上升沿写入
	delay_10us(100);
	LCD1602_E=0;//使能脚E后负跳变完成写入	
}

/*******************************************************************************
* 函 数 名       : lcd1602_write_data
* 函数功能		 : LCD1602写数据
* 输    入       : dat：数据
* 输    出    	 : 无
*******************************************************************************/
void lcd1602_write_data(u8 dat) 
{
	LCD1602_RS=1;//选择数据
	LCD1602_RW=0;//选择写
	LCD1602_E=0;
	LCD1602_DATAPORT(dat);//准备数据
	delay_10us(100);
	LCD1602_E=1;//使能脚E先上升沿写入
	delay_10us(100);
	LCD1602_E=0;//使能脚E后负跳变完成写入		
}

//LCD1602端口初始化
void lcd1602_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(LCD1602_RS_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(LCD1602_RW_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(LCD1602_E_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(LCD1602_DATAPORT_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=LCD1602_RS_PIN;  
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(LCD1602_RS_PORT,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=LCD1602_RW_PIN;  
	GPIO_Init(LCD1602_RW_PORT,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=LCD1602_E_PIN;  
	GPIO_Init(LCD1602_E_PORT,&GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin=LCD1602_DATAPORT_PIN;  
	GPIO_Init(LCD1602_DATAPORT_PORT,&GPIO_InitStructure);
}

/*******************************************************************************
* 函 数 名       : lcd1602_init
* 函数功能		 : LCD1602初始化
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void lcd1602_init(void)
{
	lcd1602_gpio_init();
	lcd1602_write_cmd(0x38);//数据总线8位，显示2行，5*7点阵/字符
	lcd1602_write_cmd(0x0c);//显示功能开，无光标，光标闪烁
	lcd1602_write_cmd(0x06);//写入新数据后光标右移，显示屏不移动
	lcd1602_write_cmd(0x01);//清屏	
}

/*******************************************************************************
* 函 数 名       : lcd1602_clear
* 函数功能		 : LCD1602清屏
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void lcd1602_clear(void)
{
	lcd1602_write_cmd(0x01);	
}

/*******************************************************************************
* 函 数 名       : lcd1602_show_string
* 函数功能		 : LCD1602显示字符
* 输    入       : x,y：显示坐标，x=0~15，y=0~1;
				   str：显示字符串
* 输    出    	 : 无
*******************************************************************************/
void lcd1602_show_string(u8 x,u8 y,u8 *str)
{
	u8 i=0;

	if(y>1||x>15)return;//行列参数不对则强制退出

	if(y<1)	//第1行显示
	{	
		while(*str!='\0')//字符串是以'\0'结尾，只要前面有内容就显示
		{
			if(i<16-x)//如果字符长度超过第一行显示范围，则在第二行继续显示
			{
				lcd1602_write_cmd(0x80+i+x);//第一行显示地址设置	
			}
			else
			{
				lcd1602_write_cmd(0x40+0x80+i+x-16);//第二行显示地址设置	
			}
			lcd1602_write_data(*str);//显示内容
			str++;//指针递增
			i++;	
		}	
	}
	else	//第2行显示
	{
		while(*str!='\0')
		{
			if(i<16-x) //如果字符长度超过第二行显示范围，则在第一行继续显示
			{
				lcd1602_write_cmd(0x80+0x40+i+x);	
			}
			else
			{
				lcd1602_write_cmd(0x80+i+x-16);	
			}
			lcd1602_write_data(*str);
			str++;
			i++;	
		}	
	}				
}

/*******************************************************************************
* 函 数 名       : lcd1602_show_num
* 函数功能		 : LCD1602显示数字字符
* 输    入       : x,y：显示坐标，x=0~15，y=0~1;
				   num：显示数字
				   mode：mode=1:转换为ASCII
				   		 mode=0:不转换为ASCII
* 输    出    	 : 无
*******************************************************************************/
void lcd1602_show_num(u8 x,u8 y,u8 num,u8 mode)
{
	u8 i=0;

	if(y>1||x>15)return;//行列参数不对则强制退出

	if(y<1)	//第1行显示
	{	
		if(i<16-x)//如果字符长度超过第一行显示范围，则在第二行继续显示
		{
			lcd1602_write_cmd(0x80+x);//第一行显示地址设置	
		}
		else
		{
			lcd1602_write_cmd(0x40+0x80+x-16);//第二行显示地址设置	
		}
		if(mode)
			lcd1602_write_data(num+0x30);//显示内容
		else
			lcd1602_write_data(num);	
	}
	else	//第2行显示
	{
		if(i<16-x) //如果字符长度超过第二行显示范围，则在第一行继续显示
		{
			lcd1602_write_cmd(0x80+0x40+x);	
		}
		else
		{
			lcd1602_write_cmd(0x80+x-16);	
		}
		if(mode)
			lcd1602_write_data(num+0x30);//显示内容
		else
			lcd1602_write_data(num);	
	}				
}

//m^n函数
//返回值:m^n次方.
u32 num_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//mode:
//[7]:0,不填充;1,填充0.
//[6:0]:保留
void lcd1602_show_nums(u8 x,u8 y,u32 num,u8 len,u8 mode)
{
	u8 t,temp;
	u8 enshow=0;

	for(t=0;t<len;t++)
	{
		temp=(num/num_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0x80)lcd1602_show_num(x+t,y,'0',0);
				else lcd1602_show_num(x+t,y,' ',0);
				continue;
			}else enshow=1; 	 
		}
		lcd1602_show_num(x+t,y,temp,1);
	}
}
