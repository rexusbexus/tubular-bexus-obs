/**************************************************


***************************************************/
#define hdcADDR 0x40
#define msADDR1 0x76
#define msADDR2 0x78

#define pressSensorPSpin TBD

#define safeModeThreshold 900
#define pressDifferentThresholdneg -20
#define pressDifferentThresholdpos 20

MS5xxx pressSensor(&Wire);
MS5xxx pressSensor2(&Wire);
MS5xxx pressSensor3(&Wire);
HDC2010 humSensor(hdcADDR);
M2M_LM75A tempSensor;

void initSensor()
{
   initSampler;
   initHumSensor;
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
   float meanPressureAmbient;
   int currSamplingRate;

   static BaseType_t xHigherPriorityTaskWoken;
   TickType_t xLastWakeTime;
   xLastWakeTime = xTaskGetTickCount ();

   while(1)
   {
      xHigherPriorityTaskWoken = pdFALSE;
      int8_t currMode = getMode();

      pressSensorread();

      /*read temperature and humidity from HDC*/
      //float temperatureHDC = humSensor.readTemp();
      float humHDC = humSensor.readHumidity();

       /*read temperature and pressure from MS1*/
      //float tempeMS1 = pressSensor.getTemperature();
      float pressureMS1 = pressSensor.GetPres();

       /*read temperature and pressure from MS2*/
      //float tempeMS2 = pressSensor2.getTemperature();
      float pressureMS2 = pressSensor.GetPres();   

      /*read temperature and pressure from MS2*/
      float pressureMS3 = pressSensor3.GetPres();
         
      curPressureMeasurement[0] = pressureMS1;
      curPressureMeasurement[1] = pressureMS2;
      curPressureMeasurement[2] = pressureMS3;
      //curPressureMeasurement[3] = 
      //curPressureMeasurement [4] = 
      //curPressureMeasurement[5] = 

      /*Save pressure measurements*/
      writeData(curPressureMeasurement, 2);

      meanPressureAmbient = (curPressureMeasurement[0]+curPressureMeasurement[1])/2;
      
      if (count == 0)
      {
         tempPressure[count] = meanPressureAmbient;
         count=1;
      }
      else
      {
         tempPressure[count] = meanPressureAmbient;
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
      else if (currMode==normalDescent && meanPressureAmbient<=safeModeThreshold)
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

std::vector<float> readData(int type)
{
  std::vector<float> dummyTemp(nrTempSensors);
  std::vector<float> dummyHum(nrHumidSensors);
  std::vector<float> dummyPress(nrPressSensors);
  std::vector<float> dummyAf(1);
  switch(type){
      /*Reading Temperature*/
      case 0 :
      
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

void pressSensorread()
{
  pressSensor.ReadProm();
  pressSensor.Readout();
  pressSensor2.ReadProm();
  pressSensor2.Readout();
  pressSensor3.ReadProm();
  pressSensor3.Readout();
}


