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
class ConectWriter;

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


        // Main Interface
        template<typename Requ>
        std::unique_ptr<RespPackage> sendMessageGetResponse(Requ const& request, ConectReader::OKMap const&    actions     = {});

        // Some Requests are multi-part this allows them to send a bunch of small request objects.
        // and retrieve the corresponding reply objects.
        template<typename Requ>
        void                         sendMessage(Requ const& request);
        std::unique_ptr<RespPackage> recvMessage(ConectReader::OKMap const& actions);

        // If things go wrong just drop the current package.
        void                         removeCurrentPackage();
    private:
        template<typename Resp, typename Requ>
        std::unique_ptr<Resp> sendHandshakeMessage(Requ const& hs, ConectReader::OKMap const& actions);
    protected:
        void conectToServer(std::string const& username,
                            std::string const& password,
                            std::string const& database,
                            Options const& options
                           );
};

    }
}

#ifndef COVERAGE_MySQL
#include "Connection.tpp"
#endif

#endif
