
#include <gtest/gtest.h>
#include "test/TableTest.h"
#include "MySQLConfig.h"
#include "ThorMySQL.h"
#include "ConectReader.h"
#include "RespPackage.h"
#include "RespPackageEOF.h"
#include "RespPackageHandShake.h"
#include "test/MockStream.h"

using ThorsAnvil::DB::SQL::Options;
using ThorsAnvil::DB::MySQL::ConectReader;
using ThorsAnvil::DB::MySQL::RespPackage;
using ThorsAnvil::DB::MySQL::RespPackageEOF;
using ThorsAnvil::DB::MySQL::RespPackageHandShake;

TEST(RespPackageTest, DowncastUniquePtrOK)
{
    // The following copied from RespPackageEOFTest.cpp
    char                buffer[] = "\x01\x00\x03\x00";
    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41, 0);

    RespPackageEOF*      eof    = new RespPackageEOF(0xFE, reader);
    // End of copied section.

    std::unique_ptr<RespPackage>        pack(eof);
    auto test = [&pack]()
    {
        std::unique_ptr<RespPackageEOF>     eof = ThorsAnvil::DB::MySQL::downcastUniquePtr<RespPackageEOF>(std::move(pack));
    };
    EXPECT_NO_THROW(
        test()
    );
}
TEST(RespPackageTest, DowncastUniquePtrFAIL)
{
    // The following copied from RespPackageEOFTest.cpp
    char                buffer[] = "\x01\x00\x03\x00";
    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41, 0);

    RespPackageEOF*      eof    = new RespPackageEOF(0xFE, reader);
    // End of copied section.

    std::unique_ptr<RespPackage>        pack(eof);
    auto test = [&pack]()
    {
        std::unique_ptr<RespPackageHandShake>     resp = ThorsAnvil::DB::MySQL::downcastUniquePtr<RespPackageHandShake>(std::move(pack));
    };
    EXPECT_THROW(
        test(),
        std::domain_error
    );
}


