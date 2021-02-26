#ifndef THORSANVIL_DB_MONGO_CMD_ADM_AUTH_H
#define THORSANVIL_DB_MONGO_CMD_ADM_AUTH_H

// https://github.com/mongodb/specifications/blob/master/source/mongodb-handshake/handshake.rst

#include "CmdAdm_Query.h"
#include "CmdAdm_Reply.h"
#include "Op_Msg.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/MongoUtility.h"
#include "ThorSerialize/JsonPrinter.h"
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
        Driver();   // Normal
        Driver(std::string const& name, std::string const& version) // Testing
            : name(name)
            , version(version)
        {}
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
    // std::string     platform;       // Optional
    friend ThorsAnvil::Serialize::Traits<Client>;
    public:
        Client(std::string const& application); // Normal
        Client(std::string const& application, std::string const& dname, std::string const& dversion)
            : application(application)
            , driver(dname, dversion)
        {}
};

class HandShake
{
    using Compression = std::vector<std::string>;

    friend ThorsAnvil::Serialize::Traits<HandShake>;
    std::int32_t    isMaster;
    std::string     saslSupportedMechs;
    std::string     hostInfo;
    Client          client;
    Compression     compression;
    public:
        HandShake(std::string const& application, std::string const& comp = "");  // Normal
        HandShake(std::string const& application, std::string const& dname, std::string const& dversion, std::string const& comp = "");
};

using ObjectID      = ThorsAnvil::Serialize::MongoUtility::ObjectID;
using UTCDateTime   = ThorsAnvil::Serialize::MongoUtility::UTCDateTime;

struct Version
{
    ObjectID            processId;
    std::int64_t        counter;
};

struct HandShakeReply: public CmdReplyBase
{
    using UserData = void;

    Version         topologyVersion;
    UTCDateTime     localTime;
    std::int32_t    maxBsonObjectSize;
    std::int32_t    maxMessageSizeBytes;
    std::int32_t    maxWriteBatchSize;
    std::int32_t    logicalSessionTimeoutMinutes;
    std::int32_t    connectionId;
    std::int32_t    minWireVersion;
    std::int32_t    maxWireVersion;
    bool            ismaster;
    bool            readOnly;
    std::vector<std::string>    saslSupportedMechs;
    std::vector<std::string>    compression;
};

struct Binary
{
    Binary(int type, std::string const& init)
        : type(type)
        , data(std::begin(init), std::end(init))
    {}
    Binary(int type)
        : type(type)
    {}
    int                 type;
    std::string         data;

    std::size_t getSize() const             {return data.size();}
    void        resize(std::size_t size)    {data.resize(size);}
    char const* getBuffer() const           {return &data[0];}
    char*       getBuffer()                 {return &data[0];}
};

class BinarySerializer: public ThorsAnvil::Serialize::MongoUtility::BinarySerializer<Binary, '\x00'>
{
    public:
        virtual void writeJson(Serialize::JsonPrinter& printer, Binary const& object) const override
        {
            printer.addValue(object.data);
        }
};

struct AuthInit
{
    AuthInit(std::string const& db, std::string const& mechanism, std::string&& payload)
        : saslStart(1)
        , mechanism(mechanism)
        , $db(db)
        , payload(0, std::move(payload))
    {}
    AuthInit(std::string const& db, std::string&& mechanism, std::string&& payload)
        : saslStart(1)
        , mechanism(std::move(mechanism))
        , $db(db)
        , payload(0, std::move(payload))
    {}
    AuthInit(std::string&& db, std::string const& mechanism, std::string&& payload)
        : saslStart(1)
        , mechanism(mechanism)
        , $db(std::move(db))
        , payload(0, std::move(payload))
    {}
    AuthInit(std::string&& db, std::string&& mechanism, std::string&& payload)
        : saslStart(1)
        , mechanism(std::move(mechanism))
        , $db(std::move(db))
        , payload(0, std::move(payload))
    {}
    std::int32_t        saslStart;
    std::string         mechanism;
    std::string         $db;
    Binary              payload;
};

struct AuthCont
{
    AuthCont(std::int32_t convId, std::string const& db, std::string&& payload)
        : saslContinue(1)
        , conversationId(convId)
        , $db(db)
        , payload(0, std::move(payload))
    {}
    AuthCont(std::int32_t convId, std::string&& db, std::string&& payload)
        : saslContinue(1)
        , conversationId(convId)
        , $db(std::move(db))
        , payload(0, std::move(payload))
    {}
    std::int32_t        saslContinue;
    std::int32_t        conversationId;
    std::string         $db;
    Binary              payload;
};

struct AuthReply: public CmdReplyBase
{
    AuthReply()
        : payload(0)
    {}
    std::int32_t        conversationId;
    bool                done;
    Binary              payload;
};

using CmdAdm_HandShake          = CmdAdm_Query<HandShake>;
using CmdAdm_HandShakeReply     = CmdAdm_Reply<HandShakeReply>;
using Op_MsgAuthInit            = Op_Msg<AuthInit>;
using Op_MsgAuthCont            = Op_Msg<AuthCont>;
using Op_MsgAuthReply           = Op_Msg<AuthReply>;

}

ThorsAnvil_MakeTraitCustomSerialize(ThorsAnvil::DB::Mongo::Binary, ThorsAnvil::DB::Mongo::BinarySerializer);

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Driver,             name, version);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::OS,                 type, name, architecture, version);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Application,        name);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Client,             application, driver, os);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::HandShake,          isMaster, saslSupportedMechs, hostInfo, client, compression);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Version,            processId, counter);
ThorsAnvil_ExpandTrait(ThorsAnvil::DB::Mongo::CmdReplyBase,
                     ThorsAnvil::DB::Mongo::HandShakeReply,     topologyVersion, localTime, maxBsonObjectSize, maxMessageSizeBytes, maxWriteBatchSize, logicalSessionTimeoutMinutes, connectionId, minWireVersion, maxWireVersion, ismaster, readOnly, saslSupportedMechs, compression);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::AuthInit,           saslStart, mechanism, payload, $db);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::AuthCont,           saslContinue, payload, conversationId, $db);
ThorsAnvil_ExpandTrait(ThorsAnvil::DB::Mongo::CmdReplyBase,
                     ThorsAnvil::DB::Mongo::AuthReply,          conversationId, done, payload);

#endif
