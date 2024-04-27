#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <inttypes.h>
#include "driver/gpio.h"
#include "esp_log.h"
//#include "gpio_types.h"
#include "led_strip.h"
#include "leds.h"


gpio_config_t GPIO_LED_CFG;
uint8_t s_led_state=1;
const char *TAG = "example";

void led_Config(void)
{
	GPIO_LED_CFG.pull_up_en = GPIO_PULLDOWN_DISABLE;
	GPIO_LED_CFG.pull_down_en = GPIO_PULLDOWN_ENABLE;
	GPIO_LED_CFG.mode = GPIO_MODE_OUTPUT;

	ESP_LOGI(TAG, "Example configured to blink addressable LED!");

	gpio_config(&GPIO_LED_CFG);
}


#ifdef CONFIG_BLINK_LED_RMT

static led_strip_handle_t led_strip;

void blink_led(LedColorRGB_t s_LED_Color)
{
    /* If the addressable LED is enabled */
    if (s_led_state) {
        /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
        led_strip_set_pixel(led_strip, 0, s_LED_Color.RedC, s_LED_Color.GreenC, s_LED_Color.BlueC);
        /* Refresh the strip to send data */
        led_strip_refresh(led_strip);
    } else {
        /* Set all LED off to clear all pixels */
        led_strip_clear(led_strip);
    }
    //ESP_LOGI(TAG, "Turning the LED %.5s!", s_led_state == true ? "1\r" : "0\r");
    s_led_state = !s_led_state;
}

void configure_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink addressable LED!");
    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO,
        .max_leds = 1, // at least one LED on board
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    /* Set all LED off to clear all pixels */
    led_strip_clear(led_strip);
}

#elif CONFIG_BLINK_LED_GPIO

void blink_led(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(BLINK_GPIO, s_led_state);
    s_led_state = !s_led_state;
}

void configure_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}

#endif
