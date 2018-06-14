/* Name: _7_sensor.ino.cpp
 * Purpose: To recieve telecommands and interpert them
 * Project: Tubular-Bexus.
 * Authors: Tubular-Bexus software group.
*/
#ifndef UNIT_TEST
#include <Ethernet2.h>

#include "_7_telecommand.h"
#include "_5_asc.h"
#include "ethernet.h"
#include "commandTranslator.h"  
#include "_8_monitor.h"

#define modeCommand 1
#define htrCommand 2
#define ascCommand 3


extern ethernet ethernet;
extern std::vector<std::vector<byte>> mode;
extern std::vector<std::vector<byte>> heaters;
extern std::vector<std::vector<byte>> asc;
extern std::vector<std::vector<byte>> ss; 


void executeMode(std::vector<std::vector<byte>> &modeC)
{
  setMode(modeC[0][0]);
}

void executeHTR(std::vector<std::vector<byte>> &heatersC)
{
  // float dummyParam [4];
  // floatval param;
  heaterControl(heatersC[0][0], heatersC [1][0]);
  // Serial.println(heatersC[0][0]);
  // Serial.println(heatersC[1][0]);
  // for (int i = 2; i<6; i++)
  // {
  //   for (int k = 0; k < 3; k++) 
  //   {
  //     param.bytes[k] = heatersC[i][k];
  //   }
  //   dummyParam[i-2] = param.val;
  // }
  // param.bytes[3] = byte(0);
  // setHeaterParameter(dummyParam);
}

void openCloseValveManual(byte pumpvalve[])
{
  pumpControl(pumpvalve[0]);
  for (int i = 1; i < 7; i++)
  {
    valvesControl(i, pumpvalve[i]);
  }
  valvesControl(11, pumpvalve[7]);
  digitalWrite(CACvalve, pumpvalve[8]);
  
}

void executeASC(std::vector<std::vector<byte>> &scC)
{
  char buf [6];
  byte pumpvalve[9];
  // float dummyParam [totalBagNumber*2];
  // floatval param;

  for(int i = 0; i < 9; i++)
  {
    pumpvalve[i] = scC[i][0];
  }
 
  // for (int i = 11; i<23; i++)
  // {
  //   for (int k = 0; k < 3; k++) 
  //   {
  //     buf[k] = scC[i][k];
  //   }
  //   // param.bytes[3] = byte(0);
  //   dummyParam[i-11] = atof(buf);
  // }
  // setASCParameter(dummyParam);
  openCloseValveManual(pumpvalve);
}



void executeSS(std::vector<std::vector<byte>> &sensorC)
{
  setSamplingRate(sensorC[0][0]);
}


void telecommand(void *pvParameters)
{
  (void) pvParameters;
  // TickType_t xLastWakeTime;
  // xLastWakeTime = xTaskGetTickCount ();

  // uint8_t curMode;
  // int commandSize; 
  /*declare all command variables*/
  // std::vector<std::vector<byte>> mode(1, std::vector<byte>(1, 0));
  // std::vector<std::vector<byte>> heaters(6, std::vector<byte>(3, 0));
  // std::vector<std::vector<byte>> asc(27, std::vector<byte>(3, 0));
  // std::vector<std::vector<byte>> ss(1, std::vector<byte>(1, 0));

  while(1)
  {
    if( xSemaphoreTake( semPeriodic, portMAX_DELAY ) == pdTRUE )
    {
      
      int row = 0;
      int col = 3;
      EthernetClient client = ethernet.checkClientAvailibility();
      byte datasize = client.available();
      byte data_tcp[datasize]; //test if using byte type will have different result
      // curMode = getMode();

      /*Read command to buffer*/
      for (int i = 0; i < datasize; i++) 
      {
        data_tcp[i] = client.read(); 
      }

      row = checkComma(data_tcp, row, datasize);

      if (checkCommand(data_tcp) == true)
      {
        // mode = collectingCommand(data_tcp, row, col, datasize, 1);
        // heaters = collectingCommand(data_tcp, row, col, datasize, 2);
        // asc = collectingCommand(data_tcp, row, col, datasize, 3);
        // ss = collectingCommand(data_tcp, row, col, datasize, 4);

        collectingCommand (data_tcp, row, col, datasize);

        /*Execute Command*/
        
        if (getMode() == manual )
        {
          executeHTR(heaters);
          executeASC(asc);
          // executeSS(ss);
        }
        else
        {
          executeMode(mode);
        }
      }
    // flagPost(3);  
    }
  }
}

void initTelecommand()
{
  // Serial.println("Im at initTelecommand");
  xTaskCreate(
    telecommand
    ,  (const portCHAR *) "telecommand"   // Name
    ,  2048  // This stack size 
    ,  NULL
    ,  3  // Priority
    ,  NULL );
  
}
#endif