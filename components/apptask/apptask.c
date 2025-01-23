#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include "esp_system.h"
//#include "esp_console.h"
//#include "esp_vfs_dev.h"
//#include "esp_vfs_fat.h"
//#include "cmd_system.h"
//#include "cmd_wifi.h"
//#include "cmd_nvs.h"
#include "rtc_if.h"
#include "freertos/FreeRTOS.h"
#include "esp_task_wdt.h"
#include "driver/gpio.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gptimer.h"
#include "esp_log.h"
#include "linenoise/linenoise.h"
#include "leds.h"
#include "sense_if.h"
#include "I2C_if.h"
#include "esp_blufi_api.h"
#include "app_timer_if.h"
#include "blufi_if.h"
#include "apptask.h"
//#include "apptask_if.h"
//TaskHandle_t xHandle = NULL;

static const char *TAG = "AppTask";
#define PROMPT_STR CONFIG_IDF_TARGET
static float temp = 0;
static uint8_t count_1Sec = 0;
static uint8_t count_5Sec = 0;
int a = 0;
char UserInput = 0;
uint8_t trigger = 1;
QueueHandle_t xQueue;
ts_BLEMsg lReceivedValue;

typedef struct
{

}BLE_AppData;

//void apptask_init(void);

void apptask_init(void)
{
//	print_mux = xSemaphoreCreateMutex();
//static uint8_t ucParameterToPass = 45;

/* This I2C taks are used for testing purposes, the test uses the 2 ES32-S3 channels
 	the 2 channels ESPS32-S3 shall be connected phisically */
//	xTaskCreate(i2c_test_task, "i2c_test_task_0", 1024 * 2, (void *)0, 10, NULL);
//	xTaskCreate(i2c_test_task, "i2c_test_task_1", 1024 * 2, (void *)1, 10, NULL);

	xQueue = xQueueCreate( 2, sizeof(ts_BLEMsg) );

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



/*	xTaskCreatePinnedToCore(
							ConsoleTask,
							"ConsoleTask",
							TASK_STACK_SIZE,
							NULL,
							3,
							TaskHandleConsole,
							CORE0);*/
}

void apptask_5ms(void *pvParameters )
{
	TickType_t xLastWakeTime = 0;
	BaseType_t xStatus = 0;
	const TickType_t xPeriod_5 = pdMS_TO_TICKS(TASK_PERIOD_5);
	//const TickType_t xTicksToWait = pdMS_TO_TICKS(5);

		xLastWakeTime = xTaskGetTickCount();
	for( ;; )
	{
		vTaskDelayUntil( &xLastWakeTime, xPeriod_5 );
		/* Task code goes here. */
		if( uxQueueMessagesWaiting( xQueue ) != 0 )
		{
			ESP_LOGI(TAG, "Queue should have been empty!\r\n");
			xStatus = xQueueReceive( xQueue, &lReceivedValue, 5);
			if(xStatus)
			{
				//ESP_LOGI(TAG, "val of ReceivedValue[0]:%i",lReceivedValue.data[0]);
				ESP_LOG_BUFFER_HEX("Received ", lReceivedValue.data, lReceivedValue.data_len);
				ESP_LOGI(TAG, "length of ReceivedValue: %" PRIu32 "\n",lReceivedValue.data_len);
			}
		}
		/*Irrigation timer alarm flag*/
		if (Alarm_flag == 1)
		{
			Alarm_flag = 0;
			printf("Irrigation Alarm completed\n");
		}

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
		/* Task code goes here. */
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
		/* Task code goes here. */
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
		/* Task code goes here. */
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
		/* Task code goes here. */

	}
}

uint8_t level = 0;
uint8_t State = 0;
uint8_t Next_state = 0;


void apptask_500ms(void *pvParameters )
{

	TickType_t xLastWakeTime;
	const TickType_t xPeriod_500 = pdMS_TO_TICKS(TASK_PERIOD_500);

		xLastWakeTime = xTaskGetTickCount();

		//esp_task_wdt_add(TaskHandle500ms);

		for(;;)
		{
			led_ConnIndicator();
			
			irrigation_task();

			if (count_1Sec == 2)
			{
				rtc_sntp_SM();

				if(level){
					level = 0;
				}
				else{
					level = 1;
				}

				//gpio_set_level(DO_00, level);
				count_1Sec = 0;
			}else{}
			
			if(count_5Sec == 10){
				temp = sense_getIntTempCelcius();
				//ESP_LOGI(TAG, "Temperature value %.02f ℃",temp);

				 //= gettimeofday()
				 count_5Sec = 0;
			}
			else{
			}

			count_1Sec++;
			count_5Sec++;

			vTaskDelayUntil( &xLastWakeTime, xPeriod_500 );
		}
}
