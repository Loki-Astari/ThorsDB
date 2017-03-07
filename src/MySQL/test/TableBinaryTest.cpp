
#include "gtest/gtest.h"
#include "test/TableTest.h"
#include <arpa/inet.h>
#include <cstdint>

/*
 * see test/data/init.sql
 * Add test data to this file in table StringTypes
 *
 */


// Bit Values can only be read into "unsigned integer types"a
TEST(TableBinaryTest, ReadBinary)      {typeGoodTest<std::string>("B",                               "SELECT B1 from BinaryTypes WHERE Id=1");} // BINARY   => MYSQL_TYPE_STRING  (its like char)
TEST(TableBinaryTest, ReadVarBinary)   {typeGoodTest<std::string>("A variable binary string",        "SELECT B2 from BinaryTypes WHERE Id=1");} // VARBINARY=> MYSQL_TYPE_VAR_STRING (its like varchar)


