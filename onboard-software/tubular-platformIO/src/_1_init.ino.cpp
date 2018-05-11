/**************************************************


***************************************************/
#ifndef UNIT_TEST

#include "_1_init.h"
#include "_2_mode.h"
#include "_3_sensor.h"
#include "_4_heater.h"
#include "_5_asc.h"
#include "_7_telecommand.h"
#include "_8_monitor.h"
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
    /* Change these values to set the current initial time */
    const uint8_t seconds = 0;
    const uint8_t minutes = 0;
    const uint8_t hours = 0;

    /* Change these values to set the current initial date */
    const uint8_t day = 1;
    const uint8_t month = 1;
    const uint16_t year = 2018;

    rtc.begin();
    rtc.setTime(hours,minutes,seconds);
    rtc.setDate(day,month,year);
}    

void initAll()
{
    pinMode(13, OUTPUT);
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