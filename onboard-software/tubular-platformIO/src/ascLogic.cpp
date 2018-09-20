/* Name: ascLogic.ino.cpp
 * Purpose: contains function related to the ASC object.
 * Project: Tubular-Bexus.
 * Authors: Tubular-Bexus software group.
*/
#include <stdlib.h>

bool ascentOrDescent(float ascParam[])
{
    if (ascParam[0] > ascParam[1])
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ascentSamplingLogic(float meanPressureAmbient, float ascParam[])
{
    if (meanPressureAmbient <= ascParam[0])
    {
        
        return true;
    }
    else
    {
        return false;
    }
}

bool descentSamplingLogic(float meanPressureAmbient, float ascParam[])
{
    if (meanPressureAmbient >= ascParam[0])
    {
        
        return true;
    }
    else
    {
        return false;
    }
}

bool samplingLimit(float meanPressureAmbient, float ascParam[]){
    bool limit = abs(ascParam[1]-ascParam[0]) >= abs(meanPressureAmbient-ascParam[0]);
    return limit;
}

