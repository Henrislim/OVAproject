/*
 * Menu.cpp
 *
 * Created: 7/1/2015 10:36:13 AM
 *  Author: 12132_000
 */ 
#define F_CPU 16000000UL
#include <string.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "ADS1115.h"

#include "Stepper.h"
#include "ADC.h"
#include "UARTtool.h"
#include "Menu.h"
#include "VolTodB.h"

Stepper  myStepper;
UARTtool myUART;
ADC2     myADC;
ADS1115 myADS1115;
VolTodB myVolTodB;


void moveup(int);
void movedown(int);
double readVoltage();
double readVoltageLite();
void replyList(int);
void menuStart();
void moveUpDownLite(int);
void stepperStop();
void goToTargetVol(double);
void reset();

int mode=0;
double voltageArray[16];
double voltageSumSample[16];
double voltageSum=0;
double voltageAverage2=0;
double voltagePottemp,voltagePot;
//double averageVoltageValue,voltageValue,voltageValue1,voltageValue2,;
//double test=3.14;
char buffer0[10],buffer1[10];//,buffer2[10],buffer3[10];
char welcome_msg[]="Welcome!";
char potv_msg[]="Pot V:";
char wiperv_msg[]="Wiper V:";
int steps_to_move_up=4;
int steps_to_move_down=-4;

char userInputString[10];
char userInputSub;
int  userInputIdex=0;

bool modelA=false;
bool modelB=false;
bool modelR=false;
bool modelT=false;


	
Menu::Menu(){
	
}

void Menu::MenuStart(){
		
		menuStart();
		myStepper.setSpeed(5);
}



ISR (USART_RX_vect){
	char userInput = UDR0;
	UDR0=userInput;
	
	if (userInput !=9)
	{
		userInputSub=userInput;
		userInputString[userInputIdex]=userInput;
		userInputIdex++;
	}
	
	else{
		
	
	
	//Setting the mode
	if (userInputSub=='A'||userInputSub=='a')
	{
		replyList(1);
		modelA=true;
		modelB=false;
		modelR=false;
		modelT=false;
	}
	
	else if (userInputSub=='B'||userInputSub=='b')
	{
		replyList(2);
		modelB=true;
		modelA=false;
		modelR=false;
		modelT=false;
	}
	else if (userInputSub=='R'||userInputSub=='r') //reset to the minimum attenuation
	{
		replyList(3);
		modelR=true;
		modelB=false;
		modelA=false;
		modelT=false;
		
	}
	else if (userInputSub=='T'||userInputSub=='t') //reset to the minimum attenuation
	{
		replyList(4);
		modelR=false;
		modelB=false;
		modelA=false;
		modelT=true;
		
	}
	else if (userInputSub==27)//ESC
	{
		menuStart();
		modelB=false;
		modelA=false;
		modelR=false;
		modelT=false;
	}

	
	//Mode A
	if (modelA)
	{
		if (userInputSub=='w'||userInputSub=='W')
		{
			//replyList(3);
			if (readVoltageLite()<4.00)
			{
				moveup(steps_to_move_up);
			}
			
		}
		else if (userInputSub=='s'||userInputSub=='S')
		{
			//replyList(4);
			if (readVoltageLite()>0.6)
			{
				movedown(steps_to_move_down);
			}
			
		}
	}
	else if (modelB)
	{
		char bufferx[10];
		double tempnum;
		dtostrf(tempnum,4,2,bufferx);
		myUART.uart_transmit_string(bufferx);
		
		if (tempnum<0.8||tempnum>4.2)
		{
			replyList(3);
		}
		else{
			replyList(4);
		}
	}
	
	else if (modelR)
	{
		reset();
	}
	
	else if (modelT)
	{
		char bufferx[10];
		double dB= atof(userInputString);
		if (dB>=0&&dB<=29)
		{
			double targetVol=myVolTodB.getVoltage(dB);
			dtostrf(targetVol,4,2,bufferx);
			myUART.uart_transmit_string(bufferx);
			goToTargetVol(targetVol);
		}
			
		
	}
	
	strcpy(userInputString,"");
	userInputIdex=0;
	
	}
	

	
	
}
void replyList(int index){
	
	
		if (index==1)
		{
			char testmsg1[]="Mode A: W + or S -";
			myUART.uart_transmit_string(testmsg1);
		}
		else if (index==2)
		{
			char testmsg2[]="Mode B: Input target voltage: 0.80-4.20";
			myUART.uart_transmit_string(testmsg2);
		}
		
		else if (index==3)
		{
			char testmsg3[]="Reset to 0 dB";
			myUART.uart_transmit_string(testmsg3);
		}
		
		else if (index==4)
		{
			char testmsg4[]="Input target dB: 0.0-(-28.5)";
			myUART.uart_transmit_string(testmsg4);
		}
		
}


void menuStart(){
			char msg1[]="Welcome to OVA650 Testing System";
			char msg2[]="========= Menu =========";
			char msg3[]="A. Step by step: W+ and S-.";
			char msg4[]="B. Target Voltage.";
			char msg5[]="R. Reset";
			char msg6[]="T. Target dB";
			
			myUART.uart_transmit_string(msg1);
			myUART.uart_transmit_string(msg2);
			myUART.uart_transmit_string(msg3);
			myUART.uart_transmit_string(msg4);
			myUART.uart_transmit_string(msg5);
			myUART.uart_transmit_string(msg6);
}

void moveup(int stepup){
	
	myStepper.step(stepup);
	_delay_ms(200);
	myStepper.motorOff();
	_delay_ms(1000);
	readVoltage();
	
	
}

void movedown(int stepdown){
	
	myStepper.step(stepdown);
	_delay_ms(200);
	myStepper.motorOff();
	_delay_ms(1000);
	readVoltage();
	
	
	
}

void moveUpDownLite(int stepdown){
	
	myStepper.step(stepdown);
	_delay_ms(300);
	myStepper.motorOff();
}

void stepperStop(){
	myStepper.motorOff();
}


double readVoltage(){
	
	double voltage=0;
	double voltageSum=0;
	for (int i=0;i<16;i++)
	{
		 voltage= myADS1115.readVoltage();
		 voltageSum=voltageSum+voltage;
	}
	voltage = voltageSum/16;
	char buffer0[10];
	//Convert double to stringw
	dtostrf(voltage, 6, 3, buffer0);
	//dtostrf(voltagePot,4,2,buffer1);
	
	myUART.uart_transmit_string(wiperv_msg);
	myUART.uart_transmit_string(buffer0);
	
	//get dB
	double dB = myVolTodB.getdB(voltage);
	dtostrf(dB, 6, 3, buffer0);
	char temp_msg[]="dB";
	myUART.uart_transmit_string(temp_msg);
	myUART.uart_transmit_string(buffer0);
	
	return voltage;
}

double readVoltageLite(){
	
	double voltage=0;
	voltage= myADS1115.readVoltage();
	return voltage;
}

void reset(){
	    double presentVol= readVoltageLite();
		double downLimit= 0.7;
		double upperLimit=4.0;
		if (presentVol>downLimit)
		{
			int tempStep= -(((presentVol-0.7)/0.03)*4);
			//char temp = 'H';
			moveUpDownLite(tempStep);
		}
		
		/*for (;tempStep>0;tempStep--)
		{
			movedown(4);
			myUART.uart_transmit(temp);
			_delay_ms(500);
		}*/
		
		while (readVoltageLite()>downLimit)
		{
			moveUpDownLite(-4);
		}
		double initVolt=readVoltageLite();
		myVolTodB.setInitVoltage(initVolt);
		readVoltage();
		char msg[]="Reset down!";
		myUART.uart_transmit_string(msg);
}

void goToTargetVol(double targetVoltage){
	double presentVoltage= readVoltageLite();
	if (targetVoltage<0.6||targetVoltage>29)
	{
	}
	else{
	if (targetVoltage>presentVoltage)
	{
		int tempStep= ((targetVoltage-presentVoltage)/0.03)*4;
		moveUpDownLite(tempStep);
		
		while (readVoltageLite()<(targetVoltage))
		{
			moveUpDownLite(4);
		}
		
		
	}
	else{
		int tempStep= ((targetVoltage-presentVoltage)/0.03)*4;
		moveUpDownLite(tempStep);
		
		while (readVoltageLite()>(targetVoltage))
		{
			moveUpDownLite(-4);
		}
	}
	}
	readVoltage();
}



