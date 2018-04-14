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
MS5xxx pressSensor4(&Wire);
MS5xxx pressSensor5(&Wire);
MS5xxx pressSensor6(&Wire);
HDC2010 humSensor(hdcADDR);
M2M_LM75A temptSensor;
M2M_LM75A temptSensor2;
M2M_LM75A temptSensor3;
M2M_LM75A temptSensor4;
M2M_LM75A temptSensor5;
M2M_LM75A temptSensor6;
M2M_LM75A temptSensor7;
M2M_LM75A temptSensor8;
M2M_LM75A temptSensor9;

void initSensor()
{
   initSampler;
   initHumSensor;
   initTemptSensors;
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
   float curTemperatureMeasurement[nrTempSensors];
   int currSamplingRate;
   float curHumMeasurement[nrHumidSensors];

   static BaseType_t xHigherPriorityTaskWoken;
   TickType_t xLastWakeTime;
   xLastWakeTime = xTaskGetTickCount ();

   while(1)
   {
      xHigherPriorityTaskWoken = pdFALSE;
      int8_t currMode = getMode();

      pressSensorread();

      /*read humidity from HDC*/
      curHumMeasurement[0] = humSensor.readHumidity();

      curTemperatureMeasurement[0] = temptSensor.getTemperature();
      curTemperatureMeasurement[1] = temptSensor2.getTemperature();
      curTemperatureMeasurement[2] = temptSensor3.getTemperature();
      curTemperatureMeasurement[3] = temptSensor4.getTemperature();
      curTemperatureMeasurement[4] = temptSensor5.getTemperature();
      curTemperatureMeasurement[5] = temptSensor6.getTemperature();
      curTemperatureMeasurement[6] = temptSensor7.getTemperature();
      curTemperatureMeasurement[7] = temptSensor8.getTemperature();
      curTemperatureMeasurement[8] = temptSensor9.getTemperature();
         
      curPressureMeasurement[0] = pressSensor.GetPres();
      curPressureMeasurement[1] = pressSensor2.GetPres();
      curPressureMeasurement[2] = pressSensor3.GetPres();
      curPressureMeasurement[3] = pressSensor4.GetPres();
      curPressureMeasurement[4] = pressSensor5.GetPres();
      curPressureMeasurement[5] = pressSensor6.GetPres();

      /*Save sensors measurements*/
      writeData(curPressureMeasurement, 2);
      writeData(curTemperatureMeasurement, 0);
      writeData(curHumMeasurement, 1);
      

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

void initTemptSensors()
{
  temptSensor.begin();
  temptSensor2.begin();
  temptSensor3.begin();
  temptSensor4.begin();
  temptSensor5.begin();
  temptSensor6.begin();
  temptSensor7.begin();
  temptSensor8.begin();
  temptSensor9.begin();
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


