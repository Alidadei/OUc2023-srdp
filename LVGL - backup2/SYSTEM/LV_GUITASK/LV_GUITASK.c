#include "stm32f4xx.h"                  // Device header
#include "lvgl.h"
#include "LV_GUITASK.h"
#include <stdio.h>
#include <time.h>

#if (1 != LV_FONT_MONTSERRAT_16 || 1 != LV_FONT_MONTSERRAT_18)
    #error Please Make Sure Enable LV_FONT_MONTSERRAT_16 & LV_FONT_MONTSERRAT_18
#endif

/* 获取当前活动屏幕的宽高 */
#define scr_act_width()  lv_obj_get_width(lv_scr_act())
#define scr_act_height() lv_obj_get_height(lv_scr_act())

/********************************COPY OR PASTE FROM HERE************************************/
static const lv_font_t* font;           /* 定义字体 */

static lv_obj_t* tile1;        /*页面对象1*/
static lv_obj_t* tile2;        /*页面对象2*/
static lv_obj_t* tile3;        /*页面对象3*/
static lv_obj_t* time_label1;	//定义时间标签
static lv_obj_t* time_label2;	//定义日期标签
static lv_obj_t* slider1;       //定义滑块1
static lv_obj_t* slider2;       //定义滑块2
static lv_obj_t* slider1_label2;//定义滑块1百分比标签
static lv_obj_t* slider2_label2;//定义滑块2百分比标签
static lv_obj_t* switch3;       //定义开关3
static lv_obj_t* switch4;       //定义开关4
static lv_obj_t* textarea;      //定义文本区域
static lv_obj_t* keyboard;      //定义键盘部件

///*日期时间数据结构体*/
RTC_TimeTypeDef RTC_TimeStruct;//时间结构体
RTC_DateTypeDef RTC_DateStruct;//日期结构体


///日期时间更新函数
void date_time_update()
{
	lv_label_set_text_fmt(time_label1,"%d:%d:%d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);//显示时分秒
	lv_label_set_text_fmt(time_label2,"20%d/%02d/%02d",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date);//显示年月日
}

///开关1回调函数
static void switch1_event_cb(lv_event_t* e)
{
    lv_event_code_t code= lv_event_get_code(e);//获取触发方式
    lv_obj_t *target = lv_event_get_target(e);//获取触发源
    if(code == LV_EVENT_VALUE_CHANGED)
    {
        if(lv_obj_has_state(target,LV_STATE_CHECKED))
        {
            printf("%s","Adult Mode ON\n");
        }
        else
        {
            printf("%s","Adult Mode OFF\n");
        }
    }
}

///开关2回调函数
static void switch2_event_cb(lv_event_t* e)
{
    lv_event_code_t code= lv_event_get_code(e);//获取触发方式
    lv_obj_t *target = lv_event_get_target(e);//获取触发源
    if(code == LV_EVENT_VALUE_CHANGED)
    {
        if(lv_obj_has_state(target,LV_STATE_CHECKED))
        {
            printf("%s","Phone Lock ON\n");
            lv_obj_add_state(target,LV_STATE_DISABLED);//手机锁开启后设置为不可改变
        }
        else
        {
            printf("%s","Phone Lock OFF\n");
        }
    }
}

///开关3回调函数
static void switch3_event_cb(lv_event_t* e)
{
    lv_event_code_t code= lv_event_get_code(e);//获取触发方式
    lv_obj_t *target = lv_event_get_target(e);//获取触发源
    if(code == LV_EVENT_VALUE_CHANGED)
    {
        if(lv_obj_has_state(target,LV_STATE_CHECKED))
        {
            printf("%s","Lighting ON\n");
            lv_slider_set_value(slider1,50,LV_ANIM_OFF);//开启开关，将滑块值置默认值50
            lv_label_set_text_fmt(slider1_label2,"%d%%",50);//同时更改百分比标签
            lv_obj_clear_state(slider1,LV_STATE_DISABLED);//同时清除滑块1的失能状态
        }
        else
        {
            printf("%s","Lighting OFF\n");
            lv_slider_set_value(slider1,0,LV_ANIM_OFF);//关闭开关，同时将滑块值置0
            lv_label_set_text_fmt(slider1_label2,"%d%%",0);//同时更改百分比标签
            lv_obj_add_state(slider1,LV_STATE_DISABLED);//同时添加滑块1的失能状态
        }
    }
}

///开关4回调函数
static void switch4_event_cb(lv_event_t* e)
{
    lv_event_code_t code= lv_event_get_code(e);//获取触发方式
    lv_obj_t *target = lv_event_get_target(e);//获取触发源
    if(code == LV_EVENT_VALUE_CHANGED)
    {
        if(lv_obj_has_state(target,LV_STATE_CHECKED))
        {
            printf("%s","Fan ON\n");
            lv_slider_set_value(slider2,50,LV_ANIM_OFF);//开启开关，将滑块值置默认值50
            lv_label_set_text_fmt(slider2_label2,"%d%%",50);//同时更改百分比标签
            lv_obj_clear_state(slider2,LV_STATE_DISABLED);//同时清除滑块2的失能状态
        }
        else
        {
            printf("%s","Fan OFF\n");
            lv_slider_set_value(slider2,0,LV_ANIM_OFF);//关闭开关，同时将滑块值置0
            lv_label_set_text_fmt(slider2_label2,"%d%%",0);//同时更改百分比标签
            lv_obj_add_state(slider2,LV_STATE_DISABLED);//同时添加滑块2的失能状态
        }
    }
}

///滑块1回调函数
static void slider1_event_cb(lv_event_t* e)
{
    lv_event_code_t code= lv_event_get_code(e);//获取触发方式
    lv_obj_t *target = lv_event_get_target(e);//获取触发源
    int slider_value = lv_slider_get_value(target);//滑块当前数值
    if(code == LV_EVENT_VALUE_CHANGED)//若值有变化
    {
        lv_label_set_text_fmt(slider1_label2,"%d%%",slider_value);
        if(slider_value == 0)
        {
            lv_obj_clear_state(switch3,LV_STATE_CHECKED);//滑块值归零后关闭开关
        }
    }
}

///滑块2回调函数
static void slider2_event_cb(lv_event_t* e)
{
    lv_event_code_t code= lv_event_get_code(e);//获取触发方式
    lv_obj_t *target = lv_event_get_target(e);//获取触发源
    int slider_value = lv_slider_get_value(target);//滑块当前数值
    if(code == LV_EVENT_VALUE_CHANGED)//若值有变化
    {
        lv_label_set_text_fmt(slider2_label2,"%d%%",slider_value);
        if(slider_value == 0)
        {
            lv_obj_clear_state(switch4,LV_STATE_CHECKED);//滑块值归零后关闭开关
        }
    }
}

///文本区域回调函数
static void textarea_event_cb(lv_event_t* e)
{
    lv_event_code_t code= lv_event_get_code(e);//获取触发方式
    lv_obj_t *target = lv_event_get_target(e);//获取触发源
    if(code == LV_EVENT_FOCUSED)
    {
        lv_obj_clear_flag(keyboard,LV_OBJ_FLAG_HIDDEN);//若开始输入则去除键盘的隐藏属性
    }
}

///键盘回调函数
static void keyboard_event_cb(lv_event_t* e)
{
    lv_event_code_t code= lv_event_get_code(e);//获取触发方式
    lv_obj_t *target = lv_event_get_target(e);//获取触发源
    const char* txt;
	txt = txt;//消除警告
    int btnid = lv_btnmatrix_get_selected_btn(target);//获取按钮索引
    if(btnid == 39)//当按下表示完成的39号按键
    {
        txt = lv_textarea_get_text(textarea);//获取文本区域的文本
        lv_obj_clear_state(textarea,LV_STATE_FOCUSED);//去除文本区域的聚焦状态
        lv_obj_add_flag(keyboard,LV_OBJ_FLAG_HIDDEN);//添加隐藏状态
    }

}

///第一页面板设置
void tileview_page1_set()
{
    //显示时分秒
    time_label1 = lv_label_create(tile1);
    lv_label_set_text_fmt(time_label1,"%d:%d:%d",21,39,23);//显示时分秒
    lv_obj_set_style_text_font(time_label1,&lv_font_montserrat_18,LV_STATE_DEFAULT);//字体大小设置
    lv_obj_align(time_label1,LV_ALIGN_CENTER,0,-scr_act_height()/6);//对齐
    //显示年月日
    time_label2 = lv_label_create(tile1);
    lv_label_set_text_fmt(time_label2,"20%d/%02d/%02d",23,9,7);//显示年月日
    lv_obj_set_style_text_font(time_label2,font,LV_STATE_DEFAULT);//字体大小设置
    lv_obj_align_to(time_label2,time_label1,LV_ALIGN_OUT_BOTTOM_MID,0,scr_act_height()/12);//对齐
}

///第二页面板设置
void tileview_page2_set()
{
    //添加页面标题文本
    lv_obj_t* label1 = lv_label_create(tile2);
    lv_label_set_text(label1,"Mode Setting");
    lv_obj_set_style_text_font(label1,&lv_font_montserrat_18,LV_STATE_DEFAULT);//字体大小设置
    lv_obj_align(label1,LV_ALIGN_TOP_MID,0,scr_act_height()/18);//字体对齐
    //添加开关1
    lv_obj_t* switch1 = lv_switch_create(tile2);
    lv_obj_set_style_bg_color(switch1,lv_color_hex(0x0ca6e8),LV_STATE_CHECKED|LV_PART_INDICATOR);//更改指示器颜色
    lv_obj_set_size(switch1,40,20);//设置开关尺寸
    lv_obj_align(switch1,LV_ALIGN_TOP_LEFT,scr_act_width()/16,scr_act_height()/6);//设置开关位置
    lv_obj_add_event_cb(switch1,switch1_event_cb,LV_EVENT_VALUE_CHANGED,NULL);//添加回调事件
    //添加开关文本1
    lv_obj_t* switch1_label = lv_label_create(tile2);
    lv_label_set_text(switch1_label,"Adult Mode");
    lv_obj_set_style_text_font(switch1_label,font,LV_STATE_DEFAULT);
    lv_obj_align_to(switch1_label,switch1,LV_ALIGN_OUT_RIGHT_MID,5,0);
    //添加开关2
    lv_obj_t* switch2 = lv_switch_create(tile2);
    lv_obj_set_style_bg_color(switch2,lv_color_hex(0x0ca6e8),LV_STATE_CHECKED|LV_PART_INDICATOR);//更改指示器颜色
    lv_obj_set_size(switch2,40,20);//设置开关尺寸
    lv_obj_align_to(switch2,switch1,LV_ALIGN_BOTTOM_MID,0,scr_act_height()/8);//设置开关位置
    lv_obj_add_event_cb(switch2,switch2_event_cb,LV_EVENT_VALUE_CHANGED,NULL);//添加回调事件
    //添加开关文本2
    lv_obj_t* switch2_label = lv_label_create(tile2);
    lv_label_set_text(switch2_label,"Phone Lock");
    lv_obj_set_style_text_font(switch2_label,font,LV_STATE_DEFAULT);
    lv_obj_align_to(switch2_label,switch2,LV_ALIGN_OUT_RIGHT_MID,5,0);
    //添加开关3
    switch3 = lv_switch_create(tile2);
    lv_obj_set_style_bg_color(switch3,lv_color_hex(0x0ca6e8),LV_STATE_CHECKED|LV_PART_INDICATOR);//更改指示器颜色
    lv_obj_set_size(switch3,40,20);//设置开关尺寸
    lv_obj_align_to(switch3,switch2,LV_ALIGN_BOTTOM_MID,0,scr_act_height()/8);//设置开关位置
    lv_obj_add_event_cb(switch3,switch3_event_cb,LV_EVENT_VALUE_CHANGED,NULL);//添加回调事件
    //添加开关文本3
    lv_obj_t* switch3_label = lv_label_create(tile2);
    lv_label_set_text(switch3_label,"Lighting");
    lv_obj_set_style_text_font(switch3_label,font,LV_STATE_DEFAULT);
    lv_obj_align_to(switch3_label,switch3,LV_ALIGN_OUT_RIGHT_MID,5,0);
    //添加开关4
    switch4 = lv_switch_create(tile2);
    lv_obj_set_style_bg_color(switch4,lv_color_hex(0x0ca6e8),LV_STATE_CHECKED|LV_PART_INDICATOR);//更改指示器颜色
    lv_obj_set_size(switch4,40,20);//设置开关尺寸
    lv_obj_align_to(switch4,switch3,LV_ALIGN_BOTTOM_MID,0,scr_act_height()/8);//设置开关位置
    lv_obj_add_event_cb(switch4,switch4_event_cb,LV_EVENT_VALUE_CHANGED,NULL);//添加回调事件
    //添加开关文本4
    lv_obj_t* switch4_label = lv_label_create(tile2);
    lv_label_set_text(switch4_label,"Fan");
    lv_obj_set_style_text_font(switch4_label,font,LV_STATE_DEFAULT);
    lv_obj_align_to(switch4_label,switch4,LV_ALIGN_OUT_RIGHT_MID,5,0);
}

///第三页面板设置
void tileview_page3_set()
{
    //添加页面标题文本
    lv_obj_t* label2 = lv_label_create(tile3);
    lv_label_set_text(label2,"Intensity Control");//设置标题文本
    lv_obj_set_style_text_font(label2,&lv_font_montserrat_18,LV_STATE_DEFAULT);//字体大小设置
    lv_obj_align(label2,LV_ALIGN_TOP_MID,0,scr_act_height()/18);//字体对齐
    //添加滑块1
    slider1 = lv_slider_create(tile3);
    lv_obj_set_style_bg_color(slider1,lv_color_hex(0x0ca6e8),LV_PART_INDICATOR);//更改指示器颜色
    lv_obj_set_style_bg_color(slider1,lv_color_hex(0xffffff),LV_PART_KNOB);//更改手柄颜色
    lv_obj_set_size(slider1,scr_act_width()/3*2,scr_act_width()/24);//设置滑块尺寸
    lv_obj_align(slider1,LV_ALIGN_CENTER,0,-scr_act_width()/8);//设置滑块位置
    lv_slider_set_range(slider1,0,100);//设置当前调节范围
    lv_slider_set_value(slider1,0,LV_ANIM_OFF);//设置当前值
    lv_obj_add_state(slider1,LV_STATE_DISABLED);//默认情况下为失能状态,直至开启开关
    lv_obj_add_event_cb(slider1,slider1_event_cb,LV_EVENT_VALUE_CHANGED,NULL);//添加事件回调
    //添加滑块1亮度图标
    lv_obj_t*  settings_label1 = lv_label_create(tile3);
    lv_label_set_text(settings_label1,LV_SYMBOL_SETTINGS);//设置为齿轮图标
    lv_obj_set_style_text_font(settings_label1,&lv_font_montserrat_14,LV_STATE_DEFAULT);//设置图标大小
    lv_obj_align_to(settings_label1,slider1,LV_ALIGN_OUT_LEFT_MID,-7,0);//设置位置
    //添加滑块1命名标签
    lv_obj_t*  slider1_label1 = lv_label_create(tile3);
    lv_label_set_text(slider1_label1,"Brightness");//设置亮度文本标签
    lv_obj_set_style_text_font(slider1_label1,font,LV_STATE_DEFAULT);//设置字体大小
    lv_obj_align_to(slider1_label1,slider1,LV_ALIGN_OUT_TOP_LEFT,0,-scr_act_height()/40);//设置对齐方式及位置
    //添加滑块1百分比标签
    slider1_label2 = lv_label_create(tile3);
    lv_label_set_text(slider1_label2,"0%");//设置文本
    lv_obj_set_style_text_font(slider1_label2,font,LV_STATE_DEFAULT);//设置字体
    lv_obj_align_to(slider1_label2,slider1,LV_ALIGN_OUT_RIGHT_MID,10,0);//设置对齐方式及位置

    //添加滑块2
    slider2 = lv_slider_create(tile3);
    lv_obj_set_style_bg_color(slider2,lv_color_hex(0x0ca6e8),LV_PART_INDICATOR);//更改指示器颜色
    lv_obj_set_style_bg_color(slider2,lv_color_hex(0xffffff),LV_PART_KNOB);//更改手柄颜色
    lv_obj_set_size(slider2,scr_act_width()/3*2,scr_act_width()/24);//设置滑块尺寸
    lv_obj_align(slider2,LV_ALIGN_CENTER,0,scr_act_height()/16);//设置滑块位置
    lv_slider_set_range(slider2,0,100);//设置当前调节范围
    lv_slider_set_value(slider2,0,LV_ANIM_OFF);//设置当前值
    lv_obj_add_state(slider2,LV_STATE_DISABLED);//默认情况下为失能状态,直至开启开关
    lv_obj_add_event_cb(slider2,slider2_event_cb,LV_EVENT_VALUE_CHANGED,NULL);//添加事件回调
    //添加滑块2风扇转速图标
    lv_obj_t*  settings_label2 = lv_label_create(tile3);
    lv_label_set_text(settings_label2,LV_SYMBOL_SETTINGS);//设置为齿轮图标
    lv_obj_set_style_text_font(settings_label2,&lv_font_montserrat_14,LV_STATE_DEFAULT);//设置字体大小
    lv_obj_align_to(settings_label2,slider2,LV_ALIGN_OUT_LEFT_MID,-7,0);//设置位置
    //添加滑块2命名标签
    lv_obj_t*  slider2_label1 = lv_label_create(tile3);
    lv_label_set_text(slider2_label1,"Fan Power");//设置风扇转速文本标签
    lv_obj_set_style_text_font(slider2_label1,font,LV_STATE_DEFAULT);//设置字体大小
    lv_obj_align_to(slider2_label1,slider2,LV_ALIGN_OUT_TOP_LEFT,0,-scr_act_height()/40);//设置对齐方式及位置
    //添加滑块2百分比标签
    slider2_label2 = lv_label_create(tile3);
    lv_label_set_text(slider2_label2,"0%");//设置文本
    lv_obj_set_style_text_font(slider2_label2,font,LV_STATE_DEFAULT);//设置字体
    lv_obj_align_to(slider2_label2,slider2,LV_ALIGN_OUT_RIGHT_MID,10,0);//设置对齐方式及位置
    //添加文本区域
    textarea = lv_textarea_create(tile3);
    lv_obj_set_size(textarea,45,30);//设置文本区域部件尺寸
    lv_obj_align(textarea,LV_ALIGN_CENTER,0,scr_act_height()/4);
    lv_obj_remove_style(textarea,NULL,LV_PART_SCROLLBAR);//去除滚动条
    lv_textarea_set_one_line(textarea,true);//单行模式
    lv_textarea_set_accepted_chars(textarea,"1234567890");//限制只接收数字
    lv_textarea_set_max_length(textarea,3);//限制接收三个字符
    lv_obj_add_event_cb(textarea,textarea_event_cb,LV_EVENT_FOCUSED,NULL);//添加回调函数
    //添加键盘部件
    keyboard = lv_keyboard_create(tile3);
    lv_keyboard_set_textarea(keyboard,textarea);//将键盘和文本区域关联
    lv_obj_add_flag(keyboard,LV_OBJ_FLAG_HIDDEN);//默认情况给键盘添加隐藏属性
    lv_obj_add_event_cb(keyboard,keyboard_event_cb,LV_EVENT_VALUE_CHANGED,NULL);//添加回调函数
}



///平铺视图部件
void tileview_set()
{
    /*有点类似与选项卡部件，但是没有标签，通过滑动屏幕实现页面切换*/
    //创建平铺视图部件
    lv_obj_t* tileview = lv_tileview_create(lv_scr_act());
    //添加页面并获取页面对象
     tile1 = lv_tileview_add_tile(tileview,0,0,LV_DIR_RIGHT);//需添加的平铺试图部件，列号，行号，允许切换的方向
     tile2 = lv_tileview_add_tile(tileview,1,0, LV_DIR_LEFT|LV_DIR_RIGHT );
     tile3 = lv_tileview_add_tile(tileview,2,0,LV_DIR_LEFT);
     tileview_page1_set();//第一页面
     tileview_page2_set();//第二页面
     tileview_page3_set();//第三页面
    //设置当前显示页面
    lv_obj_update_layout(tileview);//前置操作——更新参数！！
    lv_obj_set_tile(tileview,tile2,LV_ANIM_ON);//根据页面对象设置
    lv_obj_set_tile_id(tileview,0,0,LV_ANIM_ON);//根据页面行列设置
//    //移除滚动条
//    lv_obj_remove_style(tileview,NULL,LV_PART_SCROLLBAR);
}
/********************************COPY OR PASTE FROM HERE************************************/

void font_select(void)
{
	/* 根据活动屏幕宽度选择字体 */
	if (scr_act_width() <= 320)
	{
		font = &lv_font_montserrat_16;
	}
	else if (scr_act_width() <= 480)
	{
		font = &lv_font_montserrat_18;
	}
}


void lv_mainstart(void)
{	
	font_select();
	tileview_set();
}


