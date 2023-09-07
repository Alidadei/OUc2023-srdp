#ifndef __LV_GUITASK_H
#define __LV_GUITASK_H

///*日期时间数据结构体*/
extern RTC_TimeTypeDef RTC_TimeStruct;//时间结构体
extern RTC_DateTypeDef RTC_DateStruct;//日期结构体
void date_time_update();
void lv_mainstart(void);

#endif

