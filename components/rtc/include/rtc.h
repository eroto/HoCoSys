

void rtc_set_sntp(void);

typedef enum{
	set_SNTP_time,
	wait_SNTP_synchronization,
	print_SNTP_time,
	SNTP_done,
	MAX_SNTP_states
}rtc_SNTP_states_t;
