#ifndef THORS_ANVIL_MYSQL_CONNECTION_H
#define THORS_ANVIL_MYSQL_CONNECTION_H

#include "ConectReader.h"
#include <string>
#include <memory>

namespace ThorsAnvil
{
    namespace MySQL
    {

using ThorsAnvil::SQL::Options;
class RespPackage;
class RequPackage;
class ConectWriter;
class Authentication;
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

        void initFromHandshake(unsigned long capabilities, unsigned long charset);
        void establishSSLConnection();

        // Main Interface
        std::unique_ptr<RespPackage> sendMessageGetResponse(RequPackage const& request, bool startConv = true, ConectReader::OKMap const&    actions     = {});
        void                         sendMessage(RequPackage const& request, bool startConv = true);
        std::unique_ptr<RespPackage> recvMessage(ConectReader::OKMap const& actions = {});

        // If things go wrong just drop the current package.
        void                         removeCurrentPackage();
    protected:
        void conectToServer(std::string const& username,
                            std::string const& password,
                            std::string const& database,
                            Options const& options
                           );
};

    }
}

#endif
