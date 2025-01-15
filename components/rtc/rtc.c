#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "esp_bit_defs.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_sleep.h"
#include "nvs_flash.h"
#include "esp_netif_sntp.h"
//#include "esp_netif.h"
#include "esp_sntp.h"
//#include "lwip/ip_addr.h"


#include "blufi_if.h"
#include "rom/uart.h"
#include "rtc.h"
#include "rtc_if.h"

static const char *TAG = "SNTP";

time_t time_now;
time_t future_time, time3;


void rtc_sntp_SM(void)
{
sntp_sync_status_t sync_status;
static rtc_SNTP_states_t timeStates = set_SNTP_time;
rtc_SNTP_states_t Next_state = set_SNTP_time;
static uint8_t err_ctr = 0;

	switch(timeStates)
	{
		case SNTP_chk_connection:
			if(true == blufi_GetWiFiConStatus())
			{
				Next_state = set_SNTP_time;
			}
			else
			{				
				Next_state = SNTP_chk_connection;
				if(WAIT_CONN_RETRIES == err_ctr)
				{
					ESP_LOGI(TAG, "waiting for WIFI connection");
					err_ctr = 0;	
				}
			}
		break;
		
		case set_SNTP_time:
			ESP_LOGI(TAG, "Initializing SNTP");
			//esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");//CONFIG_SNTP_TIME_SERVER);
			esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("mx.pool.ntp.org");//CONFIG_SNTP_TIME_SERVER);
			
			config.start = false;                       // start SNTP service explicitly (after connecting)
			config.wait_for_sync = true;				//if true, a semaphore to signal time sync event is created
			config.server_from_dhcp = true;             // accept NTP offers from DHCP server, if any (need to enable *before* connecting)
			config.renew_servers_after_new_IP = true;   // let esp-netif update configured SNTP server(s) after receiving DHCP lease
			config.index_of_first_server = 1;           // updates from server num 1, leaving server 0 (from DHCP) intact
			config.ip_event_to_renew = IP_EVENT_STA_GOT_IP;
			config.smooth_sync = true;

			ESP_LOGI(TAG, "Calling esp_netif_sntp_init");
			esp_netif_sntp_init(&config);

			ESP_LOGI(TAG, "Starting netif SNTP");
			esp_netif_sntp_start();
			
			sync_status = esp_netif_sntp_sync_wait(pdMS_TO_TICKS(3));
			
			if(sync_status == ESP_OK)
			{Next_state = print_SNTP_time;}
			else
			{Next_state = wait_SNTP_synchronization;}
			
		break;

		case wait_SNTP_synchronization:
			
			//sync_status = sntp_get_sync_status();
			
			sync_status = esp_netif_sntp_sync_wait(pdMS_TO_TICKS(4));
			
			if (sync_status == ESP_OK)
			{
				ESP_LOGI(TAG, "Sync. completed");
				Next_state = print_SNTP_time;
			}
			else if (sync_status == ESP_ERR_NOT_FINISHED)
			{
				Next_state = wait_SNTP_synchronization;
				ESP_LOGI(TAG, "Waiting for sync to complete, %i",sync_status);
			}
			else
			{
				err_ctr++;
				if (err_ctr > SYNC_RETRIES)
				{
					err_ctr = 0;
					ESP_LOGI(TAG, "SNTP sync config error");
					rtc_printLocTime();
					Next_state = SNTP_done;
				}
				else{Next_state = wait_SNTP_synchronization;}
				
			}
			break;

		case print_SNTP_time:
			rtc_printLocTime();
			
			time(&time_now);
			
			future_time = time_now + (SECONDS_DAY * 5);
			
			time3 = difftime(future_time,time_now);
			
			
			// Convert the future time to a readable format
			struct tm *future_tm = localtime(&future_time);
			printf("The date two weeks from now is: %s\n", asctime(future_tm));
			
			printf("Diff time in days %lli \n",(time3/SECONDS_DAY));
			
			Next_state = SNTP_done;
			break;

		case SNTP_done:
			/*Do nothing the time is already set*/
			Next_state = SNTP_done;
			break;

		
		case MAX_SNTP_states:
		default:
			Next_state = set_SNTP_time;
			break;
	}
	timeStates = Next_state;
}

static void rtc_getCurrentTime(void)
{
	time(&time_now);
}

void rtc_printLocTime(void)
{
	time_t now;
	char strftime_buf[64];
	struct tm timeinfo;

	time(&now);
	setenv("TZ", "CST+6", 1);
			
	localtime_r(&now, &timeinfo);
	strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
	printf("The current date/time in GDL Mexico is: %s\n", strftime_buf);
	printf(" The day of the week is:%i\n", rtc_get_WeekDay());
}

int rtc_get_sec(void)
{
	struct tm timeinfo;
	rtc_getCurrentTime();
	localtime_r(&time_now, &timeinfo);
	return timeinfo.tm_sec;
}

int rtc_get_min(void)
{
	struct tm timeinfo;
	rtc_getCurrentTime();
	localtime_r(&time_now, &timeinfo);
	return timeinfo.tm_min;
}

int rtc_get_hour(void)
{
	struct tm timeinfo;
	rtc_getCurrentTime();
	localtime_r(&time_now, &timeinfo);
	return timeinfo.tm_hour;
}

int rtc_get_WeekDay(void)
{
	struct tm timeinfo;
	rtc_getCurrentTime();
	localtime_r(&time_now, &timeinfo);
	return timeinfo.tm_wday;
}

int rtc_get_MonthDay(void)
{
	struct tm timeinfo;
	rtc_getCurrentTime();
	localtime_r(&time_now, &timeinfo);
	return timeinfo.tm_mday;
}

int rtc_get_Month(void)
{
	struct tm timeinfo;
	rtc_getCurrentTime();
	localtime_r(&time_now, &timeinfo);
	return timeinfo.tm_mon;
}

int rtc_get_Year(void)
{
	struct tm timeinfo;
	rtc_getCurrentTime();
	localtime_r(&time_now, &timeinfo);
	return timeinfo.tm_year;
}

uint64_t rtc_get_FutureDate_sec(time_t *future_time)
{
	time_t time_now;
	time(&time_now);
	double DeltaTime;
	
	//future_time = time_now + (SECONDS_DAY * 5);
	if(*future_time > time_now)
	{		
		DeltaTime = difftime(*future_time,time_now);
		printf("DeltaTime %f\n",DeltaTime);
	}
	else
	{
		printf("Future time given is not in the future!\n");
		DeltaTime = 0;
	}
	
	return DeltaTime;
}



