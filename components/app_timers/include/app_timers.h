/*
 * app_timer.h
 *
 *  Created on: Nov 30, 2024
 *      Author: enrique rodriguez toscano
 */
 
#ifndef APP_TIMERS_H_
#define APP_TIMERS_H_

#define NUM_TMRS_HDL	3
#define NUM_TMRS		3

bool Alarm_flag = 0;

enum {
	IRRIGATION_DURATION_TMR,
	IRRIGATION_TIME_TMR,
	IRRIGATION_PERIOD_TMR,
	IRRIGATION_MAX_TMR,
};

typedef enum {
    TIMER_INIT,        // Timer is initialized, but not enabled
    TIMER_ENABLE,      // Timer is enabled, but is not running
    TIMER_ENABLE_WAIT, // Timer is in the middle of the enable process (Intermediate state)
    TIMER_RUN,         // Timer is in running
    TIMER_RUN_WAIT,    // Timer is in the middle of the run process (Intermediate state)
} timer_status_t;

uint8_t app_timers_init(void);
uint8_t start_irrigation_duration_tmr(void);
bool app_timer_is_running(gptimer_handle_t app_timer);
uint8_t stop_irrigation_duration_tmr(void);
static bool irrigation_duration_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx);
bool GET_IrrigationSchedule(void);
void SET_IrrigationSchedule(bool value);
uint8_t splitDays(const char *input);
uint8_t irrigation_task();
void splitHrsMin(char * t_m);
void Set_NumOfIrrigationDays(uint8_t days);
uint8_t Get_NumOfIrrigationDays(void);


#endif /* APP_TIMERS_H_ */



