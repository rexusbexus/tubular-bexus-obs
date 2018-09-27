#ifndef SIMULATION_H
#define SIMULATION_H

#include "sensorManager.h"

const int simulationPoints = 12;

struct pressureSimulation{
  int simulationTime[simulationPoints];
  float pressureSim[4][simulationPoints];
  float temperatureSim[2][simulationPoints];
  float airflowSim[nrAirFSensors][simulationPoints];
  float humSim[nrHumidSensors][simulationPoints];
};

bool checkSimulationOrNot();
pressureSimulation getSimulationData ();


#endif