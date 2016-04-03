
#include "ThorSQL/Connection.h"
#include "ThorSQL/Statement.h"
#include "gtest/gtest.h"
#include "MySQLConfig.h"

/*
unsigned long long
unsigned long
unsigned int
unsigned short
unsigned char

signed long long
signed long
signed int
signed short
signed char
char

std::string
std::vector<char>
float
double
*/

template<typename T>
void typeGoodTest(T expected, std::string const& expr)
{
    using namespace ThorsAnvil;

    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    SQL::Statement      statement(connection, expr);
    statement.execute([&expected](T const& select){
        ASSERT_EQ(expected, select);
    });
}
template<typename T, typename ExceptionType>
void typeBadTest(std::string const& expr)
{
    using namespace ThorsAnvil;

    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    SQL::Statement      statement(connection, expr);
    ASSERT_THROW(
        statement.execute([](T const& select){
            ASSERT_TRUE(false); // Should not reach here.
        }),
        ExceptionType
    );
}


TEST(IntTableTest, ReadBitIntoUChar)     { typeGoodTest<unsigned char>(0xA, "SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoUShort)    { typeGoodTest<unsigned short>(0xA, "SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoUInt)      { typeGoodTest<unsigned int>(0xA, "SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoULong)     { typeGoodTest<unsigned long>(0xA, "SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoULongLong) { typeGoodTest<unsigned long long>(0xA, "SELECT B0 FROM IntTypes where Id=1"); }

TEST(IntTableTest, ReadBitIntoChar)      { typeBadTest<char,            std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoSChar)     { typeBadTest<signed char,     std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoSShort)    { typeBadTest<signed short,    std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoSInt)      { typeBadTest<signed int,      std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoSLong)     { typeBadTest<signed long,     std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoSLongLong) { typeBadTest<signed long long,std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }

TEST(IntTableTest, ReadBitIntoString)    { typeBadTest<std::string,     std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoBlob)      { typeBadTest<std::vector<char>, std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoFloat)     { typeBadTest<float,           std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoDouble)    { typeBadTest<double,          std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }

TEST(intTableTest, ReadBit48IntoUChar)   { typeBadTest<unsigned char,   std::runtime_error>("SELECT B3 FROM IntTypes where Id=1"); }
TEST(intTableTest, ReadBit24IntoUChar)   { typeBadTest<unsigned char,   std::runtime_error>("SELECT B2 FROM IntTypes where Id=1"); }
TEST(intTableTest, ReadBit12IntoUChar)   { typeBadTest<unsigned char,   std::runtime_error>("SELECT B1 FROM IntTypes where Id=1"); }

TEST(intTableTest, ReadBit48IntoUShort)  { typeBadTest<unsigned short,  std::runtime_error>("SELECT B3 FROM IntTypes where Id=1"); }
TEST(intTableTest, ReadBit24IntoUShort)  { typeBadTest<unsigned short,  std::runtime_error>("SELECT B2 FROM IntTypes where Id=1"); }

TEST(intTableTest, ReadBit48IntoUInt)    { typeBadTest<unsigned int,    std::runtime_error>("SELECT B3 FROM IntTypes where Id=1"); }


//TEST(intTableTest, ReadTinyIntoUchar)    { typeGoodTest<unsigned char>(0x12, "SELECT U2 FORM IntTypes where Id=1");}
//TEST(intTableTest, ReadTinyIntoIchar)    { typeGoodTest<signed char>(0x12, "SELECT I2 FORM IntTypes where Id=1");}
TEST(intTableTest,ALL)
{
    using namespace ThorsAnvil;

    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    SQL::Statement      statement(connection, "SELECT * from IntTypes");
}

