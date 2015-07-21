/*
  Stepper.h - - Stepper library for Wiring/Arduino - Version 0.5
  
  Original library     (0.1) by Tom Igoe.
  Two-wire modifications   (0.2) by Sebastian Gassner
  Combination version   (0.3) by Tom Igoe and David Mellis
  Bug fix for four-wire   (0.4) by Tom Igoe, bug fix from Noah Shibley
  Half Step     for four-wire (0.5) by John Zandbergen 20091104

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

  Step C0 C1 C2 C3
     1  1  0  1  0
     2  0  1  1  0
     3  0  1  0  1
     4  1  0  0  1

  The sequence of halfstep control signals for 4 control wires is as follows:
  Step C0 C1 C2 C3
     1  1  0  1  0
     2  0  0  1  0
     3  0  1  1  0
     4  0  1  0  0
     5  0  1  0  1
     6  0  0  0  1
     7  1  0  0  1
     8  1  0  0  0

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

// ensure this library description is only included once
#ifndef Stepper_h
#define Stepper_h

// library interface description
class Stepper {
  public:
    
    Stepper();
	
    // speed setter method:
    void setSpeed(long whatSpeed);

    // mover method:
    void step(int number_of_steps);

    int version(void);
	
	void motorOff(void);

  private:
    void stepMotor(int this_step);
    
    int direction;               // Direction of rotation
    int speed;                   // Speed in RPMs
    unsigned long step_delay;    // delay between steps, in ms, based on speed
    int number_of_steps;         // total number of steps this motor can take
    int pin_count;               // whether you're driving the motor with 2 or 4 pins
    int step_number;             // which step the motor is on
    int use_half_step;           // 1 when the stepper motor is to be driven with half steps (only 4-wire)
    
  
    
    long last_step_time;      // time stamp in ms of when the last step was taken
};

#endif