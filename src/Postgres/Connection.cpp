#include "Connection.h"
#include "ConectReader.h"
#include "ConectWriter.h"
#include "Messages.h"
#include "ThorsLogging/ThorsLogging.h"

#include <iostream>

using namespace ThorsAnvil::DB::Postgres;
using ThorsAnvil::Utility::buildErrorMessage;
using ThorsAnvil::Utility::buildBugReport;

Connection::Connection(
                    std::string const& username,
                    std::string const& password,
                    std::string const& database,
                    Options const& options,
                    ConectReader& pr,
                    ConectWriter& pw)
    : packageReader(pr)
    , packageWriter(pw)
{
    startupMessage(username, database, password, options);
}

Connection::~Connection()
{
    // Shut Down
}

void Connection::startupMessage(std::string const& username, std::string const& database, std::string const& password, Options const& options)
{
    std::unique_ptr<Message>    startupMessage = std::make_unique<StartupMessage>(username, database, options);
    while (startupMessage)
    {
        sendMessage(std::move(startupMessage));
        std::unique_ptr<Message>    response = getMessage();
        if (response->isError())
        {
            ThorsLogAndThrow("ThorsAnvil::DB::Postgres::Connection",
                             "startupMessage",
                             "Got an Error: ", *response);
        }

        std::unique_ptr<Authentication> auth(dynamic_cast<Authentication*>(response.get()));
        if (!auth)
        {
            ThorsLogAndThrowCritical("ThorsAnvil::DB::Postgres::Connection",
                                     "startupMessage",
                                     "Unexpected Response: ", *response);
        }
        response.release();
        startupMessage = auth->authenticateWithServer(username, password);
    }
}

void Connection::sendMessage(std::unique_ptr<Message> message)
{
    message->send(packageWriter);
}

std::unique_ptr<Message> Connection::getMessage()
{
    char messageTypeInfo = packageReader.getMessage();
    switch (messageTypeInfo)
    {
        case 'R':   return getAuthenticationMessage();
        case 'E':   return std::make_unique<ErrorMessage>(packageReader);
        case '1':   return std::make_unique<ParseComplete>(packageReader);
        default:
            ThorsLogAndThrowCritical("ThorsAnvil::DB::Postgres::Connection",
                                     "getMessage",
                                     "Unknown Message Type: ", messageTypeInfo);
    }
}

std::unique_ptr<Message> Connection::getAuthenticationMessage()
{
    std::size_t authMessageType = packageReader.readInt32();
    switch (authMessageType)
    {
        case 0: return std::make_unique<AuthenticationOk>(packageReader);
        case 2: return std::make_unique<AuthenticationKerberosV5>(packageReader);
        case 3: return std::make_unique<AuthenticationCleartextPassword>(packageReader);
        case 5: return std::make_unique<AuthenticationMD5Password>(packageReader);
        case 6: return std::make_unique<AuthenticationSCMCredential>(packageReader);
        case 7: return std::make_unique<AuthenticationGSS>(packageReader);
        case 8: return std::make_unique<AuthenticationGSSContinue>(packageReader);
        case 9: return std::make_unique<AuthenticationSSPI>(packageReader);
        default:
            ThorsLogAndThrowCritical("ThorsAnvil::DB::Postgres::Connection",
                                     "getAuthenticationMessage",
                                     "Invalid Authentication Reply: ", authMessageType);
    }
}
