/*
 * UARTtool.h
 *
 * Created: 6/17/2015 9:06:00 PM
 *  Author: 12132_000
 */ 


#ifndef UARTTOOL_H_
#define UARTTOOL_H_

class UARTtool{
	
	public:
	UARTtool();
	void uart_transmit (char data);
	void uart_transmit_string(char data[]);
	
	
	};




#endif /* UARTTOOL_H_ */