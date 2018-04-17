
#define modeCommand 1
#define htrCommand 2
#define ascCommand 3
#define separator 0x2C
#define m 0x6D
#define d 0x64
#define h 0x68
#define t 0x74
#define r 0x72
#define a 0x61
#define s 0x73
#define c 0x63

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

std::vector<std::vector<byte>> scanBuffer(byte bufferD[], int row, int col, int datasize)
{
  
  int i = 0;
  int k = 0;
  std::vector<std::vector<byte>> command(row, std::vector<byte>(col,0));
  for (int n = 0; n < row; n++)
  {
    while(bufferD[i] != separator)
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

  byte curMode;
  int commandSize; 
  std::vector<byte> mode;
  std::vector<byte> htr;
  std::vector<byte> asc;

  while(1)
  {
    if( xSemaphoreTake( semPeriodic, portMAX_DELAY ) == pdTRUE )
    {
      
      int row = 0;
      int col = 3;
      EthernetClient client = server.available();
      byte datasize = client.available();
      byte data_tcp[datasize]; //test if using byte type will have different result
      curMode = getMode();

      /*Read command to buffer*/
      for (int i = 0; i < datasize; i++) 
      {
        data_tcp[i] = client.read(); 
      }

      row = checkComma(data_tcp, row, datasize);

      std::vector<std::vector<byte>> command (row, std::vector<byte>(col, 0));

      /*declare all command variables*/
      byte nrParam;
      byte mode[1];
      byte heaters[6][3];
      byte asc[22][3];
      byte ss[1];

      /*checkCommandsource*/
      if (checkCommand(data_tcp) == true)
      {
          command = scanBuffer(data_tcp, row, col, datasize);
          byte nrSubCommand = command[2][1];
          
          int k = 0; //int e = 0;
          while (k < r)
          {
            if (command[k][0] == m && command[k][1] == d)
            {
              nrParam = command[k+1][0];
              mode[0] = command[k+2][0];
              
            }
            if (command[k][0] == h && command[k][1] == t && command[k][2] == r)
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
            if (command[k][0] == a & command[k][1] == s && command[k][2] == c)
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
            if (command[k][0] == s & command[k][1] == s)
            {
              nrParam = command[k+1][0];
              ss[0] = command[k+2][0];
            }
          }
          executeMode(mode);
          executeHTR();
          executeASC();
          executeSS();
      }
      
    }
  }
}

int checkComma(byte data[], int j, byte datasize)
{
  for (int n = 0; n < datasize; n++)
  {
    if (data[n] == separator)
    {
      j++;
    }
  }
  return j;
}

boolean checkCommand(byte data[])
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

void executeMode(int8_t mode[])
{
  setMode(mode[0]);
}

void executeHTR(int8_t heaters[][])
{
  float dummyParam [4];
  floatval param;
  heaterControl(heaters[0][0], heaters [1][0]);
  for (int i = 2; i<6; i++)
  {
    for (int k = 0; k < 3; k++) 
    {
      param.bytes[k] = {heaters[i][k];
    }
    dummyParam[i-2] = param.val;
  }
  setHeaterParameter(dummyParam);
}

void executeASC()
{
  float dummyParam [4];
  floatval param;
 
  for (int i = 2; i<27; i++)
  {
    for (int k = 0; k < 3; k++) 
    {
      param.bytes[k] = {heaters[i][k];
    }
    param.bytes[3] = byte(0);
    dummyParam[i-2] = param.val;
  }
  setASCParameter(dummyParam);
}

