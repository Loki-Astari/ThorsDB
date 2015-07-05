
#include "PrepareStatement.h"
#include "PackageStream.h"
#include "ConectReader.h"
#include "ThorSQL/Connection.h"
#include "ThorSQL/Statement.h"
#include "MockStream.h"
#include <algorithm>
#include <gtest/gtest.h>
#include "MySQLConfig.h"

namespace ThorsAnvil
{
    namespace MySQL
    {
        namespace Detail
        {
extern void testPrintRequPackagePrepare(std::ostream& str);
extern void testPrintRequPackagePrepareClose(std::ostream& str);
extern void testPrintRequPackagePrepareExecute(std::ostream& str);
extern void testPrintRequPackagePrepareReset(std::ostream& str);
extern void testPrintRespPackagePrepare(std::ostream& str, int firstByte, ConectReader& reader);
extern void testPrintRespPackagePrepareExecute(std::ostream& str, int firstBytePrep, int firstByteExec, ConectReader& reader);
        }
    }
}

TEST(PrepareStatementTest, Create)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT * FROM People");
}
TEST(PrepareStatementTest, Execute)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT * FROM People"); //select * from People;
    long                count = 0;
    statement.execute([&count](int id, std::string name, short age, std::string sex, double height)
                        {
                            std::cout << "Got: " << id << " : " << name << " : " << age << " : " << sex << " : " << height << "\n";
                            ++count;
                        });
    ASSERT_EQ(2, count);
}
TEST(PrepareStatementTest, ExecuteToFewArguments)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT * FROM People"); //select * from People;
    long                count = 0;
    ASSERT_THROW(
        statement.execute([&count](int id, std::string name, short age, std::string sex/*, double height*/) // Not using the height
                        {}),
        std::runtime_error);
}
TEST(PrepareStatementTest, ExecuteToManyArguments)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT * FROM People"); //select * from People;
    long                count = 0;
    ASSERT_THROW(
        statement.execute([&count](int id, std::string name, short age, std::string sex, double height, long extra) // ask for more than are available
                        {}),
        std::runtime_error);
}

// Printing The Helper packages.
TEST(PrepareStatementTest, RequPackagePreparePrint)
{
    std::stringstream       output;
    ThorsAnvil::MySQL::Detail::testPrintRequPackagePrepare(output);
    ASSERT_NE(std::string::npos, output.str().find("RequPackagePrepare:"));

}
TEST(PrepareStatementTest, RequPackagePrepareClosePrint)
{
    std::stringstream       output;
    ThorsAnvil::MySQL::Detail::testPrintRequPackagePrepareClose(output);
    ASSERT_NE(std::string::npos, output.str().find("RequPackagePrepareClose:"));
}
TEST(PrepareStatementTest, RequPackagePrepareExecutePrint)
{
    std::stringstream       output;
    ThorsAnvil::MySQL::Detail::testPrintRequPackagePrepareExecute(output);
    ASSERT_NE(std::string::npos, output.str().find("RequPackagePrepareExecute:"));
}
TEST(PrepareStatementTest, RequPackagePrepareResetPrint)
{
    std::stringstream       output;
    ThorsAnvil::MySQL::Detail::testPrintRequPackagePrepareReset(output);
    ASSERT_NE(std::string::npos, output.str().find("RequPackagePrepareReset:"));
}
TEST(PrepareStatementTest, RespPackagePreparePrint)
{
    using ThorsAnvil::MySQL::ConectReader;

    char const              data[] = {'\x00', '\x00', '\x00', '\x00',
                                      '\x02', '\x00',   // Param Count
                                      '\x02', '\x00',   // Column Count
                                      '\x00',
                                      '\x00', '\x00',
                                        // Param: (2)
                                            '\x01', 'A',    // catalog
                                            '\x01', 'B',    // schema
                                            '\x01', 'C',    // table
                                            '\x01', 'D',    // orgTable
                                            '\x01', 'E',    // name
                                            '\x01', 'F',    // origName
                                            '\x0C',         // Length (fixed at 12)
                                            '\x00', '\x00', // CharSet
                                            '\x01', '\x00', '\x00', '\x00',  // column Length
                                            '\x00',         // Type
                                            '\x00', '\x00', // Flags
                                            '\x00',         // Decimal
                                            '\x00', '\x00', // Filler
                                            '\x01', 'G',    // catalog
                                            '\x01', 'H',    // schema
                                            '\x01', 'I',    // table
                                            '\x01', 'J',    // orgTable
                                            '\x01', 'K',    // name
                                            '\x01', 'L',    // origName
                                            '\x0C',         // Length (fixed at 12)
                                            '\x00', '\x00', // CharSet
                                            '\x01', '\x00', '\x00', '\x00',  // column Length
                                            '\x00',         // Type
                                            '\x00', '\x00', // Flags
                                            '\x00',         // Decimal
                                            '\x00', '\x00', // Filler
                                            // EOF
                                            '\xFE',         // EOF Marker
                                            '\x00', '\x00', // Warning
                                            '\x00', '\x00', // Status
                                        // Columns: (2)
                                            '\x01', 'A',    // catalog
                                            '\x01', 'B',    // schema
                                            '\x01', 'C',    // table
                                            '\x01', 'D',    // orgTable
                                            '\x01', 'E',    // name
                                            '\x01', 'F',    // origName
                                            '\x0C',         // Length (fixed at 12)
                                            '\x00', '\x00', // CharSet
                                            '\x01', '\x00', '\x00', '\x00',  // column Length
                                            '\x00',         // Type
                                            '\x00', '\x00', // Flags
                                            '\x00',         // Decimal
                                            '\x00', '\x00', // Filler
                                            '\x01', 'G',    // catalog
                                            '\x01', 'H',    // schema
                                            '\x01', 'I',    // table
                                            '\x01', 'J',    // orgTable
                                            '\x01', 'K',    // name
                                            '\x01', 'L',    // origName
                                            '\x0C',         // Length (fixed at 12)
                                            '\x00', '\x00', // CharSet
                                            '\x01', '\x00', '\x00', '\x00',  // column Length
                                            '\x00',         // Type
                                            '\x00', '\x00', // Flags
                                            '\x00',         // Decimal
                                            '\x00', '\x00', // Filler
                                            // EOF
                                            '\xFE',         // EOF Marker
                                            '\x00', '\x00', // Warning
                                            '\x00', '\x00',  // Status


                                            '\xFF'          // Should Still be on stream
                                        };

    MockStream              stream(data, sizeof(data));
    ConectReader            reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41, 0);
    std::stringstream       output;
    ThorsAnvil::MySQL::Detail::testPrintRespPackagePrepare(output, 0x00, reader);
    ASSERT_NE(std::string::npos, output.str().find("RespPackagePrepare:"));
    ASSERT_EQ(0xFF, reader.fixedLengthInteger<1>());
}

TEST(PrepareStatementTest, RespPackagePrepareExecutePrint)
{
    using ThorsAnvil::MySQL::ConectReader;

    char const              data[] = {'\x00', '\x00', '\x00', '\x00',
                                      '\x02', '\x00',   // Param Count
                                      '\x02', '\x00',   // Column Count
                                      '\x00',
                                      '\x00', '\x00',
                                        // Param: (2)
                                            '\x01', 'A',    // catalog
                                            '\x01', 'B',    // schema
                                            '\x01', 'C',    // table
                                            '\x01', 'D',    // orgTable
                                            '\x01', 'E',    // name
                                            '\x01', 'F',    // origName
                                            '\x0C',         // Length (fixed at 12)
                                            '\x00', '\x00', // CharSet
                                            '\x01', '\x00', '\x00', '\x00',  // column Length
                                            '\x00',         // Type
                                            '\x00', '\x00', // Flags
                                            '\x00',         // Decimal
                                            '\x00', '\x00', // Filler
                                            '\x01', 'G',    // catalog
                                            '\x01', 'H',    // schema
                                            '\x01', 'I',    // table
                                            '\x01', 'J',    // orgTable
                                            '\x01', 'K',    // name
                                            '\x01', 'L',    // origName
                                            '\x0C',         // Length (fixed at 12)
                                            '\x00', '\x00', // CharSet
                                            '\x01', '\x00', '\x00', '\x00',  // column Length
                                            '\x00',         // Type
                                            '\x00', '\x00', // Flags
                                            '\x00',         // Decimal
                                            '\x00', '\x00', // Filler
                                            // EOF
                                            '\xFE',         // EOF Marker
                                            '\x00', '\x00', // Warning
                                            '\x00', '\x00', // Status
                                        // Columns: (2)
                                            '\x01', 'A',    // catalog
                                            '\x01', 'B',    // schema
                                            '\x01', 'C',    // table
                                            '\x01', 'D',    // orgTable
                                            '\x01', 'E',    // name
                                            '\x01', 'F',    // origName
                                            '\x0C',         // Length (fixed at 12)
                                            '\x00', '\x00', // CharSet
                                            '\x01', '\x00', '\x00', '\x00',  // column Length
                                            '\x00',         // Type
                                            '\x00', '\x00', // Flags
                                            '\x00',         // Decimal
                                            '\x00', '\x00', // Filler
                                            '\x01', 'G',    // catalog
                                            '\x01', 'H',    // schema
                                            '\x01', 'I',    // table
                                            '\x01', 'J',    // orgTable
                                            '\x01', 'K',    // name
                                            '\x01', 'L',    // origName
                                            '\x0C',         // Length (fixed at 12)
                                            '\x00', '\x00', // CharSet
                                            '\x01', '\x00', '\x00', '\x00',  // column Length
                                            '\x00',         // Type
                                            '\x00', '\x00', // Flags
                                            '\x00',         // Decimal
                                            '\x00', '\x00', // Filler
                                            // EOF
                                            '\xFE',         // EOF Marker
                                            '\x00', '\x00', // Warning
                                            '\x00', '\x00',  // Status
                                        // Because we are doing this manually this byte is not in the stream
                                        // '\x02',
                                            '\x01', 'A',    // catalog
                                            '\x01', 'B',    // schema
                                            '\x01', 'C',    // table
                                            '\x01', 'D',    // orgTable
                                            '\x01', 'E',    // name
                                            '\x01', 'F',    // origName
                                            '\x0C',         // Length (fixed at 12)
                                            '\x00', '\x00', // CharSet
                                            '\x01', '\x00', '\x00', '\x00',  // column Length
                                            '\x00',         // Type
                                            '\x00', '\x00', // Flags
                                            '\x00',         // Decimal
                                            '\x00', '\x00', // Filler
                                            '\x01', 'G',    // catalog
                                            '\x01', 'H',    // schema
                                            '\x01', 'I',    // table
                                            '\x01', 'J',    // orgTable
                                            '\x01', 'K',    // name
                                            '\x01', 'L',    // origName
                                            '\x0C',         // Length (fixed at 12)
                                            '\x00', '\x00', // CharSet
                                            '\x01', '\x00', '\x00', '\x00',  // column Length
                                            '\x00',         // Type
                                            '\x00', '\x00', // Flags
                                            '\x00',         // Decimal
                                            '\x00', '\x00', // Filler
                                            // EOF
                                            '\xFE',         // EOF Marker
                                            '\x00', '\x00', // Warning
                                            '\x00', '\x00',  // Status

                                            '\xFF'          // Should Still be on stream
                                        };

    MockStream              stream(data, sizeof(data));
    ConectReader            reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41, 0);
    std::stringstream       output;
    ThorsAnvil::MySQL::Detail::testPrintRespPackagePrepareExecute(output, 0x00, 0x02, reader);
    ASSERT_NE(std::string::npos, output.str().find("RespPackagePrepareExecute:"));
    ASSERT_EQ(0xFF, reader.fixedLengthInteger<1>());
}

