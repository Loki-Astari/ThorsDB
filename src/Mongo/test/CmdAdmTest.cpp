
#include "MongoConfig.h"
#include <gtest/gtest.h>
#include "UnitTestWithConnection.h"
#include "ThorsSocket/SocketStream.h"
#include "ThorsSocket/Socket.h"
#include "CmdAdm_ListDataBases.h"


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


TEST(CmdAdmTest, ListDB)
{
    MongoConnection  connection(THOR_TESTING_MONGO_HOST, 27017, THOR_TESTING_MONGO_USER, THOR_TESTING_MONGO_PASS, THOR_TESTING_MONGO_DB, {});

    // Send Command to prove authentication worked and we have an open and working connection:
    connection << CmdAdm_ListDataBases{};

    CmdAdm_ListDataBasesReply   listOfDatabases;
    connection >> listOfDatabases;

    if (listOfDatabases)
    {

        std::cerr << "ListDB TS:   " << listOfDatabases.reply.totalSize << "\n";
        std::cerr << "ListDB DB: [";
        for (auto const& db: listOfDatabases.reply.databases)
        {
            std::cerr << "{Name: " << db.name << ", Size: " << db.sizeOnDisk << ", Empty: " << db.empty << "}";
        }
        std::cerr << "]\n";
    }
    else
    {
        std::cerr << "Failure retrieving DB List: " << listOfDatabases.getHRErrorMessage() << "\n";
    }
}

