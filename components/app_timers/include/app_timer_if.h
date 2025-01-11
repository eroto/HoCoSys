/*
 * app_timer_if.h
 *
 *  Created on: Nov 30, 2024
 *      Author: enriq
 */

#ifndef APP_TIMER_IF_H_
#define APP_TIMER_IF_H_

#include "driver/gptimer.h"

extern bool Alarm_flag;

extern uint8_t app_timers_init(void);
extern uint8_t app_timer_start(gptimer_handle_t timer);
extern uint8_t start_irrigation_duration_tmr(void);
extern uint8_t stop_irrigation_duration_tmr(void);

#endif /* APP_TIMER_IF_H_ */
