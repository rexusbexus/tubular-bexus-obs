/*
 * Name: Heater object
 * Purpose: To read the temperature sensor for sensorbox
 * and valve sensor and accordingly turn on/off heater.
 * Project: Tubular-Bexus.
 * Authors: Tubular-Bexus software group.
*/



void initHeater() {
  
  initReadingData();
  //int htrParameter[4];
}

void initReadingData() {
    xTaskCreate(
    readingData
    ,  (const portCHAR *) "readingData"   // Name
    ,  128  // This stack size 
    ,  NULL
    ,  2  // Priority
    ,  NULL );

}

/* readingData child object
 * readingData reads the temperature data at valve center and electronics box.
 * Turn on/off the two heaters accoring to parameters.
 */
void readingData(void *pvParameters)
{
  (void) pvParameters;
  float tempAtHtr [2];
  
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount ();
  
  while(1)
  {
    //Reads the current mode
    static int currMode = getMode();
    //Reads the temperature at the two sensors
    tempAtHtr[0]  = readData(0);
    tempAtHtr[1]  = readData(0);//Must make sure to get the correct tempSensors and add pointer.
    bool  htr1_flag;
    bool  htr2_flag;
    int *htrParam;

    //Compares with parameters
    if (tempAtHts[0]=<0 || tempAtHts[0]=>16777215 || tempAtHts[1]=<0 || tempAtHts[1]=>16777215)
    {
      /*
       * Check if within correct value of parameters
       * Also works for errors in tempAtHts[]
       */
    }    
    else if (*(htrParam+0)<tempAtHts[0])
    {
        htr1_flag = 1;
    }
    else if (*(htrParam+1)>tempAtHts[0])
    {
        htr1_flag = 0;
    }
    else if (*(htrParam+2)<tempAtHts[1])
    {
        htr2_flag = 1;
    }
    else if (*(htrParam+3)>tempAtHts[1])
    {
        htr2_flag = 0;
    }
    
    if htr1_flag ^^ htr2_flag  // if 0 1 or 1 0
    {
      heaterControl(htr1_flag,htr1_flag);
    }
    else if htr1_flag && htr2_flag // if 1 1
    {
      /*
       * If both heaters require to be turned on it will choose the one most
       * largest difference between their paramters and actual temperature.
       */
      if (*(htrParam+0) - tempAtHts[0])>( *(htrParam+2) - tempAtHts[1])
      {
        heaterControl(1,0);
      }
      else if (*(htrParam+0) - tempAtHts[0])<( *(htrParam+2) - tempAtHts[1])
      {
        heaterControl(0,1);
      }
      else // if 0 0
      {
        heaterControl(htr1_flag,htr1_flag);
        /*
        * The heaters does not need to operate
        */
      }
      
    } 
    vTaskDelayUntil(&xLastWakeTime, (5000 / portTICK_PERIOD_MS) ); 
  }
    
}
/*
 * END of readingData child object
 */


/* The parameter child object.
 *  int * heaterParameter returns the pointer
 *  to htrParam.
 */
int * readHeaterParameter()
{
  static int htrParam[4];
  xSemaphoreTake(sem, portMAX_DELAY);
  htrParam = htrParameter;
  xSemaphoreGive(sem);    
  return htrParam;
}

/* setHeaterParameter has an array of four(4) 
 *  elemnets and will replace the htrParameter array
 *  with new values
 */
void setHeaterParameter(int newParameter[4])
{
  xSemaphoreTake(sem, portMAX_DELAY);
  htrParameter=newParameter;
  xSemaphoreGive(sem);
}
/*
 * END of parameter child object
 */

/*
 * The heater control child object
 * 
 */
void heaterControl(int htrOne, int htrTwo)
{
  xSemaphoreTake(sem, portMAX_DELAY);
  digitalWrite(htr1_pin,htrOne)
  digitalWrite(htr2_pin,htrTwo)
  xSemaphoreGive(sem);    
}
/*
 * End of the heaterControl child object
 */
