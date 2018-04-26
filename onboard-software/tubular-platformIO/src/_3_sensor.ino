/**************************************************


***************************************************/
#include "_3_sensor.h"
#include <SD.h>
#include <Wire.h>
#include <M2M_LM75A.h>
#include <MS5611.h>
#include <MS5xxx.h>
#include <AWM43300V.h>
#include <HDC2010.h> //humidity sensor lib


#include "_6_telemetry.h"
#include "_1_init.h"
MS5xxx pressSensor(&Wire); //Ambient Pressure Sensor
MS5xxx pressSensor2(&Wire); //Ambient Pressure Sensor
MS5xxx pressSensor3(&Wire); //Ambient Pressure Sensor
MS5xxx pressSensor4(&Wire); //ValveCenter Pressure Sensor
MS5xxx pressSensor5(&Wire); //ValveCenter Pressure Sensor
MS5xxx pressSensor6(&Wire); //ValveCenter Pressure Sensor
HDC2010 humSensor(hdcADDR);
M2M_LM75A tempSensor;
M2M_LM75A tempSensor2;
M2M_LM75A tempSensor3;
M2M_LM75A tempSensor4;
M2M_LM75A tempSensor5;
M2M_LM75A tempSensor6;
M2M_LM75A tempSensor7;
M2M_LM75A tempSensor8;
M2M_LM75A tempSensor9;
AWM43300V afSensor(airFsensorPin);

int samplingRate;

void pressSensorread()
{
  pressSensor.ReadProm();
  pressSensor.Readout();
  pressSensor2.ReadProm();
  pressSensor2.Readout();
  pressSensor3.ReadProm();
  pressSensor3.Readout();
  pressSensor4.ReadProm();
  pressSensor4.Readout();
  pressSensor5.ReadProm();
  pressSensor5.Readout();
  pressSensor6.ReadProm();
  pressSensor6.Readout();
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


void initSensor()
{
   initSampler();
   initHumSensor();
   SD.begin(sdCS);
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

   //float tempPressure[nrPressSensors];
   //int count=0;
   float pressDifference = 0;
   float curPressureMeasurement[nrPressSensors];
   float curTemperatureMeasurement[nrTempSensors];
   float curHumMeasurement[nrHumidSensors];
   float curAFMeasurement[nrAirFSensors];
   float meanPressureAmbient;
   int currSamplingRate;

   static BaseType_t xHigherPriorityTaskWoken;
   TickType_t xLastWakeTime;
   xLastWakeTime = xTaskGetTickCount ();

   while(1)
   {
      
      xHigherPriorityTaskWoken = pdFALSE;
      uint8_t currMode = getMode();

      pressSensorread();

      /*read humidity from HDC*/
      //float temperatureHDC = humSensor.readTemp();
      curHumMeasurement[0] = humSensor.readHumidity();

      /*Read pressure from sensors*/
      curPressureMeasurement[0] = pressSensor.GetPres();
      curPressureMeasurement[1] = pressSensor2.GetPres();   
      curPressureMeasurement[2] = pressSensor3.GetPres();
      curPressureMeasurement[3] = pressSensor4.GetPres();
      curPressureMeasurement[4] = pressSensor5.GetPres();   
      curPressureMeasurement[5] = pressSensor6.GetPres();

      /*Read temperature from sensors*/
      curTemperatureMeasurement[0] = tempSensor.getTemperature();
      curTemperatureMeasurement[1] = tempSensor2.getTemperature();
      curTemperatureMeasurement[2] = tempSensor3.getTemperature();
      curTemperatureMeasurement[3] = tempSensor4.getTemperature();
      curTemperatureMeasurement[4] = tempSensor5.getTemperature();
      curTemperatureMeasurement[5] = tempSensor6.getTemperature();
      curTemperatureMeasurement[6] = tempSensor7.getTemperature();
      curTemperatureMeasurement[7] = tempSensor8.getTemperature();
      curTemperatureMeasurement[8] = tempSensor9.getTemperature();

      /*Read airflow from sensor*/
      curAFMeasurement[0] = afSensor.getAF();

      /*Save pressure measurements*/
      writeData(curPressureMeasurement, 2);

      /*Save temperature measurements*/
      writeData(curTemperatureMeasurement, 0);

      /*Save humidity measurements*/
      writeData(curHumMeasurement, 1);

      /*Save humidity measurements*/
      writeData(curAFMeasurement, 3);
      
      /*Save all data to SD*/
      savingFileToSD(curTemperatureMeasurement, curHumMeasurement, curPressureMeasurement);

      meanPressureAmbient = (curPressureMeasurement[0]+curPressureMeasurement[1])/2;
      
      /*Calculating Pressure Difference*/
      calculatingPressureDifference(meanPressureAmbient);

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

      /*Transmit telemetry to GS*/
      transmit();
      
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

void savingFileToSD(float temperatureData[], float humData[], float pressData[])
{
  String dataString = "";
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  for (int i = 0; i < nrTempSensors; i++)
  {
    dataString += String(temperatureData[i]);
    if (i == (nrTempSensors - 1))
    {
      dataString += "||";
    }
  }
  for (int i = 0; i < nrHumidSensors; i++)
  {
    dataString += String(humData[i]);
    if (i == (nrHumidSensors - 1))
    {
      dataString += "||";
    }
  }
  for (int i = 0; i < nrPressSensors; i++)
  {
    dataString += String(pressData[i]);
    if (i == (nrPressSensors - 1))
    {
      dataString += "||";
    }
  }
  dataFile.println(dataString);
  dataFile.close();
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
   writeDataToSensorBuffers(curMeasurements, type);
   xSemaphoreGive(sem);
}

std::vector<float> readData(int type)
{
  std::vector<float> dataFromBuffer(nrTempSensors);
  xSemaphoreTake(sem, portMAX_DELAY);
  readDataFromSensorBuffers(type);
  xSemaphoreGive(sem);
}

void initTempSensors()
{
  tempSensor.begin();
  tempSensor2.begin();
  tempSensor3.begin();
  tempSensor4.begin();
  tempSensor5.begin();
  tempSensor6.begin();
  tempSensor7.begin();
  tempSensor8.begin();
  tempSensor9.begin();
}





