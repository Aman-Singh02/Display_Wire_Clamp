#ifndef __EPOCH_TIME_H
#define __EPOCH_TIME_H

// Check if a year is a leap year
#define IS_LEAP_YEAR(year) (((year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0)) ? 1 : 0)

long Time_To_Epoch(LL_RTC_DateTypeDef date, LL_RTC_TimeTypeDef time);
void Epoch_To_Time(long epoch, LL_RTC_DateTypeDef *date, LL_RTC_TimeTypeDef *time);

#endif /* __EPOCH_TIME_H */
