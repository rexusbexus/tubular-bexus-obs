/*
 * Name: Heater object
 * Purpose: To read the temperature sensor for sensorbox
 * and valve sensor and accordingly turn on/off heater.
 * Project: Tubular-Bexus.
 * Authors: Tubular-Bexus software group.
*/
#ifndef UNIT_TEST
#include <ArduinoSTL.h>
#include <vector>

#include "_4_heater.h"

std::vector<float> htrParam(4);
float htrParameter[4];


/*
 * END of readingData child object
 */


/* The parameter child object.
 *  int * heaterParameter returns the pointer
 *  to htrParam.
 */
void readHeaterParameter()
{
  int i=0;
  xSemaphoreTake(sem, portMAX_DELAY);
  //htrParam = htrParameter;
  for ( i = 0; i < 3; i++) {
      htrParam[i] = htrParameter[i];
   }
  xSemaphoreGive(sem);    
  //return htrParam;
}

/* setHeaterParameter has an array of four(4) 
 *  elemnets and will replace the htrParameter array
 *  with new values
 */
void setHeaterParameter(float newParameter[4])
{
  xSemaphoreTake(sem, portMAX_DELAY);
  htrParameter[0]=(newParameter[0]);//,*newParameter+1,*newParameter+2,*newParameter+3);
  htrParameter[1]=(newParameter[1]);
  htrParameter[2]=(newParameter[2]);
  htrParameter[3]=(newParameter[3]);
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
  digitalWrite(htr1_pin,htrOne);
  digitalWrite(htr2_pin,htrTwo);
  xSemaphoreGive(sem);    
}



/* readingData child object
 * readingData reads the temperature data at valve center and electronics box.
 * Turn on/off the two heaters accoring to parameters.
 */
void readingData(void *pvParameters)
{
  (void) pvParameters;
  std::vector<float> tempAtHtr(2);
  bool  htr1_flag;
  bool  htr2_flag;
  
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount ();
  
  while(1)
  {
    //Reads the current mode
    static uint8_t currMode = getMode();
    //Reads the temperature at the two sensors
    tempAtHtr  = readData(0);
 //   tempAtHtr[1]  = *readData(0);//Must make sure to get the correct tempSensors and add pointer.
    
    

    //Compares with parameters
    if (tempAtHtr[0]<=0 || tempAtHtr[0]==16777215 || tempAtHtr[1]<=0 || tempAtHtr[1]>=16777215)
    {
      /*
       * Check if within correct value of parameters
       * Also works for errors in tempAtHts[]
       */
    }    
    else if (htrParam[0]<tempAtHtr[0])
    {
        htr1_flag = 1;
    }
    else if ((htrParam[1])>tempAtHtr[0])
    {
        htr1_flag = 0;
    }
    else if ((htrParam[2])<tempAtHtr[1])
    {
        htr2_flag = 1;
    }
    else if ((htrParam[3])>tempAtHtr[1])
    {
        htr2_flag = 0;
    }
    
    if (htr1_flag ^ htr2_flag)  // if 0 1 or 1 0
    {
      heaterControl(htr1_flag,htr1_flag);
    }
    else if (htr1_flag && htr2_flag) // if 1 1
    {
      /*
       * If both heaters require to be turned on it will choose the one most
       * largest difference between their paramters and actual temperature.
       */
      if (((htrParam[0]) - tempAtHtr[0])>( (htrParam[2]) - tempAtHtr[1]))
      {
        heaterControl(1,0);
      }
      else if (((htrParam[0]) - tempAtHtr[0])<( (htrParam[0]) - tempAtHtr[1]))
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

void initReadingData() {
    xTaskCreate(
    readingData
    ,  (const portCHAR *) "readingData"   // Name
    ,  128  // This stack size 
    ,  NULL
    ,  2  // Priority
    ,  NULL );

}

void initHeater() {
  
  initReadingData();
 // int htrParameter[4];

}

/*
 * End of the heaterControl child object
 */
#endif