

#ifndef COMPONENTS_RTC_INCLUDE_RTC_H_
#define COMPONENTS_RTC_INCLUDE_RTC_H_

#define SYNC_RETRIES 10

typedef enum{
	set_SNTP_time,
	wait_SNTP_synchronization,
	print_SNTP_time,
	SNTP_done,
	MAX_SNTP_states
}rtc_SNTP_states_t;


#endif /* COMPONENTS_RTC_INCLUDE_RTC_H_ */