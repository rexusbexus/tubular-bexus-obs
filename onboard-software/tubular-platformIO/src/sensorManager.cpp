#include "sensorManager.h"

float tempReading [nrTempSensors];
float humReading [nrHumidSensors];
float pressReading [nrPressSensors];
float afReading[nrAirFSensors];
float pressDifference = 0;

static float tempPressure[2] = {0};

void writeDataToSensorBuffers(float curMeasurements [], int type)
{
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

      /*Airflow data*/
      case 3 :
      for (int i=0 ; i < nrAirFSensors ; i++)
      {
          afReading[i] = curMeasurements[i];
      }
      break;
   }
}

std::vector<float> readDataFromSensorBuffers(int type)
{
  std::vector<float> dummyTemp(nrTempSensors);
  std::vector<float> dummyHum(nrHumidSensors);
  std::vector<float> dummyPress(nrPressSensors);
  std::vector<float> dummyAf(nrAirFSensors);
  switch(type){
      /*Reading Temperature*/
      case 0 :
      for (int i=0 ; i < nrTempSensors ; i++)
      {
          dummyTemp[i] = tempReading[i];
      }
      return dummyTemp;
      break;
      
      /*Reading Humidity*/
      case 1 :
      for (int i=0 ; i < nrHumidSensors ; i++)
      {
          dummyHum[i] = humReading[i];
      }
      return dummyHum;
      break;
      
      /*Reading Pressure*/
      case 2 :
      for (int i=0 ; i < nrPressSensors ; i++)
      {
          dummyPress[i] = pressReading[i];
      }
      return dummyPress;
      break;

      /* Reading Airflow*/
      case 3 :
      for (int i=0 ; i < 1 ; i++)
      {
          dummyAf[i] = afReading[i];
      }
      return dummyAf;
      break;
  }
}

// void changeTheMode(float pressDifference, uint8_t currentMode, float meanPressureAmbient)
// {
//     if (pressDifference<pressDifferentThresholdneg)
//     {
//         setMode(normalAscent);
//     }
//     else if (pressDifference>pressDifferentThresholdpos)
//     {
//         setMode(normalDescent);
//     }
//     else if (currentMode==normalDescent && meanPressureAmbient<=safeModeThreshold)
//     {
//         setMode(safeMode);
//     }
// }

float calculatingPressureDifference(float meanPressureAmbient)
{
    tempPressure[1] = meanPressureAmbient;
    
    if (tempPressure[0] == 0 )
    {
        tempPressure[0] = meanPressureAmbient;
        return 0;
    } 
    pressDifference = pressDifference + tempPressure[1] - tempPressure[0];
    tempPressure[0] = meanPressureAmbient;
    if (tempPressure[0]< -35 || tempPressure[0] > 35)
    {
        pressDifference = 0;
    }

    return pressDifference;
}