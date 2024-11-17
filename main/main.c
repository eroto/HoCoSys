
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
#include "xtensa/hal.h"


static const char *TAG = "App Main";

int32_t restart_counter = 0;

int32_t get_rstCtr(void)
{
	return restart_counter;
}

void app_main(void)
{
	// Initialize NVS
    ESP_LOGI(TAG, "Initializing Flash");
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    // Open
    printf("\nOpening Non-Volatile Storage (NVS) handle... ");
    nvs_handle_t NVS_handle;
    ret = nvs_open("storage", NVS_READWRITE, &NVS_handle);
    if (ret != ESP_OK)
    {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(ret));
    }
    else
    {
    	printf("Done\n");

        // Read
        printf("Reading restart counter from NVS ... ");
        restart_counter = 0; // value will default to 0, if not set yet in NVS
        ret = nvs_get_i32(NVS_handle, "restart_counter", &restart_counter);
        switch (ret)
        {
            case ESP_OK:
                printf("Done\n");
                printf("Restart counter = %" PRIu32 "\n", restart_counter);
                break;
                
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The value is not initialized yet!\n");
                break;
                
            default :
                printf("Error (%s) reading!\n", esp_err_to_name(ret));
    	}
    	
    	// Write
        printf("Updating restart counter in NVS ... ");
        restart_counter++;
        ret = nvs_set_i32(NVS_handle, "restart_counter", restart_counter);
        printf((ret != ESP_OK) ? "Failed!\n" : "Done\n");

        // Commit written value.
        // After setting any values, nvs_commit() must be called to ensure changes are written
        // to flash storage. Implementations may write to storage at other times,
        // but this is not guaranteed.
        printf("Committing updates in NVS ... ");
        ret = nvs_commit(NVS_handle);
        printf((ret != ESP_OK) ? "Failed!\n" : "Done\n");

        // Close
        nvs_close(NVS_handle);
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
