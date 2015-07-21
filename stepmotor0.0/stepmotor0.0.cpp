/*
 * stepmotor0.cpp
 *
 * Created: 6/14/2015 1:20:04 PM
 *  Author: 12132_000
 */ 

#define F_CPU 16000000UL
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Stepper.h"
#include "ADC.h"
#include "UARTtool.h"
#include "Menu.h"

void pin_setup();
void register_setup();

Menu     myMenu;


int main(void)
{
	pin_setup();
	register_setup();
	sei();
	myMenu.MenuStart();
   while (1)
   {
   }
	
}

void pin_setup(){
	DDRD &= !_BV(5); //PD5  T1
	//DDRD &= !_BV(3); //INT1
	DDRD |= _BV(7);
	PORTD |= _BV(7); //Used to enable the L293D
	
}

void register_setup(){
	//Enable T1
	TCCR1B |= _BV(CS10) | _BV(CS11) | _BV(CS12);
	//Enable INT1 rising edge
	//EICRA  |= _BV(ISC11)| _BV(ISC10);
	//EIMSK  |= _BV(INT1);
}

