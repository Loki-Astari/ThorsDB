
#include <gtest/gtest.h>
#include "test/TableTest.h"
#include "MySQLConfig.h"
#include "TypeReadWrite.h"
#include "ConectWriter.h"
#include "test/MockStream.h"

TEST(CornerCaseTest, UnknownMySQLType)
{
    EXPECT_EQ("Unknown???", ThorsAnvil::MySQL::mapMySQLTypeToString(0xF0));
}

class BadCorner {};
TEST(CornerCaseTest, WriteUnknownTYpe)
{
    using ThorsAnvil::MySQL::ConectWriter;

    auto test = []()
    {
        char            buffer[0];
        unsigned char   output[100];
        MockStream      stream(buffer, sizeof(buffer), output);
        ConectWriter    writer(stream);

        BadCorner       corner;
        ThorsAnvil::MySQL::writeParameterValue<BadCorner>(writer, corner);
    };
    EXPECT_THROW(
        test(),
        std::logic_error
    );
}


