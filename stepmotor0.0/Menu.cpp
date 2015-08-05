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
ADS1115  myADS1115;
VolTodB  myVolTodB;


void moveUpDown(int);
void moveUpDownLite(int);
void stepperStop();

double readVoltage();
double readVoltageLite();

void replyList(int);
void menuStart();


void goToTargetVol(double);
void reset();

int mode=0;
double voltageArray[16];
double voltageSum=0;
char buffer0[10];

char welcome_msg[]="Welcome!";
char potv_msg[]="Pot V:";
char wiperv_msg[]="Wiper V:";

int STEP_TO_UP_4=4;
int STEP_TO_DOWN_4=-4;

int STEP_TO_UP=8;
int STEP_TO_DOWN=-8;

char userInputString[10];
char userInputSub;
int  userInputIdex=0;

bool modelA=false;
bool modelR=false;
bool modelT=false;


	
Menu::Menu(){
	
}

void Menu::MenuStart(){
		menuStart();
		myStepper.setSpeed(4);//Set the speed of the stepper
		stepperStop();
}



ISR (USART_RX_vect){
	char userInput = UDR0;
	UDR0=userInput; // echo back
	
	if (userInput !=9) //tab - use to confirm the input
	{
		userInputSub=userInput;//store the last input
		userInputString[userInputIdex]=userInput;//store the string input
		userInputIdex++;
	}
	
	else{
	
	//Setting the mode
	if (userInputSub=='A'||userInputSub=='a')
	{
		replyList(1);
		modelA=true;
		modelR=false;
		modelT=false;
	}

	else if (userInputSub=='R'||userInputSub=='r') //reset to the minimum attenuation
	{
		replyList(3);
		modelR=true;
		modelA=false;
		modelT=false;
		
	}
	else if (userInputSub=='T'||userInputSub=='t') //Target voltage mode
	{
		replyList(4);
		modelR=false;
		modelA=false;
		modelT=true;
		
	}
	else if (userInputSub==27)//ESC
	{
		menuStart();
		modelA=false;
		modelR=false;
		modelT=false;
	}

	
	//Mode A: Move up or down step by step
	if (modelA)
	{
		if (userInputSub=='w'||userInputSub=='W')
		{
			if (readVoltageLite()<4.00)
			{
				moveUpDown(STEP_TO_UP);//STEP_TO_UP_4
			}
			
		}
		else if (userInputSub=='s'||userInputSub=='S')
		{
			if (readVoltageLite()>0.2)
			{
				moveUpDown(STEP_TO_DOWN);//STEP_TO_DOWN_4
			}
			
		}
	}
	
	else if (modelR)
	{
		reset();
	}
	
	else if (modelT)
	{
		if (userInputSub=='T'||userInputSub=='t')
		{ //do nothing
		}
		else{
		char bufferx[10];
		double dB= atof(userInputString);
		if (dB>0&&dB<=26.5)
		{
			double targetVol=myVolTodB.getVoltage(dB);
			if(targetVol!=0){
			dtostrf(targetVol,4,2,bufferx);
			myUART.uart_transmit_string(bufferx);
			goToTargetVol(targetVol);
			}
		}
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
			char msg1[]="Welcome";
			char msg2[]="========= Menu =========";
			char msg3[]="A. Step by step: W+ and S-.";
			char msg4[]="R. Reset";
			char msg5[]="T. Target dB";
			
			myUART.uart_transmit_string(msg1);
			myUART.uart_transmit_string(msg2);
			myUART.uart_transmit_string(msg3);
			myUART.uart_transmit_string(msg4);
			myUART.uart_transmit_string(msg5);
		
}

void moveUpDown(int stepup){
	
	myStepper.step(stepup);
	_delay_ms(200);
	myStepper.motorOff();
	_delay_ms(1000);
	readVoltage();
	
	
}

void moveUpDownLite(int stepdown){
	
	myStepper.step(stepdown);
	_delay_ms(500);
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
		
		//Move with big steps
		if (presentVol>downLimit)
		{
			int tempStep= ((presentVol-0.7)/0.026)*STEP_TO_DOWN;
			moveUpDownLite(tempStep);
		}
		
		//Move with small steps
		while (readVoltageLite()>downLimit)
		{
			moveUpDownLite(STEP_TO_DOWN_4);
		}
		
		double initVolt=readVoltageLite();
		myVolTodB.setInitVoltage(initVolt);
		
		//Finish reset
		char msg[]="Reset down!";
		readVoltage();
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
		int tempStep= ((targetVoltage-presentVoltage)/0.026)*STEP_TO_UP;
		moveUpDownLite(tempStep);
		
		while (readVoltageLite()<(targetVoltage))
		{
			moveUpDownLite(STEP_TO_UP_4);
		}
		
		
	}
	else{
		int tempStep= ((presentVoltage-targetVoltage)/0.026)*STEP_TO_DOWN;
		moveUpDownLite(tempStep);
		
		while (readVoltageLite()>(targetVoltage))
		{
			moveUpDownLite(STEP_TO_DOWN_4);
		}
	}
	}
	readVoltage();
}



