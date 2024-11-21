#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

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
#include "rtc.h"
#include "rtc_if.h"

static const char *TAG = "SNTP";

void rtc_sntp_SM(void)
{
sntp_sync_status_t sync_status;
static rtc_SNTP_states_t timeStates = set_SNTP_time;
rtc_SNTP_states_t Next_state = set_SNTP_time;
static uint8_t err_ctr = 0;

	switch(timeStates)
	{
		case set_SNTP_time:
			if(true == blufi_GetWiFiConStatus())
				{
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
					Next_state = wait_SNTP_synchronization;
				}
				else
				{
					ESP_LOGI(TAG, "waiting for WIFI connection");
					Next_state = set_SNTP_time;
				}
			break;

		case wait_SNTP_synchronization:
			
			sync_status = sntp_get_sync_status();
			if (sync_status == SNTP_SYNC_STATUS_COMPLETED)
			{
				ESP_LOGI(TAG, "Sync. completed");
				Next_state = print_SNTP_time;
			}
			else if (sync_status == SNTP_SYNC_STATUS_IN_PROGRESS)
			{
				Next_state = wait_SNTP_synchronization;
				ESP_LOGI(TAG, "Waiting for sync to complete");
			}
			else
			{
				err_ctr++;
				if (err_ctr > SYNC_RETRIES)
				{
					err_ctr = 0;
					ESP_LOGI(TAG, "SNTP config error");
					rtc_printLocTime();
					Next_state = SNTP_done;
				}
				else{Next_state = wait_SNTP_synchronization;}
				
			}
			break;

		case print_SNTP_time:
			rtc_printLocTime();
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
}
