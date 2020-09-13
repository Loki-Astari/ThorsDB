#ifndef THORSANVIL_DB_MONGO_OP_MSG_HANDSHAKE_H
#define THORSANVIL_DB_MONGO_OP_MSG_HANDSHAKE_H

// https://github.com/mongodb/specifications/blob/master/source/mongodb-handshake/handshake.rst

#include "Op_Query.h"
#include "Op_Reply.h"
#include "Op_Msg.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/MongoUtility.h"
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
    friend ThorsAnvil::Serialize::Traits<HandShake>;
    std::int32_t    isMaster;
    std::string     saslSupportedMechs;
    std::string     hostInfo;
    Client          client;
    public:
        HandShake(std::string const& application);  // Normal
        HandShake(std::string const& application, std::string const& dname, std::string const& dversion)
            : isMaster(true)
            , saslSupportedMechs("thor.loki")
            , hostInfo("BatCave.local:27017")
            , client(application, dname, dversion)
        {}
};

using ObjectID      = ThorsAnvil::Serialize::MongoUtility::ObjectID;
using UTCDateTime   = ThorsAnvil::Serialize::MongoUtility::UTCDateTime;

struct Version
{
    ObjectID            processId;
    std::int64_t        counter;
};

struct HandShakeReplyDoc
{
    double          ok;
    // If ok is zero
    int             code;
    std::string     errmsg;
    std::string     codeName;
    // If ok is 00 00 00 00 00 00 f0 3f
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
    std::vector<char>   data;

    // DataInterface needed by BinarySerializer (see below)
        std::size_t getSize() const             {return data.size();}
        void        resize(std::size_t size)    {data.resize(size);}
        char const* getBuffer() const           {return &data[0];}
        char*       getBuffer()                 {return &data[0];}
};
using BinarySerializer = ThorsAnvil::Serialize::MongoUtility::BinarySerializer<ThorsAnvil::DB::Mongo::Binary, '\x00'>;

struct AuthInit
{
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

    // Interface needed by Op_Msg
    //std::size_t getSize()   const {return ThorsAnvil::Serialize::bsonGetPrintSize(*this);}
};

struct AuthContinue
{
    AuthContinue(std::int32_t convId, std::string&& payload, std::string&& db)
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

struct AuthInitReply
{
    AuthInitReply()
        : ok(1.0)
        , payload(0)
    {}
    // When there is an error message
    // The next four fields are used.
    // ok is set to zero
    double              ok;
    std::int32_t        code;
    std::string         errmsg;
    std::string         codeName;
    // When there is a good message the following are set
    std::int32_t        conversationId;
    bool                done;
    Binary              payload;
};
struct AuthContinueReply
{
    AuthContinueReply()
        : ok(1.0)
    {}
    // When there is an error message
    // The next four fields are used.
    // ok is set to zero
    double              ok;
    std::int32_t        code;
    std::string         errmsg;
    std::string         codeName;
    // When there is a good message the following are set
};

class Op_QueryHandShake: public Op_Query<HandShake>
{
    public:
        Op_QueryHandShake(HandShake& handShake)
            : Op_Query("admin.$cmd", handShake)
        {}
        friend std::ostream& operator<<(std::ostream& stream, Op_QueryHandShake& data) {return data.print(stream);}
        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<Op_QueryHandShake> const& data);
};

using Op_ReplyHandShake     = Op_Reply<HandShakeReplyDoc>;
class Op_MsgAuthInit: public Op_Msg<Kind0<AuthInit>>
{
    public:
        Op_MsgAuthInit(std::string&& db, std::string&& mechanism, std::string&& payload)
            : Op_Msg<Kind0<AuthInit>>{AuthInit{std::move(db), std::move(mechanism), std::move(payload)}}
        {}
        friend std::ostream& operator<<(std::ostream& stream, Op_MsgAuthInit& data) {return data.print(stream);}
        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<Op_MsgAuthInit> const& data);
};
class Op_MsgAuthContinue: public Op_Msg<Kind0<AuthContinue>>
{
    public:
        Op_MsgAuthContinue(std::int32_t convId, std::string&& payload, std::string&& db)
            : Op_Msg<Kind0<AuthContinue>>(AuthContinue{convId, std::move(payload), std::move(db)})
        {}
        friend std::ostream& operator<<(std::ostream& stream, Op_MsgAuthContinue& data) {return data.print(stream);}
        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<Op_MsgAuthContinue> const& data);
};
class Op_MsgAuthInitReply: public Op_Msg<Kind0<AuthInitReply>>
{
    public:
        Op_MsgAuthInitReply()
        {}
        friend std::ostream& operator<<(std::ostream& stream, Op_MsgAuthInitReply& data) {return data.print(stream);}
        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<Op_MsgAuthInitReply> const& data);
};
class Op_MsgAuthContinueReply: public Op_Msg<Kind0<AuthContinueReply>>
{
    public:
        Op_MsgAuthContinueReply()
        {}
        friend std::ostream& operator<<(std::ostream& stream, Op_MsgAuthContinueReply& data) {return data.print(stream);}
        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<Op_MsgAuthContinueReply> const& data);
};

}


ThorsAnvil_MakeTraitCustomSerialize(ThorsAnvil::DB::Mongo::Binary, ThorsAnvil::DB::Mongo::BinarySerializer);


ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Driver,             name, version);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::OS,                 type, name, architecture, version);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Application,        name);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Client,             application, driver, os);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::HandShake,          isMaster, saslSupportedMechs, hostInfo, client);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Version,            processId, counter);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::HandShakeReplyDoc,  ok, code, errmsg, codeName, topologyVersion, localTime, maxBsonObjectSize, maxMessageSizeBytes, maxWriteBatchSize, logicalSessionTimeoutMinutes, connectionId, minWireVersion, maxWireVersion, ismaster, readOnly, saslSupportedMechs);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::AuthInit,           saslStart, mechanism, payload, $db);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::AuthInitReply,      ok, code, errmsg, codeName, conversationId, done, payload);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::AuthContinue,       saslContinue, conversationId, $db, payload);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::AuthContinueReply,  ok, code, errmsg, codeName);

#endif
