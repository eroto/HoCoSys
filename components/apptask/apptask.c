#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "leds.h"
#include "apptask_if.h"
#include "sense_if.h"
#include "apptask.h"
//TaskHandle_t xHandle = NULL;

static const char *TAG = "AppTask";
static float temp = 0;
static uint8_t count_1Sec = 0;
static uint8_t count_5Sec = 0;

void apptask_init(void)
{

//static uint8_t ucParameterToPass = 45;

	TaskHandle5ms = xTaskCreateStaticPinnedToCore(
								  apptask_5ms,
								  "AppTask5ms",
								  TASK_STACK_SIZE,
								  NULL,
								  7,
								  xStack5msTask,
								  &xTask5Buffer,
								  CORE1);

	TaskHandle10ms = xTaskCreateStaticPinnedToCore(
								  apptask_10ms,
								  "AppTask10ms",
								  TASK_STACK_SIZE,
								  NULL,
								  6,
								  xStack10msTask,
								  &xTask10Buffer,
								  CORE1);

	TaskHandle20ms = xTaskCreateStaticPinnedToCore(
								  apptask_20ms,
								  "AppTask20ms",
								  TASK_STACK_SIZE,
								  NULL,
								  5,
								  xStack20msTask,
								  &xTask20Buffer,
								  CORE1);

	TaskHandle50ms = xTaskCreateStaticPinnedToCore(
								  apptask_50ms,
								  "AppTask50ms",
								  TASK_STACK_SIZE,
								  NULL,
								  4,
								  xStack50msTask,
								  &xTask50Buffer,
								  CORE1);

	TaskHandle100ms = xTaskCreateStaticPinnedToCore(
								  apptask_100ms,
								  "AppTask100ms",
								  TASK_STACK_SIZE,
								  NULL,
								  3,
								  xStack100msTask,
								  &xTask100Buffer,
								  CORE1);

	TaskHandle500ms = xTaskCreateStaticPinnedToCore(
								  apptask_500ms,
								  "AppTask500ms",
								  TASK_STACK_SIZE,
								  NULL,
								  3,
								  xStack500msTask,
								  &xTask500Buffer,
								  CORE1);

}

void apptask_5ms(void *pvParameters )
{
	TickType_t xLastWakeTime = 0;
	const TickType_t xPeriod_5 = pdMS_TO_TICKS(TASK_PERIOD_5);

		xLastWakeTime = xTaskGetTickCount();
	for( ;; )
	{
		vTaskDelayUntil( &xLastWakeTime, xPeriod_5 );
		/* Task code goes here. */

	}
}

void apptask_10ms(void *pvParameters )
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod_10 = pdMS_TO_TICKS(TASK_PERIOD_10);

	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		vTaskDelayUntil( &xLastWakeTime, xPeriod_10 );

	}

}

void apptask_20ms(void *pvParameters )
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod_20 = pdMS_TO_TICKS(TASK_PERIOD_20);

	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		vTaskDelayUntil( &xLastWakeTime, xPeriod_20 );

	}

}

void apptask_50ms(void *pvParameters )
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod_50 = pdMS_TO_TICKS(TASK_PERIOD_50);

	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		vTaskDelayUntil( &xLastWakeTime, xPeriod_50 );

	}

}

void apptask_100ms(void *pvParameters )
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod_100 = pdMS_TO_TICKS(TASK_PERIOD_100);

	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		vTaskDelayUntil( &xLastWakeTime, xPeriod_100 );


	}
}

uint8_t level = 0;

void apptask_500ms(void *pvParameters )
{

	TickType_t xLastWakeTime;
	const TickType_t xPeriod_500 = pdMS_TO_TICKS(TASK_PERIOD_500);

		xLastWakeTime = xTaskGetTickCount();

		for(;;)
		{
			vTaskDelayUntil( &xLastWakeTime, xPeriod_500 );
			blink_led();
			s_led_state = !s_led_state;

			if (count_1Sec == 2)
			{
				if(level)
				{level = 0;}
				else
				{level = 1;}

				gpio_set_level(DO_00, level);
				count_1Sec = 0;
			}
			else if(count_5Sec == 10)
			{
				temp = sense_getIntTempCelcius();
				ESP_LOGI(TAG, "Temperature value %.02f ℃",temp);
				count_5Sec = 0;
			}
			else
			{
			}

			count_1Sec++;
			count_5Sec++;
		}
}
