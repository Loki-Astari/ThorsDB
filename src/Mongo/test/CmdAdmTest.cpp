
#include "MongoConfig.h"
#include <gtest/gtest.h>
#include "UnitTestWithConnection.h"
#include "ThorsSocket/SocketStream.h"
#include "ThorsSocket/Socket.h"
#include "CmdDB_GetLastError.h"
#include "CmdAdm_ListDataBases.h"
#include "CmdAdm_Compact.h"
#include "CmdAdm_Create.h"
#include "CmdAdm_Drop.h"


using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

class CmdAdmTest: public UnitTestWithConnection
{
    public:
        CmdAdmTest()
        {
            setCollectionToBaseLine(getConnection());
        };
};


TEST_F(CmdAdmTest, ListDataBasesB)
{
    MongoConnection&  connection    = CmdAdmTest::getConnection();

    // Send Command to prove authentication worked and we have an open and working connection:
    connection << CmdAdm_ListDataBases{};

    CmdAdm_ListDataBasesReply   listOfDataBases;
    connection >> listOfDataBases;

    if (listOfDataBases)
    {

        std::cerr << "ListDB TS:   " << listOfDataBases.reply.totalSize << "\n";
        std::cerr << "ListDB DB: [";
        for (auto const& db: listOfDataBases.reply.databases)
        {
            std::cerr << "{Name: " << db.name << ", Size: " << db.sizeOnDisk << ", Empty: " << db.empty << "}";
        }
        std::cerr << "]\n";
        EXPECT_LE(2, listOfDataBases.reply.databases.size());
    }
    else
    {
        std::cerr << "Failure retrieving DB List: " << listOfDataBases.getHRErrorMessage() << "\n";
        EXPECT_TRUE(false);
    }
}

TEST_F(CmdAdmTest, Compact)
{
    MongoConnection&  connection    = CmdAdmTest::getConnection();
    connection << CmdAdm_Compact{"ConnectionTest"s};

    CmdAdm_CompactReply reply;
    connection >> reply;

    EXPECT_EQ(1, reply.reply.ok);
/*
    connection << send_CmdDB_GetLastError("ConnectionTest");
    CmdDB_GetLastErrorReply     lastError;
    connection >> lastError;
    std::cout << lastError.reply.to_String();
*/
}
TEST_F(CmdAdmTest, Create)
{
    MongoConnection&  connection    = CmdAdmTest::getConnection();
    connection << CmdAdm_Create{"ConnectionCreateCollection"s};
    CmdAdm_CreateReply  reply;
    connection >> reply;

    std::cerr << "Checking\n";
    EXPECT_EQ(1, reply.reply.ok);
    std::cerr << "Test\n";
    if (reply.reply.ok != 1)
    {
        std::cerr << "Output\n";
        std::cerr << reply.reply.getHRErrorMessage() << "\n";
        std::cerr << "Output DONE\n";
    }

    connection << CmdAdm_ListDataBases{};
    CmdAdm_ListDataBasesReply   listOfDataBases;
    connection >> listOfDataBases;
    std::cerr << listOfDataBases.reply.databases.size() << "\n";
    bool found = false;
    for(auto const& db: listOfDataBases.reply.databases)
    {
        std::cerr << "Name: " << db.name << "\n";
        if (db.name == "ConnectionCreateCollection") {
            found = true;
            break;
        }
    }
    //EXPECT_TRUE(found);

    connection << CmdAdm_Drop("ConnectionCreateCollection"s);
    CmdAdm_DropReply    dropReply;
    connection >> dropReply;

    EXPECT_TRUE(dropReply.reply.ok);
    if (dropReply.reply.ok != 1)
    {
        std::cerr << dropReply.reply.getHRErrorMessage() << "\n";
    }
}
