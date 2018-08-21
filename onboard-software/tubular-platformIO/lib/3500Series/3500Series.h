#ifndef 3500Series_h
#define 3500Series_h

#include <Arduino.h>

class 3500Series
{
	public:
	3500Series(const byte pin);
	float getPress();
	
	private:
	byte _pin;
};



#endif