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

/*


double voltageArray[256];
double voltageSum=0;
double voltageAverage2=0;
double voltagePottemp,voltagePot;
//double averageVoltageValue,voltageValue,voltageValue1,voltageValue2,;
//double test=3.14;
char buffer0[10],buffer1[10];//,buffer2[10],buffer3[10];
char welcome_msg[]="Welcome!";
char potv_msg[]="Pot V:";
char wiperv_msg[]="Wiper V:";
int steps_to_move=-4;

Stepper myStepper;
ADC2     myADC;
UARTtool myUART;
Menu     myMenu;


pin_setup();
register_setup();
//sei();



myStepper.setSpeed(5);
//myUART.uart_transmit_string(welcome_msg);
//myStepper.step(steps_to_move);
//myStepper.step(1);
//sprintf(buffer,"%f",test);
//dtostrf(test, 4, 2, buffer);

//uart_transmit_string(buffer);
myStepper.motorOff();

while (1)
{
	if (PIND & (1<<PIND5))
	{
		_delay_ms(10);
		if (PIND & (1<<PIND5))
		{
			myStepper.step(steps_to_move);
			
		}
		_delay_ms(100);
		myStepper.motorOff();
		
		//read the pot+
		//myADC.setChannel(1);
		//voltagePottemp=myADC.startConvert();
		//voltagePot=5*voltagePottemp/1023;
		
		
		//read the wiper
		myADC.setChannel(0);
		for (int i=0;i<256;i++)
		{
			voltageArray[i]=myADC.startConvert();
		}
		
		//find the sum of 256 data
		for (int i=0;i<256;i++)
		{
			voltageSum=voltageSum+voltageArray[i];
			
		}
		//Find the average number
		voltageAverage2=(5*voltageSum)/1023/256;
		
		//Convert double to string
		dtostrf(voltageAverage2, 4, 2, buffer0);
		dtostrf(voltagePot,4,2,buffer1);
		
		//send by uart
		//myUART.uart_transmit_string(potv_msg);
		//myUART.uart_transmit_string(buffer1);
		myUART.uart_transmit_string(wiperv_msg);
		myUART.uart_transmit_string(buffer0);
		
		//reset to 0
		voltageSum=0;
		voltageAverage2=0;
		
		
		
	}
}





*/
