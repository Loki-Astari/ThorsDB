
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
TEST(TableBinaryTest, ReadBinary)      {typeGoodTest<std::string>("B",                               "SELECT B1 from BinaryTypes WHERE Id=1");} // BINARY   => MYSQL_TYPE_STRING     (its like CHAR)
TEST(TableBinaryTest, ReadVarBinary)   {typeGoodTest<std::string>("A variable binary string",        "SELECT B2 from BinaryTypes WHERE Id=1");} // VARBINARY=> MYSQL_TYPE_VAR_STRING (its like VARCHAR)
TEST(TableBinaryTest, ReadTinyBlob)    {typeGoodTest<std::string>("A Tiny Blob",                     "SELECT B3 from BinaryTypes WHERE Id=1");} // TINYBLOB => MYSQL_TYPE_BLOB       (its like TINYTEXT)
TEST(TableBinaryTest, ReadBlob)        {typeGoodTest<std::string>("A Normal Blob",                   "SELECT B4 from BinaryTypes WHERE Id=1");} // BLOB     => MYSQL_TYPE_BLOB       (its like TEXT)
TEST(TableBinaryTest, ReadMedBlob)     {typeGoodTest<std::string>("A Medium Sized Blob",             "SELECT B5 from BinaryTypes WHERE Id=1");} // MEDIUMBLOB=> MYSQL_TYPE_BLOB      (its like MEDIUMTEXT)
TEST(TableBinaryTest, ReadLongBlob)    {typeGoodTest<std::string>("This is the best I can do for long Blob","SELECT B6 from BinaryTypes WHERE Id=1");} // LONGBLOB => MYSQL_TYPE_BLOB(its like LONGTEXT)


