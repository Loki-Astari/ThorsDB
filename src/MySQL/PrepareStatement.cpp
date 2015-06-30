
#include "PrepareStatement.h"
#include "Connection.h"
#include "ConectReader.h"
#include "RequPackage.h"
#include "RespPackage.h"
#include "RespPackageEOF.h"
#include "RespPackageOK.h"
#include "RespPackageResultSet.h"
#include "RespPackageColumnDefinition.h"
#include "ThorMySQL.h"
#include <stdexcept>
#include <assert.h>
#include <iostream>


namespace ThorsAnvil
{
    namespace MySQL
    {
        namespace Detail
        {


class RequPackagePrepare: public RequPackage
{
    std::string const& statement;
    public:
        RequPackagePrepare(std::string const& statement)
            : RequPackage("RequPackagePrepare")
            , statement(statement)
        {}
        virtual  std::ostream& print(std::ostream& s)   const override {return s;}
        virtual  void build(ConectWriter& writer)       const override
        {
            writer.writeFixedLengthInteger<1>(0x16);
            writer.writeVariableLengthString(statement);
        }
};

class RespPackagePrepare: public RespPackage
{
    int     statementID;
    int     numColumns;
    int     numParams;
    int     warningCount;
    std::vector<RespPackageColumnDefinition>   paramInfo;
    std::vector<RespPackageColumnDefinition>   columnInfo;
    public:
        RespPackagePrepare(int firstByte, ConectReader& reader)
            : RespPackage(reader)
        {
            assert(firstByte == 0x00);

            // Not We have already read 1 byte (status OK)
            statementID     = reader.fixedLengthInteger<4>();
            numColumns      = reader.fixedLengthInteger<2>();
            numParams       = reader.fixedLengthInteger<2>();
                              reader.fixedLengthInteger<1>(); // reserved
            warningCount    = reader.fixedLengthInteger<2>();

            if (numParams > 0) {
                reader.reset();
                for(int loop = 0;loop < numParams; ++loop) {
                    paramInfo.emplace_back(reader);
                    reader.reset();
                }

                reader.recvMessage<RespPackageEOF>(0xFE, [](int firstByte, ConectReader& reader){return new Detail::RespPackageEOF(firstByte, reader);});
            }
            if (numColumns > 0) {
                reader.reset();
                for(int loop = 0;loop < numColumns; ++loop) {
                    columnInfo.emplace_back(reader);
                    reader.reset();
                }
                reader.recvMessage<RespPackageEOF>(0xFE, [](int firstByte, ConectReader& reader){return new Detail::RespPackageEOF(firstByte, reader);});
            }
        }

        virtual std::ostream& print(std::ostream& s)    const override  {return s;}
        int     getStatementID()                        const           {return statementID;}
};

        }
    }
}

using namespace ThorsAnvil::MySQL;

PrepareStatement::PrepareStatement(Connection& connectn, std::string const& statement)
    : Statement(statement)
    , connection(connectn)
{
    using Detail::RespPackagePrepare;
    using Detail::RequPackagePrepare;
    prepareResp = connection.sendMessage<RespPackagePrepare>(
                                    RequPackagePrepare(statement),
                                    Connection::Reset,
                                    0x00,
                                    [](int firstByte, ConectReader& reader){return new Detail::RespPackagePrepare(firstByte, reader);}
                              );
    statementID = prepareResp->getStatementID();
}

namespace ThorsAnvil
{
    namespace MySQL
    {
        namespace Detail
        {
class RequPackagePrepareClose: public RequPackage
{
    int statementID;
    public:
        RequPackagePrepareClose(int statementID)
            : RequPackage("")
            , statementID(statementID)
        {}
        virtual  std::ostream& print(std::ostream& s)   const {return s;}
        virtual  void build(ConectWriter& writer)       const
        {
            writer.writeFixedLengthInteger<1>(0x19);
            writer.writeFixedLengthInteger<4>(statementID);
        }
};
        }
    }
}

PrepareStatement::~PrepareStatement()
{
    std::cerr << "~PrepareStatement()\n";
    connection.sendMessage(Detail::RequPackagePrepareClose(statementID), Connection::Reset);
    std::cerr << "~PrepareStatement() DONE\n";
}

namespace ThorsAnvil
{
    namespace MySQL
    {
        namespace Detail
        {
class RequPackagePrepareExecute: public RequPackage
{
    int statementID;
    public:
        RequPackagePrepareExecute(int statementID)
            : RequPackage("")
            , statementID(statementID)
        {}
        virtual  std::ostream& print(std::ostream& s)   const override {return s;}
        virtual  void build(ConectWriter& writer)       const override
        {
            writer.writeFixedLengthInteger<1>(0x17);
            writer.writeFixedLengthInteger<4>(statementID);
            /*
             *  0x00    CURSOR_TYPE_NO_CURSOR
             *  0x01    CURSOR_TYPE_READ_ONLY
             *  0x02    CURSOR_TYPE_FOR_UPDATE
             *  0x04    CURSOR_TYPE_SCROLLABLE
             */
            writer.writeFixedLengthInteger<1>(0x00);
            writer.writeFixedLengthInteger<4>(1);
        }
};

class RespPackagePrepareExecute: public RespPackage
{
    int  columnCount;
    bool hasRows;
    std::vector<RespPackageColumnDefinition>   columnInfo;
    public:
        RespPackagePrepareExecute(int firstByte, ConectReader& reader, RespPackagePrepare& /*prepareResp*/)
            : RespPackage(reader)
        {
            std::cerr << "RespPackagePrepareExecute\n";
            columnCount = firstByte;
            reader.reset();
            for(int loop = 0;loop < columnCount; ++loop) {
                std::cerr << "    Reading Col Definition\n";
                columnInfo.push_back(RespPackageColumnDefinition(reader));
                reader.reset();
            }
            std::cerr << "Done\n";
            auto mark = reader.recvMessage<RespPackageEOF>(0xFE, [](int firstByte, ConectReader& reader){return new Detail::RespPackageEOF(firstByte, reader);});
            std::cerr << "MArk Flags: " << std::hex << mark->getStatusFlag() << "\n";
            std::cerr << "MArk Flags: " << std::hex << (mark->getStatusFlag() & SERVER_STATUS_CURSOR_EXISTS) << "\n";
            std::cerr << "Read EOF\n";

            hasRows = !(mark->getStatusFlag() & SERVER_STATUS_CURSOR_EXISTS);

            // Stream now contains the data followed by an EOF token
            // reader.recvMessage<RespPackageEOF>(0xFE, [](int firstByte, ConectReader& reader){return new Detail::RespPackageEOF(firstByte, reader);});
        }
        virtual  std::ostream& print(std::ostream& s)   const override {return s;}

        int  getColumnCount() const {return columnCount;}
        bool hasDataRows()    const {return hasRows;}
        std::vector<RespPackageColumnDefinition> const&  getColumns() const {return columnInfo;}
};
        }
    }
}

void PrepareStatement::doExecute()
{
    std::cerr << "doExecute\n";
    prepareExec = connection.sendMessage<Detail::RespPackagePrepareExecute>(
                                    Detail::RequPackagePrepareExecute(statementID),
                                    Connection::Reset,
                                    -1, // Does not matter what the first byte is 
                                    [this](int firstByte, ConectReader& reader){return new Detail::RespPackagePrepareExecute(firstByte, reader, *(this->prepareResp));}
                              );
    std::cerr << "doExecute Done\n";
}

namespace ThorsAnvil
{
    namespace MySQL
    {
        namespace Detail
        {
class RequPackagePrepareReset: public RequPackage
{
    int statementID;
    public:
        RequPackagePrepareReset(int statementID)
            : RequPackage("")
            , statementID(statementID)
        {}
        virtual  std::ostream& print(std::ostream& s)   const {return s;}
        virtual  void build(ConectWriter& writer)       const
        {
            writer.writeFixedLengthInteger<1>(0x1A);
            writer.writeFixedLengthInteger<4>(statementID);
        }
};
        }
    }
}

bool PrepareStatement::more()
{
    connection.packageReader.reset();
    if (!prepareExec->hasDataRows()) {
        std::cout << "No Rows Returned\n";
        return false;
    }
    std::cout << "More\n";
    nextLine = connection.recvMessage<Detail::RespPackageResultSet>(0x00, [this](int firstByte, ConectReader& reader){return new Detail::RespPackageResultSet(firstByte, reader, this->prepareExec->getColumns());});
    std::cout << "   Line: " << nextLine.get() << "\n";
    if (nextLine.get() == nullptr) {
        connection.sendMessage<Detail::RespPackageOK>(
                                    Detail::RequPackagePrepareReset(statementID),
                                    Connection::Reset,
                                    -1,
                                    [this](int, ConectReader&){return nullptr;} // Expecting an OK message.
                                );
    }
    return nextLine.get() != nullptr;
}

#ifdef COVERAGE_MySQL
/*
 * This code is only compiled into the unit tests for code coverage purposes
 * It is not part of the live code.
 */
#include "Connection.tpp"
#include "ConectReader.tpp"

template std::unique_ptr<Detail::RespPackagePrepare> Connection::sendMessage<Detail::RespPackagePrepare, Detail::RequPackagePrepare>(Detail::RequPackagePrepare const&, Connection::PacketContinuation, int, std::function<RespPackage*(int, ConectReader&)>);
template std::unique_ptr<Detail::RespPackagePrepare> ConectReader::recvMessage<Detail::RespPackagePrepare>(int, std::function<RespPackage*(int, ConectReader&)>);
template std::unique_ptr<Detail::RespPackagePrepareExecute> ConectReader::recvMessage<Detail::RespPackagePrepareExecute>(int, std::function<RespPackage*(int, ConectReader&)>);


#endif

