#ifndef __LEDS_H__
#define __LEDS_H__

/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_BLINK_GPIO

typedef struct
{
	uint32_t RedC;
	uint32_t GreenC;
	uint32_t BlueC;
}
LedColorRGB_t;

extern uint8_t s_led_state;

void configure_led(void);
void blink_led(LedColorRGB_t s_LED_Color);

#endif /*__LEDS_H__*/
