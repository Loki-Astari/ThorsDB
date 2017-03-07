
#include "gtest/gtest.h"
#include "test/TableTest.h"
#include <arpa/inet.h>
#include <cstdint>

/*
 * see test/data/init.sql
 * Add test data to this file in table StringTypes
 *
 * These types should read into a std::string
 * MYSQL_TYPE_VAR_STRING
 * MYSQL_TYPE_STRING
 * MYSQL_TYPE_BLOB
 *
 *
 * Question:
 *   Still need to have SQL table with the following field:
 *      MYSQL_TYPE_VARCHAR
 *      MYSQL_TYPE_TINY_BLOB      
 *      MYSQL_TYPE_MEDIUM_BLOB
 *      MYSQL_TYPE_LONG_BLOB
 */


// Bit Values can only be read into "unsigned integer types"a
TEST(TableStringTest, ReadCHAR2String)      {typeGoodTest<std::string>("x",                               "SELECT S1 from StringTypes WHERE Id=1");}// CHAR       => MYSQL_TYPE_STRING
TEST(TableStringTest, ReadVARCHAR2String)   {typeGoodTest<std::string>("A normal Var char",               "SELECT S2 from StringTypes WHERE Id=1");}// VARCHAR    => MYSQL_TYPE_VAR_STRING
TEST(TableStringTest, ReadTINYTEXT2String)  {typeGoodTest<std::string>("Tiny Text",                       "SELECT S3 from StringTypes WHERE Id=1");}// TINYTEXT   => MYSQL_TYPE_BLOB
TEST(TableStringTest, ReadTEXT2String)      {typeGoodTest<std::string>("This is normal text",             "SELECT S4 from StringTypes WHERE Id=1");}// TEXT       => MYSQL_TYPE_BLOB
TEST(TableStringTest, ReadMEDTEXT2String)   {typeGoodTest<std::string>("This is some medium length test", "SELECT S5 from StringTypes WHERE Id=1");}// MEDIUMTEXT => MYSQL_TYPE_BLOB
TEST(TableStringTest, ReadLONGTEXT2String)  {typeGoodTest<std::string>("This is some long text",          "SELECT S6 from StringTypes WHERE Id=1");}// LONGTEXT   => MYSQL_TYPE_BLOB


