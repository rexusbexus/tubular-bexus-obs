
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

std::vector<std::vector<int8_t>> scanBuffer(int8_t bufferD, int r, int c, int datasize)
{
  int8_t separator;
  int i = 0;
  int k = 0;
  std::vector<std::vector<int8_t>> command(r, std::vector<int8_t>(c,0));
  for (int n = 0; n < r; n++)
  {
    while(bufferD[i] != ",")
    {
      command[n][k] = bufferD[i];
      i++;
      k++;
    }
    i++;
    if (i == datasize)
    {
      break;
    }
  }
  return command;
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
      
      int r = 0;
      int c = 3;
      EthernetClient client = server.available();
      int8_t datasize = client.available();
      int8_t data_tcp[datasize]; //test if using byte type will have different result
      curMode = getMode();

      /*Read command to buffer*/
      for (int i = 0; i < datasize; i++) 
      {
        data_tcp[i] = client.read(); 
      }

      r = checkComma(data_tcp, r, datasize);

      std::vector<std::vector<int8_t>> command (r, std::vector<int8_t>(c, 0));

      /*declare all command variables*/
      int8_t nrParam;
      int8_t mode[1];
      int8_t heaters[6][3];
      int8_t asc[22][3];
      int8_t ss[1];

      /*checkCommandsource*/
      if (checkCommand(data_tcp) == true)
      {
          command = scanBuffer(data_tcp, r, c, datasize);
          int8_t nrSubCommand = command[2][1];
          
          int k = 0; //int e = 0;
          while (k < r)
          {
            if (command[k][0] == "m" && command[k][1] == "d")
            {
              nrParam = command[k+1][0];
              mode[0] = command[k+2][0];
              
            }
            if (command[k][0] == "h" && command[k][1] == "t" && command[k][2] == "r")
            {
              nrParam = command[k+1][0];
              for (int z = 0; z < nrParam; z++)
              {
                for (int x = 0; x < c; x++)
                {
                  heaters[z][x] = command[k+2+z][x];
                }
              }
            }
            if (command[k][0] == "a" & command[k][1] == "s" && command[k][2] == "c")
            {
               nrParam = command[k+1][0];
              for (int z = 0; z < nrParam; z++)
              {
                for (int x = 0; x < c; x++)
                {
                  asc[z][x] = command[k+2+z][x];
                }
              }
            }
            if (command[k][0] == "s" & command[k][1] == "s")
            {
              nrParam = command[k+1][0];
              ss[0] = command[k+2][0];
            }
          }
          executeMode();
          executeHTR();
          executeASC();
          executeSS();
      }
      
    }
  }
}

int checkComma(int8_t data[], int j, int8_t datasize)
{
  for (int n = 0; n < datasize; n++)
  {
    if (data[n] == ",")
    {
      j++;
    }
  }
  return j;
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



