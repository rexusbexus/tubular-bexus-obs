#ifndef _1_INIT_H
#define _1_INIT_H
#include <FreeRTOS_ARM.h>
#include <RTCDue.h>



extern SemaphoreHandle_t sem;
extern SemaphoreHandle_t semPeriodic;
typedef union {
    float val;
    char bytes[4];
} floatval;

typedef union {
    int16_t val;
    byte bytes[2];
} status_bytes;



void initAll();

#endif