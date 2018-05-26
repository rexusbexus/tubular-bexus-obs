    
#include "commandTranslator.h" 

std::vector<std::vector<byte>> mode(1, std::vector<byte>(1, 0));
std::vector<std::vector<byte>> heaters(6, std::vector<byte>(3, 0));
std::vector<std::vector<byte>> asc(27, std::vector<byte>(3, 0));
std::vector<std::vector<byte>> ss(1, std::vector<byte>(1, 0)); 
    
std::vector<std::vector<byte>> scanBuffer(byte bufferD[], int row, int col, byte datasize)
{
  
  int i = 0;
  
  std::vector<std::vector<byte>> command(row, std::vector<byte>(col,0));
  for (int n = 0; n < row; n++)
  {
    int k = 0;
    while(bufferD[i] != ',')
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

int checkComma(byte data[], int row, byte datasize)
{
  for (int n = 0; n < datasize; n++)
  {
    if (data[n] == ',')
    {
      row++;
    }
  }
  return row;
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


void collectingCommand (byte data_tcp[], int row, int col, byte datasize)
{
    std::vector<std::vector<byte>> command (row, std::vector<byte>(col, 0));
    int nrParam;
    command = scanBuffer(data_tcp, row, col, datasize);
    int nrSubCommand = command[1][0]-'0';
    int k = 2;
    for (int jenis = 1; jenis <= nrSubCommand; jenis++)
    {
        if (command[k][0] == 'm' && command[k][1] == 'd')
        {
            nrParam = command[k+1][0] - '0';
            mode[0][0] = command[k+2][0] - '0';
            k=k+2+nrParam;
        }

        if (k==(row))
        {
            break;
        }

        if (command[k][0] == 'h' && command[k][1] == 't')
        {
            nrParam = command[k+1][0] - '0';
            for (int z = 0; z < nrParam; z++)
            {
                for (int x = 0; x < col; x++)
                {
                    heaters[z][x] = command[k+2+z][x] - '0';
                }
            }
            k=k+2+nrParam;
        }

        if (k==(row))
        {
            break;
        }

        if (command[k][0] == 's' && command[k][1] == 'c')
        {
            nrParam = command[k+1][0] - '0';
            for (int b = 0; b < nrParam; b++)
            {
                for (int v = 0; v < col; v++)
                {
                    asc[b][v] = command[k+2+b][v] - '0';
                } 
            }
            k=k+2+nrParam;
        }

        if (k==(row))
        {
            break;
        }

        if (command[k][0] == 's' && command[k][1] == 's')
        {
            nrParam = command[k+1][0] - '0';
            ss[0][0] = command[k+2][0];
            k=k+2+nrParam;
        }

        if (k==(row))
        {
            break;
        }
        //k++;
    }
}