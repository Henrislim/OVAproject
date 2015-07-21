/*
 * ADS1115.cpp
 *
 * Created: 7/11/2015 5:09:44 PM
 *  Author: 12132_000
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>
#include "ADS1115.h"
#include "i2c.h"


I2C ADS1115i2c;


ADS1115::ADS1115(){}

	
void ADS1115::init_ADS1115(){
	
	ADS1115i2c.i2c_start(ADS1115_ADDRESS);
	ADS1115i2c.i2c_write(ADS1115_REG_POINTER_CONFIG); // set pointer to Config register
	ADS1115i2c.i2c_write(((uint8_t)ADS1115_CONF_CH0>>8)); // write MSB of the Config register
	ADS1115i2c.i2c_write(((uint8_t)ADS1115_CONF_CH0&0xFF)); // write LSB of the Config register
	ADS1115i2c.i2c_stop();
	
}

uint16_t ADS1115::readADS1115(){
	 uint16_t result;
	 uint8_t MSB=0;
	 uint8_t LSB=0;
	 
	 int a=ADS1115i2c.i2c_start(ADS1115_ADDRESS);
	 if (a==1)
	 {
	 }
	 else{
		 ADS1115i2c.i2c_write(ADS1115_REG_POINTER_CONVERT); // set pointer to X axis MSB
		 
	 }
	 ADS1115i2c.i2c_stop();

	 int b=ADS1115i2c.i2c_start((ADS1115_ADDRESS|0x01));
	 if (b==1)
	 {
	 }
	 else{
		 MSB = ADS1115i2c.i2c_read_ack();
		 LSB = ADS1115i2c.i2c_read_ack();
		 
	 }
	 ADS1115i2c.i2c_stop();

	 result =((MSB<<8)|LSB)>>4;
	 return result;
}

double ADS1115::readVoltage(){
	init_ADS1115();
	_delay_ms(20);
	uint16_t readResult= readADS1115();
	double voltage = 0.003*readResult;
	return voltage;
	
}