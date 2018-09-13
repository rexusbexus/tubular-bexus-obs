#include <AWM5102VN.h>

AWM5102VN::AWM5102VN(const byte pin)
{
	// Arduino analog pin the sensor is connected to
	_pin = pin;
	
}

float AWM5102VN::getAF()
{
	int sensVal=0;
	//int avgSensVal;
	for (int i=0;i<49;i++){
		sensVal += analogRead(_pin);
		delay(1);
	}
	sensVal = sensVal/49;
	float AF = 0;
	//AF = (10*((float(sensVal)*3.3/1023 - 0.33)) / (2.97)); //Notes: 1 Vdc = 0 SLPM
	AF = (-3.233*pow((float(sensVal)*3.3/1023),2) + 11.2*(float(sensVal)*3.3/1023) -3.525);
	return AF;
}