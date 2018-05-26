#ifndef SIMULATION_H
#define SIMULATION_H

#include "sensorManager.h"

struct pressureSimulation{
  int simulationTime[8];
  float pressureSim[2][8];
  float temperatureSim[2][8];
  float airflowSim[nrAirFSensors][8];
  float humSim[nrHumidSensors][8];
};

bool checkSimulationOrNot();
pressureSimulation getSimulationData ();


#endif