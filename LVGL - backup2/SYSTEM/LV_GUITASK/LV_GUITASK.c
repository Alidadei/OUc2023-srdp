#include "stm32f4xx.h"                  // Device header
#include "lvgl.h"
#include "LV_GUITASK.h"
#include <stdio.h>
#include <time.h>

#if (1 != LV_FONT_MONTSERRAT_16 || 1 != LV_FONT_MONTSERRAT_18)
    #error Please Make Sure Enable LV_FONT_MONTSERRAT_16 & LV_FONT_MONTSERRAT_18
#endif

/* ��ȡ��ǰ���Ļ�Ŀ�� */
#define scr_act_width()  lv_obj_get_width(lv_scr_act())
#define scr_act_height() lv_obj_get_height(lv_scr_act())

/********************************COPY OR PASTE FROM HERE************************************/
static const lv_font_t* font;           /* �������� */

static lv_obj_t* tile1;        /*ҳ�����1*/
static lv_obj_t* tile2;        /*ҳ�����2*/
static lv_obj_t* tile3;        /*ҳ�����3*/
static lv_obj_t* time_label1;	//����ʱ���ǩ
static lv_obj_t* time_label2;	//�������ڱ�ǩ
static lv_obj_t* slider1;       //���廬��1
static lv_obj_t* slider2;       //���廬��2
static lv_obj_t* slider1_label2;//���廬��1�ٷֱȱ�ǩ
static lv_obj_t* slider2_label2;//���廬��2�ٷֱȱ�ǩ
static lv_obj_t* switch3;       //���忪��3
static lv_obj_t* switch4;       //���忪��4
static lv_obj_t* textarea;      //�����ı�����
static lv_obj_t* keyboard;      //������̲���

///*����ʱ�����ݽṹ��*/
RTC_TimeTypeDef RTC_TimeStruct;//ʱ��ṹ��
RTC_DateTypeDef RTC_DateStruct;//���ڽṹ��


///����ʱ����º���
void date_time_update()
{
	lv_label_set_text_fmt(time_label1,"%d:%d:%d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);//��ʾʱ����
	lv_label_set_text_fmt(time_label2,"20%d/%02d/%02d",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date);//��ʾ������
}

///����1�ص�����
static void switch1_event_cb(lv_event_t* e)
{
    lv_event_code_t code= lv_event_get_code(e);//��ȡ������ʽ
    lv_obj_t *target = lv_event_get_target(e);//��ȡ����Դ
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

///����2�ص�����
static void switch2_event_cb(lv_event_t* e)
{
    lv_event_code_t code= lv_event_get_code(e);//��ȡ������ʽ
    lv_obj_t *target = lv_event_get_target(e);//��ȡ����Դ
    if(code == LV_EVENT_VALUE_CHANGED)
    {
        if(lv_obj_has_state(target,LV_STATE_CHECKED))
        {
            printf("%s","Phone Lock ON\n");
            lv_obj_add_state(target,LV_STATE_DISABLED);//�ֻ�������������Ϊ���ɸı�
        }
        else
        {
            printf("%s","Phone Lock OFF\n");
        }
    }
}

///����3�ص�����
static void switch3_event_cb(lv_event_t* e)
{
    lv_event_code_t code= lv_event_get_code(e);//��ȡ������ʽ
    lv_obj_t *target = lv_event_get_target(e);//��ȡ����Դ
    if(code == LV_EVENT_VALUE_CHANGED)
    {
        if(lv_obj_has_state(target,LV_STATE_CHECKED))
        {
            printf("%s","Lighting ON\n");
            lv_slider_set_value(slider1,50,LV_ANIM_OFF);//�������أ�������ֵ��Ĭ��ֵ50
            lv_label_set_text_fmt(slider1_label2,"%d%%",50);//ͬʱ���İٷֱȱ�ǩ
            lv_obj_clear_state(slider1,LV_STATE_DISABLED);//ͬʱ�������1��ʧ��״̬
        }
        else
        {
            printf("%s","Lighting OFF\n");
            lv_slider_set_value(slider1,0,LV_ANIM_OFF);//�رտ��أ�ͬʱ������ֵ��0
            lv_label_set_text_fmt(slider1_label2,"%d%%",0);//ͬʱ���İٷֱȱ�ǩ
            lv_obj_add_state(slider1,LV_STATE_DISABLED);//ͬʱ��ӻ���1��ʧ��״̬
        }
    }
}

///����4�ص�����
static void switch4_event_cb(lv_event_t* e)
{
    lv_event_code_t code= lv_event_get_code(e);//��ȡ������ʽ
    lv_obj_t *target = lv_event_get_target(e);//��ȡ����Դ
    if(code == LV_EVENT_VALUE_CHANGED)
    {
        if(lv_obj_has_state(target,LV_STATE_CHECKED))
        {
            printf("%s","Fan ON\n");
            lv_slider_set_value(slider2,50,LV_ANIM_OFF);//�������أ�������ֵ��Ĭ��ֵ50
            lv_label_set_text_fmt(slider2_label2,"%d%%",50);//ͬʱ���İٷֱȱ�ǩ
            lv_obj_clear_state(slider2,LV_STATE_DISABLED);//ͬʱ�������2��ʧ��״̬
        }
        else
        {
            printf("%s","Fan OFF\n");
            lv_slider_set_value(slider2,0,LV_ANIM_OFF);//�رտ��أ�ͬʱ������ֵ��0
            lv_label_set_text_fmt(slider2_label2,"%d%%",0);//ͬʱ���İٷֱȱ�ǩ
            lv_obj_add_state(slider2,LV_STATE_DISABLED);//ͬʱ��ӻ���2��ʧ��״̬
        }
    }
}

///����1�ص�����
static void slider1_event_cb(lv_event_t* e)
{
    lv_event_code_t code= lv_event_get_code(e);//��ȡ������ʽ
    lv_obj_t *target = lv_event_get_target(e);//��ȡ����Դ
    int slider_value = lv_slider_get_value(target);//���鵱ǰ��ֵ
    if(code == LV_EVENT_VALUE_CHANGED)//��ֵ�б仯
    {
        lv_label_set_text_fmt(slider1_label2,"%d%%",slider_value);
        if(slider_value == 0)
        {
            lv_obj_clear_state(switch3,LV_STATE_CHECKED);//����ֵ�����رտ���
        }
    }
}

///����2�ص�����
static void slider2_event_cb(lv_event_t* e)
{
    lv_event_code_t code= lv_event_get_code(e);//��ȡ������ʽ
    lv_obj_t *target = lv_event_get_target(e);//��ȡ����Դ
    int slider_value = lv_slider_get_value(target);//���鵱ǰ��ֵ
    if(code == LV_EVENT_VALUE_CHANGED)//��ֵ�б仯
    {
        lv_label_set_text_fmt(slider2_label2,"%d%%",slider_value);
        if(slider_value == 0)
        {
            lv_obj_clear_state(switch4,LV_STATE_CHECKED);//����ֵ�����رտ���
        }
    }
}

///�ı�����ص�����
static void textarea_event_cb(lv_event_t* e)
{
    lv_event_code_t code= lv_event_get_code(e);//��ȡ������ʽ
    lv_obj_t *target = lv_event_get_target(e);//��ȡ����Դ
    if(code == LV_EVENT_FOCUSED)
    {
        lv_obj_clear_flag(keyboard,LV_OBJ_FLAG_HIDDEN);//����ʼ������ȥ�����̵���������
    }
}

///���̻ص�����
static void keyboard_event_cb(lv_event_t* e)
{
    lv_event_code_t code= lv_event_get_code(e);//��ȡ������ʽ
    lv_obj_t *target = lv_event_get_target(e);//��ȡ����Դ
    const char* txt;
	txt = txt;//��������
    int btnid = lv_btnmatrix_get_selected_btn(target);//��ȡ��ť����
    if(btnid == 39)//�����±�ʾ��ɵ�39�Ű���
    {
        txt = lv_textarea_get_text(textarea);//��ȡ�ı�������ı�
        lv_obj_clear_state(textarea,LV_STATE_FOCUSED);//ȥ���ı�����ľ۽�״̬
        lv_obj_add_flag(keyboard,LV_OBJ_FLAG_HIDDEN);//�������״̬
    }

}

///��һҳ�������
void tileview_page1_set()
{
    //��ʾʱ����
    time_label1 = lv_label_create(tile1);
    lv_label_set_text_fmt(time_label1,"%d:%d:%d",21,39,23);//��ʾʱ����
    lv_obj_set_style_text_font(time_label1,&lv_font_montserrat_18,LV_STATE_DEFAULT);//�����С����
    lv_obj_align(time_label1,LV_ALIGN_CENTER,0,-scr_act_height()/6);//����
    //��ʾ������
    time_label2 = lv_label_create(tile1);
    lv_label_set_text_fmt(time_label2,"20%d/%02d/%02d",23,9,7);//��ʾ������
    lv_obj_set_style_text_font(time_label2,font,LV_STATE_DEFAULT);//�����С����
    lv_obj_align_to(time_label2,time_label1,LV_ALIGN_OUT_BOTTOM_MID,0,scr_act_height()/12);//����
}

///�ڶ�ҳ�������
void tileview_page2_set()
{
    //���ҳ������ı�
    lv_obj_t* label1 = lv_label_create(tile2);
    lv_label_set_text(label1,"Mode Setting");
    lv_obj_set_style_text_font(label1,&lv_font_montserrat_18,LV_STATE_DEFAULT);//�����С����
    lv_obj_align(label1,LV_ALIGN_TOP_MID,0,scr_act_height()/18);//�������
    //��ӿ���1
    lv_obj_t* switch1 = lv_switch_create(tile2);
    lv_obj_set_style_bg_color(switch1,lv_color_hex(0x0ca6e8),LV_STATE_CHECKED|LV_PART_INDICATOR);//����ָʾ����ɫ
    lv_obj_set_size(switch1,40,20);//���ÿ��سߴ�
    lv_obj_align(switch1,LV_ALIGN_TOP_LEFT,scr_act_width()/16,scr_act_height()/6);//���ÿ���λ��
    lv_obj_add_event_cb(switch1,switch1_event_cb,LV_EVENT_VALUE_CHANGED,NULL);//��ӻص��¼�
    //��ӿ����ı�1
    lv_obj_t* switch1_label = lv_label_create(tile2);
    lv_label_set_text(switch1_label,"Adult Mode");
    lv_obj_set_style_text_font(switch1_label,font,LV_STATE_DEFAULT);
    lv_obj_align_to(switch1_label,switch1,LV_ALIGN_OUT_RIGHT_MID,5,0);
    //��ӿ���2
    lv_obj_t* switch2 = lv_switch_create(tile2);
    lv_obj_set_style_bg_color(switch2,lv_color_hex(0x0ca6e8),LV_STATE_CHECKED|LV_PART_INDICATOR);//����ָʾ����ɫ
    lv_obj_set_size(switch2,40,20);//���ÿ��سߴ�
    lv_obj_align_to(switch2,switch1,LV_ALIGN_BOTTOM_MID,0,scr_act_height()/8);//���ÿ���λ��
    lv_obj_add_event_cb(switch2,switch2_event_cb,LV_EVENT_VALUE_CHANGED,NULL);//��ӻص��¼�
    //��ӿ����ı�2
    lv_obj_t* switch2_label = lv_label_create(tile2);
    lv_label_set_text(switch2_label,"Phone Lock");
    lv_obj_set_style_text_font(switch2_label,font,LV_STATE_DEFAULT);
    lv_obj_align_to(switch2_label,switch2,LV_ALIGN_OUT_RIGHT_MID,5,0);
    //��ӿ���3
    switch3 = lv_switch_create(tile2);
    lv_obj_set_style_bg_color(switch3,lv_color_hex(0x0ca6e8),LV_STATE_CHECKED|LV_PART_INDICATOR);//����ָʾ����ɫ
    lv_obj_set_size(switch3,40,20);//���ÿ��سߴ�
    lv_obj_align_to(switch3,switch2,LV_ALIGN_BOTTOM_MID,0,scr_act_height()/8);//���ÿ���λ��
    lv_obj_add_event_cb(switch3,switch3_event_cb,LV_EVENT_VALUE_CHANGED,NULL);//��ӻص��¼�
    //��ӿ����ı�3
    lv_obj_t* switch3_label = lv_label_create(tile2);
    lv_label_set_text(switch3_label,"Lighting");
    lv_obj_set_style_text_font(switch3_label,font,LV_STATE_DEFAULT);
    lv_obj_align_to(switch3_label,switch3,LV_ALIGN_OUT_RIGHT_MID,5,0);
    //��ӿ���4
    switch4 = lv_switch_create(tile2);
    lv_obj_set_style_bg_color(switch4,lv_color_hex(0x0ca6e8),LV_STATE_CHECKED|LV_PART_INDICATOR);//����ָʾ����ɫ
    lv_obj_set_size(switch4,40,20);//���ÿ��سߴ�
    lv_obj_align_to(switch4,switch3,LV_ALIGN_BOTTOM_MID,0,scr_act_height()/8);//���ÿ���λ��
    lv_obj_add_event_cb(switch4,switch4_event_cb,LV_EVENT_VALUE_CHANGED,NULL);//��ӻص��¼�
    //��ӿ����ı�4
    lv_obj_t* switch4_label = lv_label_create(tile2);
    lv_label_set_text(switch4_label,"Fan");
    lv_obj_set_style_text_font(switch4_label,font,LV_STATE_DEFAULT);
    lv_obj_align_to(switch4_label,switch4,LV_ALIGN_OUT_RIGHT_MID,5,0);
}

///����ҳ�������
void tileview_page3_set()
{
    //���ҳ������ı�
    lv_obj_t* label2 = lv_label_create(tile3);
    lv_label_set_text(label2,"Intensity Control");//���ñ����ı�
    lv_obj_set_style_text_font(label2,&lv_font_montserrat_18,LV_STATE_DEFAULT);//�����С����
    lv_obj_align(label2,LV_ALIGN_TOP_MID,0,scr_act_height()/18);//�������
    //��ӻ���1
    slider1 = lv_slider_create(tile3);
    lv_obj_set_style_bg_color(slider1,lv_color_hex(0x0ca6e8),LV_PART_INDICATOR);//����ָʾ����ɫ
    lv_obj_set_style_bg_color(slider1,lv_color_hex(0xffffff),LV_PART_KNOB);//�����ֱ���ɫ
    lv_obj_set_size(slider1,scr_act_width()/3*2,scr_act_width()/24);//���û���ߴ�
    lv_obj_align(slider1,LV_ALIGN_CENTER,0,-scr_act_width()/8);//���û���λ��
    lv_slider_set_range(slider1,0,100);//���õ�ǰ���ڷ�Χ
    lv_slider_set_value(slider1,0,LV_ANIM_OFF);//���õ�ǰֵ
    lv_obj_add_state(slider1,LV_STATE_DISABLED);//Ĭ�������Ϊʧ��״̬,ֱ����������
    lv_obj_add_event_cb(slider1,slider1_event_cb,LV_EVENT_VALUE_CHANGED,NULL);//����¼��ص�
    //��ӻ���1����ͼ��
    lv_obj_t*  settings_label1 = lv_label_create(tile3);
    lv_label_set_text(settings_label1,LV_SYMBOL_SETTINGS);//����Ϊ����ͼ��
    lv_obj_set_style_text_font(settings_label1,&lv_font_montserrat_14,LV_STATE_DEFAULT);//����ͼ���С
    lv_obj_align_to(settings_label1,slider1,LV_ALIGN_OUT_LEFT_MID,-7,0);//����λ��
    //��ӻ���1������ǩ
    lv_obj_t*  slider1_label1 = lv_label_create(tile3);
    lv_label_set_text(slider1_label1,"Brightness");//���������ı���ǩ
    lv_obj_set_style_text_font(slider1_label1,font,LV_STATE_DEFAULT);//���������С
    lv_obj_align_to(slider1_label1,slider1,LV_ALIGN_OUT_TOP_LEFT,0,-scr_act_height()/40);//���ö��뷽ʽ��λ��
    //��ӻ���1�ٷֱȱ�ǩ
    slider1_label2 = lv_label_create(tile3);
    lv_label_set_text(slider1_label2,"0%");//�����ı�
    lv_obj_set_style_text_font(slider1_label2,font,LV_STATE_DEFAULT);//��������
    lv_obj_align_to(slider1_label2,slider1,LV_ALIGN_OUT_RIGHT_MID,10,0);//���ö��뷽ʽ��λ��

    //��ӻ���2
    slider2 = lv_slider_create(tile3);
    lv_obj_set_style_bg_color(slider2,lv_color_hex(0x0ca6e8),LV_PART_INDICATOR);//����ָʾ����ɫ
    lv_obj_set_style_bg_color(slider2,lv_color_hex(0xffffff),LV_PART_KNOB);//�����ֱ���ɫ
    lv_obj_set_size(slider2,scr_act_width()/3*2,scr_act_width()/24);//���û���ߴ�
    lv_obj_align(slider2,LV_ALIGN_CENTER,0,scr_act_height()/16);//���û���λ��
    lv_slider_set_range(slider2,0,100);//���õ�ǰ���ڷ�Χ
    lv_slider_set_value(slider2,0,LV_ANIM_OFF);//���õ�ǰֵ
    lv_obj_add_state(slider2,LV_STATE_DISABLED);//Ĭ�������Ϊʧ��״̬,ֱ����������
    lv_obj_add_event_cb(slider2,slider2_event_cb,LV_EVENT_VALUE_CHANGED,NULL);//����¼��ص�
    //��ӻ���2����ת��ͼ��
    lv_obj_t*  settings_label2 = lv_label_create(tile3);
    lv_label_set_text(settings_label2,LV_SYMBOL_SETTINGS);//����Ϊ����ͼ��
    lv_obj_set_style_text_font(settings_label2,&lv_font_montserrat_14,LV_STATE_DEFAULT);//���������С
    lv_obj_align_to(settings_label2,slider2,LV_ALIGN_OUT_LEFT_MID,-7,0);//����λ��
    //��ӻ���2������ǩ
    lv_obj_t*  slider2_label1 = lv_label_create(tile3);
    lv_label_set_text(slider2_label1,"Fan Power");//���÷���ת���ı���ǩ
    lv_obj_set_style_text_font(slider2_label1,font,LV_STATE_DEFAULT);//���������С
    lv_obj_align_to(slider2_label1,slider2,LV_ALIGN_OUT_TOP_LEFT,0,-scr_act_height()/40);//���ö��뷽ʽ��λ��
    //��ӻ���2�ٷֱȱ�ǩ
    slider2_label2 = lv_label_create(tile3);
    lv_label_set_text(slider2_label2,"0%");//�����ı�
    lv_obj_set_style_text_font(slider2_label2,font,LV_STATE_DEFAULT);//��������
    lv_obj_align_to(slider2_label2,slider2,LV_ALIGN_OUT_RIGHT_MID,10,0);//���ö��뷽ʽ��λ��
    //����ı�����
    textarea = lv_textarea_create(tile3);
    lv_obj_set_size(textarea,45,30);//�����ı����򲿼��ߴ�
    lv_obj_align(textarea,LV_ALIGN_CENTER,0,scr_act_height()/4);
    lv_obj_remove_style(textarea,NULL,LV_PART_SCROLLBAR);//ȥ��������
    lv_textarea_set_one_line(textarea,true);//����ģʽ
    lv_textarea_set_accepted_chars(textarea,"1234567890");//����ֻ��������
    lv_textarea_set_max_length(textarea,3);//���ƽ��������ַ�
    lv_obj_add_event_cb(textarea,textarea_event_cb,LV_EVENT_FOCUSED,NULL);//��ӻص�����
    //��Ӽ��̲���
    keyboard = lv_keyboard_create(tile3);
    lv_keyboard_set_textarea(keyboard,textarea);//�����̺��ı��������
    lv_obj_add_flag(keyboard,LV_OBJ_FLAG_HIDDEN);//Ĭ����������������������
    lv_obj_add_event_cb(keyboard,keyboard_event_cb,LV_EVENT_VALUE_CHANGED,NULL);//��ӻص�����
}



///ƽ����ͼ����
void tileview_set()
{
    /*�е�������ѡ�����������û�б�ǩ��ͨ��������Ļʵ��ҳ���л�*/
    //����ƽ����ͼ����
    lv_obj_t* tileview = lv_tileview_create(lv_scr_act());
    //���ҳ�沢��ȡҳ�����
     tile1 = lv_tileview_add_tile(tileview,0,0,LV_DIR_RIGHT);//����ӵ�ƽ����ͼ�������кţ��кţ������л��ķ���
     tile2 = lv_tileview_add_tile(tileview,1,0, LV_DIR_LEFT|LV_DIR_RIGHT );
     tile3 = lv_tileview_add_tile(tileview,2,0,LV_DIR_LEFT);
     tileview_page1_set();//��һҳ��
     tileview_page2_set();//�ڶ�ҳ��
     tileview_page3_set();//����ҳ��
    //���õ�ǰ��ʾҳ��
    lv_obj_update_layout(tileview);//ǰ�ò����������²�������
    lv_obj_set_tile(tileview,tile2,LV_ANIM_ON);//����ҳ���������
    lv_obj_set_tile_id(tileview,0,0,LV_ANIM_ON);//����ҳ����������
//    //�Ƴ�������
//    lv_obj_remove_style(tileview,NULL,LV_PART_SCROLLBAR);
}
/********************************COPY OR PASTE FROM HERE************************************/

void font_select(void)
{
	/* ���ݻ��Ļ���ѡ������ */
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


