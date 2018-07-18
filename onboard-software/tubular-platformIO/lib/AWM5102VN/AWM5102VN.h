#ifndef AWM5102VN_h
#define AWM5102VN_h

#include <Arduino.h>

class AWM5102VN
{
	public:
	AWM5102VN(const byte pin);
	float getAF();
	
	private:
	byte _pin;
};



#endif