#ifndef __LV_GUITASK_H
#define __LV_GUITASK_H

///*����ʱ�����ݽṹ��*/
extern RTC_TimeTypeDef RTC_TimeStruct;//ʱ��ṹ��
extern RTC_DateTypeDef RTC_DateStruct;//���ڽṹ��
extern uint8_t PhonelockTime;//�ֻ�����ʱ��
extern uint8_t ReminderTime;//����ʱ����
void date_time_update();//���ڸ���
void Temp_Bright_Update(uint8_t temp,uint8_t brightness);//�¶����ȸ���
void lv_mainstart(void);//����lvglҳ��
void tile_page_set();//���õ�ǰƽ����ͼҳ��Ľӿں���(����Ϊ��ҳ��)
#endif

