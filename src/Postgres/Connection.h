#ifndef THORS_ANVIL_DB_POSTGRESS_CONNECTION_H
#define THORS_ANVIL_DB_POSTGRESS_CONNECTION_H

#include "ThorsDB/SQLUtil.h"
#include <string>
#include <memory>

namespace ThorsAnvil::DB::Postgres
{

using ThorsAnvil::DB::SQL::Options;
class ConectReader;
class ConectWriter;
class Message;

class Connection
{
    public:
        ConectReader&   packageReader;
        ConectWriter&   packageWriter;

        Connection(std::string const& username,
                   std::string const& password,
                   std::string const& database,
                   Options const& options,
                   ConectReader& packageReader,
                   ConectWriter& packageWriter);
        virtual ~Connection();

    private:
        void startupMessage(std::string const& username, std::string const& database, std::string const& password, Options const& options);
        void sendMessage(std::unique_ptr<Message> message);
        std::unique_ptr<Message> getMessage();
        std::unique_ptr<Message> getAuthenticationMessage();
};

}

//#ifndef COVERAGE_Postgres
//#include "Connection.tpp"
//#endif

#endif
