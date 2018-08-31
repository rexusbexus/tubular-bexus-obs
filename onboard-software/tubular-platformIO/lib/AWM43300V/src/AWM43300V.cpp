#include <AWM43300V.h>

AWM43300V::AWM43300V(const byte pin) :
{
	// Arduino analog pin the sensor is connected to
	_pin = pin;
}

float AWM43300V::getAF()
{
	uint16_t sensVal = analogRead(_pin);
	float AF = sensVal*3.3/1024//0;
	//AF = 12.184*(sensVal^4) - 108.71*(sensVal^3) + 344.34*(sensVal^2) - 359.86*sensVal + 100;
	return AF;
}