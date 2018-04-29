#ifndef UNIT_TEST
#include <Ethernet2.h>

#include "_7_telecommand.h"
#include "_5_asc.h"
#include "ethernet.h"
#include "commandTranslator.h"  

#define modeCommand 1
#define htrCommand 2
#define ascCommand 3


ethernet ethernet2;



void executeMode(std::vector<std::vector<byte>> &mode)
{
  setMode(mode[0][0]);
}

void executeHTR(std::vector<std::vector<byte>> &heaters)
{
  float dummyParam [4];
  floatval param;
  heaterControl(heaters[0][0], heaters [1][0]);
  for (int i = 2; i<6; i++)
  {
    for (int k = 0; k < 3; k++) 
    {
      param.bytes[k] = heaters[i][k];
    }
    dummyParam[i-2] = param.val;
  }
  param.bytes[3] = byte(0);
  setHeaterParameter(dummyParam);
}

void executeASC(std::vector<std::vector<byte>> &sc)
{
  float dummyParam [4];
  byte pumpvalve[11];
  floatval param;

  for(int i = 0; i < 11; i++)
  {
    pumpvalve[i] = sc[i][0];
  }
 
  for (int i = 11; i<27; i++)
  {
    for (int k = 0; k < 3; k++) 
    {
      param.bytes[k] = sc[i][k];
    }
    param.bytes[3] = byte(0);
    dummyParam[i-2] = param.val;
  }
  setASCParameter(dummyParam);
}

void openCloseValveManual(byte pumpvalve[])
{
  if (pumpvalve[0] == 0)
  {
    pumpControl(pumpvalve[0]);
    for (int i = 1; i < 11; i++)
    {
      valvesControl(i, pumpvalve[i]);
    }
  }
  else if (pumpvalve[0] == 1)
  {
    valvesControl(11, 1); //delay(10);
    pumpControl(pumpvalve[0]);
    valvesControl(11, 0); //delay(10);
    for (int i = 1; i < 11; i++)
    {
      valvesControl(i, pumpvalve[i]);
    }
  }
}

void executeSS(std::vector<std::vector<byte>> &sensor)
{
  setSamplingRate(sensor[0][0]);
}


void telecommand(void *pvParameters)
{
  (void) pvParameters;
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount ();

  uint8_t curMode;
  int commandSize; 
  byte **modes;
  byte **htr;
  byte **sc;
  byte **sensor;
  /*declare all command variables*/
  std::vector<std::vector<byte>> mode(1, std::vector<byte>(1, 0));
  std::vector<std::vector<byte>> heaters(6, std::vector<byte>(3, 0));
  std::vector<std::vector<byte>> asc(27, std::vector<byte>(3, 0));
  std::vector<std::vector<byte>> ss(1, std::vector<byte>(1, 0));

  while(1)
  {
    if( xSemaphoreTake( semPeriodic, portMAX_DELAY ) == pdTRUE )
    {
      
      int row = 0;
      int col = 3;
      EthernetClient client = ethernet2.checkClientAvailibility();
      byte datasize = client.available();
      byte data_tcp[datasize]; //test if using byte type will have different result
      curMode = getMode();

      /*Read command to buffer*/
      for (int i = 0; i < datasize; i++) 
      {
        data_tcp[i] = client.read(); 
      }

      row = checkComma(data_tcp, row, datasize);

      if (checkCommand(data_tcp) == true)
      {
        mode = collectingCommand(data_tcp, row, col, datasize, 1);
        heaters = collectingCommand(data_tcp, row, col, datasize, 2);
        asc = collectingCommand(data_tcp, row, col, datasize, 3);
        ss = collectingCommand(data_tcp, row, col, datasize, 4);

        

        /*Execute Command*/
        executeMode(mode);
        if (getMode() == manual )
        {
          executeHTR(heaters);
          executeASC(asc);
          executeSS(ss);
        }
      }
      
    }
  }
}

void initTelecommand()
{
  xTaskCreate(
    telecommand
    ,  (const portCHAR *) "telecommand"   // Name
    ,  128  // This stack size 
    ,  NULL
    ,  3  // Priority
    ,  NULL );
  
}
#endif