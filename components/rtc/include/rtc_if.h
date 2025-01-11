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
extern int rtc_get_hour(void);
extern int rtc_get_min(void);
extern int rtc_get_sec(void);
extern int rtc_get_WeekDay(void);
extern int rtc_get_MonthDay(void);
extern int rtc_get_Month(void);
extern int rtc_get_Year(void);
extern uint8_t splitDays(const char *input, char *days);
extern uint64_t rtc_get_FutureDate_sec(time_t *future_time);


#endif /* COMPONENTS_RTC_INCLUDE_RTC_IF_H_ */
