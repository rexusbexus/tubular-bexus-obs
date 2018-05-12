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


#include "ethernet.h"
#include "_1_init.h"
#include "_6_telemetry.h"

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

int samplingRate = 500;

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

void savingDataToSD(float temperatureData[], float humData[], float pressData[])
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

static void writeData(float curMeasurements [], int type)
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

bool checkSimulationOrNot()
{
  File sim = SD.open("trig.txt");
  if (sim)
  {
    sim.close();
    return false;
  }
  else
  {
    sim.close();
    return true;
  }
}

struct pressureSimulation{
  int simulationTime[8];
  int pressureSim[8];
};

std::vector<int> getPressure(char all_data[])
{
  char buf[8];
  std::vector<int> pressure(8);
  int i = 0;  int b = 0;
  while(b<8)
  {
    int k = 0;
    while(1)
    {
      if (all_data[i] == ',')
      {
        i++;
        break;
      }
      buf[k] = all_data[i];
      
      i++; k++;
    }
    pressure[b] = atoi(buf);
    
    b++;
  }
  return pressure;
}

std::vector<int> getSeconds(char all_data[])
{
  char buf[8];
  std::vector<int> seconds(8);
  int i = 0;  int b = 0;
  while(b<8)
  {
    int k = 0;
    while(1)
    {
      if (all_data[i] == ',')
      {
        i++;
        break;
      }
      buf[k] = all_data[i];
      
      i++; k++;
    }
    seconds[b] = atoi(buf);
    
    b++;
  }
  return seconds;
}

pressureSimulation getPressureSimulationData ()
{
  File sim = SD.open("siP.txt", FILE_READ);
  pressureSimulation pressSim_struct;
  if (sim)
  {
    char all_data[sim.size()];
    int i = 0;
    
    while (sim.available())
    {
      all_data[i] = sim.read();
      i++;
    }
    
    std::vector<int> pressure = getPressure(all_data);
    for (int g = 0; g < 8; g++)
    {
      pressSim_struct.pressureSim[g] = pressure[g];
    }
    sim.close();
  }

  File simT = SD.open("siPT.txt", FILE_READ);
  
  if(simT)
  {
    char all_data[simT.size()];
    int i = 0;
    
    while (simT.available())
    {
      all_data[i] = simT.read();
      i++;
    }
    
    std::vector<int> seconds = getSeconds(all_data);
    for (int g = 0; g < 8; g++)
    {
      pressSim_struct.simulationTime[g] = seconds[g];
    }
    simT.close();
  }
  return pressSim_struct;
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
   bool simulationOrNot = checkSimulationOrNot();

   while(1)
   {
      /*Test*/
        digitalWrite(13, HIGH);
      xHigherPriorityTaskWoken = pdFALSE;
      uint8_t currMode = getMode();

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
        curTemperatureMeasurement[8] = tempSensor9.getTemperature();
        
      
        /*Read airflow from sensor*/
        curAFMeasurement[0] = afSensor.getAF();

      if (simulationOrNot == false)
      {
        pressSensorread();

        /*Read pressure from sensors*/
        curPressureMeasurement[0] = pressSensor.getPressure();
        curPressureMeasurement[1] = pressSensor2.getPressure();   
        curPressureMeasurement[2] = pressSensor3.getPressure();
        curPressureMeasurement[3] = pressSensor4.getPressure();
        curPressureMeasurement[4] = pressSensor5.getPressure();   
        curPressureMeasurement[5] = pressSensor6.getPressure();

      }
      else
      {
        
        /*Simulation*/
        pressureSimulation press_sim_data = getPressureSimulationData();
        int secondsNow = rtc.getSeconds() + (rtc.getMinutes()*60) + (rtc.getHours()*3600);
        for (int seq = 0; seq < 7; seq++)
        {
          if (secondsNow > press_sim_data.simulationTime[seq] && secondsNow < press_sim_data.simulationTime[seq+1])
          {
            /*Read pressure from sensors*/
            curPressureMeasurement[0] = press_sim_data.pressureSim[seq];
            curPressureMeasurement[1] = press_sim_data.pressureSim[seq];
            curPressureMeasurement[2] = press_sim_data.pressureSim[seq];
            curPressureMeasurement[3] = press_sim_data.pressureSim[seq];
            curPressureMeasurement[4] = press_sim_data.pressureSim[seq]; 
            curPressureMeasurement[5] = press_sim_data.pressureSim[seq];
          }
        }
        if (secondsNow > press_sim_data.simulationTime[7])
          {
            /*Read pressure from sensors*/
            curPressureMeasurement[0] = press_sim_data.pressureSim[7];
            curPressureMeasurement[1] = press_sim_data.pressureSim[7];
            curPressureMeasurement[2] = press_sim_data.pressureSim[7];
            curPressureMeasurement[3] = press_sim_data.pressureSim[7];
            curPressureMeasurement[4] = press_sim_data.pressureSim[7]; 
            curPressureMeasurement[5] = press_sim_data.pressureSim[7];
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
        savingDataToSD(curTemperatureMeasurement, curHumMeasurement, curPressureMeasurement);

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
      EthernetClient client = ethernet.checkClientAvailibility();
      if(client.available()>0)
      {
         xSemaphoreGiveFromISR(semPeriodic, &xHigherPriorityTaskWoken );
      }

      /*Check current sampling rate*/
      currSamplingRate = getSamplingRate();
      flagPost(0);
      vTaskDelayUntil(&xLastWakeTime, (currSamplingRate / portTICK_PERIOD_MS) );
   }
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

void initSensor()
{
   SD.begin(sdCS);
   initSampler();
   initHumSensor();
}
#endif