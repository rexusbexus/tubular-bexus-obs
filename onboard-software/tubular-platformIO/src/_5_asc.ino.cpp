/*
 * Name: asc object
 * Purpose: To read the pressure sensor from buffer
 * and accordingly activate sampling logic.
 * Project: Tubular-Bexus.
 * Authors: Tubular-Bexus software group.
*/
#ifndef UNIT_TEST
#include <SD.h>
#include "_4_heater.h"
#include "_5_asc.h"
#include "ascLogic.h"
#include "_8_monitor.h"


float ascParameter[totalBagNumber*2];
extern RTCDue rtc;
int secondsOpen;
int flushStartTime;
int pumpStartTime;
int valveBagStartTime;
//  int bagFillingTime [] = {10, 10, 10, 10, 10, 10};
int bagFillingTime [totalBagNumber];// = {44, 47, 53, 50, 48, 41};
float current_volume = 0;
int lastMeasurement = 0;
extern float medianPressureAmbient;
int bagcounter = 1;
bool samplingFlag = false;
File bagfile;


std::vector<float> getASCParam(int bag)
{
  std::vector<float> dummyParameter(2);
  xSemaphoreTake(sem, portMAX_DELAY);
  dummyParameter[0] = ascParameter[(bag*2)-2];
  dummyParameter[1] = ascParameter[(bag*2)-1];
  xSemaphoreGive(sem);
  return dummyParameter;
}

std::vector<float> processInitialAscParameters(char scParameters[])
{
  //Serial.println("I'm at processInitialAscParameters");
  //Serial.println(String(scParameters));
  std::vector<float> newASCParameter(totalBagNumber*2);
  
  int i = 0; int z = 0;
  //int sizeParam = sizeof(scParameters)/sizeof(byte);
  while(z<totalBagNumber*2)
  {
    int k = 0;
    char buf[6] = {0};
    while(1)
    {
      if (scParameters[i] == ',')
      {
        i++;
        break;
      }
      buf[k] = scParameters[i];
      
      i++; k++;
    }
    newASCParameter[z] = atof(buf);
    //Serial.println(newASCParameter[z]);
    z++;
  }
  return newASCParameter;
}

void initAscParameters()
{
  
  File dataParam = SD.open("asc.txt", FILE_READ);
  if (dataParam)
  {
    char scParameters[dataParam.size()];
    int i = 0;
    while (dataParam.available())
    {
        scParameters[i] = dataParam.read();
        i++;
    }
    float newParameter[totalBagNumber*2];
    std::vector<float> newParameterV = processInitialAscParameters(scParameters);
    for (int scP = 0; scP < totalBagNumber*2; scP++)
    {
        newParameter[scP] = newParameterV[scP];
    }
    dataParam.close();
    setASCParameter(newParameter);
  }
  else
  {
    float backupAscParam[] = {85,65,56.8,36.8,71.2,91.2,95,111,131,151,183.3,203.3};
    setASCParameter(backupAscParam);
    Serial.println("Failed to open asc.txt");
  }
  
}

std::vector<int> processInitialTimeParameters(char timeParameters[])
{
  //Serial.println("I'm at processInitialTimeParameters");
  //Serial.println(String(timeParameters));
  std::vector<int> newTimeParameter(totalBagNumber);
  
  int i = 0; int z = 0;
  //int sizeParam = sizeof(timeParameters)/sizeof(byte);
  while(z<totalBagNumber)
  {
    int k = 0;
    char buf[6] = {0};
    while(1)
    {
      if (timeParameters[i] == ',')
      {
        i++;
        break;
      }
      buf[k] = timeParameters[i];
      
      i++; k++;
    }
    newTimeParameter[z] = atoi(buf);
    //Serial.println(newTimeParameter[z]);
    z++;
  }
  return newTimeParameter;
}

void initbagfillingTime()
{
  
  File timeParam = SD.open("fillTime.txt", FILE_READ);
  if (timeParam)
  {
    char timeParameters[timeParam.size()];
    int i = 0;
    while (timeParam.available())
    {
        timeParameters[i] = timeParam.read();
        i++;
    }
    //int newParameter[totalBagNumber];
    std::vector<int> newParameterV = processInitialTimeParameters(timeParameters);
    for (int timeP = 0; timeP < totalBagNumber; timeP++)
    {
        bagFillingTime[timeP] = newParameterV[timeP];
    }
    timeParam.close();
    //bagFillingTime = newParameter;
    //setASCParameter(newParameter);
  }
  else
  {
    int deafultBagFillingTime [] = {44, 47, 53, 50, 48, 41};
    //int deafultBagFillingTime [] = {90, 70, 600, 20, 48, 41};
    for (int timeP = 0; timeP < totalBagNumber; timeP++)
    {
    bagFillingTime [timeP] = deafultBagFillingTime[timeP];   
    }
    
    Serial.println("Failed to open fillTime.txt");
  } 

}

int initBagCount()
{
  bagfile = SD.open("bagfile.txt");
  char bagSign = '7'; // Initilised to 7 since is the last bag and will cause the least amount of damage.
  if(bagfile){
    bagSign = bagfile.read();
    bagfile.close();
  }
  return int(bagSign - '0');
}

bool incBagCounter(int savedBag)
{
  bagfile = SD.open("bagfile.txt", (O_READ | O_WRITE));
  if (bagfile) {
    bagfile.print(savedBag);
    bagfile.close();
    return true;
  }
  return false;
}

void setASCParameter(float newParameter[])
{
  xSemaphoreTake(sem, portMAX_DELAY);
  for (int i = 0; i < totalBagNumber*2; i++)
  {
    ascParameter[i]=newParameter[i];
  }
  xSemaphoreGive(sem);
}

void samplingScheduler(int whichBag, float newParameter[])
{
  xSemaphoreTake(sem, portMAX_DELAY);
  ascParameter[(2*whichBag)-2] = newParameter[0];
  ascParameter[(2*whichBag)-1] = newParameter[1];
  
  xSemaphoreGive(sem);
  Serial.println(whichBag);
  Serial.println(ascParameter[(2*whichBag)-2]);
  Serial.println(ascParameter[(2*whichBag)-1]);
}

void initValvesControl()
{
  pinMode(valve1, OUTPUT);
  pinMode(valve2, OUTPUT);
  pinMode(valve3, OUTPUT);
  pinMode(valve4, OUTPUT);
  pinMode(valve5, OUTPUT);
  pinMode(valve6, OUTPUT);
  pinMode(valve7, OUTPUT);
  pinMode(valve8, OUTPUT);
  pinMode(valve9, OUTPUT);
  pinMode(valve10, OUTPUT);
  pinMode(flushValve, OUTPUT);
  pinMode(CACvalve, OUTPUT);
}

void pumpControl(int cond)
{
  if (cond==1) {
    //digitalWrite(htr1_pin,0);
    heaterControl(0,0); //Turn off heaters when pump is on.
  }
  // Serial.println("Pump control taking sem");
  xSemaphoreTake(sem, portMAX_DELAY);
  // Serial.println("Entering pump control");
  // switch (cond){
  //   case 0:
  //   digitalWrite(pumpPin, LOW);
  //   break;

  //   case 1:
  //   heaterControl(0,0); //Turn off heaters when pump is on.
  //   digitalWrite(pumpPin, HIGH);
  //   break;
  // }
  digitalWrite(pumpPin, cond);
  xSemaphoreGive(sem);
  // Serial.println("Leaving pump control");
}

void valvesControl(int valve, int cond)
{
  // Serial.println("valves control taking sem");
  xSemaphoreTake(sem, portMAX_DELAY);
  // Serial.println("Entering valves control");
  if (cond==1)
  {
    switch (valve)
    {
      case 1:
      digitalWrite(valve1, HIGH);
      break;

      case 2:
      digitalWrite(valve2, HIGH);
      break;

      case 3:
      digitalWrite(valve3, HIGH);
      break;

      case 4:
      digitalWrite(valve4, HIGH);
      break;

      case 5:
      digitalWrite(valve5, HIGH);
      break;

      case 6:
      digitalWrite(valve6, HIGH);
      break;

      case 7:
      digitalWrite(valve7, HIGH);
      break;

      case 8:
      digitalWrite(valve8, HIGH);
      break;

      case 9:
      digitalWrite(valve9, HIGH);
      break;

      case 10:
      digitalWrite(valve10, HIGH);
      break;

      case 11:
      digitalWrite(flushValve, HIGH);
      break;
  }
  }
  else {
    switch (valve){
    case 1:
    digitalWrite(valve1, LOW);
    break;

    case 2:
    digitalWrite(valve2, LOW);
    break;

    case 3:
    digitalWrite(valve3, LOW);
    break;

    case 4:
    digitalWrite(valve4, LOW);
    break;

    case 5:
    digitalWrite(valve5, LOW);
    break;

    case 6:
    digitalWrite(valve6, LOW);
    break;

    case 7:
    digitalWrite(valve7, LOW);
    break;

    case 8:
    digitalWrite(valve8, LOW);
    break;

    case 9:
    digitalWrite(valve9, LOW);
    break;

    case 10:
    digitalWrite(valve10, LOW);
    break;

    case 11:
    digitalWrite(flushValve, LOW);
    break;

    default:
    break;
  }
  
  }
  xSemaphoreGive(sem);
  // Serial.println("Leaving valves control");
}

int getCurrentTime()
{
  int secondsNow = rtc.getSeconds() + (rtc.getMinutes()*60) + (rtc.getHours()*3600);
  return secondsNow;
}

int getValvePin(int whichBag)
{
  switch (whichBag)
  {
    case 1:
    return valve1;

    case 2:
    return valve2;

    case 3:
    return valve3;

    case 4:
    return valve4;

    case 5:
    return valve5;

    case 6:
    return valve6;

  }
  return 0;
}

int ascentSequence(float meanPressureAmbient, float ascParam[], int bagcounter)
{
  digitalWrite(CACvalve, HIGH);
  
  // int secondsNow = getCurrentTime();
  int valveBag = digitalRead(getValvePin(bagcounter));
  int valveFlush = digitalRead(flushValve);
  int pumpState = digitalRead(pumpPin);

  std::vector<float> current_pressure = readData(2);
  std::vector<float> current_flowrate = readData(3);
  current_flowrate[0] = current_flowrate[0]/60;

  float volume_limit = 2.4;
  float pressure_limit = 112;

  if (samplingFlag == false)
  {
    samplingFlag = ascentSamplingLogic(meanPressureAmbient, ascParam);
  }
  else
  {
    samplingFlag = samplingLimit(meanPressureAmbient, ascParam);
  }
  
  
  if (samplingFlag)
  {
    
    if (valveBag == closeState)
    {
      if (valveFlush == closeState)
      {
          if (pumpState == openState && (pumpStartTime+1 < getCurrentTime()))
          {
            valvesControl(11, openState);
            flushStartTime = getCurrentTime();
          }

          if (pumpState == closeState)
          {
            pumpControl(openState);
            pumpStartTime = getCurrentTime();
          }

      }
      else
      {
          if (getCurrentTime() > (flushStartTime+flushingTime))
          {
            valvesControl(11, closeState);
            valvesControl(bagcounter, openState);
            valveBagStartTime = getCurrentTime();
          }
      }
    }
    else
    {
      float press_diff_in_bag = current_pressure[3] - medianPressureAmbient;
      //press_diff_in_bag = float abs(press_diff_in_bag);
      if (lastMeasurement==0)
      {
        current_volume = current_volume + (current_flowrate[0] * (getCurrentTime() - valveBagStartTime));
      } 
      else
      {
       current_volume = current_volume + (current_flowrate[0] * (getCurrentTime() - lastMeasurement)); 
      }
      lastMeasurement = getCurrentTime();

      if (getCurrentTime() > (valveBagStartTime+bagFillingTime[bagcounter-1]) || current_volume >= volume_limit || press_diff_in_bag > pressure_limit)
      {
        valvesControl(bagcounter, closeState);
        pumpControl(closeState);
        current_volume = 0;
        lastMeasurement = 0;
        bagcounter++;
        samplingFlag = false;
        incBagCounter(bagcounter);
      }
    }

  }
  else
  {
    if (valveBag == openState || digitalRead(flushValve)==1 || digitalRead(pumpPin)==1)
    {
      valvesControl(bagcounter, closeState);
      pumpControl(closeState);
      bagcounter++;
      incBagCounter(bagcounter);
      samplingFlag = false;
      current_volume = 0;
      lastMeasurement = 0;
      valvesControl(11, 0);
    }
  }
  
  return bagcounter;
  
}

int descentSequence(float meanPressureAmbient, float ascParam[], int bagcounter)
{
  digitalWrite(CACvalve, HIGH);

  int valveBag = digitalRead(getValvePin(bagcounter));
  int valveFlush = digitalRead(flushValve);
  int pumpState = digitalRead(pumpPin);

  std::vector<float> current_pressure = readData(2);
  std::vector<float> current_flowrate = readData(3);
  current_flowrate[0] = current_flowrate[0]/60;

  float volume_limit = 2.4;
  float pressure_limit = 112;

  if (samplingFlag == false)
  {
    samplingFlag = descentSamplingLogic(meanPressureAmbient, ascParam);
  }
  else
  {
    samplingFlag = samplingLimit(meanPressureAmbient, ascParam);
  }
  

  if (samplingFlag)
  {
    if (valveBag == closeState)
    {
      if (valveFlush == closeState)
      {
          if (pumpState == openState && (pumpStartTime+1 < getCurrentTime()))
          {
            valvesControl(11, openState);
            flushStartTime = getCurrentTime();
          }

          if (pumpState == closeState)
          {
            pumpControl(openState);
            pumpStartTime = getCurrentTime();
          }

      }
      else
      {
          if (getCurrentTime() > (flushStartTime+flushingTime))
          {
            valvesControl(11, closeState);
            valvesControl(bagcounter, openState);
            valveBagStartTime = getCurrentTime();
          }
      }
    }
    else
    {
      float press_diff_in_bag = current_pressure[3] - medianPressureAmbient;

      if (lastMeasurement==0)
      {
        current_volume = current_volume + (current_flowrate[0] * (getCurrentTime() - valveBagStartTime));
      } 
      else
      {
       current_volume = current_volume + (current_flowrate[0] * (getCurrentTime() - lastMeasurement)); 
      }
      lastMeasurement = getCurrentTime();
      
      if (getCurrentTime() > (valveBagStartTime+bagFillingTime[bagcounter-1]) || current_volume > volume_limit || press_diff_in_bag > pressure_limit)
      {
        valvesControl(bagcounter, closeState);
        pumpControl(closeState);
        current_volume = 0;
        lastMeasurement = 0;
        bagcounter++;
        incBagCounter(bagcounter);
        samplingFlag = false;
      }
    }
  }
  else
  {
    if (valveBag == openState || digitalRead(flushValve)==1 || digitalRead(pumpPin)==1)
    {
      valvesControl(bagcounter, closeState);
      pumpControl(closeState);
      bagcounter++;
      incBagCounter(bagcounter);
      current_volume = 0;
      lastMeasurement = 0;
      valvesControl(11, 0);
      samplingFlag = false;
    }
  }
  
  return bagcounter;
}

void reading(void *pvParameters)
{
   (void) pvParameters;
   std::vector<float> dummyParam(2);
   float ascParam[2];
   
   std::vector<float> currPressure(nrPressSensors);
   float meanPressureAmbient;
   uint8_t currMode;
   
   TickType_t xLastWakeTime;
   xLastWakeTime = xTaskGetTickCount ();

   while(1)
   {
      Serial.println("I'm at asc periodic");
      // Serial.println("Bags Filling time:");
      // for (int timeP = 0; timeP < totalBagNumber; timeP++)
      // {
      //   Serial.print("Bag Time: "); Serial.println(bagFillingTime[timeP]);
      // }
      Serial.print("bagcounter: "); Serial.println(bagcounter);
      currMode = getMode();
     
     dummyParam = getASCParam(bagcounter);
     ascParam[0] = dummyParam[0]; // lower limit
     ascParam[1] = dummyParam[1]; // upper limit
    //  Serial.println(ascParam[0]);
    //  Serial.println(ascParam[1]);
     currPressure = readData(2);

     /*Calculating mean pressure from several pressure sensors*/
    //  meanPressureAmbient = (currPressure[0]+currPressure[1])/2;
    meanPressureAmbient = medianPressureAmbient;

     switch (currMode){
     /*Standby*/
     case standbyMode:
     
     break;
     
     /*Normal - Ascent*/
     case normalAscent:
     digitalWrite(CACvalve, HIGH);
     if (ascentOrDescent(ascParam))
     {
       if (bagcounter <= totalBagNumber)
       {
         bagcounter = ascentSequence(meanPressureAmbient, ascParam, bagcounter);
       }
     }
     else
     {
       if (bagcounter <= totalBagNumber)
       {
        bagcounter = descentSequence(meanPressureAmbient, ascParam, bagcounter);
       }
     }
     break;
     
     /*Normal - Descent*/
     case normalDescent:
     digitalWrite(CACvalve, HIGH);
     if (!ascentOrDescent(ascParam))
     {
       if (bagcounter <= totalBagNumber)
       {
         bagcounter = descentSequence(meanPressureAmbient, ascParam, bagcounter);
       }
     }
     else
     {
       if (bagcounter <= totalBagNumber)
       {
         bagcounter = ascentSequence(meanPressureAmbient, ascParam, bagcounter);
       }
     }
     break;
     
     /*SAFE*/
     case safeMode:
     digitalWrite(CACvalve, LOW);
     valvesControl(11, closeState);
     pumpControl(closeState);
     for (int sd = 1;sd <= 6; sd++)
     {
       valvesControl(sd, closeState);
     }
     break;

     case manual:
     
     break;
   }

   flagPost(2);
   Serial.println("I'm leaving asc periodic");
   vTaskDelayUntil(&xLastWakeTime, (800 / portTICK_PERIOD_MS) );
   }
}

void initReading()
{
  //Serial.println("Im at initReading");
  xTaskCreate(
    reading
    ,  (const portCHAR *) "reading"   // Name
    ,  2048  // This stack size 
    ,  NULL
    ,  2  // Priority
    ,  NULL );
}

void initPumpControl()
{
  pinMode(pumpPin, OUTPUT);
}

void initASC()
{
  // Serial.println("Im at initAsc");
  bagcounter = initBagCount();
  initbagfillingTime();
  initAscParameters();
  initPumpControl();
  initValvesControl();
  initReading();
  
}
#endif