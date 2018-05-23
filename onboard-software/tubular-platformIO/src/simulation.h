#ifndef SIMULATION_H
#define SIMULATION_H

#include "sensorManager.h"

struct pressureSimulation{
  int simulationTime[8];
  int pressureSim[2][8];
  int temperatureSim[2][8];
  int airflowSim[nrAirFSensors][8];
  int humSim[nrHumidSensors][8];
};

bool checkSimulationOrNot();
pressureSimulation getSimulationData ();


#endif