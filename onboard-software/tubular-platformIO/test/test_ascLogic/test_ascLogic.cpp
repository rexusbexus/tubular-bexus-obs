#include "ascLogic.h"
#include "unity.h"

#ifdef UNIT_TEST

void test_normalSamplingLogic_pressure_in_range()
{
    float pressure = 400.351;
    float param [] = {300, 450};
    bool result = ascentSamplingLogic(pressure, param);
    TEST_ASSERT_TRUE(result == true);
    
}

void test_normalSamplingLogic_pressure_not_in_range()
{
    float pressure = 600;
    float param [] = {300, 450};
    bool result = ascentSamplingLogic(pressure, param);
    TEST_ASSERT_TRUE(result == false);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_normalSamplingLogic_pressure_in_range);
    RUN_TEST(test_normalSamplingLogic_pressure_not_in_range);
    UNITY_END();

    return 0;
}

#endif