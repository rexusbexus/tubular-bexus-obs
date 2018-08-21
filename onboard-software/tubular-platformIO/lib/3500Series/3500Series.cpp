#include <3500Series.h>

3500Series::3500Series(const byte pin)
{
	// Arduino analog pin the sensor is connected to
	_pin = pin;
	
}

float 3500Series::getPress()
{
	int sensVal = analogRead(_pin);
	float P = 0;
	P = sensVal/1023; //Notes: 0 Vdc = 0 bar
	return P;
}