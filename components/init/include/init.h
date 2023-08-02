
#ifndef __SYSINIT_H_
#define __SYSINIT_H_

uint8_t sysinit1(void);
uint8_t sysinit2(void);

uint8_t appinit1(void);
uint8_t appinit2(void);

extern void blink_led(void);
extern uint8_t s_led_state;

#endif /* __SYSINIT_H_ */
