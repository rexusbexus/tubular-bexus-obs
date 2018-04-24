/**************************************************


***************************************************/
#include "_2_mode.h"

void initMode()
{
   setMode(standbyMode);
}

int getMode()
{
   int tempt;
   xSemaphoreTake(sem, portMAX_DELAY);
   tempt = state;
   xSemaphoreGive(sem);
   return(tempt);
}

static void setMode(int8_t curMode)
{
   xSemaphoreTake(sem, portMAX_DELAY);
   state = curMode;
   xSemaphoreGive(sem);
}

