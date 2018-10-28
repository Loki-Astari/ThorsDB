
#include "SQLUtil.h"

#include "gtest/gtest.h"
#include <sstream>
#include <string>


TEST(SQLUtilTest, ErrorMessage)
{
    std::string message = ThorsAnvil::errorMsg("This", "is", "a", "list");
    ASSERT_EQ(message, "Thisisalist");
}
TEST(SQLUtilTest, ErrorMessageWithNull)
{
    char* data = nullptr;
    std::string message = ThorsAnvil::errorMsg("This", "is", data, "a", "list");
    ASSERT_EQ(message, "Thisisalist");
}
