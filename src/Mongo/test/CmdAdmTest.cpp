
#include "MongoConfig.h"
#include <gtest/gtest.h>
#include "UnitTestWithConnection.h"
#include "ThorsSocket/SocketStream.h"
#include "ThorsSocket/Socket.h"
#include "CmdDB_GetLastError.h"
#include "CmdAdm_ListDataBases.h"
#include "CmdAdm_Compact.h"


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
    }
    else
    {
        std::cerr << "Failure retrieving DB List: " << listOfDataBases.getHRErrorMessage() << "\n";
    }
}

TEST_F(CmdAdmTest, Compact)
{
    MongoConnection&  connection    = CmdAdmTest::getConnection();
    connection << CmdAdm_Compact{"ConnectionTest"s};

    connection << send_CmdDB_GetLastError("ConnectionTest");
    CmdDB_GetLastErrorReply     lastError;
    connection >> lastError;
    std::cout << lastError.reply.to_String();

}
