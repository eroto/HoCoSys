#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "driver/gptimer.h"
#include "rtc_if.h"
#include "app_timers.h"
#include "driver/gptimer_etm.h"


#define HORAS (24u)
#define MINUTOS (60u)
#define SEGUNDOS (60u)

#define MONDAY		1
#define TUESDAY		2
#define WEDNESDAY	3
#define THURSDAY	4
#define FRIDAY		5
#define SATURDAY	6
#define SUNDAY		7



uint8_t days[7];
int irrigation_hr = 0;
int irrigation_min = 0;
uint8_t NumOfIrrigationDays = 0;

typedef struct 
{
	uint8_t b0 :1;
	uint8_t b1 :1;
	uint8_t b2 :1;
	uint8_t b3 :1;
	uint8_t b4 :1;
	uint8_t b5 :1;
	uint8_t b6 :1;
	uint8_t b7 :1;
}uin8_bits_t;

union
{
	uint8_t byte;
	uin8_bits_t irrigation_bits;
}irrigation_flags; 

gptimer_handle_t irrigation_timers_hdl[NUM_TMRS_HDL] ={[0 ... NUM_TMRS_HDL-1] = NULL};
//gptimer_handle_t irrigation_timer = NULL;


/*
gptimer_handle_t irrigation_duration = NULL;
gptimer_handle_t irrigation_timer = NULL;
gptimer_handle_t irrigation_period = NULL;
*/

gptimer_config_t irrigation_cfg = {
	.clk_src = GPTIMER_CLK_SRC_DEFAULT,
	.direction = GPTIMER_COUNT_UP,
	.resolution_hz = 1 * 1.5 * 1000, // 1.5 KHz
	.intr_priority = 0,
	//.backup_before_sleep = 0,
	};
	
gptimer_alarm_config_t alarm_config = {
    .reload_count = 0, // counter will reload with 0 on alarm event
    .alarm_count = 22500, // 1min @resolution 1500 Hz: 60/(1/1500)
    .flags.auto_reload_on_alarm = true, // enable auto-reload
};

typedef struct {
    uint64_t event_count;
} example_queue_element_t;


uint8_t app_timers_init(void)
{
	/*
	for(u_int16_t i=0; i<IRRIGATION_MAX_TMR; i++)
	{
		ESP_ERROR_CHECK(gptimer_new_timer( &irrigation_cfg, &irrigation_timers_hdl[i]));
	}
	*/
	
	for(uint8_t i = 0; i < 7; i++)
	{
		days[i]=' ';
	} 
	
	ESP_ERROR_CHECK(gptimer_new_timer( &irrigation_cfg, &irrigation_timers_hdl[IRRIGATION_DURATION_TMR]));
	
	ESP_ERROR_CHECK(gptimer_set_alarm_action(irrigation_timers_hdl[IRRIGATION_DURATION_TMR], &alarm_config));
	
	gptimer_event_callbacks_t cbs = {
    .on_alarm = irrigation_duration_alarm_cb, // register user callback
    };
	
	ESP_ERROR_CHECK(gptimer_register_event_callbacks(irrigation_timers_hdl[IRRIGATION_DURATION_TMR], &cbs, NULL));
	ESP_ERROR_CHECK(gptimer_enable(irrigation_timers_hdl[IRRIGATION_DURATION_TMR]));
	printf("irrigation timer initialized\n");
	
	//ESP_ERROR_CHECK(gptimer_start(irrigation_timers_hdl[IRRIGATION_DURATION_TMR]));
	
	return true;
}

static bool irrigation_duration_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{
    //BaseType_t high_task_awoken = pdFALSE;
    //QueueHandle_t queue = (QueueHandle_t)user_ctx;
    // Retrieve the count value from event data
    //example_queue_element_t ele = {
    //    .event_count = edata->count_value
    //};
    // Optional: send the event data to other task by OS queue
    // Do not introduce complex logics in callbacks
    // Suggest dealing with event data in the main loop, instead of in this callback
    //xQueueSendFromISR(queue, &ele, &high_task_awoken);
    // return whether we need to yield at the end of ISR
    //return high_task_awoken == pdTRUE;
    
    gptimer_stop(timer);
    Alarm_flag = 1;
    
    return 0;
}

uint8_t start_irrigation_duration_tmr(void)
{
	bool running;
	
	running = app_timer_is_running(irrigation_timers_hdl[IRRIGATION_DURATION_TMR]);
	
	if(running)
	{printf("Timer is running\n");}
	else
	{
		ESP_ERROR_CHECK(gptimer_start(irrigation_timers_hdl[IRRIGATION_DURATION_TMR]));
		printf("irrigation duration timer started\n");
	}
	
	return 0;
}

uint8_t stop_irrigation_duration_tmr(void)
{
	bool running;
	
	running = app_timer_is_running(irrigation_timers_hdl[IRRIGATION_DURATION_TMR]);
	
	if(running)
	{
		ESP_ERROR_CHECK(gptimer_stop(irrigation_timers_hdl[IRRIGATION_DURATION_TMR]));
		printf("irrigation duration timer stopped\n");
	}
	else
	{printf("Timer it's not running\n");}
	
	return 0;
}

bool app_timer_is_running(gptimer_handle_t app_timer)
{	
	uint64_t value = 255;
	bool running = 0;
	
	gptimer_get_raw_count(app_timer, &value);
	
	if (value > 0)
	{running = 1; }
	else
	{running = 0;}
	
	return running;
}


void SET_IrrigationSchedule(bool value)
{
	irrigation_flags.irrigation_bits.b0 = value;
}

bool GET_IrrigationSchedule(void)
{
	return irrigation_flags.irrigation_bits.b0;
}

void Set_NumOfIrrigationDays(uint8_t days)
{
	NumOfIrrigationDays = days;
}

uint8_t Get_NumOfIrrigationDays(void)
{
	return NumOfIrrigationDays;
}

uint8_t irrigation_task()
{	
	int C_weekDay = 0;
	int C_hr = 0;
	int C_min = 0;
	int C_seg = 0;
	int T_weekDay = 0;
	int T_hr = 0;
	int T_min = 0;
	int seconds_to_irrigation = 0;
	uint8_t i = 0;
	//int weekday = 0;
	
	if (GET_IrrigationSchedule())
	{
		/*get current day and time*/		 
		 C_weekDay = rtc_get_WeekDay();
		 C_hr = rtc_get_hour();
		 C_min = rtc_get_min();
		 C_seg = rtc_get_sec();
		 T_hr = irrigation_hr;
		 T_min = irrigation_min;
		 
		 for(i = 0; i < NumOfIrrigationDays; i++)
		 {
			 /*Get the irrigation request from the user*/
			 T_weekDay = days[i];
			
			 /*Irrigation day selected is the same as the currenty day*/
			 if(T_weekDay == C_weekDay)
			 {
				/*Hour selected to irritate is th same as the current hour*/
				if (T_hr == C_hr)
				{
					if(T_min > C_min)
					{
						/*Calculate seconds to irrigation*/
						seconds_to_irrigation = ((T_min - C_min)*SEGUNDOS)- C_seg;
					}
					else if(T_min == C_min)
					{seconds_to_irrigation = 1;}
					
					else
					{
						if(NumOfIrrigationDays == 1)
						{/*TODO: Do the calculus of seconds to irrigation*/}
					}
				}
					
				/*Hour selected to irritate is greater than current hour*/
				else if(T_hr > C_hr)
				{
					seconds_to_irrigation = ((((((T_hr - C_hr) * MINUTOS) - C_min) + T_min)*SEGUNDOS)- C_seg);
				}
				else
				{/*Target hour is less than current hour, that means time passed and next day shall
				be checked and time recalculated*/
					if(NumOfIrrigationDays == 1)
					{
						/*There is no more days to check, compute the seconds to irrigate*/
						seconds_to_irrigation = (((((((((7 - C_weekDay) + T_weekDay) * HORAS) - C_hr) + T_hr) * MINUTOS) - C_min) + T_min) * SEGUNDOS) - C_seg;
					}
					/* Go for to the next day programmed*/
				}
			 }
			 /*Irrigation day selected is less than current day*/
			 else if(T_weekDay < C_weekDay)
			 {
				if(NumOfIrrigationDays == 1)
				{
					/*There is no more days to check, compute the seconds to irrigate*/
					seconds_to_irrigation = (((((((((7 - C_weekDay) + T_weekDay) * HORAS) - C_hr) + T_hr) * MINUTOS) - C_min) + T_min) * SEGUNDOS) - C_seg;	
				}
			 	/* Go for the next day programmed*/
			 	
			 }
			 else/*Irrigation day selected is grater then current day*/
			 {
				seconds_to_irrigation = ((((((((T_weekDay - C_weekDay)* HORAS) - C_hr) + T_hr) * MINUTOS) - C_min) + T_min) * SEGUNDOS) - C_seg;
			 }
		 }
		
		printf("seconds_to_irrigation: %i", seconds_to_irrigation); 
	}
	else 
	{/* do nothing irrigation schedule not set*/}
	
	return 0;
	
}


uint8_t splitDays(const char *input)
{
	int j = 0;
	int i = 0;
	uint8_t count = 0;
	for (i = 0; input[i] != '\0'; i++)
	{
		/*There is not user input validity check "LMIJVSD"
		since the input will be limited by the UI input*/
		if (input[i] != ' ')
		{
			switch(input[i])
			{
				case 'L':
					days[j++]= MONDAY;
					count++;
				break;
				
				case 'M':
					days[j++]=TUESDAY;
					count++;
				break;
				
				case 'I':
					days[j++]=WEDNESDAY;
					count++;
				break;
				
				case 'J':
					days[j++]=THURSDAY;
					count++;
				break;
				
				case 'V':
					days[j++]= FRIDAY;
					count++;
				break;
				
				case 'S':
					days[j++]= SATURDAY;
					count++;
				break;
				
				case 'D':
					days[j++]= SUNDAY;
					count++;
				break;
				
				default:
					/*Do nothing*/
				break;
			}
		}
	}

	for(uint8_t i = 0; i < count; i++)
	{
		printf("Day %i: %i\n",i, days[i]);
	}
			
	return count;
}

void splitHrsMin(char * t_m)
{

	irrigation_hr = atoi(&t_m[0]);
	irrigation_min = atoi(&t_m[3]);
}

