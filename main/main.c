
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "init.h"


static const char *TAG = "App Main";



void app_main(void)
{
	ESP_LOGI(TAG, "Calling Sysinit1()");
	sysinit1();
	ESP_LOGI(TAG, "Calling Appinit1()");
	appinit1();
	ESP_LOGI(TAG, "Calling Appinit2()");
	appinit2();
	ESP_LOGI(TAG, "Calling Sysinit2()");
	sysinit2();

	ESP_LOGI(TAG, "Scheduler shall start now!");
    //while (1) {
        /* Toggle the LED state */
        //s_led_state = !s_led_state;
        //vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        //sleep()1
    //}
}
