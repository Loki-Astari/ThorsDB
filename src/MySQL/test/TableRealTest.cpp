
#include "gtest/gtest.h"
#include "test/TableTest.h"
#include <arpa/inet.h>
#include <cstdint>

/*
 * see test/data/init.sql
 * Add test data to this file in table RealTypes
 *
 * FLOAT, DOUBLE
 *
 * FLOAT => float or double or long double
 * DOUBLE => double or long double

 * Should be an error trying to put it in:
 *
 * Integer Types
 */


// Bit Values can only be read into "unsigned integer types"
TEST(TableRealTest, ReadFLOAT2float)        {typeGoodTest<float>        (11, "SELECT R2 FROM RealTypes where Id=1");}
TEST(TableRealTest, ReadFLOAT2double)       {typeGoodTest<double>       (11, "SELECT R2 FROM RealTypes where Id=1");}
TEST(TableRealTest, ReadFLOAT2longdouble)   {typeGoodTest<long double>  (11, "SELECT R2 FROM RealTypes where Id=1");}
TEST(TableRealTest, ReadDOUBLE2float)       {typeBadTest<float,         std::logic_error>("SELECT R1 FROM RealTypes where Id=1");}
TEST(TableRealTest, ReadDOUBLE2double)      {typeGoodTest<double>       (10, "SELECT R1 FROM RealTypes where Id=1");}
TEST(TableRealTest, ReadDOUBLE2longdouble)  {typeGoodTest<long double>  (10, "SELECT R1 FROM RealTypes where Id=1");}
TEST(TableRealTest, ReadREAL2float)         {typeBadTest<float,          std::logic_error>("SELECT R3 FROM RealTypes where Id=1");}
TEST(TableRealTest, ReadREAL2double)        {typeGoodTest<double>       (14, "SELECT R3 FROM RealTypes where Id=1");}
TEST(TableRealTest, ReadREAL2doubledouble)  {typeGoodTest<long double>  (14, "SELECT R3 FROM RealTypes where Id=1");}

TEST(TableRealTest, ReadDecimal2float)         {typeGoodTest<float>        (12, "SELECT D1 FROM RealTypes where Id=1");}
TEST(TableRealTest, ReadDecimal2double)        {typeGoodTest<double>       (12, "SELECT D1 FROM RealTypes where Id=1");}
TEST(TableRealTest, ReadDecimal2doubledouble)  {typeGoodTest<long double>  (12, "SELECT D1 FROM RealTypes where Id=1");}
TEST(TableRealTest, ReadNumeric2float)         {typeGoodTest<float>        (13, "SELECT D2 FROM RealTypes where Id=1");}
TEST(TableRealTest, ReadNumeric2double)        {typeGoodTest<double>       (13, "SELECT D2 FROM RealTypes where Id=1");}
TEST(TableRealTest, ReadNumeric2doubledouble)  {typeGoodTest<long double>  (13, "SELECT D2 FROM RealTypes where Id=1");}


TEST(TableRealTest, ReadDOUBLE2ULL)         {typeBadTest<unsigned long long,std::logic_error>("SELECT R1 FROM RealTypes where Id=1");}
TEST(TableRealTest, ReadDOUBLE2UL)          {typeBadTest<unsigned long, std::logic_error>("SELECT R1 FROM RealTypes where Id=1");}
TEST(TableRealTest, ReadDOUBLE2UI)          {typeBadTest<unsigned int,  std::logic_error>("SELECT R1 FROM RealTypes where Id=1");}
TEST(TableRealTest, ReadDOUBLE2US)          {typeBadTest<unsigned short,std::logic_error>("SELECT R1 FROM RealTypes where Id=1");}
TEST(TableRealTest, ReadDOUBLE2UC)          {typeBadTest<unsigned char, std::logic_error>("SELECT R1 FROM RealTypes where Id=1");}

TEST(TableRealTest, ReadDOUBLESULL)         {typeBadTest<signed long long,std::logic_error>("SELECT R1 FROM RealTypes where Id=1");}
TEST(TableRealTest, ReadDOUBLESUL)          {typeBadTest<signed long,   std::logic_error>("SELECT R1 FROM RealTypes where Id=1");}
TEST(TableRealTest, ReadDOUBLESUI)          {typeBadTest<signed int,    std::logic_error>("SELECT R1 FROM RealTypes where Id=1");}
TEST(TableRealTest, ReadDOUBLESUS)          {typeBadTest<signed short,  std::logic_error>("SELECT R1 FROM RealTypes where Id=1");}
TEST(TableRealTest, ReadDOUBLESUC)          {typeBadTest<signed char,   std::logic_error>("SELECT R1 FROM RealTypes where Id=1");}

TEST(TableRealTest, ReadDOUBLESC)           {typeBadTest<char,          std::logic_error>("SELECT R1 FROM RealTypes where Id=1");}


