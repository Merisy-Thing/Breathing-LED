#ifndef __MAIN_H_
#define __MAIN_H_

//#define
#define PIN_LOW(pin)		(PORTB &= ~(1<<pin))
#define PIN_HIGH(pin)		(PORTB |= (1<<pin))

#define PIN_INPUT(pin)		(DDRB &= ~(1<<pin))
#define PIN_OUTPUT(pin)		(DDRB |= (1<<pin))

#define PIN_GET(pin)		(PINB & (1<<pin))

//Pin define
#define LED_CTRL_PIN	PB4
#define PWM_CTRL_PIN	PB0

#define LED_TURN_ON()	PIN_HIGH(LED_CTRL_PIN)
#define LED_TURN_OFF()	PIN_LOW(LED_CTRL_PIN)

#define SUCC	1
#define FAIL	0

#define TRUE	1
#define FALSE	0


#endif

