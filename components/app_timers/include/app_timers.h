/*
 * app_timer.h
 *
 *  Created on: Nov 30, 2024
 *      Author: enrique rodriguez toscano
 */
 
#ifndef APP_TIMER_H_
#define APP_TIMER_H_

#define NUM_TMRS_HDL	3
#define NUM_TMRS		3

enum {
	IRRIGATION_DURATION_TMR,
	IRRIGATION_TIME_TMR,
	IRRIGATION_PERIOD_TMR,
	IRRIGATION_MAX_TMR,
};

void app_timers_init(void);
static bool irrigation_duration_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx);


#endif /* APP_TIMER_H_ */



