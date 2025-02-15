#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "esp_task_wdt.h"
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

#define IRRIGATION_TASK_STACK_SIZE	3548


uint8_t days[7];
int irrigation_hr = 0;
int irrigation_min = 0;
uint8_t NumOfIrrigationDays = 0;
bool Irrigation_Alarm_flag;
TaskHandle_t HandlerIrrigationTask = NULL;
s_IrrigationInfo_t IrrigationInfoReceived;

#define APP_TIMER_GETget(val) 

bool irrigation_duration_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx);

uint8_t Days_to_Irrigate[7]={0,0,0,0,0,0,0};

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

typedef enum
{
	WAITING_USER_INPUT,
	CALC_SECONDS,
	SET_TIMER,
	IDLE,
	MAX_IRR_STATE
}e_irrigation_states;

union
{
	uint8_t byte;
	uin8_bits_t irrigation_bits;
}irrigation_flags; 

e_irrigation_states irrigation_task_states = WAITING_USER_INPUT;

gptimer_config_t irrigation_cfg = {
	.clk_src = GPTIMER_CLK_SRC_DEFAULT,
	.direction = GPTIMER_COUNT_UP,
	.resolution_hz = 1 * 1.5 * 1000, // 1.5 KHz
	.intr_priority = 0,
	//.backup_before_sleep = 0,
	};
	
gptimer_alarm_config_t alarm_config_duration = {
    .reload_count = 0, // counter will reload with 0 on alarm event
    .alarm_count = 90000, // 1min @resolution 1500 Hz: 60/(1/1500)
    .flags.auto_reload_on_alarm = true, // enable auto-reload
};

gptimer_alarm_config_t alarm_config_time = {
    .reload_count = 0, // counter will reload with 0 on alarm event
    .alarm_count = 90000, // 1min @resolution 1500 Hz: 60/(1/1500)
    .flags.auto_reload_on_alarm = true, // enable auto-reload
};

gptimer_handle_t irrigation_timers_hdl[NUM_TMRS_HDL] ={[0 ... NUM_TMRS_HDL-1] = NULL};
gptimer_alarm_config_t *irrigation_alarm_cfgs[NUM_TMRS] = {&alarm_config_duration, &alarm_config_time};

/*
gptimer_handle_t irrigation_duration = NULL;
gptimer_handle_t irrigation_timer = NULL;
gptimer_handle_t irrigation_period = NULL;
*/


typedef struct {
    uint64_t event_count;
} example_queue_element_t;


uint8_t app_timers_init(void)
{
	
	uint8_t i=0;
	
	for(uint8_t i = 0; i < 7; i++)
	{
		days[i]=' ';
	} 
	
	gptimer_event_callbacks_t cbs = {
    .on_alarm = irrigation_duration_alarm_cb, // register user callback
    };
	
	
	for(i=0; i<IRRIGATION_MAX_TMR; i++)
	{
		ESP_ERROR_CHECK(gptimer_new_timer( &irrigation_cfg, &irrigation_timers_hdl[i]));
		ESP_ERROR_CHECK(gptimer_set_alarm_action(irrigation_timers_hdl[i], irrigation_alarm_cfgs[i]));
		ESP_ERROR_CHECK(gptimer_register_event_callbacks(irrigation_timers_hdl[i], &cbs, NULL));
		ESP_ERROR_CHECK(gptimer_enable(irrigation_timers_hdl[i]));
	}
	
	/*
	ESP_ERROR_CHECK(gptimer_new_timer( &irrigation_cfg, &irrigation_timers_hdl[IRRIGATION_DURATION_TMR]));
	
	ESP_ERROR_CHECK(gptimer_set_alarm_action(irrigation_timers_hdl[IRRIGATION_DURATION_TMR], &alarm_config_duration));
	
	ESP_ERROR_CHECK(gptimer_register_event_callbacks(irrigation_timers_hdl[IRRIGATION_DURATION_TMR], &cbs, NULL));
	ESP_ERROR_CHECK(gptimer_enable(irrigation_timers_hdl[IRRIGATION_DURATION_TMR]));
	*/
	printf("irrigation timers initialized\n");
	
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
    Irrigation_Alarm_flag = 1;
    
    return 0;
}


uint8_t start_irrigation_tmr(int time)
{
	bool running;
	
	running = app_timer_is_running(irrigation_timers_hdl[IRRIGATION_TIME_TMR]);
	
	if(running)
	{printf("Timer is running\n");}
	else
	{
		alarm_config_time.alarm_count = time; 
		ESP_ERROR_CHECK(gptimer_set_alarm_action(irrigation_timers_hdl[IRRIGATION_TIME_TMR], &alarm_config_time));
		ESP_ERROR_CHECK(gptimer_start(irrigation_timers_hdl[IRRIGATION_TIME_TMR]));
		printf("irrigation timer started\n");
	}
	
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
		alarm_config_duration.alarm_count = 25000; 
		ESP_ERROR_CHECK(gptimer_set_alarm_action(irrigation_timers_hdl[IRRIGATION_DURATION_TMR], &alarm_config_duration));
		ESP_ERROR_CHECK(gptimer_start(irrigation_timers_hdl[IRRIGATION_DURATION_TMR]));
		printf("irrigation duration started\n");
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


void SET_IrrigationTimer()
{
	
}

void SET_IrrigationSchedule(bool value)
{
	irrigation_flags.irrigation_bits.b0 = value;
}

void SET_IrrigationTimeCalc(bool value)
{
	irrigation_flags.irrigation_bits.b1 = value;
}

bool GET_IrrigationSchedule(void)
{
	return irrigation_flags.irrigation_bits.b0;
}

bool GET_IrrigationTimeCalc(void)
{
	return irrigation_flags.irrigation_bits.b1;
}

void Set_NumOfIrrigationDays(uint8_t days)
{
	NumOfIrrigationDays = days;
}

uint8_t Get_NumOfIrrigationDays(void)
{
	return NumOfIrrigationDays;
}


void irrigation_task(void* pvParameters )
{	
	static int C_weekDay = 0;
	static int C_hr = 0;
	static int C_min = 0;
	static int C_seg = 0;
	static int T_weekDay = 0;
	static int T_hr = 0;
	static int T_min = 0;
	static int seconds_to_irrigation = 0;
	uint8_t num_of_days = 0;
	int time = 0;
	uint8_t i = 0;
	int duration = 0;
	e_irrigation_states nextstate = MAX_IRR_STATE;
	//int weekday = 0;
	
	while(1)
	{
		//esp_task_wdt_reset();
		
	switch(irrigation_task_states)
	{
	case WAITING_USER_INPUT:
		//esp_task_wdt_reset();
		if (GET_IrrigationSchedule())
		{
			/*get current day and time*/
			printf("Get Current day and time\n");		 
			 C_weekDay = rtc_get_WeekDay();
			 C_hr = rtc_get_hour();
			 C_min = rtc_get_min();
			 C_seg = rtc_get_sec();
			 T_hr = irrigation_hr;
			 T_min = irrigation_min;
			 
			 num_of_days = app_timers_ConvertDays(IrrigationInfoReceived.irrigation_days);
			 Set_NumOfIrrigationDays(num_of_days);
			 splitHrsMin(IrrigationInfoReceived.irrigation_time);
			 duration = atoi(IrrigationInfoReceived.irrigation_duration);		 
			 nextstate = CALC_SECONDS;
		 }
		 else
		 {
			printf("Waiting Get Current day and time\n");
			nextstate = WAITING_USER_INPUT;
		 }
	break;
	
	case CALC_SECONDS:
		for(i = 0; i < NumOfIrrigationDays; i++)
		 {
			 printf("Calculating seconds, loop:%i\n",i);
			 esp_task_wdt_reset();
			 T_weekDay = days[i];/*Get the irrigation Target day from the user*/
			
			 /*Irrigation day selected is the same as the currenty day*/
			 if(T_weekDay == C_weekDay)
			 {
				if (T_hr == C_hr)/*Hour selected to irrigate is the same as the current hour*/
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
						if(NumOfIrrigationDays == 1) /*Are there more days to check?*/
						{seconds_to_irrigation = (((((((((7 - C_weekDay) + T_weekDay) * HORAS) - C_hr) + T_hr) * MINUTOS) - C_min) + T_min) * SEGUNDOS) - C_seg;}
						/* else Go for to the next day programmed*/
					}
				}
				else if(T_hr > C_hr)/*Hour selected to irritate is greater than current hour*/
				{
					seconds_to_irrigation = ((((((T_hr - C_hr) * MINUTOS) - C_min) + T_min)*SEGUNDOS)- C_seg);
				}
				else/*Target hour is less than current hour, that means time passed and next day shall be checked and time recalculated*/
				{
					if(NumOfIrrigationDays == 1)/*Are there more days to check?*/
					{
						seconds_to_irrigation = (((((((((7 - C_weekDay) + T_weekDay) * HORAS) - C_hr) + T_hr) * MINUTOS) - C_min) + T_min) * SEGUNDOS) - C_seg;
					}
					/* Else Go for to the next day programmed*/
				}
			 }
			 /*Irrigation day selected is less than current day*/
			 else if(T_weekDay < C_weekDay)
			 {
				if(NumOfIrrigationDays == 1)/*Are there more days to check?*/
				{seconds_to_irrigation = (((((((((7 - C_weekDay) + T_weekDay) * HORAS) - C_hr) + T_hr) * MINUTOS) - C_min) + T_min) * SEGUNDOS) - C_seg;}
			 	/*else Go for the next day programmed*/
			 }
			 else/*Irrigation day selected is grater then current day*/
			 {
				seconds_to_irrigation = ((((((((T_weekDay - C_weekDay)* HORAS) - C_hr) + T_hr) * MINUTOS) - C_min) + T_min) * SEGUNDOS) - C_seg;
			 }
		 }
		
		printf("seconds_to_irrigation: %i\n", seconds_to_irrigation);
		SET_IrrigationTimeCalc(true); 
		nextstate = SET_TIMER;
	break;
	
	case SET_TIMER:
	time = seconds_to_irrigation * 1500;  
	start_irrigation_tmr(time);
	SET_IrrigationTimeCalc(true); 
	SET_IrrigationSchedule(false);
	nextstate = IDLE;
	printf("SET_TIMER: Irrigation timer set to:%i",time);
	break;
	
	case IDLE:
	if(GET_IrrigationTimeCalc())
	{nextstate = IDLE;}
	else
	{nextstate = WAITING_USER_INPUT;}
	printf("IDLE Deleting Task");
	vTaskDelete(HandlerIrrigationTask);
	break;
	
	case MAX_IRR_STATE:
	printf("MAX_IRR_STATE Deleting Task");
	vTaskDelete(HandlerIrrigationTask);
	break;
	
	default:
	printf("Default Deleting Task");
	vTaskDelete(HandlerIrrigationTask);
	break;
	}
	
	irrigation_task_states = nextstate;
	}
	/*
	if (GET_IrrigationSchedule())
	{
		//get current day and time		 
		 C_weekDay = rtc_get_WeekDay();
		 C_hr = rtc_get_hour();
		 C_min = rtc_get_min();
		 C_seg = rtc_get_sec();
		 T_hr = irrigation_hr;
		 T_min = irrigation_min;
		 
		 for(i = 0; i < NumOfIrrigationDays; i++)
		 {
			 T_weekDay = days[i];//Get the irrigation Target day from the user
			
			 //Irrigation day selected is the same as the currenty day
			 if(T_weekDay == C_weekDay)
			 {
				if (T_hr == C_hr)//Hour selected to irrigate is the same as the current hour
				{
					if(T_min > C_min)
					{
						//Calculate seconds to irrigation
						seconds_to_irrigation = ((T_min - C_min)*SEGUNDOS)- C_seg;
					}
					else if(T_min == C_min)
					{seconds_to_irrigation = 1;}
					
					else
					{
						if(NumOfIrrigationDays == 1) //Are there more days to check?
						{seconds_to_irrigation = (((((((((7 - C_weekDay) + T_weekDay) * HORAS) - C_hr) + T_hr) * MINUTOS) - C_min) + T_min) * SEGUNDOS) - C_seg;}
						//else Go for to the next day programmed
					}
				}
				else if(T_hr > C_hr)//Hour selected to irritate is greater than current hour
				{
					seconds_to_irrigation = ((((((T_hr - C_hr) * MINUTOS) - C_min) + T_min)*SEGUNDOS)- C_seg);
				}
				else//Target hour is less than current hour, that means time passed and next day shall be checked and time recalculated
				{
					if(NumOfIrrigationDays == 1)//Are there more days to check?
					{
						seconds_to_irrigation = (((((((((7 - C_weekDay) + T_weekDay) * HORAS) - C_hr) + T_hr) * MINUTOS) - C_min) + T_min) * SEGUNDOS) - C_seg;
					}
					// Else Go for to the next day programmed
				}
			 }
			 //Irrigation day selected is less than current day
			 else if(T_weekDay < C_weekDay)
			 {
				if(NumOfIrrigationDays == 1)//Are there more days to check?
				{seconds_to_irrigation = (((((((((7 - C_weekDay) + T_weekDay) * HORAS) - C_hr) + T_hr) * MINUTOS) - C_min) + T_min) * SEGUNDOS) - C_seg;}
			 	//else Go for the next day programmed
			 }
			 else//Irrigation day selected is grater then current day
			 {seconds_to_irrigation = ((((((((T_weekDay - C_weekDay)* HORAS) - C_hr) + T_hr) * MINUTOS) - C_min) + T_min) * SEGUNDOS) - C_seg;}
		 }
		
		printf("seconds_to_irrigation: %i", seconds_to_irrigation);
		SET_IrrigationTimeCalc(true); 
	}
	else 
	{// do nothing irrigation schedule not set}
	*/
}


uint8_t app_timers_ConvertDays(const char *input)
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


bool GET_Irrigation_Alarm_flag(void)
{
	return Irrigation_Alarm_flag;
}

void SET_Irrigation_Alarm_flag(bool value)
{
	Irrigation_Alarm_flag = value;
}

void app_timer_startIrrigationTask(s_IrrigationInfo_t* UserInfo)
{
	
	//memcpy((void *) &IrrigationInfoReceived, (const void *) UserInfo, sizeof(UserInfo));
	
	IrrigationInfoReceived.irrigation_days = UserInfo->irrigation_days;
	IrrigationInfoReceived.irrigation_time = UserInfo->irrigation_time;
	IrrigationInfoReceived.irrigation_duration = UserInfo->irrigation_duration;
	
	printf("Days:%s\n",IrrigationInfoReceived.irrigation_days);
	printf("Time:%s\n",IrrigationInfoReceived.irrigation_time);
	printf("Duration:%s\n",IrrigationInfoReceived.irrigation_duration);
	xTaskCreate(irrigation_task,
						  "Irrigation_Task",
						  IRRIGATION_TASK_STACK_SIZE,
						  (void *)UserInfo,
						  2,
						  &HandlerIrrigationTask);
						  
	esp_task_wdt_add(HandlerIrrigationTask);
}
