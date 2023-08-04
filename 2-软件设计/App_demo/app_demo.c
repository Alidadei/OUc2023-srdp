/*
作者：嗨小易（QQ：3443792007）
版权：本教程版权归嗨小易（QQ：3443792007）所有，未经授权不得转载、摘编或用于其他商业用途！！！
	  一经发现，后果自负！！！
*/

#include "app_demo.h"
#include "SysTick.h"
#include "key.h"
#include "lcd1602.h"
#include "ds18b20.h"
#include "pwm.h"
#include "dc_motor.h"


//定义结构体
_sys_ctrl sys_ctrl;


//相关控制模块管脚定义
#define IRED_PIN 	PBin(12)//人体感应
#define SOUND_PIN 	PBin(13)//声音检测

void ctrl_pin_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	 
	GPIO_Init(GPIOB,&GPIO_InitStructure); 	   /* 初始化GPIO */ 
}

//参数初始化
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

//系统初始界面显示
void sys_open_show(void)
{
	lcd1602_clear();
	lcd1602_show_string(0,0,"Tp:    C");
	lcd1602_show_string(14,0,"Au");
	lcd1602_show_string(0,1,"TpS:  -  C");
	lcd1602_show_string(11,1,"WR WS");
}

//系统数据获取
void sys_data_get(void)
{
	static u8 i=0;

	while(1)
	{
		//温度获取
		i++;
		if(i%5==0)
		{
			TIM_Cmd(TIM3,DISABLE);//关闭定时器
			sys_ctrl.temp=DS18B20_GetTemperture()*10;
			TIM_Cmd(TIM3,ENABLE);//打开定时器
		}
		
		//人体红外检测
		if(IRED_PIN==0)sys_ctrl.man=1;
		else sys_ctrl.man=0;
		
		//声音检测
		if(SOUND_PIN==0)sys_ctrl.sound=1;
		else sys_ctrl.sound=0;

		break;
	}	
}

//系统数据显示
void sys_data_show(void)
{
	u8 buf[5];

	while(1)
	{
		//温度显示
		buf[0]=sys_ctrl.temp/100+0x30;
		buf[1]=sys_ctrl.temp%100/10+0x30;
		buf[2]='.';
		buf[3]=sys_ctrl.temp%100%10+0x30;
		buf[4]='\0';
		lcd1602_show_string(3,0,buf);

		//温度阈值显示
		lcd1602_show_nums(4,1,sys_ctrl.templ,2,0);
		lcd1602_show_nums(7,1,sys_ctrl.temph,2,0);

		//阈值闪烁指示
		switch(sys_ctrl.mode)
		{
			case 1://温度下限
				lcd1602_show_string(4,1,"  ");
				delay_ms(50);
				lcd1602_show_nums(4,1,sys_ctrl.templ,2,0);
				break;
			case 2://温度上限
				lcd1602_show_string(7,1,"  ");
				delay_ms(50);
				lcd1602_show_nums(7,1,sys_ctrl.temph,2,0);
				break;
		}

		//档位显示
		lcd1602_show_nums(10,0,sys_ctrl.gear,1,0);
		
		//手动状态显示
		if(sys_ctrl.auto_flag)lcd1602_show_string(14,0,"SD");
		else lcd1602_show_string(14,0,"Au");
		
		//人体红外感应状态显示
		if(sys_ctrl.man)lcd1602_show_string(11,1,"YR");
		else lcd1602_show_string(11,1,"WR");
		
		//声音感应状态显示
		if(sys_ctrl.sound)lcd1602_show_string(14,1,"YS");
		else lcd1602_show_string(14,1,"WS");

		break;			
	}
}

//系统数据设置
void sys_data_set(void)
{
	u8 key=0;
	
	key=KEY_Scan(0);
	//自动手动模式设置
	if(key==KEY4_PRESS)
	{
		if(sys_ctrl.mode!=0)sys_ctrl.mode=0;
		else
			sys_ctrl.auto_flag=!sys_ctrl.auto_flag;
	}
	//阈值模式设置
	else if(key==KEY3_PRESS)
	{
		sys_ctrl.mode++;
		if(sys_ctrl.mode>2)sys_ctrl.mode=1;
	}
	//阈值设置模式下，K1和K2加减
	if(sys_ctrl.mode!=0)
	{
		//加
		if(key==KEY1_PRESS)
		{
			switch(sys_ctrl.mode)
			{
				case 1://温度下限
					sys_ctrl.templ++;
					if(sys_ctrl.templ>99)sys_ctrl.templ=0;
					break;
				case 2://温度上限
					sys_ctrl.temph++;
					if(sys_ctrl.temph>99)sys_ctrl.temph=0;
					break;
			}	
		}
		//减
		else if(key==KEY2_PRESS)
		{
			switch(sys_ctrl.mode)
			{
				case 1://温度下限
					sys_ctrl.templ--;
					if(sys_ctrl.templ<0)sys_ctrl.templ=99;
					break;
				case 2://温度上限
					sys_ctrl.temph--;
					if(sys_ctrl.temph<0)sys_ctrl.temph=99;
					break;
			}	
		}	
	}
	//手动模式下，档位调节
	if(sys_ctrl.auto_flag==1)
	{
		//加速
		if(key==KEY1_PRESS)
		{
			sys_ctrl.gear++;
			if(sys_ctrl.gear>5)sys_ctrl.gear=5;
		}
		//减速
		else if(key==KEY2_PRESS)
		{
			sys_ctrl.gear--;
			if(sys_ctrl.gear<0)sys_ctrl.gear=0;
		}		
	}
}

//系统功能控制
void sys_fun_ctrl(void)
{
	//手动模式下，速度控制
	if(sys_ctrl.auto_flag==1)
	{
		pwm_set_duty(sys_ctrl.gear*20);//占空比	
	}
	//自动模式下，速度控制
	else
	{
		//如果红外检测有人或检测有声音
		if(sys_ctrl.man==1 || sys_ctrl.sound==1)
		{
			//如果温度高于上限，温度越高，风扇越快
			if(sys_ctrl.temp>=sys_ctrl.temph*10)
			{
				if(sys_ctrl.temp>=sys_ctrl.temph*10 && sys_ctrl.temp<sys_ctrl.temph*10+20)
					sys_ctrl.gear=3;
				else if(sys_ctrl.temp>=sys_ctrl.temph*10+20 && sys_ctrl.temp<sys_ctrl.temph*10+40)
					sys_ctrl.gear=4;
				else if(sys_ctrl.temp>=sys_ctrl.temph*10+40)
					sys_ctrl.gear=5;
						
			}
			//如果温度低于下限，风扇停止
			else if(sys_ctrl.temp<=sys_ctrl.templ*10)
				sys_ctrl.gear=0;
			//温度在阈值范围内，风扇以固定速度运行
			else 
				sys_ctrl.gear=2;
		}
		//如果红外检测无人且检测没有声音
		else
			sys_ctrl.gear=0;
		
		pwm_set_duty(sys_ctrl.gear*20);//占空比
	}	
}

//应用控制系统
void appdemo_show(void)
{
	u8 i=10;
	sys_parm_init();//系统参数初始化
	KEY_Init();
	ctrl_pin_init();
	DS18B20_Init();
	lcd1602_init();//LCD1602初始化
	//读取温度数据，等待稳定
	while(i--)
	{
		sys_ctrl.temp=DS18B20_GetTemperture()*10;
		delay_ms(100);	
	}
	dc_motor_init();
	pwm_init(100-1,800-1);//初始时，PWM占空比为0
	pwm_set_duty(0);
	dc_motor_dir_ctrl(0);
	sys_open_show();//系统初始界面显示

	while(1)
	{
		sys_data_get();//系统数据获取
		sys_data_show();//系统数据显示
		sys_data_set();//系统数据设置
		sys_fun_ctrl();//系统功能控制		
	}
}
