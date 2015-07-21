/*
 * ADC.cpp
 *
 * Created: 6/16/2015 8:14:11 PM
 *  Author: 12132_000
 */ 


#include "ADC.h"
#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>


ADC2::ADC2(){
	//Channel ADC0 is chosen
	//MUX3...0 =0000
	//Configure the PORTC0
	DDRC &= !_BV(0);
	//Enable ADC with prescaller 128 (16000000/128=125000Hz)
	//Required 50K-200KHz
	ADCSRA |= _BV(ADEN)|_BV(ADPS2)|_BV(ADPS1)|_BV(ADPS0);
	//Internal 1.1V Voltage Reference with external capacitor at AREF pin 
	ADMUX |=  _BV(REFS0);//|_BV(REFS1);
	
	
	
}

void ADC2::setChannel(int channel)
{
	switch (channel) {
		case 0:
		ADMUX =0b01000000;
		break;
		
		case 1:
		ADMUX =0b01000001;
		break;
		
		case 2:
		ADMUX =0b01000010;
		break;
		
		case 3:
		ADMUX =0b01000011;
		break;
		
		case 4:
		ADMUX =0b01000100;
		break;
		
		case 5:
		ADMUX =0b01000101;
		break;
		
		case 6:
		ADMUX =0b01000110;
		break;
		
		case 7:
		ADMUX =0b01000111;
		break;
		
		case 8:
		ADMUX =0b01001000;
		break;
		
	
	}
	
}

double ADC2::startConvert(void){
	
	double value; 
	
	 
	//Start conversion
	ADCSRA |= _BV(ADSC);
	//Wait for cnversion
	while (!(ADCSRA &(1<<ADIF)));
	//Clear ADIF
	ADCSRA |= _BV(ADIF);
    
	value = ADCL |(ADCH<<8);
	
	return value;
}

