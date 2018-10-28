
#include "MD5.h"

#include "gtest/gtest.h"
#include <cctype>


TEST(MD5Test, Create)
{
    std::string result = ThorsAnvil::Util::md5("TestString");
    std::transform(std::begin(result), std::end(result), std::begin(result), [](char x){return ::toupper(x);});
    ASSERT_EQ("5B56F40F8828701F97FA4511DDCD25FB", result);
}
TEST(MD5Test, CreateFromObjectEmpty)
{
    ThorsAnvil::Util::MD5       test;
    test.update("JustString", 10);
    test.finalize();
    std::string result = test.hexdigest();
    std::transform(std::begin(result), std::end(result), std::begin(result), [](char x){return ::toupper(x);});
    ASSERT_EQ("7919A48CA7C57EEA995FBCB00AB3936B", result);
}
TEST(MD5Test, CreateFromObjectTestString)
{
    ThorsAnvil::Util::MD5       test("Init");
    test.update("WithInit", 10);
    test.finalize();
    std::string result = test.hexdigest();
    std::transform(std::begin(result), std::end(result), std::begin(result), [](char x){return ::toupper(x);});
    ASSERT_EQ("95B19F7739B0B7EA7D6B07586BE54F36", result);
}
TEST(MD5Test, CreateFromObjectTestStringStream)
{
    ThorsAnvil::Util::MD5       test("Init");
    test.update("WithInit", 10);
    test.finalize();
    std::stringstream resultStream;
    resultStream << test;
    std::string result = resultStream.str();
    std::transform(std::begin(result), std::end(result), std::begin(result), [](char x){return ::toupper(x);});
    ASSERT_EQ("95B19F7739B0B7EA7D6B07586BE54F36", result);
}
TEST(MD5Test, NotFinalized)
{
    ThorsAnvil::Util::MD5       test;
    std::string result = test.hexdigest();
    ASSERT_EQ("", result);
}

