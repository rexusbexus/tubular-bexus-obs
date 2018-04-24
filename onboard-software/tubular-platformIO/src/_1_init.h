#ifndef _1_INIT_H
#define _1_INIT_H

#include <FreeRTOS_ARM.h>
#include <RTCDue.h>
static SemaphoreHandle_t sem;
static SemaphoreHandle_t semPeriodic;
typedef union {
    float val;
    uint8_t bytes[4];
} floatval;

RTCDue rtc(XTAL);//Init start RTC object.


void initAll();

#endif