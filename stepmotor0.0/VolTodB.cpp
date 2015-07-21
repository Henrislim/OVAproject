/*
 * CPPFile1.cpp
 *
 * Created: 7/18/2015 12:15:03 PM
 *  Author: 12132_000
 */ 

#include "VolTodB.h"
#include <stdlib.h>

double static voltagePoint[] = {0,0.802,0.9285,1.055,1.1795,1.3035,1.432,1.558,1.682,1.81,1.931,2.047,2.18,2.305,2.43,2.56,2.67,2.81,2.93,3.055,3.18,3.31,3.43,3.56,3.68,3.8,3.925,4.055};
double static k[]={-8.4383,-9.0240,-9.3759,-8.8337,-8.8376,-8.2690,-7.2632,-6.8190,-6.7341,-5.8107,-8.6300, -9.3397, -8.9357,-8.6917, -9.4855,-8.3984,-9.0112, -8.3600,-7.6434,-8.1179, -8.0753,-8.7957,-9.1952,-9.1295,-9.2685,-9.6407, -11.3759, -10.0213};
double static t[]={5.7987,6.1970,6.4794,5.9629,5.9792,5.3368,4.0244,3.3762,3.2552,1.7153,6.8552,8.2216,7.3785,6.8769,8.6709,5.9893,7.5672,5.8135,3.7710,5.1349,5.0075,7.3585,8.6997,8.4799,9.0063,10.4154,16.9396,11.7330};
double static dBPoint[]={0,-1.05,-2.22,-3.345,-4.42,-5.44,-6.375,-7.24,-8.055,-8.8,-9.81,-10.955,-12.095,-13.21,-14.395,-15.515,-16.61,-17.675,-18.65,-19.675,-20.65,-21.71,-22.83,-23.98,-25.095,-26.29,-27.615,-28.925};
double initVolDifference=0;

VolTodB::VolTodB(){};
	
double VolTodB::getdB(double voltage){
	
	int size = sizeof(voltagePoint);
	for (int i=0;i<size-1;i++)
	{
		if (voltage>voltagePoint[i]&&voltage<=voltagePoint[i+1])
		{
			return (k[i]*(voltage-initVolDifference)+t[i]);
		}
	}
	
}

void VolTodB::setInitVoltage(double voltage){
	initVolDifference= voltage-0.687;
}

double VolTodB::getVoltage(double dB){
	int size = sizeof(dBPoint);
	double inv_dB=0-dB;
	double voltage=0;
	if (inv_dB==0)
	{
		voltage=((inv_dB-t[0])/k[0]);
	}
	
	else{
	for (int i=0;i<size-1;i++)
	{
		if (inv_dB<dBPoint[i]&&inv_dB>=dBPoint[i+1])
		{
			voltage= ((inv_dB-t[i])/k[i]);
		}
	}
	
	}
	return voltage;
}


