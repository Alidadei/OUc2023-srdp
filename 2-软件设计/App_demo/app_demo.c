/*
���ߣ���С�ף�QQ��3443792007��
��Ȩ�����̳̰�Ȩ����С�ף�QQ��3443792007�����У�δ����Ȩ����ת�ء�ժ�������������ҵ��;������
	  һ�����֣�����Ը�������
*/

#include "app_demo.h"
#include "SysTick.h"
#include "key.h"
#include "lcd1602.h"
#include "ds18b20.h"
#include "pwm.h"
#include "dc_motor.h"


//����ṹ��
_sys_ctrl sys_ctrl;


//��ؿ���ģ��ܽŶ���
#define IRED_PIN 	PBin(12)//�����Ӧ
#define SOUND_PIN 	PBin(13)//�������

void ctrl_pin_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����ṹ�����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13;  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	 
	GPIO_Init(GPIOB,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */ 
}

//������ʼ��
void sys_parm_init(void)
{
	sys_ctrl.mode=0;
	sys_ctrl.temph=30;
	sys_ctrl.templ=25;
	sys_ctrl.auto_flag=0;
	sys_ctrl.gear=0;
	sys_ctrl.man=0;	
	sys_ctrl.sound=0;
}

//ϵͳ��ʼ������ʾ
void sys_open_show(void)
{
	lcd1602_clear();
	lcd1602_show_string(0,0,"Tp:    C");
	lcd1602_show_string(14,0,"Au");
	lcd1602_show_string(0,1,"TpS:  -  C");
	lcd1602_show_string(11,1,"WR WS");
}

//ϵͳ���ݻ�ȡ
void sys_data_get(void)
{
	static u8 i=0;

	while(1)
	{
		//�¶Ȼ�ȡ
		i++;
		if(i%5==0)
		{
			TIM_Cmd(TIM3,DISABLE);//�رն�ʱ��
			sys_ctrl.temp=DS18B20_GetTemperture()*10;
			TIM_Cmd(TIM3,ENABLE);//�򿪶�ʱ��
		}
		
		//���������
		if(IRED_PIN==0)sys_ctrl.man=1;
		else sys_ctrl.man=0;
		
		//�������
		if(SOUND_PIN==0)sys_ctrl.sound=1;
		else sys_ctrl.sound=0;

		break;
	}	
}

//ϵͳ������ʾ
void sys_data_show(void)
{
	u8 buf[5];

	while(1)
	{
		//�¶���ʾ
		buf[0]=sys_ctrl.temp/100+0x30;
		buf[1]=sys_ctrl.temp%100/10+0x30;
		buf[2]='.';
		buf[3]=sys_ctrl.temp%100%10+0x30;
		buf[4]='\0';
		lcd1602_show_string(3,0,buf);

		//�¶���ֵ��ʾ
		lcd1602_show_nums(4,1,sys_ctrl.templ,2,0);
		lcd1602_show_nums(7,1,sys_ctrl.temph,2,0);

		//��ֵ��˸ָʾ
		switch(sys_ctrl.mode)
		{
			case 1://�¶�����
				lcd1602_show_string(4,1,"  ");
				delay_ms(50);
				lcd1602_show_nums(4,1,sys_ctrl.templ,2,0);
				break;
			case 2://�¶�����
				lcd1602_show_string(7,1,"  ");
				delay_ms(50);
				lcd1602_show_nums(7,1,sys_ctrl.temph,2,0);
				break;
		}

		//��λ��ʾ
		lcd1602_show_nums(10,0,sys_ctrl.gear,1,0);
		
		//�ֶ�״̬��ʾ
		if(sys_ctrl.auto_flag)lcd1602_show_string(14,0,"SD");
		else lcd1602_show_string(14,0,"Au");
		
		//��������Ӧ״̬��ʾ
		if(sys_ctrl.man)lcd1602_show_string(11,1,"YR");
		else lcd1602_show_string(11,1,"WR");
		
		//������Ӧ״̬��ʾ
		if(sys_ctrl.sound)lcd1602_show_string(14,1,"YS");
		else lcd1602_show_string(14,1,"WS");

		break;			
	}
}

//ϵͳ��������
void sys_data_set(void)
{
	u8 key=0;
	
	key=KEY_Scan(0);
	//�Զ��ֶ�ģʽ����
	if(key==KEY4_PRESS)
	{
		if(sys_ctrl.mode!=0)sys_ctrl.mode=0;
		else
			sys_ctrl.auto_flag=!sys_ctrl.auto_flag;
	}
	//��ֵģʽ����
	else if(key==KEY3_PRESS)
	{
		sys_ctrl.mode++;
		if(sys_ctrl.mode>2)sys_ctrl.mode=1;
	}
	//��ֵ����ģʽ�£�K1��K2�Ӽ�
	if(sys_ctrl.mode!=0)
	{
		//��
		if(key==KEY1_PRESS)
		{
			switch(sys_ctrl.mode)
			{
				case 1://�¶�����
					sys_ctrl.templ++;
					if(sys_ctrl.templ>99)sys_ctrl.templ=0;
					break;
				case 2://�¶�����
					sys_ctrl.temph++;
					if(sys_ctrl.temph>99)sys_ctrl.temph=0;
					break;
			}	
		}
		//��
		else if(key==KEY2_PRESS)
		{
			switch(sys_ctrl.mode)
			{
				case 1://�¶�����
					sys_ctrl.templ--;
					if(sys_ctrl.templ<0)sys_ctrl.templ=99;
					break;
				case 2://�¶�����
					sys_ctrl.temph--;
					if(sys_ctrl.temph<0)sys_ctrl.temph=99;
					break;
			}	
		}	
	}
	//�ֶ�ģʽ�£���λ����
	if(sys_ctrl.auto_flag==1)
	{
		//����
		if(key==KEY1_PRESS)
		{
			sys_ctrl.gear++;
			if(sys_ctrl.gear>5)sys_ctrl.gear=5;
		}
		//����
		else if(key==KEY2_PRESS)
		{
			sys_ctrl.gear--;
			if(sys_ctrl.gear<0)sys_ctrl.gear=0;
		}		
	}
}

//ϵͳ���ܿ���
void sys_fun_ctrl(void)
{
	//�ֶ�ģʽ�£��ٶȿ���
	if(sys_ctrl.auto_flag==1)
	{
		pwm_set_duty(sys_ctrl.gear*20);//ռ�ձ�	
	}
	//�Զ�ģʽ�£��ٶȿ���
	else
	{
		//������������˻���������
		if(sys_ctrl.man==1 || sys_ctrl.sound==1)
		{
			//����¶ȸ������ޣ��¶�Խ�ߣ�����Խ��
			if(sys_ctrl.temp>=sys_ctrl.temph*10)
			{
				if(sys_ctrl.temp>=sys_ctrl.temph*10 && sys_ctrl.temp<sys_ctrl.temph*10+20)
					sys_ctrl.gear=3;
				else if(sys_ctrl.temp>=sys_ctrl.temph*10+20 && sys_ctrl.temp<sys_ctrl.temph*10+40)
					sys_ctrl.gear=4;
				else if(sys_ctrl.temp>=sys_ctrl.temph*10+40)
					sys_ctrl.gear=5;
						
			}
			//����¶ȵ������ޣ�����ֹͣ
			else if(sys_ctrl.temp<=sys_ctrl.templ*10)
				sys_ctrl.gear=0;
			//�¶�����ֵ��Χ�ڣ������Թ̶��ٶ�����
			else 
				sys_ctrl.gear=2;
		}
		//��������������Ҽ��û������
		else
			sys_ctrl.gear=0;
		
		pwm_set_duty(sys_ctrl.gear*20);//ռ�ձ�
	}	
}

//Ӧ�ÿ���ϵͳ
void appdemo_show(void)
{
	u8 i=10;
	sys_parm_init();//ϵͳ������ʼ��
	KEY_Init();
	ctrl_pin_init();
	DS18B20_Init();
	lcd1602_init();//LCD1602��ʼ��
	//��ȡ�¶����ݣ��ȴ��ȶ�
	while(i--)
	{
		sys_ctrl.temp=DS18B20_GetTemperture()*10;
		delay_ms(100);	
	}
	dc_motor_init();
	pwm_init(100-1,800-1);//��ʼʱ��PWMռ�ձ�Ϊ0
	pwm_set_duty(0);
	dc_motor_dir_ctrl(0);
	sys_open_show();//ϵͳ��ʼ������ʾ

	while(1)
	{
		sys_data_get();//ϵͳ���ݻ�ȡ
		sys_data_show();//ϵͳ������ʾ
		sys_data_set();//ϵͳ��������
		sys_fun_ctrl();//ϵͳ���ܿ���		
	}
}
