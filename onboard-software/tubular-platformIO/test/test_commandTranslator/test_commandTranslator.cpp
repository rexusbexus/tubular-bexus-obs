#include "commandTranslator.h"
#include "unity.h"

#ifdef UNIT_TEST

extern std::vector<std::vector<byte>> mode;
extern std::vector<std::vector<byte>> heaters;
extern std::vector<std::vector<byte>> asc;
extern std::vector<std::vector<byte>> ss; 

void test_checkCommand_with_correct_command()
{
    byte telecommand[] = {"tub"};
    boolean result = checkCommand(telecommand);
    TEST_ASSERT_TRUE(result == true);
}

void test_checkCommand_with_false_command()
{
    byte telecommand[] = {"god"};
    boolean result = checkCommand(telecommand);
    TEST_ASSERT_TRUE(result == false);
}

void test_checkComma_with_five_commas()
{
    byte telecommand[] = {"tub,3,md,1,1,"};
    int row = checkComma(telecommand, 0, sizeof(telecommand));
    TEST_ASSERT_EQUAL_INT(5, row);
}

void test_checkComma_with_no_commas()
{
    byte telecommand[] = {"tub3md11"};
    int row = checkComma(telecommand, 0, sizeof(telecommand));
    TEST_ASSERT_EQUAL_INT(0, row);
}

void test_scanBuffer_with_correct_command()
{
    int col = 3;
    byte telecommand[] = {"tub,3,md,1,1,"};
    int row = checkComma(telecommand, 0, sizeof(telecommand));
    TEST_ASSERT_EQUAL_INT(5, row);
    std::vector<std::vector<byte>> command(row, std::vector<byte>(col,0));
    command = scanBuffer(telecommand, row, col, sizeof(telecommand));
    TEST_ASSERT_EQUAL_UINT8(telecommand[0], command[0][0]);
    TEST_ASSERT_EQUAL_UINT8(telecommand[2], command[0][2]);
    TEST_ASSERT_EQUAL_UINT8(telecommand[4], command[1][0]);
    TEST_ASSERT_EQUAL_UINT8(telecommand[11], command[4][0]);
}

void test_collectCommand_with_valid_command()
{
    int col = 3;
    byte telecommand[] = {"tub,3,md,1,1,"};
    int row = checkComma(telecommand, 0, sizeof(telecommand));
    collectingCommand (telecommand, row, col, sizeof(telecommand));

    TEST_ASSERT_EQUAL_UINT8(telecommand[11], mode[0][0]);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_checkCommand_with_correct_command);
    RUN_TEST(test_checkCommand_with_false_command);
    RUN_TEST(test_checkComma_with_five_commas);
    RUN_TEST(test_checkComma_with_no_commas);
    RUN_TEST(test_scanBuffer_with_correct_command);
    RUN_TEST(test_collectCommand_with_valid_command);
    UNITY_END();

    return 0;
}

#endif