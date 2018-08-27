#include <Series3500.h>

Series3500::Series3500(const byte pin)
{
	// Arduino analog pin the sensor is connected to
	_pin = pin;
	
}

float Series3500::getPress()
{
	int sensVal = analogRead(_pin);
	float P = 0;
	P = float(sensVal)*1000/1023; //Notes: 0 Vdc = 0 bar
	return P;
}