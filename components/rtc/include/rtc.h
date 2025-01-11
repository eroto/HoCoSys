
#include <stdint.h>

#ifndef COMPONENTS_RTC_INCLUDE_RTC_H_
#define COMPONENTS_RTC_INCLUDE_RTC_H_

#define SYNC_RETRIES 10
#define WAIT_CONN_RETRIES 20

//1 * 24 * 60 * 60
#define  SECONDS_DAY 86400
// 60 sec * 60 min
#define  SECONDS_HR 3600

 

typedef enum{
	SNTP_chk_connection,
	set_SNTP_time,
	wait_SNTP_synchronization,
	print_SNTP_time,
	SNTP_done,
	MAX_SNTP_states
}rtc_SNTP_states_t;


void rtc_sntp_SM(void);
void rtc_printLocTime(void);
int rtc_get_hour(void);
int rtc_get_min(void);
int rtc_get_sec(void);
int rtc_get_WeekDay(void);
int rtc_get_MonthDay(void);
int rtc_get_Month(void);
int rtc_get_Year(void);
uint64_t rtc_get_FutureDate_sec(time_t *future_time);
uint8_t splitDays(const char *input, char *days);

static void rtc_getCurrentTime(void);

#endif /* COMPONENTS_RTC_INCLUDE_RTC_H_ */