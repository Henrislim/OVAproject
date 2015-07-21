/*********************************
millis.c - sample Arduino millis()
on an ATTiny85 (use timer0 overflow
interrupt do to it)
Code in public domain
**********************************/

#define F_CPU 16000000UL
#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include "millis.h"

/* prototypes */
//void setup(void);
//void loop(void);

/* some vars */
uint64_t _millis = 0;
uint16_t _1000us = 0;
uint64_t old_millis = 0;
	

Millis::Millis(void){
	;
	}

/* interrupts routines */
// timer overflow occur every 0.256 ms
ISR(TIMER0_OVF_vect) {
	_1000us += 128;
	while (_1000us > 1000) {
		_millis++;
		_1000us -= 1000;
	}
}

// safe access to millis counter
uint64_t Millis::millis() {
	uint64_t m;
	cli();
	m = _millis;
	sei();
	return m;
}

void Millis::setup(void) {
	// LED IO
	//DDRB |= _BV(PB3); // set LED pin as output
	//PORTB |= _BV(PB3); // turn the LED on

	/* interrup setup */
	// prescale timer0 to 1/8th the clock rate
	// overflow timer0 every 0.256 ms
	TCCR0B |= (1<<CS01);
	// enable timer overflow interrupt
	TIMSK0  |= 1<<TOIE0;

	// Enable global interrupts
	sei();
}

