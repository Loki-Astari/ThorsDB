
#include "PrepareStatement.h"
#include "Connection.h"
#include "ConectReader.h"
#include "RequPackage.h"
#include "RespPackageEOF.h"
#include "RespPackageOK.h"
#include "RespPackageResultSet.h"
#include "RespPackageColumnDefinition.h"
#include "ThorMySQL.h"
#include <cassert>


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
        virtual  std::ostream& print(std::ostream& s)   const override
        {
            return s << "RequPackagePrepare: statement: " << statement << "\n";
        }
        virtual  void build(ConectWriter& writer)       const override
        {
            writer.writeFixedLengthInteger<1>(0x16);
            writer.writeVariableLengthString(statement);
        }
};

class RequPackagePrepareClose: public RequPackage
{
    int statementID;
    public:
        RequPackagePrepareClose(int statementID)
            : RequPackage("RequPackagePrepareClose")
            , statementID(statementID)
        {}
        virtual  std::ostream& print(std::ostream& s)   const
        {
            return s << "RequPackagePrepareClose: statementID: " << statementID << "\n";
        }
        virtual  void build(ConectWriter& writer)       const
        {
            writer.writeFixedLengthInteger<1>(0x19);
            writer.writeFixedLengthInteger<4>(statementID);
        }
};

class RequPackagePrepareExecute: public RequPackage
{
    int statementID;
    public:
        RequPackagePrepareExecute(int statementID)
            : RequPackage("RequPackagePrepareExecute")
            , statementID(statementID)
        {}
        virtual  std::ostream& print(std::ostream& s)   const override
        {
            return s << "RequPackagePrepareExecute: statementID: " << statementID << "\n";
        }
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

class RequPackagePrepareReset: public RequPackage
{
    int statementID;
    public:
        RequPackagePrepareReset(int statementID)
            : RequPackage("RequPackagePrepareReset")
            , statementID(statementID)
        {}
        virtual  std::ostream& print(std::ostream& s)   const
        {
            return s << "RequPackagePrepareReset: statementID: " << statementID << "\n";
        }
        virtual  void build(ConectWriter& writer)       const
        {
            writer.writeFixedLengthInteger<1>(0x1A);
            writer.writeFixedLengthInteger<4>(statementID);
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

                reader.recvMessage<RespPackageEOF>();
            }
            if (numColumns > 0) {
                reader.reset();
                for(int loop = 0;loop < numColumns; ++loop) {
                    columnInfo.emplace_back(reader);
                    reader.reset();
                }
                reader.recvMessage<RespPackageEOF>();
            }
        }

        virtual std::ostream& print(std::ostream& s)    const override
        {
            s << "RespPackagePrepare: statementID" << statementID << " numColumns: " << numColumns << " numParams: " << numParams << " warningCount: " << warningCount << "\n";
            s << "\tparamInfo:\n";
            for(auto const& loop: paramInfo) {
                s << "\t\t" << loop;
            }
            s << "\tcolumnInfo:\n";
            for(auto const& loop: columnInfo) {
                s << "\t\t" << loop;
            }
            return s;
        }
        int     getStatementID()                        const           {return statementID;}
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
            columnCount = firstByte;
            reader.reset();
            for(int loop = 0;loop < columnCount; ++loop) {
                columnInfo.push_back(RespPackageColumnDefinition(reader));
                reader.reset();
            }
            auto mark = reader.recvMessage<RespPackageEOF>(0xFE);
            hasRows = !(mark->getStatusFlag() & SERVER_STATUS_CURSOR_EXISTS);

            // Stream now contains the data followed by an EOF token
        }
        virtual  std::ostream& print(std::ostream& s)   const override
        {
            return s << "RespPackagePrepareExecute: columnCount: " << columnCount << " hasRows: " << hasRows << "\n";
        }

        int  getColumnCount() const {return columnCount;}
        bool hasDataRows()    const {return hasRows;}
        std::vector<RespPackageColumnDefinition> const&  getColumns() const {return columnInfo;}
};

        }
    }
}

using namespace ThorsAnvil::MySQL;

PrepareStatement::PrepareStatement(Connection& connectn, std::string const& statement)
    : Statement(statement)
    , connection(connectn)
{
    prepareResp = connection.sendMessage<Detail::RespPackagePrepare>(
                                    Detail::RequPackagePrepare(statement),
                                    Connection::Reset,
                                    0x00,
                                    [](int firstByte, ConectReader& reader){
                                        return new Detail::RespPackagePrepare(firstByte, reader);
                                    }
                              );
    statementID = prepareResp->getStatementID();
}

PrepareStatement::~PrepareStatement()
{
    connection.sendMessage(Detail::RequPackagePrepareClose(statementID), Connection::Reset);
}

void PrepareStatement::doExecute()
{
    prepareExec = connection.sendMessage<Detail::RespPackagePrepareExecute>(
                                    Detail::RequPackagePrepareExecute(statementID),
                                    Connection::Reset,
                                    -1, // Does not matter what the first byte is 
                                    [this](int firstByte, ConectReader& reader){
                                        return new Detail::RespPackagePrepareExecute(firstByte, reader, *(this->prepareResp));
                                    }
                              );
}

bool PrepareStatement::more()
{
    connection.packageReader.reset();
    if (!prepareExec->hasDataRows()) {
        return false;
    }
    nextLine = connection.recvMessage<Detail::RespPackageResultSet>(
                                    0x00,
                                    [this](int firstByte, ConectReader& reader){
                                        return new Detail::RespPackageResultSet(firstByte, reader, this->prepareExec->getColumns());
                                    });
    if (nextLine.get() == nullptr) {
        connection.sendMessage<Detail::RespPackageOK>(
                                    Detail::RequPackagePrepareReset(statementID),
                                    Connection::Reset,
                                    -1 // Only looking for the OK message. Anything else will throw an exception.
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

