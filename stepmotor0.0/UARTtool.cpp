/*
 * UARTtool.cpp
 *
 * Created: 6/17/2015 9:08:12 PM
 *  Author: 12132_000
 */ 

#define F_CPU 16000000UL
#define BAUD 9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR

#include "UARTtool.h"
#include <avr/io.h>
#include <string.h>

UARTtool::UARTtool(){
	UBRR0H = (BAUDRATE>>8);                      // shift the register right by 8 bits
	UBRR0L = BAUDRATE;                           // set baud rate
	UCSR0B|= (1<<RXCIE0)|(1<<TXEN0)|(1<<RXEN0);                // enable receiver and transmitter
	UCSR0C|= (0<<UCSZ02)|(1<<UCSZ01)|(1<<UCSZ00);   // 8bit data format
	
}

void UARTtool::uart_transmit (char data){
	while (!( UCSR0A & (1<<UDRE0)));                // wait while register is free
	UDR0 = data;
}

void UARTtool::uart_transmit_string(char data[]){
	
	int i= strlen(data);
	for (int j=0;j<=i;j++)
	{
		uart_transmit(data[j]);
		
	}
	uart_transmit(0x0A);
	uart_transmit(0x0D);//newline
	
}