#ifndef _5_ASC_H
#define _5_ASC_H

#include <ArduinoSTL.h>
#include <vector>
// #include "advancedFunctions.h"

#include "_1_init.h"
#include "_3_sensor.h"

/*
 * Pin 20, 21 reserved for SDA and SCL for I2C connection with temperature sensors and humidity sensor
 */
#define pumpPin 22
#define valve1 28
#define valve2 25
#define valve3 27
#define valve4 23
#define valve5 26
#define valve6 24
#define valve7 29
#define valve8 30
#define valve9 31
#define valve10 32
#define flushValve 33
#define CACvalve 34

#define closeState 0
#define openState 1
#define totalBagNumber 6
#define flushingTime 60

//extern float ascParameter[16];
void initASC();
void setASCParameter(float newParameter[16]);
void samplingScheduler(int whichBag, float newParameter[]);
void valvesControl(int valve, int cond);
void pumpControl(int cond);
int getValvePin(int whichBag);
int getCurrentTime();

#endif