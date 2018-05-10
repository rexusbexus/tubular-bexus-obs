#include "heaterLogic.h"
#include "unity.h"

#ifdef UNIT_TEST

extern std::vector<float> htrParam;
extern std::vector<float> tempAtHtr;

//min: 0, max: 16777215
void test_logic_of_heaterCompare1(void)
{
    htrParam    = {15, 21, 15, 21};
    tempAtHtr   = {20, 35};
    struct heater flags = heaterCompare();
    //TEST_ASSERT_FALSE(flags.htr1_flag == false);
    TEST_ASSERT_TRUE(flags.htr2_flag == false);
}

void test_logic_of_heaterCompare2(void)
{
    htrParam    = {15, 21, 37, 45};
    tempAtHtr   = {20, 35};
    struct heater flags = heaterCompare();
    //TEST_ASSERT_TRUE(flags.htr1_flag == false);
    TEST_ASSERT_TRUE(flags.htr2_flag == true);
}

void test_logic_of_heater_prio(void){
    htrParam    = {21, 25, 24, 25};
    tempAtHtr   = {20, 20};
    struct heater flags = heaterCompare();
    TEST_ASSERT_FALSE(flags.htr1_flag == false);
    //TEST_ASSERT_TRUE(flags.htr2_flag == true);
}

void test_logic_of_heater_sane(void){
    htrParam    = {20, 25, 20, 25};
    tempAtHtr   = {15, 30};
    struct heater flags = heaterCompare();
    TEST_ASSERT_TRUE(flags.htr1_flag == true);
    //TEST_ASSERT_TRUE(flags.htr2_flag == false);
}


int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_logic_of_heaterCompare1);
    RUN_TEST(test_logic_of_heaterCompare2);
    RUN_TEST(test_logic_of_heater_prio);
    RUN_TEST(test_logic_of_heater_sane);
    UNITY_END();

    return 0;
}

#endif