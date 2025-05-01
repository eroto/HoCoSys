/*
 * app_timer.h
 *
 *  Created on: Nov 30, 2024
 *      Author: enrique rodriguez toscano
 */
 
#ifndef APP_TIMERS_H_
#define APP_TIMERS_H_

#include <stdint.h>
#define NUM_TMRS_HDL	2
#define NUM_TMRS		2

typedef struct
{
	char *irrigation_days;
	char *irrigation_time;
	char *irrigation_duration;
}s_IrrigationInfo_t;

enum {
	IRRIGATION_DURATION_TMR,
	IRRIGATION_TIME_TMR,
	IRRIGATION_MAX_TMR
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
uint8_t start_irrigation_tmr(int seconds);
bool app_timer_is_running(gptimer_handle_t app_timer);
uint8_t stop_irrigation_duration_tmr(void);
static bool irrigation_duration_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx);
bool GET_IrrigationSchedule(void);
bool GET_IrrigationTimeCalc(void);
void SET_IrrigationSchedule(bool value);
void SET_IrrigationTimeCalc(bool value);
void app_timers_Fill_IrrigationArray(const char *input,uint8_t *IrrigationArray);
void app_timer_copy_UsrInput(s_IrrigationInfo_t* UserInfo);
void irrigation_task(void);
void splitHrsMin(char * t_m);
void Set_NumOfIrrigationDays(uint8_t days);
uint8_t Get_NumOfIrrigationDays(void);
bool GET_Irrigation_Alarm_flag(void);
void SET_Irrigation_Alarm_flag(bool value);
void app_timer_startIrrigationTask(void);


#endif /* APP_TIMERS_H_ */



