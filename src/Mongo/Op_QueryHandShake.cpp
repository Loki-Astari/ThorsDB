#include "Op_QueryHandShake.h"


using namespace ThorsAnvil::DB::Mongo;

Driver::Driver()
    : name("ThorsAnvil::Mongo::Driver")
    , version("v1.0")
{}

OS::OS()
{
    using UtsName = struct utsname;
    UtsName  buffer;
    int result = uname(&buffer);
    if (result != 0)
    {
        type = "unknown";
        return;
    }
    type         = buffer.sysname;
#ifdef   __APPLE__
    FILE* fp = popen("sw_vers -productName", "r");
    if (fp == nullptr)
    {
        throw std::runtime_error("Bad");
    }
    char pBuffer[100];
    while (fgets(pBuffer, sizeof(pBuffer), fp) != NULL) {
        name += pBuffer;
    }
    if (name[name.size() -1] == '\n')
    {
        name.resize(name.size() -1);
    }
#endif
    architecture = buffer.machine;
    version      = buffer.release;
}

Client::Client(std::string const& application)
    : application(application)
    , platform("ThorDB-Build")
{}

HandShake::HandShake(std::string const& application)
    : isMaster(true)
    , saslSupportedMechs("thor.loki")
    , hostInfo("BatCave.local:27017")
    , client(application)
{}


