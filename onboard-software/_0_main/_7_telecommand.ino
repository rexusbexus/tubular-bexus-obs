
#define modeCommand 1
#define htrCommand 2

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

void telecommand()
{
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount ();
  int mode;
  int commandSize; 

  while(1)
  {
    if( xSemaphoreTake( semPeriodic, portMAX_DELAY ) == pdTRUE )
    {
     
      EthernetClient client = server.available();
      int datasize = client.available();
      char data_tcp[datasize]; //test if using byte type will have different result

      
      for (int i = 0; i < datasize; i++) 
      {
        data_tcp[i] = client.read(); 
      }

      /*checkCommandsource*/
      if (checkCommand(data_tcp) == true)
      {
        commandSize = data_tcp[4];
        
      }
      else
      {
        
      }
      
    }
  }
}

boolean checkCommand(char data[])
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

char* extractData(char data[], int type)
{
  int arrayLength = sizeof(data) / sizeof(char);
  int htrCommandData [2]; 
  
  switch (type){
    case modeCommand:
    return data[8];
    break;

    case htrCommand:
    for (int i = 13; i < (i+data[12]); i++)
    {
       htrCommandData[i-13] = data[i];
    }
    return htrCommandData;
    break;
  }
}

