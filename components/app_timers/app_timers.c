#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include "freertos/FreeRTOS.h"
#include "driver/gptimer.h"
#include "app_timers.h"
#include "driver/gptimer_etm.h"

gptimer_handle_t irrigation_timers_hdl[NUM_TMRS_HDL] ={[0 ... NUM_TMRS_HDL-1] = NULL};


/*
gptimer_handle_t irrigation_duration = NULL;
gptimer_handle_t irrigation_timer = NULL;
gptimer_handle_t irrigation_period = NULL;
*/

gptimer_config_t irrigation_cfg = {
	.clk_src = GPTIMER_CLK_SRC_DEFAULT,
	.direction = GPTIMER_COUNT_UP,
	.resolution_hz = 1 * 0.05 * 1000, //50Hz
	.intr_priority = 0,
	//.backup_before_sleep = 0,
	};
	
gptimer_alarm_config_t alarm_config = {
    .reload_count = 0, // counter will reload with 0 on alarm event
    .alarm_count = 1200, // 1min @resolution 50 Hz: 60/(1/50) = 0.02
    .flags.auto_reload_on_alarm = true, // enable auto-reload
};

typedef struct {
    uint64_t event_count;
} example_queue_element_t;


void app_timers_init(void)
{
/*
	for(u_int16_t i=0; i<IRRIGATION_MAX_TMR; i++)
	{
		ESP_ERROR_CHECK(gptimer_new_timer( &irrigation_cfg, &irrigation_timers_hdl[i]));
	}
	
	ESP_ERROR_CHECK(gptimer_set_alarm_action(irrigation_timers_hdl[IRRIGATION_PERIOD_TMR], &alarm_config));
	
	gptimer_event_callbacks_t cbs = {
    .on_alarm = irrigation_duration_alarm_cb, // register user callback
    };
	
	ESP_ERROR_CHECK(gptimer_register_event_callbacks(irrigation_timers_hdl[IRRIGATION_PERIOD_TMR], &cbs, NULL));
	ESP_ERROR_CHECK(gptimer_enable(irrigation_timers_hdl[IRRIGATION_PERIOD_TMR]));
	ESP_ERROR_CHECK(gptimer_start(irrigation_timers_hdl[IRRIGATION_PERIOD_TMR]));
*/	
}

static bool irrigation_duration_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{
    BaseType_t high_task_awoken = pdFALSE;
//    QueueHandle_t queue = (QueueHandle_t)user_ctx;
    // Retrieve the count value from event data
    example_queue_element_t ele = {
        .event_count = edata->count_value
    };
    // Optional: send the event data to other task by OS queue
    // Do not introduce complex logics in callbacks
    // Suggest dealing with event data in the main loop, instead of in this callback
//    xQueueSendFromISR(queue, &ele, &high_task_awoken);
    // return whether we need to yield at the end of ISR
    return high_task_awoken == pdTRUE;
}
