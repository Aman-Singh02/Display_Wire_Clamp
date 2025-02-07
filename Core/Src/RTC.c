/******************** (C) COPYRIGHT 2023 EDS_INDIA  ****************************
 * File Name     : RTC.c     			       	                                     *
 * Author        : TEC SANTOSH KUMAR                                           *
 * Date          : 07/04/2023                                                  *
 * Description   : This file Contains func to Initialize controller STM32F4.   *
 * Revision	: 0.0							                                                 *
 *******************************************************************************/

#include "main.h"

/*************************************************************************
 * @brief  Display the current time and date.
 * @param  ShowTime : pointer to buffer
 * @param  ShowDate : pointer to buffer
 * @param Time Formate : 2023-02-14T12:10:32.878+05:30
 * @retval None
 **/
uint32_t RTC_SS;
void RTC_ShowTime()
{
  /* Show the RTC current Time */
  /* Display time Format : hh:mm:ss */
  // RTC_SS = __LL_RTC_GET_SECOND(RTC);
  /* RTC_MM =*/ LL_RTC_TIME_GetMinute(RTC);
  /* RTC_HH = */ LL_RTC_TIME_GetHour(RTC);
  
  //sprintf(RTC_Time,"%02d:%02d:%02d",RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds);
  //sprintf((char *)RTC_Time, "%02d:%02d:%02d",RTC_HH, RTC_MM, RTC_SS);
}

void RTC_ShowDate()
{
  /* Display date Format : yy-mm-dd */ 
  // RTC_DD = LL_RTC_DATE_GetDay(RTC);
  // RTC_MH = LL_RTC_DATE_GetMonth(RTC);
  // RTC_YY = LL_RTC_DATE_GetYear(RTC);
  
 // sprintf(RTC_Date,"20%02d-%02d-%02d",RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Day);
}

/**
 * @brief  Set the current Time.
 * @param  SetTime
 * @retval None
 **/
void RTC_SetTime(unsigned char HH, unsigned char MM, unsigned char SS)
{
   // RTC_TimeStruct.TimeFormat = LL_RTC_TIME_FORMAT_AM_OR_24;
    RTC_TimeStruct.Hours = HH;
    RTC_TimeStruct.Minutes = MM;
    RTC_TimeStruct.Seconds = SS;
    if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
      Error_Handler();
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 0x32F2);

}

/**
 * @brief  Set the current Date.
 * @param  SetDate
 * @retval None
 **/
void RTC_SetDate(unsigned char YY, unsigned char MM, unsigned char DD)
{
    RTC_DateStruct.Year  = YY;
    RTC_DateStruct.Month = MM;
    RTC_DateStruct.Day   = DD;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
    Error_Handler();
  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 0x32F2);
}