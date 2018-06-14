/* Name: _2_mode.ino.cpp
 * Purpose: To manage and determine wich mode the software shall operate in.
 * Project: Tubular-Bexus.
 * Authors: Tubular-Bexus software group.
*/
#ifndef UNIT_TEST
#include "_2_mode.h"
#include <FreeRTOS_ARM.h>
#include "_1_init.h"

uint8_t state;

uint8_t getMode()
{
//    Serial.println("I'm at getMode");
   uint8_t tempt;
   xSemaphoreTake(sem, portMAX_DELAY);
   tempt = state;
   xSemaphoreGive(sem);
//    Serial.println("Leaving getMode");
   return(tempt);
}

void setMode(uint8_t curMode)
{
   xSemaphoreTake(sem, portMAX_DELAY);
   state = curMode;
   xSemaphoreGive(sem);
}

void initMode()
{
   //Serial.println("I'm at initMode");
   setMode(standbyMode);
}

#endif