#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "driver/gptimer.h"
#include "app_timers.h"
#include "driver/gptimer_etm.h"

gptimer_handle_t irrigation_timers_hdl[NUM_TMRS_HDL] ={[0 ... NUM_TMRS_HDL-1] = NULL};
//gptimer_handle_t irrigation_timer = NULL;


/*
gptimer_handle_t irrigation_duration = NULL;
gptimer_handle_t irrigation_timer = NULL;
gptimer_handle_t irrigation_period = NULL;
*/

gptimer_config_t irrigation_cfg = {
	.clk_src = GPTIMER_CLK_SRC_DEFAULT,
	.direction = GPTIMER_COUNT_UP,
	.resolution_hz = 1 * 1.5 * 1000, // 1.5 KHz
	.intr_priority = 0,
	//.backup_before_sleep = 0,
	};
	
gptimer_alarm_config_t alarm_config = {
    .reload_count = 0, // counter will reload with 0 on alarm event
    .alarm_count = 22500, // 1min @resolution 1500 Hz: 60/(1/1500)
    .flags.auto_reload_on_alarm = true, // enable auto-reload
};

typedef struct {
    uint64_t event_count;
} example_queue_element_t;


uint8_t app_timers_init(void)
{
	/*
	for(u_int16_t i=0; i<IRRIGATION_MAX_TMR; i++)
	{
		ESP_ERROR_CHECK(gptimer_new_timer( &irrigation_cfg, &irrigation_timers_hdl[i]));
	}
	*/
	
	ESP_ERROR_CHECK(gptimer_new_timer( &irrigation_cfg, &irrigation_timers_hdl[IRRIGATION_DURATION_TMR]));
	
	ESP_ERROR_CHECK(gptimer_set_alarm_action(irrigation_timers_hdl[IRRIGATION_DURATION_TMR], &alarm_config));
	
	gptimer_event_callbacks_t cbs = {
    .on_alarm = irrigation_duration_alarm_cb, // register user callback
    };
	
	ESP_ERROR_CHECK(gptimer_register_event_callbacks(irrigation_timers_hdl[IRRIGATION_DURATION_TMR], &cbs, NULL));
	ESP_ERROR_CHECK(gptimer_enable(irrigation_timers_hdl[IRRIGATION_DURATION_TMR]));
	printf("irrigation timer initialized\n");
	
	//ESP_ERROR_CHECK(gptimer_start(irrigation_timers_hdl[IRRIGATION_DURATION_TMR]));
	
	return true;
}

static bool irrigation_duration_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{
    //BaseType_t high_task_awoken = pdFALSE;
    //QueueHandle_t queue = (QueueHandle_t)user_ctx;
    // Retrieve the count value from event data
    //example_queue_element_t ele = {
    //    .event_count = edata->count_value
    //};
    // Optional: send the event data to other task by OS queue
    // Do not introduce complex logics in callbacks
    // Suggest dealing with event data in the main loop, instead of in this callback
    //xQueueSendFromISR(queue, &ele, &high_task_awoken);
    // return whether we need to yield at the end of ISR
    //return high_task_awoken == pdTRUE;
    
    gptimer_stop(timer);
    Alarm_flag = 1;
    
    return 0;
}

uint8_t start_irrigation_duration_tmr(void)
{
	bool running;
	
	running = app_timer_is_running(irrigation_timers_hdl[IRRIGATION_DURATION_TMR]);
	
	if(running)
	{printf("Timer is running\n");}
	else
	{
		ESP_ERROR_CHECK(gptimer_start(irrigation_timers_hdl[IRRIGATION_DURATION_TMR]));
		printf("irrigation duration timer started\n");
	}
	
	return 0;
}

uint8_t stop_irrigation_duration_tmr(void)
{
	bool running;
	
	running = app_timer_is_running(irrigation_timers_hdl[IRRIGATION_DURATION_TMR]);
	
	if(running)
	{
		ESP_ERROR_CHECK(gptimer_stop(irrigation_timers_hdl[IRRIGATION_DURATION_TMR]));
		printf("irrigation duration timer stopped\n");
	}
	else
	{printf("Timer it's not running\n");}
	
	return 0;
}

bool app_timer_is_running(gptimer_handle_t app_timer)
{	
	uint64_t value = 255;
	bool running = 0;
	
	gptimer_get_raw_count(app_timer, &value);
	
	if (value > 0)
	{running = 1; }
	else
	{running = 0;}
	
	return running;
}


