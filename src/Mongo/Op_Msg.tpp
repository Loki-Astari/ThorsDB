#ifndef THORSANVIL_DB_MONGO_OP_MSG_TPP
#define THORSANVIL_DB_MONGO_OP_MSG_TPP

#include "ThorSerialize/Traits.h"
#include "ThorSerialize/BsonThor.h"
#include "ThorSerialize/JsonThor.h"

namespace ThorsAnvil::DB::Mongo
{

// ---- Kind0

template<typename Data>
template<typename... Args>
inline Kind0<Data>::Kind0(Args&&... arg)
    : data{std::forward<Args>(arg)...}
{}


template<typename Data>
inline std::size_t Kind0<Data>::getSize(std::ostream& stream) const
{
    using DataTraits = ThorsAnvil::Serialize::Traits<Data>;
    ThorsAnvil::Serialize::BsonPrinter printer(stream);

    return 1 + DataTraits::getPrintSize(printer, data, false);
}

template<typename Data>
inline std::ostream& Kind0<Data>::print(std::ostream& stream) const
{
    stream.write("\x00", 1);
    stream << ThorsAnvil::Serialize::bsonExporter(data);
    return stream;
}

template<typename Data>
inline std::ostream& Kind0<Data>::printHR(std::ostream& stream) const
{
    return stream << "Kind: 0\n"
                  << ThorsAnvil::Serialize::jsonExporter(data) << "\n";
}

// ---- Kind1

// ---- OP_Msg

template<typename... Kind>
template<typename... Args>
inline OP_Msg<Kind...>::OP_Msg(Args&&... arg)
    : header(OpCode::OP_MSG)
    , flagBits(OP_MsgFlag::empty)
    , sections(std::forward<Args>(arg)...)
    , checksum(0)
{}

template<typename... Kind>
inline std::ostream& OP_Msg<Kind...>::print(std::ostream& stream)
{
    bool showCheckSum = flagBits & OP_MsgFlag::checksumPresent;

    std::size_t sectionSize = 0;
    std::apply([&stream, &sectionSize](auto const& section){sectionSize += section.getSize(stream);}, sections);

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
inline std::ostream& OP_Msg<Kind...>::printHR(std::ostream& stream)
{
    bool showCheckSum = flagBits & OP_MsgFlag::checksumPresent;

    std::size_t sectionSize = 0;
    std::apply([&stream, &sectionSize](auto const& section){sectionSize += section.getSize(stream);}, sections);

    std::size_t dataSize    = sizeof(flagBits) + sectionSize + (showCheckSum ? sizeof(checksum) : 0);
    header.prepareToSend(dataSize);

    stream << make_hr(header)
           << "flagBits:    " << ThorsAnvil::Serialize::jsonExporter(flagBits) << "\n";

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

}

#endif
