/**************************************************


***************************************************/
#define hdcADDR 0x40
#define msADDR1 0x76
#define msADDR2 0x78

#define pressSensorPSpin TBD

#define safeModeThreshold 900
#define pressDifferentThresholdneg -20
#define pressDifferentThresholdpos 20

Melon_MS5607 pressSensor();
Melon_MS5607 pressSensor2();
HDC2010 humSensor(hdcADDR);

void initSensor()
{
   initSampler;
   initHumSensor;
   initPressSensor;
}

void initSampler()
{
    xTaskCreate(
      sampler
      ,  (const portCHAR *)"Sampler"   // A name just for humans
      ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
      ,  NULL
      ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
      ,  NULL );
}


void sampler(void *pvParameters)
{
   (void) pvParameters;

   float tempPressure[nrPressSensors];
   int count=0;
   float pressDifference;
   float curPressureMeasurement[nrPressSensors];
   float meanPressure;
   int currSamplingRate;

   static BaseType_t xHigherPriorityTaskWoken;
   TickType_t xLastWakeTime;
   xLastWakeTime = xTaskGetTickCount ();

   while(1)
   {
      xHigherPriorityTaskWoken = pdFALSE;
      static int currMode = getMode();

      /*read temperature and humidity from HDC*/
      //float temperatureHDC = humSensor.readTemp();
      float humHDC = humSensor.readHumidity();

       /*read temperature and pressure from MS1*/
      //float tempeMS1 = pressSensor.getTemperature();
      float pressureMS1 = pressSensor.getPressure();

       /*read temperature and pressure from MS2*/
      //float tempeMS2 = pressSensor2.getTemperature();
      float pressureMS2 = pressSensor2.getPressure();      

      curPressureMeasurement[0] = pressureMS1;
      curPressureMeasurement[1] = pressureMS2;
      //curPressureMeasurement[2] = 
      //curPressureMeasurement[3] = 
      //curPressureMeasurement [4] = 
      //curPressureMeasurement[5] = 

      /*Save pressure measurements*/
      writeData(curPressureMeasurement, 2);

      for (int j = 0; j < nrPressSensors; j++)
      {
        meanPressure = meanPressure + curPressureMeasurement[j];
      }
      meanPressure = meanPressure/nrPressSensors;
      
      if (count == 0)
      {
         tempPressure[count] = meanPressure;
         count=1;
      }
      else
      {
         tempPressure[count] = meanPressure;
         pressDifference = tempPressure[1] - tempPressure[0];
         count=0;
      }

      /*Change mode if the condition is satisfied*/
      if (pressDifference<pressDifferentThresholdneg)
      {
         setMode(normalAscent);
      }
      else if (pressDifference>pressDifferentThresholdpos)
      {
         setMode(normalDescent);
      }
      else if (currMode==normalDescent && meanPressure<=safeModeThreshold)
      {
         setMode(safeMode);
      }


      /*Listen to GS*/
      EthernetClient client = server.available();
      if(client.available()>0)
      {
         xSemaphoreGiveFromISR(semPeriodic, &xHigherPriorityTaskWoken );
      }

      /*Check current sampling rate*/
      currSamplingRate = getSamplingRate();
      vTaskDelayUntil(&xLastWakeTime, (currSamplingRate / portTICK_PERIOD_MS) );
   }
}

static void setSamplingRate(int curSamplingRate)
{
   xSemaphoreTake(sem, portMAX_DELAY);
   samplingRate = curSamplingRate;
   xSemaphoreGive(sem);
}

int getSamplingRate(void)
{
   int tempSamplingRate;
   xSemaphoreTake(sem, portMAX_DELAY);
   tempSamplingRate = samplingRate;
   xSemaphoreGive(sem);
   return tempSamplingRate;
   
}

static void writeData(float curMeasurements [], int type)
{
   xSemaphoreTake(sem, portMAX_DELAY);
   
   switch(type) {
      /*Temperature data*/
      case 0 :
      for (int i=0 ; i < nrTempSensors ; i++)
      {
          tempReading[i] = curMeasurements[i];
      }
      break;
      
      /*Humidity data*/
      case 1 :  
      for (int i=0 ; i < 1 ; i++)
      {
          humReading [i] = curMeasurements[i];
      }
      break;

      /*Pressure data*/
      case 2 :
      for (int i=0 ; i < nrPressSensors ; i++)
      {
          pressReading[i] = curMeasurements[i];
      }
      break;
   }
   
   xSemaphoreGive(sem);
}

float* readData(int type)
{
  switch(type){
      /*Reading Temperature*/
      case 0 :
      float dummyTemp [nrTempSensors];
      xSemaphoreTake(sem, portMAX_DELAY);
      for (int i=0 ; i < nrTempSensors ; i++)
      {
          dummyTemp[i] = tempReading[i];
      }
      xSemaphoreGive(sem);
      return dummyTemp;
      break;
      
      /*Reading Humidity*/
      case 1 :
      float dummyHum [nrHumidSensors];
      xSemaphoreTake(sem, portMAX_DELAY);
      for (int i=0 ; i < nrHumidSensors ; i++)
      {
          dummyHum[i] = humReading[i];
      }
      xSemaphoreGive(sem);
      return dummyHum;
      break;
      
      /*Reading Pressure*/
      case 2 :
      float dummyPress [nrPressSensors];
      xSemaphoreTake(sem, portMAX_DELAY);
      for (int i=0 ; i < nrPressSensors ; i++)
      {
          dummyPress[i] = pressReading[i];
      }
      xSemaphoreGive(sem);
      return dummyPress;
      break;

      /* Reading Airflow*/
      case 3 :
      float dummyAf [1];
      xSemaphoreTake(sem, portMAX_DELAY);
      for (int i=0 ; i < 1 ; i++)
      {
          dummyAf[i] = afReading[i];
      }
      xSemaphoreGive(sem);
      return dummyAf;
      break;
  }
  

}


void initHumSensor()
{
    
   // Initialize I2C communication
   humSensor.begin();
    
   // Begin with a device reset
   humSensor.reset();

   // Configure Measurements
   humSensor.setMeasurementMode(TEMP_AND_HUMID);  // Set measurements to temperature and humidity
   humSensor.setRate(FIVE_HZ);                     // Set measurement frequency to 1 Hz
   humSensor.setTempRes(FOURTEEN_BIT);
   humSensor.setHumidRes(FOURTEEN_BIT);

   //begin measuring
   humSensor.triggerMeasurement();
}

//float* hdc2010Read()
//{
//   float temperatureHDC = humSensor.readTemp();
//   float humHDC = humSensor.readHumidity();
//   static float measurements [2] = { temperatureHDC, humHDC};
//
//   return measurements;
//}

void initPressSensor()
{
   pressSensor.reset();
   pressSensor.begin(msADDR1);
   pressSensor.setOversamplingRate(MS5607_OSR2048);

   pressSensor2.reset();
   pressSensor2.begin(msADDR2);
   pressSensor2.setOversamplingRate(MS5607_OSR2048);
}


