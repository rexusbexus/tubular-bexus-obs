#include <AWM43300V.h>

AWM43300V::AWM43300V(const byte pin)
{
	// Arduino analog pin the sensor is connected to
	_pin = pin;
	
}

float AWM43300V::getAF()
{
	int sensVal = analogRead(_pin);
	float AF = 0;
	AF = 2.5*sensVal - 2.5; //Notes: 1 Vdc = 0 SLPM
	return AF;
}