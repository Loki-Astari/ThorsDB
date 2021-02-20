#ifndef THORSANVIL_DB_MONGO_OP_MSG_TPP
#define THORSANVIL_DB_MONGO_OP_MSG_TPP

#ifndef THORSANVIL_DB_MONGO_OP_MSG_H
#error  "This should only be included from Op_Msg.h"
#endif

#include "ThorSerialize/BsonThor.h"
#include "ThorSerialize/JsonThor.h"

namespace ThorsAnvil::DB::Mongo
{

// ---- Kind0

template<typename Data>
template<typename... Args>
inline Kind0<Data>::Kind0(Args&&... args)
    : data{std::forward<Args>(args)...}
{}

template<typename Data>
inline std::size_t Kind0<Data>::getSize() const
{
    return 1 + ThorsAnvil::Serialize::bsonGetPrintSize(data);
}

template<typename Data>
inline std::ostream& Kind0<Data>::print(std::ostream& stream) const
{
    stream.write("\x00", 1);
    stream << ThorsAnvil::Serialize::bsonExporter(data);
    return stream;
}

template<typename Data>
std::istream& Kind0<Data>::parse(std::istream& stream)
{
    char val;
    stream.read(&val, 1);
    // assert val == 0
    stream >> ThorsAnvil::Serialize::bsonImporter(data);
    return stream;
}

template<typename Data>
inline std::ostream& Kind0<Data>::printHR(std::ostream& stream) const
{
    return stream << "Kind: 0\n"
                  << ThorsAnvil::Serialize::jsonExporter(data) << "\n";
}

// ---- Kind1

// ---- Op_Msg

template<typename... Kind>
template<typename... Args>
inline Op_Msg<Kind...>::Op_Msg(Args&&... args)
    : Op_Msg(OP_MsgFlag::empty, std::forward<Args>(args)...)
{}

template<typename... Kind>
template<typename... Args>
inline Op_Msg<Kind...>::Op_Msg(OP_MsgFlag flags, Args&&... args)
    : header(OpCode::OP_MSG)
    , flags(flags)
    , sections(std::forward<Args>(args)...)
    , checksum(0)
{
    header.prepareToSend(getSize());
}

template<typename... Kind>
std::size_t Op_Msg<Kind...>::getSize() const
{
    std::size_t sectionSize = 0;
    std::apply([&sectionSize](auto const& section){sectionSize += section.getSize();}, sections);

    bool        showCheckSum = (flags & OP_MsgFlag::checksumPresent) != OP_MsgFlag::empty;
    std::size_t dataSize     = sizeof(flags) + sectionSize + (showCheckSum ? sizeof(checksum) : 0);
    return dataSize;
}

template<typename... Kind>
template<typename... Args>
inline Op_MsgReply<Kind...>::Op_MsgReply(Args&&... args)
    : Op_Msg<Kind...>({}, std::forward<Args>(args)...)
{}

template<typename... Kind>
inline std::ostream& Op_Msg<Kind...>::print(std::ostream& stream) const
{
    stream << header
           << make_LE(flags);

    // Stream the sections;
    std::apply([&stream](auto& section){stream << section;}, sections);

    // Output the checksum only if we said it would be there.
    bool showCheckSum = (flags & OP_MsgFlag::checksumPresent) != OP_MsgFlag::empty;
    if (showCheckSum)
    {
        stream << make_LE(checksum);
    }
    return stream;
}

template<typename... Kind>
inline std::ostream& Op_Msg<Kind...>::printHR(std::ostream& stream) const
{
    stream << make_hr(header)
           << "flagBits:    " << flags << "\n";

    // Stream the sections;
    std::apply([&stream](auto& section){stream << make_hr(section);}, sections);

    // Output the checksum only if we said it would be there.
    bool showCheckSum = (flags & OP_MsgFlag::checksumPresent) != OP_MsgFlag::empty;
    if (showCheckSum)
    {
        stream << "Checksum: " << checksum << "\n";
    }
    else
    {
        stream << "Checksum: Not calculated\n";
    }
    return stream;
}

template<typename... Kind>
std::istream& Op_Msg<Kind...>::parse(std::istream& stream)
{
    stream >> header >> make_LE(flags);
    std::apply([&stream](auto& section){stream >> section;}, sections);

    bool expectCheckSum = (flags & OP_MsgFlag::checksumPresent) != OP_MsgFlag::empty;
    if (expectCheckSum)
    {
        stream >> make_LE(checksum);
    }
    return stream;
}

}

#endif
