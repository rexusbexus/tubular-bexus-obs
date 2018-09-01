#include "simulation.h"
#include <SD.h>
#include <vector>
#include "sensorManager.h"

// File timeSim;
// pressureSimulation Sim_struct;

std::vector<int> getSeconds(char all_data[])
{
  // char buf[8];
  std::vector<int> seconds(8);
  int i = 0;  int b = 0;
  while(b<8)
  {
    int k = 0;
    char buf[8] = {0};
    while(1)
    {
      if (all_data[i] == ',')
      {
        i++;
        break;
      }
      buf[k] = all_data[i];
      
      i++; k++;
    }
    seconds[b] = atoi(buf);
    
    b++;
  }
  return seconds;
}

bool checkSimulationOrNot()
{
  File sim = SD.open("sim.txt", FILE_READ);
  if (sim)
  {
    sim.close();
    return true;
  }
  else
  {
    Serial.println("Simulation file not found");
    return false;
  }
}

std::vector<std::vector<int>> getTemperature(char all_data[])
{
  // char buf[8];
  std::vector<std::vector<int>> temperature(2, std::vector<int> (8,0));
  int i = 0;  int c = 0; int b = 0;
  while(b<2)
  {
    while(1)
    {
        int k = 0;
        char buf[8] = {0};
        if (all_data[i] == '|')
        {
            i++; 
            break;
        }
        while(1)
        {
            if (all_data[i] == ',')
            {
                i++; 
                break;
            }
            buf[k] = all_data[i];
      
            i++; k++;
        }
        temperature[b][c] = atoi(buf);
        c++;
    }
    b++; c = 0;
  }
  return temperature;
}

std::vector<std::vector<int>> getHumidity(char all_data[])
{
  // char buf[8];
  std::vector<std::vector<int>> humidity(nrHumidSensors, std::vector<int> (8,0));
  int i = 0;  int c = 0; int b = 0;
  while(b<nrHumidSensors)
  {
    while(1)
    {
        int k = 0;
        char buf[8] = {0};
        if (all_data[i] == '|')
        {
            i++; 
            break;
        }
        while(1)
        {
            if (all_data[i] == ',')
            {
                i++; 
                break;
            }
            buf[k] = all_data[i];
      
            i++; k++;
        }
        humidity[b][c] = atoi(buf);
        c++;
    }
    b++; c = 0;
  }
  return humidity;
}

std::vector<std::vector<int>> getAirflow(char all_data[])
{
  // char buf[8];
  std::vector<std::vector<int>> Airflow(nrAirFSensors, std::vector<int> (8,0));
  int i = 0;  int c = 0; int b = 0;
  while(b<nrAirFSensors)
  {
    while(1)
    {
        int k = 0;
        char buf[8] = {0};
        if (all_data[i] == '|')
        {
            i++; 
            break;
        }
        while(1)
        {
            if (all_data[i] == ',')
            {
                i++; 
                break;
            }
            buf[k] = all_data[i];
      
            i++; k++;
        }
        Airflow[b][c] = atoi(buf);
        c++;
    }
    b++; c = 0;
  }
  return Airflow;
}

std::vector<std::vector<float>> getPressure(char all_data[])
{
  
  std::vector<std::vector<float>> pressure(3, std::vector<float> (8,0));
  int i = 0;  int c = 0; int b = 0;
  while(b<3)
  {
    while(1)
    {
        int k = 0;
        char buf[8] = {0};
        if (all_data[i] == '|')
        {
            i++; 
            break;
        }
        while(1)
        {
            if (all_data[i] == '|')
            {
              // i++; 
              break;
            }
            if (all_data[i] == ',')
            {
                i++; 
                break;
            }
            buf[k] = all_data[i];
      
            i++; k++;
        }
        pressure[b][c] = atof(buf);
        // Serial.println(pressure[b][c]);
        c++;
    }
    b++; c = 0;
  }
  return pressure;
}



// pressureSimulation getSimulationData ()
// {
//   pressureSimulation Sim_struct;
//   File simP = SD.open("siP.txt", FILE_READ);
  
//   if (simP)
//   {
//     Serial.println("siP");
//     char press_data[simP.size()];
//     int i = 0;
    
//     while (simP.available())
//     {
//       press_data[i] = simP.read();
//       i++;
//     }
//     Serial.println();
//     // Serial.println(simP.size());
//     // Serial.flush();
//     // Serial.println(String(press_data));
    
//     std::vector<std::vector<int>> pressure = getPressure(press_data);
//     for (int g = 0; g < 2; g++)
//     {
//       for (int c = 0; c < 8; c++)
//       {
//           Sim_struct.pressureSim[g][c] = pressure[g][c];
//       }
//     }
//     simP.close();
//   }

//   File simTe = SD.open("siT.txt", FILE_READ);
//   if (simTe)
//   {
//     Serial.println("siT");
//     char temp_data[simTe.size()];
//     int i = 0;
    
//     while (simTe.available())
//     {
//       temp_data[i] = simTe.read();
//       i++;
//     }
    
//     std::vector<std::vector<int>> temperature = getTemperature(temp_data);
//     for (int g = 0; g < 2; g++)
//     {
//       for (int c = 0; c < 8; c++)
//       {
//           Sim_struct.temperatureSim[g][c] = temperature[g][c];
//       }
//     }
//     simTe.close();
//   }

//   File simH = SD.open("siH.txt", FILE_READ);
//   if (simH)
//   {
//     Serial.println("siH");
//     char hum_data[simH.size()];
//     int i = 0;
    
//     while (simH.available())
//     {
//       hum_data[i] = simH.read();
//       i++;
//     }
    
//     std::vector<std::vector<int>> humidity = getHumidity(hum_data);
//     for (int g = 0; g < nrHumidSensors; g++)
//     {
//       for (int c = 0; c < 8; c++)
//       {
//           Sim_struct.humSim[g][c] = humidity[g][c];
//       }
//     }
//     simH.close();
//   }

//   File simAF = SD.open("siA.txt", FILE_READ);
//   if (simAF)
//   {
//     Serial.println("siA");
//     char AF_data[simAF.size()];
//     int i = 0;
    
//     while (simAF.available())
//     {
//       AF_data[i] = simAF.read();
//       i++;
//     }
    
//     std::vector<std::vector<int>> Airflow = getAirflow(AF_data);
//     for (int g = 0; g < nrAirFSensors; g++)
//     {
//       for (int c = 0; c < 8; c++)
//       {
//           Sim_struct.airflowSim[g][c] = Airflow[g][c];
//       }
//     }
//     simAF.close();
//   }

//   File simT = SD.open("siTi.txt", FILE_READ);
//   if(simT)
//   {
//     Serial.println("siTi");
//     char time_data[simT.size()];
//     int i = 0;
    
//     while (simT.available())
//     {
//       time_data[i] = simT.read();
//       i++;
//     }
    
//     std::vector<int> seconds = getSeconds(time_data);
//     for (int g = 0; g < 8; g++)
//     {
//       Sim_struct.simulationTime[g] = seconds[g];
//     }
//     simT.close();
//   }
//   return Sim_struct;
// }


pressureSimulation getSimulationData ()
{
  pressureSimulation Sim_struct;
  // float pressureSimData [] = {1000,75,46.8,81.2,103,141,193.3,1000};
  float temperatureSimData [] = {20,15,10,5,-5,-10,5,20};
  float humSimData [] = {0,0,0,0,0,0,0,0};
  float airflowSimData [] = {400,1000,300,500,692,582,932,412};
  // int timeSimData [] = {0,5400,6900,14520,14700,15000,15300,17700};
  // for (int k = 0; k < 2; k++)
  // {
  //   for (int g = 0; g < 8; g++)
  //   {
  //     Sim_struct.pressureSim[k][g] = pressureSimData[g];
  //   }
  // }


  for (int k = 0; k < 2; k++)
  {
    for (int g = 0; g < 8; g++)
    {
      Sim_struct.temperatureSim[k][g] = temperatureSimData[g];
    }
  }

  for (int k = 0; k < 1; k++)
  {
    for (int g = 0; g < 8; g++)
    {
      Sim_struct.humSim[k][g] = humSimData[g];
    }
  }

  for (int k = 0; k < 1; k++)
  {
    for (int g = 0; g < 8; g++)
    {
      Sim_struct.airflowSim[k][g] = airflowSimData[g];
    }
  }

 
  // for (int g = 0; g < 8; g++)
  // {
  //   Sim_struct.simulationTime[g] = timeSimData[g];
  // }
  File simP = SD.open("siP.txt", FILE_READ);
  
  if (simP)
  {
    // Serial.println("siP");
    char press_data[simP.size()];
    int i = 0;
    
    while (simP.available())
    {
      press_data[i] = simP.read();
      i++;
    }
    // Serial.println(String(press_data));
    // delay(10000);
    std::vector<std::vector<float>> pressure = getPressure(press_data);
    for (int g = 0; g < 3; g++)
    {
      for (int c = 0; c < 8; c++)
      {
          Sim_struct.pressureSim[g][c] = pressure[g][c];
      }
    }
    simP.close();
  }
  else
  {
    Serial.println("Failed to open siP.txt");
    float pressureSimData [] = {1000,75,46.8,81.2,103,141,193.3,1000};
    for (int k = 0; k < 3; k++)
    {
      for (int g = 0; g < 8; g++)
      {
        Sim_struct.pressureSim[k][g] = pressureSimData[g];
      }
    }
  }

  File timeSim = SD.open("tim.txt", FILE_READ);
  // Serial.println(timeSim);
  if(timeSim)
  {
    char time_data[timeSim.size()];
    int i = 0;
    
    // Serial.println("Start reading siTi.txt");

    while (timeSim.available())
    {
      time_data[i] = timeSim.read();
      i++;
    }
    // Serial.println("Finished reading siTi.txt");

    std::vector<int> seconds = getSeconds(time_data);
    for (int g = 0; g < 8; g++)
    {
      Sim_struct.simulationTime[g] = seconds[g];
    }
    timeSim.close();
  }
  else
  {
    Serial.println("Failed to open tim.txt");
    int timeSimData [] = {0,5400,6900,14520,14700,15000,15300,17700};
    for (int g = 0; g < 8; g++)
    {
      Sim_struct.simulationTime[g] = timeSimData[g];
    }
    // timeSim.close();
  }

  return Sim_struct;
}