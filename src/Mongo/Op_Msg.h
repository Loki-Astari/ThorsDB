#ifndef THORSANVIL_DB_MONGO_OP_MSG_H
#define THORSANVIL_DB_MONGO_OP_MSG_H

/*
 * $    Usage:  Op_Msg
 *
 * >        This is not designed to be used directly. See HandShake.h for example
 * >        I would expect you to define the Kind and what goes into it as part of a typedef.
 */

#include "Op.h"
#include "Util.h"
#include "View.h"
#include "Op_MsgHeader.h"
#include "ThorSerialize/Traits.h"

#include <iostream>

// Op_Msg: https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/#op-msg

namespace ThorsAnvil::DB::Mongo
{

enum class OP_MsgFlag: std::int32_t
{
    empty           = 0,
    checksumPresent = 1 << 0,               // The message ends with 4 bytes containing a CRC-32C [1] checksum. See Checksum for details.
    moreToCome      = 1 << 1,               // Another message will follow this one without further action from the receiver.
                                            // The receiver MUST NOT send another message until receiving one with moreToCome set to 0 as
                                            // sends may block, causing deadlock. Requests with the moreToCome bit set will not receive a reply.
                                            // Replies will only have this set in response to requests with the exhaustAllowed bit set.
    exhaustAllowed  = 1 << 16,              // The client is prepared for multiple replies to this request using the moreToCome bit.
                                            // The server will never produce replies with the moreToCome bit set unless the request has this bit set.
};
ThorsAnvil_MakeEnumFlag(OP_MsgFlag, empty, checksumPresent, moreToCome, exhaustAllowed);

template<typename S1>
using KindB     = std::pair<std::string, ViewType<S1>>;

template<typename Section0, typename... Section1>
class Op_Msg
{
    protected:
        // Need so we can get Op_MsgReply as the way to read responses from the server
        // and differentiate between reads and writes.
        template<typename S1>
        using Kind1     = std::pair<KindB<S1>, std::uint32_t>;
        using Sections  = std::tuple<Kind1<Section1>...>;

        Op_MsgHeader            header;             // standard message header
        OP_MsgFlag              flags;              // Flags
        // See: https://github.com/mongodb/specifications/blob/master/source/message/OP_MSG.rst
        // Here I have explicitly separated the two types of sections into
        //  * section0:     Which will be serialized as Kind0
        //  * sections:     Which will be serialized as Kind1
        //
        // Because of C++ strong typing each member of "sections" can have its own "C++" type.
        // Though each member of sections can potentially have a distinct type.
        //
        // Note 1: The section 0 object will be stored inside the message.
        // Note 2: To prevent copying sections data will not be store here.
        //         A reference to the objects will be stored in the object so the
        //         user should ensure lifetimes live beyond this object
        //
        Section0                section0;           // Data sections of the message
        Sections                sections;           // The data of the message
        mutable std::int32_t    checksum;           // checksum of message (calculated on serialization)

    public:
        Op_Msg(OP_MsgFlag flags, Section0 action, KindB<Section1>&&... views);

        Op_Msg() {}

        Section0 const& getAction()                 const           {return section0;}
        std::int32_t    getMessageLength()          const           {return header.getMessageLength();}
        void            setCompression(std::int8_t compressionType) {header.setCompression(compressionType);}

        std::ostream& print(std::ostream& stream)   const;
        std::ostream& printHR(std::ostream& stream) const;


        // Insert & FindModify
        Op_Msg& byPass();

        // Insert & Delete
        Op_Msg& unordered();

        // Insert & Delete & FindModify
        Op_Msg& setWrieConcern(int w = 1, bool j = false, std::time_t wtimeout = 0);

        // Insert & Find & FindModify & GetLastError & GetMore
        Op_Msg& setComment(std::string c);

        // Find & FindModify
        // Op_Msg& addFileds(std::initializer_list<std::string> fieldNames);
        // Op_Msg& addHint(std::string hint);

        // Find & GetMore
        // Op_Msg& setBatchSize(std::size_t val);
        // Op_Msg& setMaxTimeout(std::size_t val);

        // Find
        // Op_Msg& setSkip(std::size_t val);
        // Op_Msg& setLimit(std::size_t val);
        // Op_Msg& oneBatch(bool val = true);
        // Op_Msg& addReadConcern(ReadConcernLevel val);
        // Op_Msg& addMax(std::string field, int val);
        // Op_Msg& addMin(std::string field, int val);
        // Op_Msg& justKeys(bool val = false);
        // Op_Msg& showId(bool val = true);
        // Op_Msg& tailableCursor(bool val = true);
        // Op_Msg& tailedCursorAwait(bool val = true);
        // Op_Msg& setNoCursorTimeout(bool val = true);
        // Op_Msg& setAllowPartialResults(bool val = true);
        // Op_Msg& useDisk(bool val = true);

        // FindModify
        // Op_Msg& setNew(bool val = true);
        // Op_Msg& setUpsert(bool val = true);

        // GetLastError
        // Op_Msg& waitFoolDiskFlush(bool val = true);
        // Op_Msg& waitForReplication(std::int32_t count);
        // Op_Msg& setWaitTimeout(std::int32_t millisec);
    private:
        std::uint32_t   getSize();
};

template<typename Section0, typename... Section1>
class Op_MsgReply: public Op_Msg<Section0, Section1...>
{
    public:
        explicit operator   bool()              const;
        virtual bool        isOk()              const;
        virtual std::string getHRErrorMessage() const;

        std::istream& parse(std::istream& stream);
};

template<typename Section0, typename... Section1>
std::ostream& operator<<(std::ostream& stream, Op_Msg<Section0, Section1...> const& msg)
{
    return msg.print(stream);
}

template<typename Section0, typename... Section1>
std::istream& operator>>(std::istream& stream, Op_MsgReply<Section0, Section1...>& msg)
{
    return msg.parse(stream);
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...> send_Op_Msg(Section0&& action, KindB<Section1>&&... data)
{
    return Op_Msg<Section0, Section1...>(OP_MsgFlag::empty, std::move(action), std::forward<KindB<Section1>>(data)...);
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...> send_Op_Msg(OP_MsgFlag flags, Section0&& action, KindB<Section1>&&... data)
{
    return Op_Msg<Section0, Section1...>(flags, std::move(action), std::forward<KindB<Section1>>(data)...);
}


}

#include "Op_Msg.tpp"

#endif
