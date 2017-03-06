
#include "gtest/gtest.h"
#include "test/TableTest.h"
#include <arpa/inet.h>
#include <cstdint>

/*

INSERT INTO  People (ID, Name, Age, Sex, Height) VALUES (14, "Tom Smith", 32, 'M', 56.34);
INSERT INTO  People (ID, Name, Age, Sex, Height) VALUES (38, "Mary Twit", 29, 'F', 34.45678);
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


