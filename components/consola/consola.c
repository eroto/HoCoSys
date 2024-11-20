#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_console.h"
#include "argtable3/argtable3.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "sense_if.h"
#include "esp_wifi.h"
#include "blufi_if.h"
#include "rtc_if.h"
#include "init_if.h"
#include "consola.h"


#define PROMPT_STR CONFIG_IDF_TARGET

extern int32_t get_rstCtr(void);
extern void rtc_printLocTime(void);

/* Command handler for user input */
static int cmd_echo(int argc, char** argv)
{
    for (int i = 1; i < argc; i++) {
        printf("%s ", argv[i]);
    }
    printf("\n");
    return 0;
}

/*'restart' command restarts the program */
static int restart(int argc, char **argv)
{
    ESP_LOGI(TAG, "Restarting");
    esp_restart();
}

/* Get chip internal temperature in ºC */
static int cmd_temp(int argc, char** argv)
{
	float temp = sense_getIntTempCelcius();
	printf("%f Cº\n",temp);
    return 0;
}

//char *ThePrefix = "sta";

static int ipconfig(int argc, char **argv)
{
	wifi_mode_t WiFi_mode;
	esp_wifi_get_mode((wifi_mode_t *)&WiFi_mode);
	printf("Wifi Modes: Invalid=0 STA=1, AP=2, APSTA=3\n");
	printf("Wifi Mode:%d\n",WiFi_mode);
	bluefi_ip_info((char *)"sta");
	return 0;
}

static int ResetCtr(int argc, char **argv)
{
	int32_t ctr = get_rstCtr();
	printf("Number of restarts:%li\n",ctr);
	return 0;
}

static int loctime(int argc, char **argv)
{
	rtc_printLocTime();
	return 0;
}

static int rstreason(int argc, char **argv)
{
	esp_reset_reason_t RstCode = esp_reset_reason();

	printf("Reset Reason code:%s\n", ResetResons[RstCode]);
	return 0;
}

// Register the "echo" command
void register_loctime_command(void) {
    const esp_console_cmd_t cmd = {
        .command = "loctime",
        .help = "Prints local time",
        .hint = NULL,
        .func = &loctime,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}

// Register the "echo" command
void register_echo_command(void) {
    const esp_console_cmd_t cmd = {
        .command = "echo",
        .help = "Prints the input string to console",
        .hint = NULL,
        .func = &cmd_echo,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}

// Register the "echo" command
void register_temp_command(void) {
    const esp_console_cmd_t cmd = {
        .command = "temp",
        .help = "Prints the SOC internal temperature",
        .hint = NULL,
        .func = &cmd_temp,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}

static void register_restart(void)
{
    const esp_console_cmd_t cmd = {
        .command = "restart",
        .help = "Software reset of the chip",
        .hint = NULL,
        .func = &restart,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

static void register_ipconfig(void)
{
    const esp_console_cmd_t cmd = {
        .command = "ipconfig",
        .help = "Print IP configuration details",
        .hint = NULL,
        .func = &ipconfig,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

static void register_ResetCtr(void)
{
    const esp_console_cmd_t cmd = {
        .command = "ResetCtr",
        .help = "Print the number system restarts",
        .hint = NULL,
        .func = &ResetCtr,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}


static void register_rstreason(void)
{
    const esp_console_cmd_t cmd = {
        .command = "rstreason",
        .help = "Print the las reset reason",
        .hint = NULL,
        .func = &rstreason,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

/*
static void register_version(void)
{
    const esp_console_cmd_t cmd = {
        .command = "version",
        .help = "Get version of chip and SDK",
        .hint = NULL,
        .func = &get_version,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}
*/

void consola_Init(void)
{
	esp_console_repl_t *repl = NULL;
	    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
	    /* Prompt to be printed before each line.
	     * This can be customized, made dynamic, etc.
	     */
	    repl_config.prompt = PROMPT_STR ">";
	    repl_config.max_cmdline_length = 256;

	    /*initialize_nvs() is already called as the system init*/

	#if CONFIG_CONSOLE_STORE_HISTORY
		initialize_filesystem();
		repl_config.history_save_path = HISTORY_PATH;
		ESP_LOGI(TAG, "Command history enabled");
	#else
		ESP_LOGI(TAG, "Command history disabled");
	#endif

	    /* Register commands */
	    esp_console_register_help_command();
	    register_echo_command();
	    register_temp_command();
	    register_restart();
	    register_ipconfig();
	    register_ResetCtr();
	    register_loctime_command();
	    register_rstreason();
	    //register_system();
	    //register_wifi();
	    //register_nvs();

#if defined(CONFIG_ESP_CONSOLE_UART_DEFAULT) || defined(CONFIG_ESP_CONSOLE_UART_CUSTOM)
    esp_console_dev_uart_config_t hw_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_uart(&hw_config, &repl_config, &repl));

#elif defined(CONFIG_ESP_CONSOLE_USB_CDC)
    esp_console_dev_usb_cdc_config_t hw_config = ESP_CONSOLE_DEV_CDC_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_usb_cdc(&hw_config, &repl_config, &repl));

#elif defined(CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG)
    esp_console_dev_usb_serial_jtag_config_t hw_config = ESP_CONSOLE_DEV_USB_SERIAL_JTAG_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_usb_serial_jtag(&hw_config, &repl_config, &repl));

#else
#error Unsupported console type
#endif

    ESP_ERROR_CHECK(esp_console_start_repl(repl));
}
