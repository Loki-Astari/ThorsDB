
#include "MD5.h"

#include "gtest/gtest.h"
#include <cctype>


TEST(MD5Test, Create)
{
    std::string result = ThorsAnvil::Util::md5("TestString");
    std::transform(std::begin(result), std::end(result), std::begin(result), [](char x){return ::toupper(x);});
    ASSERT_EQ("5B56F40F8828701F97FA4511DDCD25FB", result);
}

