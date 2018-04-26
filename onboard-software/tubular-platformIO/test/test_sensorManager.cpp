#include "sensorManager.h"
#include "unity.h"

#ifdef UNIT_TEST

void test_writeDataToSensorBuffers_temperature_input(void)
{
    float testVar[nrTempSensors] = {1.123,2.4123,3.124,4.12451,5.12512,6.123,7.125,8.125,9.613421};
    int min = 0;
    writeDataToSensorBuffers(testVar, min);
    TEST_ASSERT_EQUAL_FLOAT(1.123, tempReading[0]);
   
}


int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_writeDataToSensorBuffers_temperature_input);
    UNITY_END();

    return 0;
}

#endif