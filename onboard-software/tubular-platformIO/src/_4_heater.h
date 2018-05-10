#ifndef _4_HEATER_H
#define _4_HEATER_H

#define htr1_pin 35
#define htr2_pin 36

#include <Arduino.h>
#include "advancedFunctions.h"

#include "_1_init.h"
#include "_2_mode.h"
#include "_3_sensor.h"

void initHeater();
void setHeaterParameter(float newParameter[4]);
void heaterControl(bool htrOne, bool htrTwo);

#endif