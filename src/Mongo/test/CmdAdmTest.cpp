
#include "MongoConfig.h"
#include <gtest/gtest.h>
#include "UnitTestWithConnection.h"
#include "ThorsSocket/SocketStream.h"
#include "ThorsSocket/Socket.h"
#include "CmdDB_GetLastError.h"
#include "CmdAdm_ListDataBases.h"
#include "CmdAdm_ListCollections.h"
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
}
TEST_F(CmdAdmTest, Create_List_Drop_Collection)
{
    MongoConnection&  connection    = CmdAdmTest::getConnection();
    connection << CmdAdm_Create{"ConnectionCreateCollection"s};
    CmdAdm_CreateReply  reply;
    connection >> reply;

    if (reply.reply.ok != 1)
    {
        std::cerr << reply.reply.getHRErrorMessage() << "\n";
    }
    EXPECT_EQ(1, reply.reply.ok);

    connection << CmdAdm_ListCollections{};
    CmdAdm_ListCollectionsReply   listOfCollections;
    connection >> listOfCollections;

    bool found = false;
    for(auto const& db: listOfCollections.reply.collectionInfo)
    {
        if (db.name == "ConnectionCreateCollection")
        {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found);

    connection << CmdAdm_Drop("ConnectionCreateCollection"s);
    CmdAdm_DropReply    dropReply;
    connection >> dropReply;

    if (dropReply.reply.ok != 1)
    {
        std::cerr << dropReply.reply.getHRErrorMessage() << "\n";
    }
    EXPECT_EQ(1, dropReply.reply.ok);
}
