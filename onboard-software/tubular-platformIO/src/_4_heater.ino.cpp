/*
 * Name: Heater object
 * Purpose: To read the temperature sensor for sensorbox
 * and valve sensor and accordingly turn on/off heater.
 * Project: Tubular-Bexus.
 * Authors: Tubular-Bexus software group.
*/
#ifndef UNIT_TEST
#include <SD.h>
#include <ArduinoSTL.h>
#include <vector>
#include "heaterLogic.h"

#include "_4_heater.h"

extern std::vector<float> htrParam;
extern std::vector<float> tempAtHtr;
float htrParameter[4];
//std::vector<char> htr_flag[2];

std::vector<float> processInitialHtrParameters(uint8_t htrParameters[])
{
  std::vector<float> newParameter(4);
  int i = 0; int k = 0;
  int sizeParam = sizeof(htrParameters)/sizeof(uint8_t);
  while(i < sizeParam)
  {
    if (htrParameters[i] != ',')
    {
      newParameter[k] = htrParameters[i] - '0';
      i++; k++;
    }
    else
    {
      i++;
    }
  }
  return newParameter;
}

void initHtrParameters()
{
  File dataParam = SD.open("htrParameters.txt");
  uint8_t htrParameters[] = {dataParam.read()};
  float newParameter[4];
  std::vector<float> newParameterV = {processInitialHtrParameters(htrParameters)};
  for (int scP = 0; scP < 4; scP++)
  {
    newParameter[scP] = newParameterV[scP];
  }
  dataParam.close();
  setHeaterParameter(newParameter);
}


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
void heaterControl(bool htrOne, bool htrTwo)
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
  
  //bool htr1_flag;// = htr_flag(0);
  //bool htr2_flag;// = htr_flag(1);
  
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount ();
  
  while(1)
  {
    //Reads the current mode
    static uint8_t currMode = getMode();
    //Reads the temperature at the two sensors
    tempAtHtr  = readData(0);
 //   tempAtHtr[1]  = *readData(0);//Must make sure to get the correct tempSensors.
  ;;
    struct heater htrflag = heaterCompare();
    heaterControl(htrflag.htr1_flag,htrflag.htr2_flag);

    
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
  initHtrParameters();
  pinMode(htr1_pin, OUTPUT);
  pinMode(htr2_pin, OUTPUT);
  initReadingData();
 // int htrParameter[4];

}

/*
 * End of the heaterControl child object
 */
#endif