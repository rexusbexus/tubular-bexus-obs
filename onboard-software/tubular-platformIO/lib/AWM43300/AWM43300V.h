#ifndef AWM43300V_h
#define AWM43300V_h

#include <Arduino.h>

class AWM43300V
{
	public:
	AWM43300V(const byte pin);
	float getAF();
	
	private:
	byte _pin;
};



#endif