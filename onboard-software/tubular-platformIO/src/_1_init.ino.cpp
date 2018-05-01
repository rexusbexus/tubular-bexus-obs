/**************************************************


***************************************************/
#ifndef UNIT_TEST

#include "_1_init.h"
#include "_2_mode.h"
#include "_3_sensor.h"
#include "_4_heater.h"
#include "_5_asc.h"
#include "_7_telecommand.h"
#include "ethernet.h"

SemaphoreHandle_t sem;
SemaphoreHandle_t semPeriodic;
ethernet ethernet;
RTCDue rtc(XTAL);

void initSemaphore()
{
    sem = xSemaphoreCreateMutex();
    semPeriodic = xSemaphoreCreateBinary();
}

void initRTC()
{
    
    rtc.begin();
    rtc.setTime(0,0,0);
    rtc.setDate(1,1,2018);
}    

void initAll()
{
    ethernet.initEthernet();
    initSemaphore();
    initRTC();
    initMode();
    initSensor();
    initHeater();
    initASC();
    initTelecommand();
    
}

#endif