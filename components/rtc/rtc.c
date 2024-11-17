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
//#include "esp_sntp.h"
//#include "lwip/ip_addr.h"


#include "blufi_if.h"
#include "rtc.h"

time_t now;
char strftime_buf[64];
struct tm timeinfo;

static const char *TAG = "SNTP";

void rtc_set_sntp(void)
{
static rtc_SNTP_states_t timeStates = set_SNTP_time;
rtc_SNTP_states_t Next_state = set_SNTP_time;

	switch(timeStates)
	{
		case set_SNTP_time:
			if(true == blufi_GetWiFiConStatus())
				{
					ESP_LOGI(TAG, "Initializing SNTP");
					esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");//CONFIG_SNTP_TIME_SERVER);
					config.start = false;                       // start SNTP service explicitly (after connecting)
					config.server_from_dhcp = true;             // accept NTP offers from DHCP server, if any (need to enable *before* connecting)
					config.renew_servers_after_new_IP = true;   // let esp-netif update configured SNTP server(s) after receiving DHCP lease
					config.index_of_first_server = 1;           // updates from server num 1, leaving server 0 (from DHCP) intact
					config.ip_event_to_renew = IP_EVENT_STA_GOT_IP;
					config.smooth_sync = true;

					ESP_LOGI(TAG, "Initializing and starting SNTP");
					esp_netif_sntp_init(&config);

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
			if (esp_netif_sntp_sync_wait(pdMS_TO_TICKS(500)) != ESP_OK)
			{
				ESP_LOGI(TAG, "Failed to update system time within 10s timeout");
				Next_state = set_SNTP_time;
			}
			else
			{
				ESP_LOGI(TAG, "SNTP config complete");
				Next_state = print_SNTP_time;
			}
			break;

		case print_SNTP_time:
			time(&now);
			localtime_r(&now, &timeinfo);
			strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
			ESP_LOGI(TAG, "The current date/time in Shanghai is: %s", strftime_buf);
			Next_state = SNTP_done;
			break;

		case SNTP_done:
			/*Do nothing the time is already set*/
			Next_state = SNTP_done;
			break;

		case MAX_SNTP_states:
			Next_state = set_SNTP_time;
			break;
	}
	timeStates = Next_state;
}
