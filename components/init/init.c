#include <stdio.h>
#include <stdlib.h>
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
#include "init.h"

//uint8_t s_led_state = 0;

 temperature_sensor_handle_t IntTempSensorHanlder = NULL;

uint8_t sysinit1(void)
{
	uint8_t result = 0;

	/*initialize Internal Temperature Sensor*/
	sense_init();

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
