#include "HandShake.h"
#include "ThorsLogging/ThorsLogging.h"

#include <stdio.h>

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
#if defined(__APPLE__)
    FILE* fp = popen("sw_vers -productName", "r");
#elif defined(__linux__)
    FILE* fp = popen("lsb_release -a", "r");
#endif
    if (fp == nullptr)
    {
        ThorsLogAndThrowFatal("ThorsAnvil::DB::Mongo::OS", "OS", "Can not get OS Information");
    }
    char pBuffer[100];
    while (fgets(pBuffer, sizeof(pBuffer), fp) != NULL)
    {
#if defined(__APPLE__)
        name += pBuffer;
#elif defined(__linux__)
        char pValue[100];
        if (sscanf(pBuffer, " Distributor ID: %s", pValue) == 1)
        {
            name = pValue;
        }
#endif
    }
    if (name[name.size() -1] == '\n')
    {
        name.resize(name.size() -1);
    }

    architecture = buffer.machine;
    version      = buffer.release;
}

Application::Application(std::string const& application)
    : name(application)
{}

Client::Client(std::string const& application)
    : application(application)
    // , platform("ThorDB-Build")
{}

HandShake::HandShake(std::string const& application)
    : isMaster(true)
    , saslSupportedMechs("thor.loki")
    , hostInfo("BatCave.local:27017")
    , client(application)
{}
