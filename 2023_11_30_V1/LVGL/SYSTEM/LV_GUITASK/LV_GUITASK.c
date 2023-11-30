#include "lvgl.h"
#include "LV_GUITASK.h"
#include <stdio.h>


#if (1 != LV_FONT_MONTSERRAT_10 || 1 != LV_FONT_MONTSERRAT_14)
    #error Please Make Sure Enable LV_FONT_MONTSERRAT_10 & LV_FONT_MONTSERRAT_14
#endif

static const lv_font_t* font;       /* �������� */

/* ��ȡ��ǰ���Ļ�Ŀ�� */
#define scr_act_width() lv_obj_get_width(lv_scr_act())
#define scr_act_height() lv_obj_get_height(lv_scr_act())

void font_select(void)
{
	/* ���ݻ��Ļ���ѡ������ */
	if (scr_act_width() <= 320)
	{
		font = &lv_font_montserrat_10;
	}
	else if (scr_act_width() <= 480)
	{
		font = &lv_font_montserrat_14;
	}
}


void lv_mainstart(void)
{	
	font_select();
	lv_obj_t* switch_obj = lv_switch_create(lv_scr_act());
	lv_obj_set_size(switch_obj,60,30);
	lv_obj_align(switch_obj,LV_ALIGN_CENTER,0,0);
}


