#ifndef SERIES3500_H
#define SERIES3500_H

#include <Arduino.h>

class Series3500
{
	public:
	Series3500(const byte pin);
	float getPress();
	
	private:
	byte _pin;
};



#endif