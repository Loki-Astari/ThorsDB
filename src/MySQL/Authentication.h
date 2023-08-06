#ifndef THORSANVIL_DB_MYSQL_AUTHENTICATION_H
#define THORSANVIL_DB_MYSQL_AUTHENTICATION_H

#include "ConectReader.h"
#include "RespPackage.h"
#include "ThorsDB/SQLUtil.h"
#include <string>
#include <utility>

namespace ThorsAnvil::DB::MySQL
{

using ThorsAnvil::DB::Access::Options;

class Connection;
class RespPackage;
class RespPackageAuthMoreData;

class Authetication
{
    protected:
        Connection&                 connection;
        Options const&              options;

    public:
        Authetication(Connection& connection, Options const& options);
        virtual ~Authetication();

        virtual std::string getAuthenticationString(std::string const& username,
                                                    std::string const& password,
                                                    std::string const& database,
                                                    std::string const& pluginData) = 0;

        virtual std::unique_ptr<RespPackage> customAuthenticate(std::unique_ptr<RespPackageAuthMoreData> /*msg*/,
                                                                std::string const& /*username*/,
                                                                std::string const& /*password*/,
                                                                std::string const& /*database*/,
                                                                std::string const& /*pluginData*/) {return nullptr;}
        virtual std::string getPluginName() const = 0;
};

std::unique_ptr<Authetication> getAuthenticatonMethod(Connection& connection, std::string const& authPluginName, Options const& options = Options());

}

#endif
