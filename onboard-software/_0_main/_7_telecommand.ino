EthernetServer server = EthernetServer(4000);


void initTelecommand()
{
  server.begin();

  xTaskCreate(
    telecommand
    ,  (const portCHAR *) "telecommand";   // Name
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

  while(1)
  {
    if( xSemaphoreTake( semPeriodic, portMAX_DELAY ) == pdTRUE )
    {
     
      EthernetClient client = server.available();
      int datasize = client.available();
      char data_tcp[datasize]; //test if using byte type will have different result

      /*mode change command*/
      for (int i = 0; i < datasize; i++) 
      {
        data_tcp[i] = client.read(); 
      }
      
    }
  }
}

