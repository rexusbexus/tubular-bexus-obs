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
	AF = (float(sensVal)*3.3/1023) - 1) / (2.3); //Notes: 1 Vdc = 0 SLPM
	return AF;
}