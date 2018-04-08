/**************************************************


***************************************************/

#define standbyMode 0
#define normalAscent 1
#define normalDescent 2
#define safeMode 3
#define manual 4

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

