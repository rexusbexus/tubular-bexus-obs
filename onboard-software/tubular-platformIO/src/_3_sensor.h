#ifndef _3_SENSOR_H
#define _3_SENSOR_H

#include <ArduinoSTL.h>
#include <vector>
// #include "advancedFunctions.h"
#include "_1_init.h"
#include "_2_mode.h"
#include "sensorManager.h"
#include <RTCDue.h>

#define hdcADDR 0x40
#define msADDR1 0x76
#define msADDR2 0x78

#define pressSensorPSpin TBD
#define sdCS 4
#define sdPin           4

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
#define pressSensorPin7 43 //Temperature
#define LED 44
/*Airflow sensor will be connected to pin A0 (analog) */
#define airFsensorPin   0
#define staticPressPin 1


#define safeModeThreshold 880
#define pressDifferentThresholdneg -20
#define pressDifferentThresholdpos 20


void initSensor();
void setSamplingRate(int curSamplingRate);
void writeData(float curMeasurements [], int type);
void savingDataToSD(float temperatureData[], float humData[], float pressData[], float afData[]);
std::vector<float> readData(int type);

#endif
