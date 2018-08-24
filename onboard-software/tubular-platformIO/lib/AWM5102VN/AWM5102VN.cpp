#include <AWM5102VN.h>

AWM5102VN::AWM5102VN(const byte pin)
{
	// Arduino analog pin the sensor is connected to
	_pin = pin;
	
}

float AWM5102VN::getAF()
{
	int sensVal = analogRead(_pin);
	float AF = 0;
	AF = 2.5*(float(sensVal)*5/1023) - 2.5; //Notes: 1 Vdc = 0 SLPM
	return AF;
}