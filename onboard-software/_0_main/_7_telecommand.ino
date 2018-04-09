
#define modeCommand 1
#define htrCommand 2
#define ascCommand 3

void initTelecommand()
{
  server.begin();

  xTaskCreate(
    telecommand
    ,  (const portCHAR *) "telecommand"   // Name
    ,  128  // This stack size 
    ,  NULL
    ,  3  // Priority
    ,  NULL );
  
}

std::vector<int8_t> extractData(int8_t data[], int type)
{
  //int arrayLength = sizeof(data) / sizeof(char);
  std::vector<int8_t> htrCommandData(2); 
  std::vector<int8_t> modeCommandData(1); 
  std::vector<int8_t> ascCommandData(3); 
  
  switch (type){
    case modeCommand:
    if (data[7] == 1)
    {
      modeCommandData[0] = data[8];
    }
    return modeCommandData;
    break;

    case htrCommand:
    for (int i = 13; i < (i+data[12]); i++)
    {
       htrCommandData[i-13] = data[i];
    }
    return htrCommandData;
    break;
    
    case ascCommand:
    for (int i = 19; i < (i+data[18]); i++)
    {
      ascCommandData[i-19] = data[i];
    }
    return ascCommandData;
    break;
  }
}

void telecommand(void *pvParameters)
{
  (void) pvParameters;
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount ();

  int8_t curMode;
  int commandSize; 
  std::vector<int8_t> mode;
  std::vector<int8_t> htr;
  std::vector<int8_t> asc;

  while(1)
  {
    if( xSemaphoreTake( semPeriodic, portMAX_DELAY ) == pdTRUE )
    {
     
      EthernetClient client = server.available();
      int8_t datasize = client.available();
      int8_t data_tcp[datasize]; //test if using byte type will have different result

      
      for (int i = 0; i < datasize; i++) 
      {
        data_tcp[i] = client.read(); 
      }

      /*checkCommandsource*/
      if (checkCommand(data_tcp) == true)
      {
        commandSize = data_tcp[4];
        mode = extractData(data_tcp, modeCommand);
        asc = extractData(data_tcp, ascCommand);
        htr = extractData(data_tcp, htrCommand);
        if (mode[0] == manual || curMode == manual)
        {
          if (data_tcp[7] == 1)
          {
            setMode(mode[0]);
          }
          else if (data_tcp[12] > 0)
          {
            heaterControl(htr[0], htr[1]);
          }
          if (data_tcp[18] > 0)
          {
            pumpControl(asc[0]);
            valvesControl(asc[1], asc[2]);
          }
        }
        else
        {
          setMode(mode[0]);
        }
      }
      else if (checkCommand(data_tcp) == false)
      {
        
      }
      else if (data_tcp[0] == 'p' && data_tcp[1] == 'r' && data_tcp[2] == 'o')
      {
        
      }
      
    }
  }
}

boolean checkCommand(int8_t data[])
{
  if (data[0] == 't' && data[1] == 'u' && data[2] == 'b')
  {
    return true;
  }
  else
  {
    return false;
  }
}



