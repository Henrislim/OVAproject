/*
 * VolTodB.h
 *
 * Created: 7/18/2015 12:13:22 PM
 *  Author: 12132_000
 */ 


#ifndef VOLTODB_H_
#define VOLTODB_H_

class VolTodB{
	
	public:
	
	VolTodB();
	void setInitVoltage(double voltage);
	double getdB(double voltage);
	double getVoltage(double dB);
	

};



#endif /* VOLTODB_H_ */