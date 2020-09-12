#ifndef THORSANVIL_DB_MONGO_OP_MSG_HANDSHAKE_H
#define THORSANVIL_DB_MONGO_OP_MSG_HANDSHAKE_H

// https://github.com/mongodb/specifications/blob/master/source/mongodb-handshake/handshake.rst

#include "Op_Query.h"
#include "ThorSerialize/Traits.h"
#include <string>
#include <sys/utsname.h>

namespace ThorsAnvil::DB::Mongo
{

class Driver
{
    std::string     name;
    std::string     version;
    friend ThorsAnvil::Serialize::Traits<Driver>;
    public:
        Driver();
};
class OS
{
    std::string     type;
    std::string     name;           // Optional
    std::string     architecture;   // Optional
    std::string     version;        // Optional
    friend ThorsAnvil::Serialize::Traits<OS>;
    public:
        OS();
};
class Application
{
    std::string     name;
    friend ThorsAnvil::Serialize::Traits<Application>;
    public:
        Application(std::string const& application);
};
class Client
{
    Application     application;
    Driver          driver;
    OS              os;
    std::string     platform;       // Optional
    friend ThorsAnvil::Serialize::Traits<Client>;
    public:
        Client(std::string const& application);
};

class HandShake
{
    friend ThorsAnvil::Serialize::Traits<HandShake>;
    bool            isMaster;
    std::string     saslSupportedMechs;
    std::string     hostInfo;
    Client          client;
    public:
        HandShake(std::string const& application);
};

class Op_QueryHandShake: public Op_Query<HandShake>
{
    public:
        template<typename... Args>
        Op_QueryHandShake(std::string const& applicationName)
            : Op_Query("admin.$cmd", applicationName)
        {}
        friend std::ostream& operator<<(std::ostream& stream, Op_QueryHandShake& data) {return data.print(stream);}
        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<Op_QueryHandShake> const& data);
};

}
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Driver,         name, version);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::OS,             type, name, architecture, version);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Application,    name);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Client,         application, driver, os, platform);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::HandShake,      isMaster, saslSupportedMechs, hostInfo, client);

#endif
