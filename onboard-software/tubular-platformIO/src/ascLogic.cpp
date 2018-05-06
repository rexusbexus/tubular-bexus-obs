


bool normalSamplingLogic(float meanPressureAmbient, float ascParam[])
{
    if (meanPressureAmbient >= ascParam[0] && meanPressureAmbient<= (ascParam[1]))
    {
        return true;
    }
    else
    {
        return false;
    }
}
