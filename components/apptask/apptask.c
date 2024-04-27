#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "esp_task_wdt.h"
#include "driver/gpio.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "leds.h"
#include "sense_if.h"
#include "apptask_if.h"
#include "I2C_if.h"
#include "esp_blufi_api.h"
#include "blufi_if.h"
#include "apptask.h"
//TaskHandle_t xHandle = NULL;

static const char *TAG = "AppTask";
static float temp = 0;
static uint8_t count_1Sec = 0;
static uint8_t count_5Sec = 0;
int a = 0;
char UserInput = 0;
uint8_t trigger = 1;

void apptask_init(void)
{
//	print_mux = xSemaphoreCreateMutex();
//static uint8_t ucParameterToPass = 45;

/* This IcX taks are used for testing purposes, the test uses the 2 ES32-S3 channels
 	the 2 channels ESPS32-S3 shall be connected phisically */
//	xTaskCreate(i2c_test_task, "i2c_test_task_0", 1024 * 2, (void *)0, 10, NULL);
//	xTaskCreate(i2c_test_task, "i2c_test_task_1", 1024 * 2, (void *)1, 10, NULL);

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

		for(;;){
			LedColorRGB_t Color;
			if(blufi_GetBLEConStatus())
			{
				Color.RedC = 5;
				Color.BlueC = 125;
				Color.GreenC =  10;
			}
			else if(blufi_GetWiFiConStatus())
			{
				Color.RedC = 5;
				Color.BlueC = 5;
				Color.GreenC = 128;
			}
			else
			{
				Color.RedC = 128;
				Color.BlueC = 0;
				Color.GreenC =  0;
			}

			blink_led(Color);
			//s_led_state = !s_led_state;

			if (count_1Sec == 2)
			{
				switch(State){
				case 0:
					ESP_LOGI(TAG, "Enter IP address last number");
					Next_state = 1;
					break;

				case 1:
					UserInput = fgetc(stdin);
					if (UserInput!=0xFF)
					{
						fputc(UserInput, stdout);
						Next_state = 2;
					}
					else{
						Next_state = 1;
					}

					/*scanf("enter a number %d",&a);
					ESP_LOGI(TAG, "Number, entered %d",a);

					if(a < 255 && a >1){
						Next_state = 2;
						ESP_LOGI(TAG, "User Input: %d.",a);
					}
					else {Next_state = 1;}
					*/
				break;
				case 2:
					if(UserInput < 255 && UserInput>1){
						Next_state = 3;
						//esp_task_wdt_reset();
					}
					else if(UserInput == 0){
						Next_state = 1;
					}
					else{
						ESP_LOGI(TAG, "%d is not a number. Enter A number between2 and 255",UserInput);
						Next_state = 0;
					}
					break;
				case 3:
					ESP_LOGI(TAG, "Entered IP was : %d",UserInput);
					ESP_LOGI(TAG, "IP Address to use 192.168.0.%d",UserInput);
					Next_state = 4;
					break;
				case 4:
					if(blufi_GetBLEConStatus() == false){
						ESP_LOGI(TAG, "Open BlueFi App. to connect to BlueTooth with the device!");
						Next_state = 4;
					}
					else{
						Next_state = 5;
					}
					break;
				case 5:
					if(blufi_GetBLEConStatus() == true && blufi_GetWiFiConStatus()== false){
						ESP_LOGI(TAG, "Click >>Connect<< on the BlueFi App. and select your WiFi Network");
						Next_state = 5;
					}
					else{
						ESP_LOGI(TAG, "WiFi Network Connected");
						Next_state = 6;
					}
					break;
				default:
					Next_state = 4;
					break;
				}
				State = Next_state;

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
				ESP_LOGI(TAG, "Temperature value %.02f ℃",temp);
				count_5Sec = 0;
			}
			else{
			}

			count_1Sec++;
			count_5Sec++;

			vTaskDelayUntil( &xLastWakeTime, xPeriod_500 );
		}
}
