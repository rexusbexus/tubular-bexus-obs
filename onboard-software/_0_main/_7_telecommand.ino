
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



