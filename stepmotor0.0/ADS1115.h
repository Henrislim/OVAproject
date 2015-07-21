/*
 * ADS1115.h
 *
 * Created: 7/11/2015 5:05:29 PM
 *  Author: 12132_000
 */ 


#ifndef ADS1115_H_
#define ADS1115_H_
#define ADS1115_ADDRESS				(0x90)
#define ADS1115_REG_POINTER_CONFIG  (0x01)
#define ADS1115_REG_POINTER_CONVERT (0x00)
#define ADS1115_CONF_CH0			((1<<15) | (0<<14) | (0<<13) | (0<<12) | (0<<11) | (0<<10) | (0<<9) | (1<<8) | (1<<7) | (1<<6) | (1<<5) | (0<<4)|(0<<3) | (0<<2) | (1<<1) | (1<<0))


class ADS1115{
	public:
	
	ADS1115();
	void init_ADS1115(void);
	uint16_t readADS1115(void);
	double readVoltage(void);

	
};





#endif /* ADS1115_H_ */