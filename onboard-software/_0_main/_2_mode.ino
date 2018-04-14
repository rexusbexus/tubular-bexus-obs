/**************************************************


***************************************************/

#define standbyMode int8_t(0)
#define normalAscent int8_t(1)
#define normalDescent int8_t(2)
#define safeMode int8_t(3)
#define manual int8_t(4)

void initMode()
{

}

int getMode()
{
   int tempt;
   xSemaphoreTake(sem, portMAX_DELAY);
   tempt = state;
   xSemaphoreGive(sem);
   return(tempt);
}

static void setMode(int curMode)
{
   xSemaphoreTake(sem, portMAX_DELAY);
   state = curMode;
   xSemaphoreGive(sem);
}

