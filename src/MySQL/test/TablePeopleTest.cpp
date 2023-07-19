
#include "gtest/gtest.h"
#include "test/TableTest.h"
#ifdef __WINNT__
#else
#include <arpa/inet.h>
#endif
#include <cstdint>

/*
 * see test/data/init.sql
 * Add test data to this file in table People
 *
 */


// Bit Values can only be read into "unsigned integer types"a
TEST(TablePeopleTest, Read1Name)      {typeGoodTest<std::string>("Tom Smith",         "SELECT Name   FROM People WHERE Id=1");}
TEST(TablePeopleTest, Read1Age)       {typeGoodTest<int>(32,                          "SELECT Age    FROM People WHERE Id=1");}
TEST(TablePeopleTest, Read1Sex)       {typeGoodTest<std::string>("M",                 "SELECT Sex    FROM People WHERE Id=1");}
TEST(TablePeopleTest, Read1Height)    {typeGoodTest<double>(56.34,                    "SELECT Height FROM People WHERE Id=1");}

TEST(TablePeopleTest, Read2Name)      {typeGoodTest<std::string>("Mary Twit",         "SELECT Name   FROM People WHERE Id=2");}
TEST(TablePeopleTest, Read2Age)       {typeGoodTest<int>(29,                          "SELECT Age    FROM People WHERE Id=2");}
TEST(TablePeopleTest, Read2Sex)       {typeGoodTest<std::string>("F",                 "SELECT Sex    FROM People WHERE Id=2");}
TEST(TablePeopleTest, Read2Height)    {typeGoodTest<double>(34.45678,                 "SELECT Height FROM People WHERE Id=2");}


