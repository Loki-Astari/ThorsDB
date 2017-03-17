
#include "PrepareStatement.h"
#include "PackageStream.h"
#include "ConectReader.h"
#include "ThorSQL/Connection.h"
#include "ThorSQL/Statement.h"
#include "MockStream.h"
#include <algorithm>
#include <gtest/gtest.h>
#include "MySQLConfig.h"

#include "RequPackagePrepare.h"
#include "RequPackagePrepareClose.h"
#include "RequPackagePrepareExecute.h"
#include "RequPackagePrepareReset.h"
#include "RespPackagePrepare.h"
#include "RespPackagePrepareExecute.h"

namespace ThorsAnvil
{
    namespace MySQL
    {
extern void testPrintRequPackagePrepare(std::ostream& str);
extern void testPrintRequPackagePrepareClose(std::ostream& str);
extern void testPrintRequPackagePrepareExecute(std::ostream& str);
extern void testPrintRequPackagePrepareReset(std::ostream& str);
extern void testPrintRespPackagePrepare(std::ostream& str, int firstByte, ConectReader& reader);
extern void testPrintRespPackagePrepareExecute(std::ostream& str, int firstBytePrep, int firstByteExec, ConectReader& reader);
    }
}


// Printing The Helper packages.
TEST(PrepareStatementPrinterTest, RequPackagePreparePrint)
{
    std::stringstream       output;
    ThorsAnvil::MySQL::testPrintRequPackagePrepare(output);
    ASSERT_NE(std::string::npos, output.str().find("RequPackagePrepare:"));

}
TEST(PrepareStatementPrinterTest, RequPackagePrepareClosePrint)
{
    std::stringstream       output;
    ThorsAnvil::MySQL::testPrintRequPackagePrepareClose(output);
    ASSERT_NE(std::string::npos, output.str().find("RequPackagePrepareClose:"));
}
TEST(PrepareStatementPrinterTest, RequPackagePrepareExecutePrint)
{
    std::stringstream       output;
    ThorsAnvil::MySQL::testPrintRequPackagePrepareExecute(output);
    ASSERT_NE(std::string::npos, output.str().find("RequPackagePrepareExecute:"));
}
TEST(PrepareStatementPrinterTest, RequPackagePrepareResetPrint)
{
    std::stringstream       output;
    ThorsAnvil::MySQL::testPrintRequPackagePrepareReset(output);
    ASSERT_NE(std::string::npos, output.str().find("RequPackagePrepareReset:"));
}
TEST(PrepareStatementPrinterTest, RespPackagePreparePrint)
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
    ThorsAnvil::MySQL::testPrintRespPackagePrepare(output, 0x00, reader);
    ASSERT_NE(std::string::npos, output.str().find("RespPackagePrepare:"));
    ASSERT_EQ(0xFF, reader.fixedLengthInteger<1>());
}

TEST(PrepareStatementPrinterTest, RespPackagePrepareExecutePrint)
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
    ThorsAnvil::MySQL::testPrintRespPackagePrepareExecute(output, 0x00, 0x02, reader);
    ASSERT_NE(std::string::npos, output.str().find("RespPackagePrepareExecute:"));
    ASSERT_EQ(0xFF, reader.fixedLengthInteger<1>());
}

