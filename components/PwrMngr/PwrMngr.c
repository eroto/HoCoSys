#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <time.h>
//#include <sys/time.h>
#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"
//#include "driver/uart.h"
#include "esp_sleep.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_check.h"
#include "PwrMngr.h"

#define TIMER_WAKEUP_TIME_US    (2 * 1000 * 1000)
static const char *TAG = "timer_wakeup";

esp_err_t PwrMngr_register_timer_wakeup(void)
{
    ESP_RETURN_ON_ERROR(esp_sleep_enable_timer_wakeup(TIMER_WAKEUP_TIME_US), TAG, "Configure timer as wakeup source failed");
    ESP_LOGI(TAG, "timer wakeup source is ready");
    return ESP_OK;
}

void PwrMngr_Init(void)
{
	PwrMngr_register_timer_wakeup();
}

void PwrMngr_EnterLightSleepMode()
{
	/* Enter sleep mode */
        esp_light_sleep_start();
}