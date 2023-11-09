#ifndef __LV_GUITASK_H
#define __LV_GUITASK_H

///*日期时间数据结构体*/
extern RTC_TimeTypeDef RTC_TimeStruct;//时间结构体
extern RTC_DateTypeDef RTC_DateStruct;//日期结构体
void date_time_update();//日期更新
void Temp_Bright_Update(uint8_t temp,uint8_t brightness);//温度亮度更新
void lv_mainstart(void);

#endif

