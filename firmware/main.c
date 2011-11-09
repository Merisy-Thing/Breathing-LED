/*
********************************************************************************
*		Creat by pajoke@2011.11.8
********************************************************************************
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include "main.h"

uint8_t PWM_Tab[] PROGMEM= {
	1, 1, 2, 3, 5, 8, 11, 15, 20, 25, 30, 36, 43, 49, 56, 64, 72, 80, 88, 97,
	105, 114, 123, 132, 141, 150, 158, 167, 175, 183, 191, 199, 206, 212, 219,
	225, 230, 235, 240, 244, 247, 250, 252, 253, 254, 255, 254, 253, 252, 250,
	247, 244, 240, 235, 230, 225, 219, 212, 206, 199, 191, 183, 175, 167, 158,
	150, 141, 132, 123, 114, 105, 97, 88, 80, 72, 64, 56, 49, 43, 36, 30, 25,
	20, 15, 11, 8, 5, 3, 2, 1, 0
};

/* Hardware init */
void MCU_init(void)
{
	//Clock
	CLKPR = 1<<CLKPCE;
	CLKPR = (1<<CLKPS2) | (1<<CLKPS0);	//300KHz

	//Pin
	PIN_HIGH(LED_CTRL_PIN);
	PIN_OUTPUT(LED_CTRL_PIN);

	// COM0A1 - COM0A0 (Set OC0A on Compare Match, clear OC0A at TOP)
	// WGM01 - WGM00 (set fast PWM)
	PIN_OUTPUT(PWM_CTRL_PIN);
	TCCR0A |= ((1 << COM0A1) | (1 << COM0A0) | (1 << WGM01) | (1 << WGM00)); 
	OCR0A = 0; // initialize Output Compare Register A to 0
	TCCR0B |= (1 << CS00); // Start timer at Fcpu

	sleep_enable();

	sei();	//Enables global interrupt
	//cli();	//Disables global interrupt
}

/* WDT interrupt */
ISR(WDT_vect, ISR_NAKED)
{
	reti();
}

/* Main Function */
int main(void)
{
	volatile uint8_t tab_index;
	volatile uint8_t pwm_value;

	MCU_init();

	tab_index = 0;
	while(1) {
		pwm_value = pgm_read_byte(&PWM_Tab[tab_index]);
		tab_index++;
		if(pwm_value == 0) {
			tab_index = 1;
			pwm_value = 1;//PWM_Tab[0] == 1
		}
		OCR0A = pwm_value;

		// set watchdog in interrupt mode and 2k cycles
		WDTCR = (0<<WDE)|(1<<WDTIE);

		//reset the watchdog timer to full value and sleep until it pops an interrupt
		wdt_reset();
		sleep_cpu();

		// set watchdog in interrupt mode and 4k cycles
		WDTCR = (0<<WDE)|(1<<WDTIE)|(1<<WDP0);

		//reset the watchdog timer to full value and sleep until it pops an interrupt
		wdt_reset();
		sleep_cpu();
	}
	return 0;
}


