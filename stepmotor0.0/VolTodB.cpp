/*
 * CPPFile1.cpp
 *
 * Created: 7/18/2015 12:15:03 PM
 *  Author: 12132_000
 */ 

#include "VolTodB.h"
#include <stdlib.h>

/*double static k[]={-9.0813,-9.1250,-9.3145,-8.7932,-8.4170,-7.4241,-7.1714,-6.7448,-6.0808 ,-7.3536,-7.8850,-8.2806,-7.5702,-8.1768,-8.5203,-7.7763,-7.9440,-7.7021,-7.9900,-7.8245,-8.3325,-8.9753,-8.6513,-8.4263,-8.7839,-9.8709,-8.8915};
double static t[]={6.2364,6.2740,6.4810,5.9996,5.5852,4.3105,3.9547,3.3000,2.1832,4.5256,5.5160,6.3275,4.8400,6.2129,7.0910,5.2258,5.6522,4.9654,5.8438,5.3193,6.9737,9.1212,8.0369,7.2034,8.4323,12.5096,8.7266};
double static voltagePoint[] ={0,0.801,0.927,1.05,1.175,1.3005,1.425,1.5495,1.6725,1.797,1.9155,2.04,2.166,2.2905,2.42,2.541,2.6655,2.788,2.909,3.03,3.1545,3.279,3.4015,3.526,3.6495,3.7695,3.8895, 4.0};
double static dBPoint[]={0,-1.035,-2.17,-3.25,-4.31,-5.333,-6.26,-7.155,-7.98,-8.725,-9.595,-10.565,-11.58,-12.515,-13.535,-14.54,-15.515,-16.495,-17.415,-18.375,-19.34,-20.325,-21.4,-22.48,-23.585,-24.705,-25.87,-26.5};*/

double static voltagePoint[] = {0,0.927,1.05, 1.175, 1.697, 1.893, 2.4915, 2.616, 2.885, 3.0075,3.123,3.247,3.371,3.496,3.615,3.7375,3.846};
double static k[]={-9.5238, -9.5238, -9.5652, -7.5000, -5.7692, -7.7778, -6.3333, -8.3333,-6.5385,-7.5555,-7.8164,-8.4284,-8.1759,-8.7927,-7.9222,-9.4167};
double static t[]={6.5429, 6.6814, 6.8370, 4.4900, 1.6438, 5.3033, 1.5770, 6.6900, 1.6054,4.8079,5.6471,7.6412,6.8137,8.9675,5.8061,11.3685};
double static dBPoint[]={0,-2.17, -3.25, -4.31, -8.135, -9.395, -14.14, -15.105, -17.255, -18.185,-18.785,-19.75,-20.765,-21.775,-22.83,-23.83,-24.84};

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
			return voltage;
		}
	}
	}
	return voltage;
	
}





/*
double static k[]={-9.0813,-9.1250,-9.3145,-8.7932,-8.4170,-7.4241,-7.1714,-6.7448,-6.0808 ,-7.3536,-7.8850,-8.2806,-7.5702,-8.1768,-8.5203,-7.7763,-7.9440,-7.7021,-7.9900,-7.8245,-8.3325,-8.9753,-8.6513,-8.4263,-8.7839,-9.8709,-8.8915};
double static t[]={6.2364,6.2740,6.4810,5.9996,5.5852,4.3105,3.9547,3.3000,2.1832,4.5256,5.5160,6.3275,4.8400,6.2129,7.0910,5.2258,5.6522,4.9654,5.8438,5.3193,6.9737,9.1212,8.0369,7.2034,8.4323,12.5096,8.7266};
double static voltagePoint[] ={0,0.801,0.927,1.05,1.175,1.3005,1.425,1.5495,1.6725,1.797,1.9155,2.04,2.166,2.2905,2.42,2.541,2.6655,2.788,2.909,3.03,3.1545,3.279,3.4015,3.526,3.6495,3.7695,3.8895, 4.0};
double static dBPoint[]={0,-1.035,-2.17,-3.25,-4.31,-5.333,-6.26,-7.155,-7.98,-8.725,-9.595,-10.565,-11.58,-12.515,-13.535,-14.54,-15.515,-16.495,-17.415,-18.375,-19.34,-20.325,-21.4,-22.48,-23.585,-24.705,-25.87,26.5};
	
//voltagePoint is the separating point between successive intervals，whose value is the average of the tail of interval n and interval n+1;
double static voltagePoint[] = {0,0.802,0.9285,1.055,1.1795,1.3035,1.432,1.558,1.682,1.81,1.931,2.047,2.18,2.305,2.43,2.56,2.67,2.81,2.93,3.055,3.18,3.31,3.43,3.56,3.68,3.8,3.925,4.055};
//coefficient
double static k[]={-8.4383,-9.0240,-9.3759,-8.8337,-8.8376,-8.2690,-7.2632,-6.8190,-6.7341,-5.8107,-8.6300, -9.3397, -8.9357,-8.6917, -9.4855,-8.3984,-9.0112, -8.3600,-7.6434,-8.1179, -8.0753,-8.7957,-9.1952,-9.1295,-9.2685,-9.6407, -11.3759, -10.0213};
//intercept
double static t[]={5.7987,6.1970,6.4794,5.9629,5.9792,5.3368,4.0244,3.3762,3.2552,1.7153,6.8552,8.2216,7.3785,6.8769,8.6709,5.9893,7.5672,5.8135,3.7710,5.1349,5.0075,7.3585,8.6997,8.4799,9.0063,10.4154,16.9396,11.7330};
//dBPoint is the break point between successive intervals 
double static dBPoint[]={0,-1.05,-2.22,-3.345,-4.42,-5.44,-6.375,-7.24,-8.055,-8.8,-9.81,-10.955,-12.095,-13.21,-14.395,-15.515,-16.61,-17.675,-18.65,-19.675,-20.65,-21.71,-22.83,-23.98,-25.095,-26.29,-27.615,-28.925};*/