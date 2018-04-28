#include "sensorManager.h"
#include "unity.h"

#ifdef UNIT_TEST

void test_writeDataToSensorBuffers_correct_input(void)
{
    int min[4] = {0, 1, 2, 3};
    float testTempe[nrTempSensors] = {1.123,2.4123,3.124,4.12451,5.12512,6.123,7.125,8.125,9.613421};
    writeDataToSensorBuffers(testTempe, min[0]);
    TEST_ASSERT_EQUAL_FLOAT(1.123, tempReading[0]);

    float testHum[nrHumidSensors] = {93.45};
    writeDataToSensorBuffers(testHum, min[1]);
    TEST_ASSERT_EQUAL_FLOAT(93.45, humReading[0]);
    
    float testPress[nrPressSensors] = {230.4312, 421.2412, 432.123, 952.124, 732.2123, 1300.823};
    writeDataToSensorBuffers(testPress, min[2]);
    TEST_ASSERT_EQUAL_FLOAT(230.4312, pressReading[0]);

    float testAF[nrAirFSensors] = {3000.223};
    writeDataToSensorBuffers(testAF, min[3]);
    TEST_ASSERT_EQUAL_FLOAT(3000.223, afReading[0]);
}

void test_writeDataToSensorBuffers_superbig_input(void)
{
    int min[4] = {0, 1, 2, 3};
    float testTempe[nrTempSensors] = {1.123, 4000000, 3.124, 4.12451, 5.12512, 6.123, 7.125, 8.125, 9.613421};
    writeDataToSensorBuffers(testTempe, min[0]);
    TEST_ASSERT_EQUAL_FLOAT(4000000, tempReading[1]);

    float testHum[nrHumidSensors] = {50000};
    writeDataToSensorBuffers(testHum, min[1]);
    TEST_ASSERT_EQUAL_FLOAT(50000, humReading[0]);
    
    float testPress[nrPressSensors] = {230.4312, 421.2412, 7382738, 952.124, 732.2123, 1300.823};
    writeDataToSensorBuffers(testPress, min[2]);
    TEST_ASSERT_EQUAL_FLOAT(7382738, pressReading[2]);

    float testAF[nrAirFSensors] = {9876543};
    writeDataToSensorBuffers(testAF, min[3]);
    TEST_ASSERT_EQUAL_FLOAT(9876543, afReading[0]);
}

void test_writeDataToSensorBuffers_neg_big_input(void)
{
    int min[4] = {0, 1, 2, 3};
    float testTempe[nrTempSensors] = {1.123, -4000000, 3.124, 4.12451, 5.12512, 6.123, 7.125, 8.125, 9.613421};
    writeDataToSensorBuffers(testTempe, min[0]);
    TEST_ASSERT_EQUAL_FLOAT(-4000000, tempReading[1]);

    float testHum[nrHumidSensors] = {-50000};
    writeDataToSensorBuffers(testHum, min[1]);
    TEST_ASSERT_EQUAL_FLOAT(-50000, humReading[0]);
    
    float testPress[nrPressSensors] = {230.4312, 421.2412, -7382738, 952.124, 732.2123, 1300.823};
    writeDataToSensorBuffers(testPress, min[2]);
    TEST_ASSERT_EQUAL_FLOAT(-7382738, pressReading[2]);

    float testAF[nrAirFSensors] = {-9876543};
    writeDataToSensorBuffers(testAF, min[3]);
    TEST_ASSERT_EQUAL_FLOAT(-9876543, afReading[0]);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_writeDataToSensorBuffers_correct_input);
    RUN_TEST(test_writeDataToSensorBuffers_superbig_input);
    RUN_TEST(test_writeDataToSensorBuffers_neg_big_input);
    UNITY_END();

    return 0;
}

#endif