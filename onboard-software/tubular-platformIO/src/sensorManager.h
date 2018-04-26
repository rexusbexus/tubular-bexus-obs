#ifndef WHICHDATA_H
#define WHICHDATA_H

#include <vector>
#include <cstdint>



#define nrTempSensors   9
#define nrPressSensors  6
#define nrHumidSensors  1
#define nrAirFSensors   1



extern float tempReading [nrTempSensors]; //array size might change according to the fix quantity
extern float humReading [nrHumidSensors];
extern float pressReading [nrPressSensors]; //array size might change according to the fix quantity
extern float afReading[nrAirFSensors];

void writeDataToSensorBuffers(float curMeasurements [], int type);
std::vector<float> readDataFromSensorBuffers(int type);
void changeTheMode(float pressDifference, uint8_t currentMode, float meanPressureAmbient);
float calculatingPressureDifference(float meanPressureAmbient);


#endif