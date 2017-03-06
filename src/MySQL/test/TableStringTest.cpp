
#include "gtest/gtest.h"
#include "test/TableTest.h"
#include <arpa/inet.h>
#include <cstdint>

/*
 * MYSQL_TYPE_VARCHAR
 * MYSQL_TYPE_VAR_STRING
 * MYSQL_TYPE_STRING
 *
 * These types should read into a std::string
 */


// Bit Values can only be read into "unsigned integer types"a
TEST(TableStringTest, ReadCHAR2String)      {typeGoodTest<std::string>("x",                               "SELECT S1 from StringTypes WHERE Id=1");}// CHAR
TEST(TableStringTest, ReadVARCHAR2String)   {typeGoodTest<std::string>("A normal Var char",               "SELECT S2 from StringTypes WHERE Id=1");}// VARCHAR    => MYSQL_TYPE_VAR_STRING
TEST(TableStringTest, ReadTINYTEXT2String)  {typeGoodTest<std::string>("Tiny Text",                       "SELECT S3 from StringTypes WHERE Id=1");}// TINYTEXT   => MYSQL_TYPE_BLOB
TEST(TableStringTest, ReadTEXT2String)      {typeGoodTest<std::string>("This is normal text",             "SELECT S4 from StringTypes WHERE Id=1");}// TEXT       => MYSQL_TYPE_BLOB
TEST(TableStringTest, ReadMEDTEXT2String)   {typeGoodTest<std::string>("This is some medium length test", "SELECT S5 from StringTypes WHERE Id=1");}// MEDIUMTEXT => MYSQL_TYPE_BLOB
TEST(TableStringTest, ReadLONGTEXT2String)  {typeGoodTest<std::string>("This is some long text",          "SELECT S6 from StringTypes WHERE Id=1");}// LONGTEXT   => MYSQL_TYPE_BLOB


