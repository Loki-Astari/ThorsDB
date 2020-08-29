#include "Connection.h"
#include "RequPackage.h"
#include "RequPackagePrepareReset.h"
#include "RequPackagePrepareExecute.h"
#include "RequPackagePrepareClose.h"
#include "RequPackagePrepare.h"
#include "RespPackageOK.h"
#include "RespPackagePrepare.h"
#include "RespPackagePrepareExecute.h"
#include "PrepareStatement.h"
#include "ThorsIOUtil/Utility.h"

using namespace ThorsAnvil::DB::MySQL;
using ThorsAnvil::Utility::buildErrorMessage;
using ThorsAnvil::Utility::buildBugReport;

PrepareStatement::ValidatorStream::ValidatorStream(std::vector<RespPackageColumnDefinition> const& colu)
    : Common::StreamSimple(-1)
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
                // fallthrough
            case MYSQL_TYPE_LONG:
            case MYSQL_TYPE_INT24:
            case MYSQL_TYPE_FLOAT:
                validateInfo.append(2, '\x0'); // 2 and fall => 4 total
                // fallthrough
            case MYSQL_TYPE_SHORT:
            case MYSQL_TYPE_YEAR:
                validateInfo.append(1, '\x0'); // 1 and fall => 2 total
                // fallthrough
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
                        buildBugReport("ThrosAnvil::MySQL::PrepareStatement::ValidatorStream", "ValidatorStream",
                                  "Unknown Type returned by server. Type: ", std::hex, col.type, " " , mapMySQLTypeToString(col.type)
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
        throw SQL::Lib::ValidationTmpError("Too many parameters in callback function.");
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

PrepareStatement::PrepareStatement(Connection& connectn, std::string const& statement)
    : Statement(statement)
    , connection(connectn)
    , prepareResp(downcastUniquePtr<RespPackagePrepare>(
                                connection.sendMessageGetResponse(
                                    RequPackagePrepare(statement),
                                    true,
                                    {{0x00, [](int firstByte, ConectReader& reader)
                                            {return new RespPackagePrepare(firstByte, reader);}
                                     }
                                    }
                                )
                 ))
    , statementID(prepareResp->getStatementID())
    , validatorStream(prepareResp->getColumns())
    , validatorReader(validatorStream)
    , nextLine(new RespPackageResultSet(0x00, validatorReader, prepareResp->getColumns()))
    , bindBuffer(prepareResp->getParams())
{}

PrepareStatement::~PrepareStatement()
{
    connection.sendMessage(RequPackagePrepareClose(statementID));
}

void PrepareStatement::doExecute()
{
    modificationOK.reset();
    prepareExec.reset();

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
                buildErrorMessage("ThrosAnvil::MySQL::PrepareStatement", "doExecute", errorMessage));
    }

    std::unique_ptr<RespPackage> tmp = connection.sendMessageGetResponse(
                                    RequPackagePrepareExecute(statementID, bindBuffer),
                                    true,
                                    {{-1, // Does not matter what the first byte is
                                        [this](int firstByte, ConectReader& reader)
                                        {
                                            return new RespPackagePrepareExecute(
                                                                    firstByte,
                                                                    reader,
                                                                    *(this->prepareResp)
                                                                );
                                        }
                                     }
                                    }
                              );
    if (tmp->isOK())
    {
        modificationOK = downcastUniquePtr<RespPackageOK>(std::move(tmp));
    }
    else
    {
        prepareExec = downcastUniquePtr<RespPackagePrepareExecute>(std::move(tmp));
    }
}

bool PrepareStatement::more()
{
    connection.packageReader.reset();
    if (!prepareExec->hasDataRows())
    {
        return false;
    }
    std::unique_ptr<RespPackage> message
                        = connection.recvMessage(
                                    {{0x00,
                                        [this](int firstByte, ConectReader& reader)
                                        {
                                            return new RespPackageResultSet(
                                                                    firstByte,
                                                                    reader,
                                                                    this->prepareExec->getColumns()
                                                                );
                                        }
                                     }
                                    });
    nextLine = downcastUniquePtr<RespPackageResultSet>(std::move(message));
    bool moreResult = nextLine.get() != nullptr;
    if (!moreResult)
    {
        std::unique_ptr<RespPackage> resp = connection.sendMessageGetResponse(RequPackagePrepareReset(statementID));

        // Need to make sure we got an RespPackageOK
        // Otherwise there is a real problem.
        downcastUniquePtr<RespPackageOK>(std::move(resp));

        validatorStream.reset();
        bindBuffer.reset();
        nextLine.reset(new RespPackageResultSet(0x00, validatorReader, this->prepareResp->getColumns()));
    }
    return moreResult;
}

bool PrepareStatement::isSelect() const
{
    return prepareResp->isSelect();
}

long PrepareStatement::rowsAffected() const
{
    return modificationOK->getAffectedRows();
}

long PrepareStatement::lastInsertID() const
{
    return modificationOK->getLastInsertID();
}

void PrepareStatement::abort()
{
    connection.removeCurrentPackage();
    while (more())
    {
        connection.removeCurrentPackage();
    }
}
