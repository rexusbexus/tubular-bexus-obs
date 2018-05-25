/**************************************************


***************************************************/
#ifndef UNIT_TEST
#include "_3_sensor.h"
#include <SD.h>
#include <Wire.h>
#include <M2M_LM75A.h>
// #include <MS5611.h>
// #include <MS5xxx.h>
#include <AWM43300V.h>
#include <HDC2010.h> //humidity sensor lib
#include <Ethernet2.h>
#include <MS5607.h>
#include <FreeRTOS_ARM.h>


#include "ethernet.h"
#include "_1_init.h"
#include "_5_asc.h"
#include "_6_telemetry.h"
#include "_8_monitor.h"
#include "simulation.h"

extern ethernet ethernet;
extern RTCDue rtc;

MS5607 pressSensor(pressSensorPin1); //Ambient Pressure Sensor
MS5607 pressSensor2(pressSensorPin2); //Ambient Pressure Sensor
MS5607 pressSensor3(pressSensorPin3); //Ambient Pressure Sensor
MS5607 pressSensor4(pressSensorPin4); //ValveCenter Pressure Sensor
MS5607 pressSensor5(pressSensorPin5); //ValveCenter Pressure Sensor
MS5607 pressSensor6(pressSensorPin6); //ValveCenter Pressure Sensor
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

pressureSimulation sim_data;
 
bool simulationOrNot;
extern SemaphoreHandle_t sem;
int samplingRate = 1000;

void pressSensorread()
{
  int i = 0;
  pressSensor.readADC_calc(i);;
  // pressSensor.Readout();
  pressSensor2.readADC_calc(i);
  // pressSensor2.Readout();
  pressSensor3.readADC_calc(i);;
  // pressSensor3.Readout();
  pressSensor4.readADC_calc(i);;
  // pressSensor4.Readout();
  pressSensor5.readADC_calc(i);;
  // pressSensor5.Readout();
  pressSensor6.readADC_calc(i);;
  // pressSensor6.Readout();
}



void initHumSensor()
{
  //Serial.println("I'm at initHumSensor");  
   // Initialize I2C communication
   humSensor.begin();
    
   // Begin with a device reset
   //humSensor.reset();

   // Configure Measurements
   humSensor.setMeasurementMode(TEMP_AND_HUMID);  // Set measurements to temperature and humidity
   humSensor.setRate(FIVE_HZ);                     // Set measurement frequency to 1 Hz
   humSensor.setTempRes(FOURTEEN_BIT);
   humSensor.setHumidRes(FOURTEEN_BIT);

   //begin measuring
   humSensor.triggerMeasurement();
}

void savingDataToSD(float temperatureData[], float humData[], float pressData[], float afData[])
{
  // Serial.println("I'm at savingDataToSD");
  String dataString = "";
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile)
  {
    // Serial.println("I'm at dataFile");
    for (int i = 0; i < nrTempSensors; i++)
    {
      dataString += String(temperatureData[i]);
      dataString += ",";
      if (i == (nrTempSensors - 1))
      {
        dataString += "||";
      }
    }
    dataFile.print(dataString);
    dataString = "";
    for (int i = 0; i < nrHumidSensors; i++)
    {
      dataString += String(humData[i]);
      dataString += ",";
      if (i == (nrHumidSensors - 1))
      {
        dataString += "||";
      }
    }
    dataFile.print(dataString);
    dataString = "";
    for (int i = 0; i < nrPressSensors; i++)
    {
      dataString += String(pressData[i]);
      dataString += ",";
      if (i == (nrPressSensors - 1))
      {
        dataString += "||";
      }
    }
    dataFile.print(dataString);
    dataString = "";
    for (int i = 0; i < nrAirFSensors; i++)
    {
      dataString += String(afData[i]);
      dataString += ",";
      if (i == (nrAirFSensors - 1))
      {
        dataString += "||";
      }
    }
    dataFile.print(dataString);
    dataFile.println();
    dataFile.close();
    Serial.println("Exiting dataFile");
  }
  else
  {
    Serial.println("Failed to open datalog.txt");
  }
}

void setSamplingRate(int curSamplingRate)
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

void writeData(float curMeasurements [], int type)
{
   xSemaphoreTake(sem, portMAX_DELAY);
   writeDataToSensorBuffers(curMeasurements, type);
   xSemaphoreGive(sem);
}

std::vector<float> readData(int type)
{
  std::vector<float> dataFromBuffer;
  xSemaphoreTake(sem, portMAX_DELAY);
  dataFromBuffer = readDataFromSensorBuffers(type);
  xSemaphoreGive(sem);
  return dataFromBuffer;
}

void initTempSensors()
{
  //Serial.println("I'm at initTempSensors");
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


void sampler(void *pvParameters)
{
   (void) pvParameters;

   //float tempPressure[nrPressSensors];
   //int count=0;
   float pressDifference = 0;
   float curPressureMeasurement[nrPressSensors] = {0};
   float curTemperatureMeasurement[nrTempSensors] = {0};
   float curHumMeasurement[nrHumidSensors] = {0};
   float curAFMeasurement[nrAirFSensors] = {0};
   float meanPressureAmbient;
   int currSamplingRate;

   static BaseType_t xHigherPriorityTaskWoken;
   TickType_t xLastWakeTime;
   xLastWakeTime = xTaskGetTickCount ();
   

   while(1)
   {
      Serial.println("I'm at sensor periodic");
      xHigherPriorityTaskWoken = pdFALSE;
      uint8_t currMode = getMode();

      if (!simulationOrNot)
      {
        pressSensorread();

        /*Read pressure from sensors*/
        curPressureMeasurement[0] = pressSensor.getPressure();
        curPressureMeasurement[1] = pressSensor2.getPressure();   
        curPressureMeasurement[2] = pressSensor3.getPressure();
        curPressureMeasurement[3] = pressSensor4.getPressure();
        curPressureMeasurement[4] = pressSensor5.getPressure();   
        curPressureMeasurement[5] = pressSensor6.getPressure();

        /*read humidity from HDC*/
        //float temperatureHDC = humSensor.readTemp();
        curHumMeasurement[0] = humSensor.readHumidity();

        /*Read temperature from sensors*/
        curTemperatureMeasurement[0] = tempSensor.getTemperature();
        curTemperatureMeasurement[1] = tempSensor2.getTemperature();
        curTemperatureMeasurement[2] = tempSensor3.getTemperature();
        curTemperatureMeasurement[3] = tempSensor4.getTemperature();
        curTemperatureMeasurement[4] = tempSensor5.getTemperature();
        curTemperatureMeasurement[5] = tempSensor6.getTemperature();
        curTemperatureMeasurement[6] = tempSensor7.getTemperature();
        curTemperatureMeasurement[7] = tempSensor8.getTemperature();
        curTemperatureMeasurement[8] = tempSensor9.getTemperature(); //pressure sensor
        
      
        /*Read airflow from sensor*/
        curAFMeasurement[0] = afSensor.getAF();
        Serial.println("I'm at normal");

      }
      else
      {
        Serial.println("I'm at simulation");
        /*Simulation*/
        int secondsNow = getCurrentTime();
        for (int seq = 0; seq < 7; seq++)
        {
          if (secondsNow > sim_data.simulationTime[seq] && secondsNow < sim_data.simulationTime[seq+1])
          {
            /*Read pressure from sensors*/
            for (int l = 0; l < nrPressSensors; l++)
            {
              if (l<2)
              {
                curPressureMeasurement[l] = sim_data.pressureSim[l][seq];
              }
              else
              {
                curPressureMeasurement[l] = 0;
              }
            }
            for (int l = 0; l < nrTempSensors; l++)
            {
              if (l<2)
              {
                curTemperatureMeasurement[l] = sim_data.temperatureSim[l][seq];
              }
              else
              {
                curTemperatureMeasurement[l] = 0;
              }
            }
            for (int l = 0; l < nrHumidSensors; l++)
            {
              curHumMeasurement[l] = sim_data.humSim[l][seq];
            }
            for (int l = 0; l < nrAirFSensors; l++)
            {
              curAFMeasurement[l] = sim_data.airflowSim[l][seq];
            }
          }
        }
        if (secondsNow > sim_data.simulationTime[7])
          {
            /*Read pressure from sensors*/
            for (int l = 0; l < nrPressSensors; l++)
            {
              if (l<2)
              {
                curPressureMeasurement[l] = sim_data.pressureSim[l][7];
              }
              else
              {
                curPressureMeasurement[l] = 0;
              }
            }
            for (int l = 0; l < nrTempSensors; l++)
            {
              if (l<2)
              {
                curTemperatureMeasurement[l] = sim_data.temperatureSim[l][7];
              }
              else
              {
                curTemperatureMeasurement[l] = 0;
              }
            }
            for (int l = 0; l < nrHumidSensors; l++)
            {
              curHumMeasurement[l] = sim_data.humSim[l][7];
            }
            for (int l = 0; l < nrAirFSensors; l++)
            {
              curAFMeasurement[l] = sim_data.airflowSim[l][7];
            }
          }
        
      }

        /*Save pressure measurements*/
        writeData(curPressureMeasurement, 2);

        /*Save temperature measurements*/
        writeData(curTemperatureMeasurement, 0);

        /*Save humidity measurements*/
        writeData(curHumMeasurement, 1);

        /*Save humidity measurements*/
        writeData(curAFMeasurement, 3);
      
        /*Save all data to SD*/
        savingDataToSD(curTemperatureMeasurement, curHumMeasurement, curPressureMeasurement, curAFMeasurement);

      meanPressureAmbient = (curPressureMeasurement[0]+curPressureMeasurement[1])/2;
      
      /*Calculating Pressure Difference*/
      pressDifference = calculatingPressureDifference(meanPressureAmbient);

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
      EthernetClient client = ethernet.checkClientAvailibility();
      if(client.available()>0)
      {
         xSemaphoreGiveFromISR(semPeriodic, &xHigherPriorityTaskWoken );
      }

      /*Check current sampling rate*/
      currSamplingRate = getSamplingRate();
      flagPost(0);
      Serial.println("I'm leaving sensor periodic");
      vTaskDelayUntil(&xLastWakeTime, (currSamplingRate / portTICK_PERIOD_MS) );
   }
}

void initSampler()
{
    //Serial.println("I'm at initSampler");
    xTaskCreate(
      sampler
      ,  (const portCHAR *)"Sampler"   // A name just for humans
      ,  2048  // This stack size can be checked & adjusted by reading the Stack Highwater
      ,  NULL
      ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
      ,  NULL );
}

void initSensor()
{
   //Serial.println("I'm at initSensor");
   simulationOrNot = checkSimulationOrNot(); 
   if (simulationOrNot)
   {
        sim_data = getSimulationData();
        Serial.println("Succes getting simulation data");
   }
   initHumSensor();
   initTempSensors();
   initSampler();
}
#endif