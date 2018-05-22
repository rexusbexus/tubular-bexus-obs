/**************************************************


***************************************************/
#ifndef UNIT_TEST
#include "_2_mode.h"
#include <FreeRTOS_ARM.h>
#include "_1_init.h"

uint8_t state;

uint8_t getMode()
{
   uint8_t tempt;
   xSemaphoreTake(sem, portMAX_DELAY);
   tempt = state;
   xSemaphoreGive(sem);
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