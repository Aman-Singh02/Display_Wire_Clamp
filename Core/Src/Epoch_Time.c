#include "main.h"

// Days in each month for non-leap (0) and leap years (1)
const int daysInMonth[2][12] = {{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},  // Non-leap year
                                {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}}; // Leap year

/*******************************************************************************
* Function Name : Time_To_Epoch
* Description : Converts RTC Date and Time to Epoch time (seconds since 1970-01-01 00:00:00 UTC)
* Input         : RTC_DateTypeDef, RTC_TimeTypeDef
* Output        : None
* Return        : long (Epoch time)
*******************************************************************************/

long Time_To_Epoch(LL_RTC_DateTypeDef date, LL_RTC_TimeTypeDef time) 
{
    int year = date.Year + 2000;
    int isLeap = IS_LEAP_YEAR(year);
    long totalDays = (year - 1970) * 365L;
    
    /* colculate total days from 1969 to 2024 */
    totalDays += ((year - 1969) / 4) - ((year - 1901) / 100) + ((year - 1601) / 400);

    for (int m = 0; m < date.Month - 1; m++)
        totalDays += daysInMonth[isLeap][m];

    totalDays += date.Day - 1;

    return totalDays * 86400L + time.Hours * 3600 + time.Minutes * 60 + time.Seconds;
}


/*******************************************************************************
* Function Name : Time_To_Epoch
* Description   : Converts Epoch time (seconds since 1970-01-01 00:00:00 UTC) to RTC Date and Time
* Input         : long (Epoch time), RTC_DateTypeDef, RTC_TimeTypeDef
* Output        : None
* Return        : None
*******************************************************************************/
void Epoch_To_Time(long epoch, LL_RTC_DateTypeDef *date, LL_RTC_TimeTypeDef *time) 
{
    /* Seconds in a day */
    long days = epoch / 86400L;
    int secondsOfDay = epoch % 86400L;

    /* Time calculation */
    time->Hours = secondsOfDay / 3600;
    time->Minutes = (secondsOfDay / 60) % 60;
    time->Seconds = secondsOfDay % 60;

    /* Year calculation */
    int year = 1970;
    while (days >= (365 + IS_LEAP_YEAR(year))) {
        days -= (365 + IS_LEAP_YEAR(year));
        year++;
    }
    date->Year = year - 2000;

    /* Month and day calculation */
    const int *daysInCurrentYear = daysInMonth[IS_LEAP_YEAR(year)];
    int month = 0;
    while (days >= daysInCurrentYear[month]) { days -= daysInCurrentYear[month++];}
    
    date->Month = month + 1;
    date->Day = days + 1;
}