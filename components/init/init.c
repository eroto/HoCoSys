#include <stdio.h>
#include <stdlib.h>
#include "esp_log.h"
//#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <inttypes.h>
#include <stdbool.h>
#include <unistd.h>
#include <inttypes.h>
#include "driver/temperature_sensor.h"
#include "leds.h"
#include "apptask_if.h"
#include "sense_if.h"
#include "I2C_if.h"
#include "init.h"

//uint8_t s_led_state = 0;


static const char *TAG = "InitModule";

temperature_sensor_handle_t IntTempSensorHanlder = NULL;

uint8_t sysinit1(void)
{

	esp_err_t result =  ESP_FAIL;

	/*initialize Internal Temperature Sensor*/
	result = sense_init();

	/*Initialize I2C Master*/
	result = i2c_master_init();

	if (result != ESP_OK)
		{
			ESP_LOGI(TAG, "i2c_master_init Initialization Error");
			return result;
		}

	result = i2c_slave_init();

	if (result != ESP_OK)
	{
		ESP_LOGI(TAG, "i2c_slave_init Initialization Error");
		return result;
	}

return result;
}


uint8_t sysinit2(void)
{
	//Allow other core to finish initialization
	//vTaskDelay(pdMS_TO_TICKS(100));

	uint8_t result = 0;

return result;
}

uint8_t appinit1(void)
{
	uint8_t result = 0;
	/* Configure the peripheral according to the LED type */
	configure_led();

	return result;
}

uint8_t appinit2(void)
{

	apptask_init();

	uint8_t result = 0;

return result;
}
