#ifndef THORSANVIL_DB_MONGO_OP_MSG_TPP
#define THORSANVIL_DB_MONGO_OP_MSG_TPP

#include "ThorSerialize/Traits.h"
#include "ThorSerialize/BsonThor.h"
#include "ThorSerialize/JsonThor.h"

namespace ThorsAnvil::DB::Mongo
{

// ---- Kind0

template<typename Data>
inline Kind0<Data>::Kind0(Data& data)
    : data(data)
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
inline Op_Msg<Kind...>::Op_Msg(Kind&&... kind)
    : header(OpCode::OP_MSG)
    , flagBits(OP_MsgFlag::empty)
    , sections(std::move(kind)...)
    , checksum(0)
{}

template<typename... Kind>
inline Op_Msg<Kind...>::Op_Msg(OP_MsgFlag flag, Kind&&... kind)
    : header(OpCode::OP_MSG)
    , flagBits(flag)
    , sections(std::move(kind)...)
    , checksum(0)
{}

template<typename... Kind>
inline std::ostream& Op_Msg<Kind...>::print(std::ostream& stream)
{
    bool showCheckSum = (flagBits & OP_MsgFlag::checksumPresent) != OP_MsgFlag::empty;

    std::size_t sectionSize = 0;
    std::apply([&sectionSize](auto const& section){sectionSize += section.getSize();}, sections);

    std::size_t dataSize    = sizeof(flagBits) + sectionSize + (showCheckSum ? sizeof(checksum) : 0);
    header.prepareToSend(dataSize);

    stream << header
           << make_LE(flagBits);

    // Stream the sections;
    std::apply([&stream](auto& section){stream << section;}, sections);

    // Output the checksum only if we said it would be there.
    if (showCheckSum)
    {
        stream << make_LE(checksum);
    }
    return stream;
}

template<typename... Kind>
inline std::ostream& Op_Msg<Kind...>::printHR(std::ostream& stream)
{
    bool showCheckSum = (flagBits & OP_MsgFlag::checksumPresent) != OP_MsgFlag::empty;

    std::size_t sectionSize = 0;
    std::apply([&sectionSize](auto const& section){sectionSize += section.getSize();}, sections);

    std::size_t dataSize    = sizeof(flagBits) + sectionSize + (showCheckSum ? sizeof(checksum) : 0);
    header.prepareToSend(dataSize);

    stream << make_hr(header)
           << "flagBits:    " << flagBits << "\n";

    // Stream the sections;
    std::apply([&stream](auto& section){stream << make_hr(section);}, sections);

    // Output the checksum only if we said it would be there.
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
    stream >> header >> make_LE(flagBits);
    std::apply([&stream](auto& section){stream >> section;}, sections);

    bool expectCheckSum = (flagBits & OP_MsgFlag::checksumPresent) != OP_MsgFlag::empty;
    if (expectCheckSum)
    {
        stream >> checksum;
    }
    return stream;
}

}

#endif