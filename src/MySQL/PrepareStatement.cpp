#include "PrepareStatement.h"
#include "TypeReadWrite.h"
#include "Connection.h"
#include "ConectReader.h"
#include "RequPackage.h"
#include "RespPackageEOF.h"
#include "RespPackageOK.h"
#include "RespPackageResultSet.h"
#include "RespPackageColumnDefinition.h"
#include "ThorMySQL.h"
#include <cassert>
#include <sstream>


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
            : RequPackage("RequPackagePrepare", "Prepare")
            , statement(statement)
        {}
        virtual  std::ostream& print(std::ostream& s)   const override
        {
            return s << "RequPackagePrepare: statement: " << statement << "\n";
        }
        virtual  void build(ConectWriter& writer)       const override
        {
            // https://dev.mysql.com/doc/internals/en/com-stmt-prepare.html#com-stmt-prepare
            writer.writeFixedLengthInteger<1>(COM_STMT_PREPARE);
            writer.writeVariableLengthString(statement);
        }
};
void testPrintRequPackagePrepare(std::ostream& str)
{
    str << RequPackagePrepare("Test");
}

class RequPackagePrepareClose: public RequPackage
{
    int statementID;
    public:
        RequPackagePrepareClose(int statementID)
            : RequPackage("RequPackagePrepareClose", "Prepare-Close")
            , statementID(statementID)
        {}
        virtual  std::ostream& print(std::ostream& s)   const
        {
            return s << "RequPackagePrepareClose: statementID: " << statementID << "\n";
        }
        virtual  void build(ConectWriter& writer)       const
        {
            // https://dev.mysql.com/doc/internals/en/com-stmt-close.html#com-stmt-close
            writer.writeFixedLengthInteger<1>(COM_STMT_CLOSE);
            writer.writeFixedLengthInteger<4>(statementID);
        }
};
void testPrintRequPackagePrepareClose(std::ostream& str)
{
    str << RequPackagePrepareClose(1);
}

class RequPackagePrepareExecute: public RequPackage
{
    int                 statementID;
    BindBuffer const&   bindBuffer;
    public:
        RequPackagePrepareExecute(int statementID, BindBuffer const& bindBuffer)
            : RequPackage("RequPackagePrepareExecute", "Prepare-Execute")
            , statementID(statementID)
            , bindBuffer(bindBuffer)
        {}
        virtual  std::ostream& print(std::ostream& s)   const override
        {
            return s << "RequPackagePrepareExecute: statementID: " << statementID << "\n";
        }
        virtual  void build(ConectWriter& writer)       const override
        {
            // https://dev.mysql.com/doc/internals/en/com-stmt-execute.html#com-stmt-execute
            writer.writeFixedLengthInteger<1>(COM_STMT_EXECUTE);
            writer.writeFixedLengthInteger<4>(statementID);
            /*
             *  0x00    CURSOR_TYPE_NO_CURSOR
             *  0x01    CURSOR_TYPE_READ_ONLY
             *  0x02    CURSOR_TYPE_FOR_UPDATE
             *  0x04    CURSOR_TYPE_SCROLLABLE
             */
            writer.writeFixedLengthInteger<1>(0x00);
            writer.writeFixedLengthInteger<4>(1);
            bindBuffer.bindToMySQL(writer);
        }
};
void testPrintRequPackagePrepareExecute(std::ostream& str)
{
    std::vector<Detail::RespPackageColumnDefinition>    cols;
    BindBuffer  bindBuffer(cols);
    str << RequPackagePrepareExecute(1, bindBuffer);
}

class RequPackagePrepareReset: public RequPackage
{
    int statementID;
    public:
        RequPackagePrepareReset(int statementID)
            : RequPackage("RequPackagePrepareReset", "Prepare-Reset")
            , statementID(statementID)
        {}
        virtual  std::ostream& print(std::ostream& s)   const
        {
            return s << "RequPackagePrepareReset: statementID: " << statementID << "\n";
        }
        virtual  void build(ConectWriter& writer)       const
        {
            // https://dev.mysql.com/doc/internals/en/com-stmt-reset.html#com-stmt-reset
            writer.writeFixedLengthInteger<1>(COM_STMT_RESET);
            writer.writeFixedLengthInteger<4>(statementID);
        }
};
void testPrintRequPackagePrepareReset(std::ostream& str)
{
    str << RequPackagePrepareReset(1);
}

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
            : RespPackage(reader, "Prepare")
        {
            // https://dev.mysql.com/doc/internals/en/com-stmt-prepare-response.html#com-stmt-prepare-response
            assert(firstByte == 0x00);

            // Not We have already read 1 byte (status OK)
            statementID     = reader.fixedLengthInteger<4>();
            numColumns      = reader.fixedLengthInteger<2>();
            numParams       = reader.fixedLengthInteger<2>();
                              reader.fixedLengthInteger<1>(); // reserved
            warningCount    = reader.fixedLengthInteger<2>();

            if (numParams > 0)
            {
                reader.reset();
                for (int loop = 0;loop < numParams; ++loop)
                {
                    paramInfo.emplace_back(reader);
                    reader.reset();
                }

                reader.recvMessageEOF();
            }
            if (numColumns > 0)
            {
                reader.reset();
                for (int loop = 0;loop < numColumns; ++loop)
                {
                    columnInfo.emplace_back(reader);
                    reader.reset();
                }
                reader.recvMessageEOF();
            }
            /* To prevent exceptions in RespPackageResultSet when two many arguments are provided
             * We are going to push an extra fake column into the columns here.
             *
             * This is compensated for in Validation Stream which will detect the error
             * and generate a more appropriate exception that will happen during validation
             * rather than at runtime.
             */
             columnInfo.push_back(RespPackageColumnDefinition::getFakeColumn(MYSQL_TYPE_TINY));
        }

        virtual std::ostream& print(std::ostream& s)    const override
        {
            s << "RespPackagePrepare: statementID" << statementID
              << " numColumns: " << numColumns
              << " numParams: " << numParams
              << " warningCount: " << warningCount << "\n";
            s << "\tparamInfo:\n";
            for (auto const& loop: paramInfo)
            {
                s << "\t\t" << loop;
            }
            s << "\tcolumnInfo:\n";
            for (auto const& loop: columnInfo)
            {
                s << "\t\t" << loop;
            }
            return s;
        }
        int     getStatementID()                        const               {return statementID;}
        std::vector<RespPackageColumnDefinition> const&  getColumns() const {return columnInfo;}
        std::vector<RespPackageColumnDefinition> const&  getParams()  const {return paramInfo;}
};
void testPrintRespPackagePrepare(std::ostream& str, int firstByte, ConectReader& reader)
{
    str << RespPackagePrepare(firstByte, reader);
}

class RespPackagePrepareExecute: public RespPackage
{
    int  columnCount;
    bool hasRows;
    std::vector<RespPackageColumnDefinition>   columnInfo;
    public:
        RespPackagePrepareExecute(int firstByte, ConectReader& reader, RespPackagePrepare& /*prepareResp*/)
            : RespPackage(reader, "Prepare-Execute")
        {
            // https://dev.mysql.com/doc/internals/en/binary-protocol-resultset.html#binary-protocol-resultset
            columnCount = firstByte;
            reader.reset();
            for (int loop = 0;loop < columnCount; ++loop)
            {
                columnInfo.push_back(RespPackageColumnDefinition(reader));
                reader.reset();
            }
            auto mark = reader.recvMessageEOF();
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
void testPrintRespPackagePrepareExecute(std::ostream& str, int firstBytePrep, int firstByteExec, ConectReader& reader)
{
    RespPackagePrepare  prep(firstBytePrep, reader);
    str << RespPackagePrepareExecute(firstByteExec, reader, prep);
}

        }


PrepareStatement::ValidatorStream::ValidatorStream(std::vector<Detail::RespPackageColumnDefinition> const& colu)
    : MySQLStream(0)
    , columns(colu)
    , position(0)
    , errorReading(false)
{
    int nullmaplength   = (columns.size() + 7 + 2) / 8;
    validateInfo.append(nullmaplength, '\0');
    for (auto const& col: columns)
    {
        switch (col.type)
        {
            case MYSQL_TYPE_VAR_STRING:
            case MYSQL_TYPE_STRING:
            case MYSQL_TYPE_VARCHAR:
            case MYSQL_TYPE_TINY_BLOB:
            case MYSQL_TYPE_MEDIUM_BLOB:
            case MYSQL_TYPE_BLOB:
            case MYSQL_TYPE_LONG_BLOB:
                validateInfo.append(1, '\x0');
                break;
            case MYSQL_TYPE_DECIMAL:
            case MYSQL_TYPE_NEWDECIMAL:
                validateInfo.append(1, '\x1');
                validateInfo.append(1, '0');   // Character zero.
                break;
            case MYSQL_TYPE_LONGLONG:
            case MYSQL_TYPE_DOUBLE:
                validateInfo.append(4, '\x0'); // 4 and fall => 8 total
            case MYSQL_TYPE_LONG:
            case MYSQL_TYPE_INT24:
            case MYSQL_TYPE_FLOAT:
                validateInfo.append(2, '\x0'); // 2 and fall => 4 total
            case MYSQL_TYPE_SHORT:
            case MYSQL_TYPE_YEAR:
                validateInfo.append(1, '\x0'); // 1 and fall => 2 total
            case MYSQL_TYPE_TINY:
                validateInfo.append(1, '\x0'); // 1          => 1 total
                break;
            case MYSQL_TYPE_DATE:
            case MYSQL_TYPE_DATETIME:
            case MYSQL_TYPE_TIMESTAMP:
                validateInfo.append(1, '\x4');
                validateInfo.append(4, '\x0');
                break;
            case MYSQL_TYPE_TIME:
                validateInfo.append(1, '\x8');
                validateInfo.append(8, '\x0');
                break;
            case MYSQL_TYPE_BIT:
                validateInfo.append(1, '\x0');  // Use a string length encoded string representation.
                break;
            case MYSQL_TYPE_NULL:
            case MYSQL_TYPE_NEWDATE:
            case MYSQL_TYPE_TIMESTAMP2:
            case MYSQL_TYPE_DATETIME2:
            case MYSQL_TYPE_TIME2:
            case MYSQL_TYPE_ENUM:
            case MYSQL_TYPE_SET:
            case MYSQL_TYPE_GEOMETRY:
            default:
                throw std::domain_error(
                        bugReport("ThrosAnvil::MySQL::PrepareStatement::ValidatorStream::ValidatorStream: ",
                                  "Unknown Type returned by server. Type: ", std::hex, col.type
                      ));

        }
    }
    // Buffer
}

void PrepareStatement::ValidatorStream::read(char* buffer, std::size_t len)
{
    // Because we have a fake column on the end.
    // Any attempt to read it should result in an error.
    // See: RespPackagePrepare::RespPackagePrepare
    if (position + len >= validateInfo.size())
    {
        errorReading    = true;
        // This causes this to unwind back to the SQL where it is caught.
        // The doExecute() will then be called where all the errors generated
        // during validation are checked and handeled in a single place.
        throw SQL::ValidationTmpError("Too many parameters in callback function.");
    }
    std::copy(&validateInfo[position], &validateInfo[position + len], buffer);
    position += len;
}

bool PrepareStatement::ValidatorStream::tooMany() const
{
    return errorReading;
}

bool PrepareStatement::ValidatorStream::tooFew() const
{
    // Compensate for fake column by adding +1 (MYSQL_TYPE_TINY)
    // See: RespPackagePrepare::RespPackagePrepare
    return (position + 1) != validateInfo.size();
}

void PrepareStatement::ValidatorStream::reset()
{
    position    = 0;
}

    }
}

using namespace ThorsAnvil::MySQL;

PrepareStatement::PrepareStatement(Connection& connectn, std::string const& statement)
    : Statement(statement)
    , connection(connectn)
    , prepareResp(downcastUniquePtr<Detail::RespPackagePrepare>(
                                connection.sendMessageGetResponse(
                                    Detail::RequPackagePrepare(statement),
                                    {{0x00, [](int firstByte, ConectReader& reader)
                                            {return new Detail::RespPackagePrepare(firstByte, reader);}
                                     }
                                    }
                                )
                 ))
    , statementID(prepareResp->getStatementID())
    , validatorStream(prepareResp->getColumns())
    , validatorReader(validatorStream)
    , nextLine(new Detail::RespPackageResultSet(0x00, validatorReader, prepareResp->getColumns()))
    , bindBuffer(prepareResp->getParams())
{}

PrepareStatement::~PrepareStatement()
{
    connection.sendMessage(Detail::RequPackagePrepareClose(statementID));
}

void PrepareStatement::doExecute()
{
    std::string errorMessage;
    if (validatorStream.tooFew())
    {
        errorMessage    += "Not all returned values are being used by the callback function.";
    }
    if (validatorStream.tooMany())
    {
        errorMessage    += "You have more parameters in your callback than are specified in the select.";
    }
    if (bindBuffer.countBoundParameters() < prepareResp->getParams().size())
    {
        errorMessage    += "Not all bind points have parameters bound.";
    }
    if (bindBuffer.countBoundParameters() > prepareResp->getParams().size())
    {
        errorMessage    += "Too many bound values. You have more values than '?'.";
    }
    if (!errorMessage.empty())
    {
        throw std::logic_error(
                errorMsg("ThrosAnvil::MySQL::PrepareStatement::doExecute: ", errorMessage));
    }

    std::unique_ptr<RespPackage> tmp = connection.sendMessageGetResponse(
                                    Detail::RequPackagePrepareExecute(statementID, bindBuffer),
                                    {{-1, // Does not matter what the first byte is
                                        [this](int firstByte, ConectReader& reader)
                                        {
                                            return new Detail::RespPackagePrepareExecute(
                                                                    firstByte,
                                                                    reader,
                                                                    *(this->prepareResp)
                                                                );
                                        }
                                     }
                                    }
                              );
    prepareExec = downcastUniquePtr<Detail::RespPackagePrepareExecute>(std::move(tmp));
}

bool PrepareStatement::more()
{
    connection.packageReader.reset();
    if (!prepareExec->hasDataRows())
    {
        return false;
    }
    nextLine = connection.recvMessage<Detail::RespPackageResultSet>(
                                    {{0x00,
                                        [this](int firstByte, ConectReader& reader)
                                        {
                                            return new Detail::RespPackageResultSet(
                                                                    firstByte,
                                                                    reader,
                                                                    this->prepareExec->getColumns()
                                                                );
                                        }
                                     }
                                    });
    bool moreResult = nextLine.get() != nullptr;
    if (!moreResult)
    {
        std::unique_ptr<RespPackage> resp = connection.sendMessageGetResponse(Detail::RequPackagePrepareReset(statementID));

        // Need to make sure we got an RespPackageOK
        // Otherwise there is a real problem.
        downcastUniquePtr<Detail::RespPackageOK>(std::move(resp));

        validatorStream.reset();
        nextLine.reset(new Detail::RespPackageResultSet(0x00, validatorReader, this->prepareResp->getColumns()));
    }
    return moreResult;
}

void PrepareStatement::abort()
{
    connection.removeCurrentPackage();
    while (more())
    {
        connection.removeCurrentPackage();
    }
}

#ifdef COVERAGE_MySQL
/*
 * This code is only compiled into the unit tests for code coverage purposes
 * It is not part of the live code.
 */
#include "Connection.tpp"
#include "ConectReader.tpp"

template
std::unique_ptr<Detail::RespPackageOK> ConectReader::recvMessage<Detail::RespPackageOK>
(std::map<int, std::function<RespPackage* (int, ConectReader&)>> const&, bool);

template
std::unique_ptr<Detail::RespPackagePrepare>
ConectReader::recvMessage<Detail::RespPackagePrepare>
(ConectReader::OKMap const&, bool);

template
std::unique_ptr<Detail::RespPackagePrepareExecute>
ConectReader::recvMessage<Detail::RespPackagePrepareExecute>
(ConectReader::OKMap const&, bool);


#endif
