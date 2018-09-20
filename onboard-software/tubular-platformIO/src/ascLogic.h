#ifndef ASCLOGIC_H
#define ASCLOGIC_H


bool ascentSamplingLogic(float meanPressureAmbient, float ascParam[]);
bool descentSamplingLogic(float meanPressureAmbient, float ascParam[]);
bool ascentOrDescent(float ascParam[]);
bool samplingLimit(float meanPressureAmbient, float ascParam[]);

#endif