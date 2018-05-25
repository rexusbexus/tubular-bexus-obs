#ifndef _1_INIT_H
#define _1_INIT_H
#include <FreeRTOS_ARM.h>
#include <RTCDue.h>



extern SemaphoreHandle_t sem;
extern SemaphoreHandle_t semPeriodic;
typedef union {
    float val;
    uint8_t bytes[4];
} floatval;

typedef union {
    uint16_t val;
    uint8_t bytes[2];
} status_bytes;



void initAll();

#endif