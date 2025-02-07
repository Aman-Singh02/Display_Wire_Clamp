/******************** (C) COPYRIGHT 2015 EDS_INDIA  ****************************
* File Name     : RTC.h     			       	                               *
* Author        : TEC SANTOSH KUMAR                                            *
* Date          : 07/04/2023                                                   *
* Description   : This file Contains func to Initialize controller STM32F4.    *
* Revision	: 0.0							                                   *
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H
#define __RTC_H

void RTC_ShowTime();
void RTC_ShowDate();
void RTC_SetTime(unsigned char HH, unsigned char MM, unsigned char SS);
void RTC_SetDate(unsigned char YY, unsigned char MM, unsigned char DD);

#endif