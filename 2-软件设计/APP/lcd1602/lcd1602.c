/*
���ߣ���С�ף�QQ��3443792007��
��Ȩ�����̳̰�Ȩ����С�ף�QQ��3443792007�����У�δ����Ȩ����ת�ء�ժ�������������ҵ��;������
	  һ�����֣�����Ը�������
*/

#include "lcd1602.h"
#include "SysTick.h"

/*******************************************************************************
* �� �� ��       : lcd1602_write_cmd
* ��������		 : LCD1602д����
* ��    ��       : cmd��ָ��
* ��    ��    	 : ��
*******************************************************************************/
void lcd1602_write_cmd(u8 cmd)
{
	LCD1602_RS=0;//ѡ������
	LCD1602_RW=0;//ѡ��д
	LCD1602_E=0;
	LCD1602_DATAPORT(cmd);//׼������
	delay_10us(100);
	LCD1602_E=1;//ʹ�ܽ�E��������д��
	delay_10us(100);
	LCD1602_E=0;//ʹ�ܽ�E���������д��	
}

/*******************************************************************************
* �� �� ��       : lcd1602_write_data
* ��������		 : LCD1602д����
* ��    ��       : dat������
* ��    ��    	 : ��
*******************************************************************************/
void lcd1602_write_data(u8 dat) 
{
	LCD1602_RS=1;//ѡ������
	LCD1602_RW=0;//ѡ��д
	LCD1602_E=0;
	LCD1602_DATAPORT(dat);//׼������
	delay_10us(100);
	LCD1602_E=1;//ʹ�ܽ�E��������д��
	delay_10us(100);
	LCD1602_E=0;//ʹ�ܽ�E���������д��		
}

//LCD1602�˿ڳ�ʼ��
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
* �� �� ��       : lcd1602_init
* ��������		 : LCD1602��ʼ��
* ��    ��       : ��
* ��    ��    	 : ��
*******************************************************************************/
void lcd1602_init(void)
{
	lcd1602_gpio_init();
	lcd1602_write_cmd(0x38);//��������8λ����ʾ2�У�5*7����/�ַ�
	lcd1602_write_cmd(0x0c);//��ʾ���ܿ����޹�꣬�����˸
	lcd1602_write_cmd(0x06);//д�������ݺ������ƣ���ʾ�����ƶ�
	lcd1602_write_cmd(0x01);//����	
}

/*******************************************************************************
* �� �� ��       : lcd1602_clear
* ��������		 : LCD1602����
* ��    ��       : ��
* ��    ��    	 : ��
*******************************************************************************/
void lcd1602_clear(void)
{
	lcd1602_write_cmd(0x01);	
}

/*******************************************************************************
* �� �� ��       : lcd1602_show_string
* ��������		 : LCD1602��ʾ�ַ�
* ��    ��       : x,y����ʾ���꣬x=0~15��y=0~1;
				   str����ʾ�ַ���
* ��    ��    	 : ��
*******************************************************************************/
void lcd1602_show_string(u8 x,u8 y,u8 *str)
{
	u8 i=0;

	if(y>1||x>15)return;//���в���������ǿ���˳�

	if(y<1)	//��1����ʾ
	{	
		while(*str!='\0')//�ַ�������'\0'��β��ֻҪǰ�������ݾ���ʾ
		{
			if(i<16-x)//����ַ����ȳ�����һ����ʾ��Χ�����ڵڶ��м�����ʾ
			{
				lcd1602_write_cmd(0x80+i+x);//��һ����ʾ��ַ����	
			}
			else
			{
				lcd1602_write_cmd(0x40+0x80+i+x-16);//�ڶ�����ʾ��ַ����	
			}
			lcd1602_write_data(*str);//��ʾ����
			str++;//ָ�����
			i++;	
		}	
	}
	else	//��2����ʾ
	{
		while(*str!='\0')
		{
			if(i<16-x) //����ַ����ȳ����ڶ�����ʾ��Χ�����ڵ�һ�м�����ʾ
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
* �� �� ��       : lcd1602_show_num
* ��������		 : LCD1602��ʾ�����ַ�
* ��    ��       : x,y����ʾ���꣬x=0~15��y=0~1;
				   num����ʾ����
				   mode��mode=1:ת��ΪASCII
				   		 mode=0:��ת��ΪASCII
* ��    ��    	 : ��
*******************************************************************************/
void lcd1602_show_num(u8 x,u8 y,u8 num,u8 mode)
{
	u8 i=0;

	if(y>1||x>15)return;//���в���������ǿ���˳�

	if(y<1)	//��1����ʾ
	{	
		if(i<16-x)//����ַ����ȳ�����һ����ʾ��Χ�����ڵڶ��м�����ʾ
		{
			lcd1602_write_cmd(0x80+x);//��һ����ʾ��ַ����	
		}
		else
		{
			lcd1602_write_cmd(0x40+0x80+x-16);//�ڶ�����ʾ��ַ����	
		}
		if(mode)
			lcd1602_write_data(num+0x30);//��ʾ����
		else
			lcd1602_write_data(num);	
	}
	else	//��2����ʾ
	{
		if(i<16-x) //����ַ����ȳ����ڶ�����ʾ��Χ�����ڵ�һ�м�����ʾ
		{
			lcd1602_write_cmd(0x80+0x40+x);	
		}
		else
		{
			lcd1602_write_cmd(0x80+x-16);	
		}
		if(mode)
			lcd1602_write_data(num+0x30);//��ʾ����
		else
			lcd1602_write_data(num);	
	}				
}

//m^n����
//����ֵ:m^n�η�.
u32 num_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//mode:
//[7]:0,�����;1,���0.
//[6:0]:����
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
