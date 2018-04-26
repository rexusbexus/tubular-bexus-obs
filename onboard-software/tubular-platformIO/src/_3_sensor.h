#ifndef _3_SENSOR_H
#define _3_SENSOR_H

#include <ArduinoSTL.h>
#include <vector>
#include "_1_init.h"
#include "_2_mode.h"
#include "sensorManager.h"

#define hdcADDR 0x40
#define msADDR1 0x76
#define msADDR2 0x78

#define pressSensorPSpin TBD
#define sdCS 4

/*
   Pressusre sensors and  will use MOSI-SDI, MISO-SDO, SCK-SCLK, and digital pins-(CSB)chipselect pins.
*/
/*The followings are chipselect pins */
#define pressSensorPin1 37
#define pressSensorPin2 38
#define pressSensorPin3 39
#define pressSensorPin4 40
#define pressSensorPin5 41
#define pressSensorPin6 42
/*Airflow sensor will be connected to pin 43 */
#define airFsensorPin   A10
#define sdPin           10

#define safeModeThreshold 900
#define pressDifferentThresholdneg -20
#define pressDifferentThresholdpos 20

extern int samplingRate;


void initSensor();
static void setSamplingRate(int curSamplingRate);
std::vector<float> readData(int type);

#endif