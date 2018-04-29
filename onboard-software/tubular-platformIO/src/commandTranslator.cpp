    
#include "commandTranslator.h"  
    
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


std::vector<std::vector<byte>> collectingCommand (byte data_tcp[], int row, int col, int datasize, int which)
{   
    std::vector<std::vector<byte>> command (row, std::vector<byte>(col, 0)); 
    /*declare all command variables*/
    byte nrParam;
    std::vector<std::vector<byte>> mode(1, std::vector<byte>(1, 0));
    std::vector<std::vector<byte>> heaters(6, std::vector<byte>(3, 0));
    std::vector<std::vector<byte>> asc(27, std::vector<byte>(3, 0));
    std::vector<std::vector<byte>> ss(1, std::vector<byte>(1, 0));
    
    command = scanBuffer(data_tcp, row, col, datasize);
    byte nrSubCommand = command[2][1];
          
    int k = 0; //int e = 0;
    while (k < row)
    {
        if (command[k][0] == m && command[k][1] == d)
        {
            nrParam = command[k+1][0];
            mode[0][0] = command[k+2][0];
            k=+nrParam;
            k=+2;
        }
        if (command[k][0] == h && command[k][1] == t)
        {
            nrParam = command[k+1][0];
            for (int z = 0; z < nrParam; z++)
            {
                for (int x = 0; x < col; x++)
                {
                    heaters[z][x] = command[k+2+z][x];
                }  
            }
            k=+nrParam;
            k=+2;
        }
        if (command[k][0] == s && command[k][1] == c)
        {
            nrParam = command[k+1][0];
            for (int z = 0; z < nrParam; z++)
            {
                for (int x = 0; x < col; x++)
                {
                    asc[z][x] = command[k+2+z][x];
                }
            }
            k=+nrParam;
            k=+2;
        }
        if (command[k][0] == s && command[k][1] == s)
        {
            nrParam = command[k+1][0];
            ss[0][0] = command[k+2][0];
            k=+nrParam;
            k=+2;
        }
    }
        switch (which)
        {
        case 1:
        return mode;
        case 2:
        return heaters;
        case 3:
        return asc;
        case 4:
        return ss;
        }
    
    
}