
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "esp_blufi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "leds.h"
#include "init.h"


static const char *TAG = "App Main";

void app_main(void)
{

    esp_err_t ret;

    // Initialize NVS
    ESP_LOGI(TAG, "Initializing Flash");
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

	ESP_LOGI(TAG, "Calling Sysinit1()");
	sysinit1();

	ESP_LOGI(TAG, "Calling Sysinit2()");
	sysinit2();

	ESP_LOGI(TAG, "Calling Appinit1()");
	appinit1();

	ESP_LOGI(TAG, "Calling Appinit2()");
	appinit2();

	ESP_LOGI(TAG, "Scheduler shall start now!");

	/*
	 Unlike Vanilla FreeRTOS, users of FreeRTOS in ESP-IDF must never call
	 TaskStartScheduler() and vTaskEndScheduler(). Instead, ESP-IDF starts
	 FreeRTOS automatically. Users must define a void app_main(void) function
	 which acts as the entry point for user's application and is automatically
	 invoked on ESP-IDF startup.
	 */
}
