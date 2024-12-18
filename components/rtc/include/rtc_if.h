/*
 * rtc_if.h
 *
 *  Created on: Nov 18, 2024
 *      Author: enriq
 */

#ifndef COMPONENTS_RTC_INCLUDE_RTC_IF_H_
#define COMPONENTS_RTC_INCLUDE_RTC_IF_H_

#include <time.h>

extern void rtc_sntp_SM(void);
extern void rtc_printLocTime(void);
extern int rtc_get_WeekDay(time_t *time_now);
extern int rtc_get_MonthDay(time_t *time_now);
uint64_t rtc_get_FutureDate_sec(time_t *future_time);

#endif /* COMPONENTS_RTC_INCLUDE_RTC_IF_H_ */
