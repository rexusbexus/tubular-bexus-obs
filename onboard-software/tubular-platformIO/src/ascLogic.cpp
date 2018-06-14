/* Name: ascLogic.ino.cpp
 * Purpose: contains function related to the ASC object.
 * Project: Tubular-Bexus.
 * Authors: Tubular-Bexus software group.
*/

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
    if (meanPressureAmbient <= ascParam[0] && meanPressureAmbient >= (ascParam[1]))
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
    if (meanPressureAmbient >= ascParam[0] && meanPressureAmbient <= (ascParam[1]))
    {
        
        return true;
    }
    else
    {
        return false;
    }
}

