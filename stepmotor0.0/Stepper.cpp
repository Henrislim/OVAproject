/*
  Stepper.cpp - - Stepper library for Wiring/Arduino - Version 0.4
  
  Original library     (0.1) by Tom Igoe.
  Two-wire modifications   (0.2) by Sebastian Gassner
  Combination version   (0.3) by Tom Igoe and David Mellis
  Bug fix for four-wire   (0.4) by Tom Igoe, bug fix from Noah Shibley  
  Added Function to turn off motors (0.5) by Matthew Carlson

  Drives a unipolar or bipolar stepper motor using  2 wires or 4 wires

  When wiring multiple stepper motors to a microcontroller,
  you quickly run out of output pins, with each motor requiring 4 connections. 

  By making use of the fact that at any time two of the four motor
  coils are the inverse  of the other two, the number of
  control connections can be reduced from 4 to 2. 

  A slightly modified circuit around a Darlington transistor array or an L293 H-bridge
  connects to only 2 microcontroler pins, inverts the signals received,
  and delivers the 4 (2 plus 2 inverted ones) output signals required
  for driving a stepper motor.

  The sequence of control signals for 4 control wires is as follows:

  THIS IS INCORRECT
  Step C0 C1 C2 C3
     1  1  0  1  0
     2  0  1  1  0
     3  0  1  0  1
     4  1  0  0  1
  THIS IS INCORRECT
    Step C0 C1 C2 C3
	  1  0110
      2  1100
      3  1001
      4  0011
	  
    Step A- B+ B- A+
	  1  0  0  1  1
	  2  0  1  0  1
	  3  1  1  0  0
	  4  1  0  1  0
	  
	  
  
  

  The sequence of controls signals for 2 control wires is as follows
  (columns C1 and C2 from above):

  Step C0 C1
     1  0  1
     2  1  1
     3  1  0
     4  0  0

  The circuits can be found at 
 
http://www.arduino.cc/en/Tutorial/Stepper
 
 
 */


//#include "WProgram.h"
#include "Stepper.h"
#include "avr/io.h"
#include "millis.h"
#include <stdlib.h>




/*
 * two-wire constructor.
 * Sets which wires should control the motor.
 */
/*Stepper::Stepper(int number_of_steps, int motor_pin_1, int motor_pin_2)
{
  this->step_number = 0;      // which step the motor is on
  this->speed = 0;        // the motor speed, in revolutions per minute
  this->direction = 0;      // motor direction
  this->last_step_time = 0;    // time stamp in ms of the last step taken
  this->number_of_steps = number_of_steps;    // total number of steps for this motor
  
  // Arduino pins for the motor control connection:
  this->motor_pin_1 = motor_pin_1;
  this->motor_pin_2 = motor_pin_2;

  // setup the pins on the microcontroller:
  pinMode(this->motor_pin_1, OUTPUT);
  pinMode(this->motor_pin_2, OUTPUT);
  
  // When there are only 2 pins, set the other two to 0:
  this->motor_pin_3 = 0;
  this->motor_pin_4 = 0;
  
  // pin_count is used by the stepMotor() method:
  this->pin_count = 2;
}*/


/*
 *   constructor for four-pin version
 *   Sets which wires should control the motor.
 */

Millis myMillis;

Stepper::Stepper()
{
  step_number = 0;      // which step the motor is on
  speed = 0;        // the motor speed, in revolutions per minute
  direction = 0;      // motor direction
  last_step_time = 0;    // time stamp in ms of the last step taken
  number_of_steps = 1024;
    //this->number_of_steps = number_of_steps;    // total number of steps for this motor
  /*
  // Arduino pins for the motor control connection:
  this->motor_pin_1 = motor_pin_1;
  this->motor_pin_2 = motor_pin_2;
  this->motor_pin_3 = motor_pin_3;
  this->motor_pin_4 = motor_pin_4;

  // setup the pins on the microcontroller:
  pinMode(this->motor_pin_1, OUTPUT);
  pinMode(this->motor_pin_2, OUTPUT);
  pinMode(this->motor_pin_3, OUTPUT);
  pinMode(this->motor_pin_4, OUTPUT);
  */
  
  DDRB= 0X0F;
  PORTB &= !_BV(0);
  PORTB &= !_BV(1);
  PORTB &= !_BV(2);
  PORTB &= !_BV(3);
  
  // pin_count is used by the stepMotor() method:  
  pin_count = 4; 
  
  //Initialize the milis
  
  myMillis.setup();
  
}

/*
  Sets the speed in revs per minute

*/
void Stepper::setSpeed(long whatSpeed)
{
  step_delay = 60L * 1000L / this->number_of_steps / whatSpeed;
}

/*
  Moves the motor steps_to_move steps.  If the number is negative, 
   the motor moves in the reverse direction.
 */
void Stepper::step(int steps_to_move)
{  
  int steps_left = abs(steps_to_move);  // how many steps to take
  
  // determine direction based on whether steps_to_mode is + or -:
  if (steps_to_move > 0) {direction = 1;}
  if (steps_to_move < 0) {direction = 0;}
    
    
  // decrement the number of steps, moving one step each time:
  while(steps_left > 0) {
  // move only if the appropriate delay has passed:
  if (myMillis.millis() - last_step_time >= step_delay) {//this->step_delay
      // get the timeStamp of when you stepped:
      last_step_time = myMillis.millis();
      // increment or decrement the step number,
      // depending on direction:
      if (direction == 1) {
		  step_number++;
        if (step_number == number_of_steps) {
          step_number = 0;
        }
      } 
      else { 
        if (step_number == 0) {
         step_number = number_of_steps;
        }
        step_number--;
      }
      // decrement the steps left:
      steps_left--;
      // step the motor to step number 0, 1, 2, or 3:
      stepMotor(step_number % 8);
    }
  }
}

/*
 * Moves the motor forward or backwards.
 */
void Stepper::stepMotor(int thisStep)
{
	
	/*
	   Step A- B+ B- A+
	   1  0  0  1  1
	   2  0  1  0  1
	   3  1  1  0  0
	   4  1  0  1  0
	   
	    PORTB |= _BV(0);
		PORTB &= !_BV(0);
		
		PORTB |= _BV(1);
		PORTB &= !_BV(1);
		
		PORTB |= _BV(2);
		PORTB &= !_BV(2);
		
		PORTB |= _BV(3);
		PORTB &= !_BV(3);
		
	*/
   
	   switch (thisStep) {
		   
		   case 0:    // 0011
		   PORTB &= !_BV(0);
		   PORTB &= !_BV(1);
		   PORTB |= _BV(3);
		   PORTB |= _BV(2);
		   break;
		   
		   case 1:    // 0001
		   PORTB &= !_BV(0);
		   PORTB &= !_BV(1);
		   PORTB &= !_BV(2);
		   PORTB |= _BV(3);
		   break;
		   
		   case 2:    // 0101
           PORTB &= !_BV(0);
		   PORTB &= !_BV(2);
		   PORTB |= _BV(1);
		   PORTB |= _BV(3);
		   break;
		   
		   case 3:    // 0100
           PORTB &= !_BV(0);
		   PORTB &= !_BV(2);
		   PORTB &= !_BV(3);
		   PORTB |= _BV(1);
		   break;
		   
		   case 4:    //1100
		   PORTB &= !_BV(2);
		   PORTB &= !_BV(3);
		   PORTB |= _BV(0);
		   PORTB |= _BV(1);
		   break;
		   
		   case 5:    //1000
		   PORTB &= !_BV(2);
		   PORTB &= !_BV(3);
		   PORTB &= !_BV(1);
		   PORTB |= _BV(0);
		   break;
		   
		   case 6:    //0010
		   PORTB &= !_BV(3);
		   PORTB &= !_BV(1);
		   PORTB |= _BV(2);
		   PORTB |= _BV(0);
		   break;
		   
		   case 7:    //1010
		   PORTB &= !_BV(3);
		   PORTB &= !_BV(1);
		   PORTB &= !_BV(0);
		   PORTB |= _BV(2);
		   break;
		  
	   }
 
}

/*
  version() returns the version of the library:
*/
int Stepper::version(void)
{
  return 5;
}

void Stepper::motorOff(void)
{
     //digitalWrite(motor_pin_1, LOW);
     //digitalWrite(motor_pin_2, LOW);
     //digitalWrite(motor_pin_3, LOW);
     //digitalWrite(motor_pin_4, LOW);
	  PORTB &= !_BV(0);
	  PORTB &= !_BV(1);
	  PORTB &= !_BV(2);
	  PORTB &= !_BV(3);
}


//This is 4 correct
/* case 0:    // 0011
		   PORTB &= !_BV(0);
		   PORTB &= !_BV(1);
		   PORTB |= _BV(3);
		   PORTB |= _BV(2);

		   break;
		   case 1:    // 0101
           PORTB &= !_BV(0);
		   PORTB &= !_BV(2);
		   PORTB |= _BV(1);
		   PORTB |= _BV(3);
		   
		  
		   break;
		   case 2:    //1100
		  
		   PORTB &= !_BV(2);
		   PORTB &= !_BV(3);
		   PORTB |= _BV(0);
		   PORTB |= _BV(1);

		   break;
		   case 3:    //1010
		   PORTB &= !_BV(3);
		   PORTB &= !_BV(1);
		   PORTB |= _BV(2);
		   PORTB |= _BV(0);
		   
		   break;*/
		  

/*

case 0:    // 1010
PORTB &= !_BV(3);	//digitalWrite(motor_pin_4, LOW);11
PORTB &= !_BV(1);	//digitalWrite(motor_pin_2, LOW);8
PORTB |= _BV(0);	//digitalWrite(motor_pin_1, HIGH);10
PORTB |= _BV(2);	//digitalWrite(motor_pin_3, HIGH);9

break;
case 1:    // 0110

PORTB &= !_BV(0);	//digitalWrite(motor_pin_1, LOW);10
PORTB &= !_BV(3);	//digitalWrite(motor_pin_4, LOW);11
PORTB |= _BV(1);	//digitalWrite(motor_pin_2, HIGH);8
PORTB |= _BV(2);	//digitalWrite(motor_pin_3, HIGH);9
break;
case 2:    //0101

PORTB &= !_BV(0);//digitalWrite(motor_pin_1, LOW);10
PORTB &= !_BV(2);//digitalWrite(motor_pin_3, LOW);9
PORTB |= _BV(3);//digitalWrite(motor_pin_4, HIGH);11
PORTB |= _BV(1);//digitalWrite(motor_pin_2, HIGH);8

break;
case 3:    //1001
PORTB &= !_BV(1);//digitalWrite(motor_pin_2, LOW);8
PORTB &= !_BV(2);//digitalWrite(motor_pin_3, LOW);9
PORTB |= _BV(0);//digitalWrite(motor_pin_1, HIGH);10
PORTB |= _BV(3);//digitalWrite(motor_pin_4, HIGH);11
break;

*/

/*
 case 0:    // 1010
 PORTB &= !_BV(3);	//digitalWrite(motor_pin_4, l);
 PORTB &= !_BV(0);	//digitalWrite(motor_pin_1, l);
 PORTB |= _BV(1);	//digitalWrite(motor_pin_2, h);
 PORTB |= _BV(2);	//digitalWrite(motor_pin_3, h);
 
 break;
 case 1:    // 0110
 
 
 PORTB &= !_BV(3);	//digitalWrite(motor_pin_4, l);
 PORTB &= !_BV(2);	//digitalWrite(motor_pin_3, l);
 PORTB |= _BV(1);	//digitalWrite(motor_pin_2, h);
 PORTB |= _BV(0);	//digitalWrite(motor_pin_1, h);
 break;
 case 2:    //0101
 
 
 PORTB &= !_BV(2);//digitalWrite(motor_pin_3, LOW);
 PORTB &= !_BV(1);//digitalWrite(motor_pin_2, l);
 PORTB |= _BV(3);//digitalWrite(motor_pin_4, HIGH);
 PORTB |= _BV(0);//digitalWrite(motor_pin_1, h);
 
 
 break;
 case 3:    //1001
 PORTB &= !_BV(1);//digitalWrite(motor_pin_2, l);
 PORTB &= !_BV(0);//digitalWrite(motor_pin_1, l);
 PORTB |= _BV(2);//digitalWrite(motor_pin_3, h);
 PORTB |= _BV(3);//digitalWrite(motor_pin_4, HIGH);
 break;
*/