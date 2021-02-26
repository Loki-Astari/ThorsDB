#ifndef THORSANVIL_DB_MONGO_OP_MSG_TPP
#define THORSANVIL_DB_MONGO_OP_MSG_TPP

#ifndef THORSANVIL_DB_MONGO_OP_MSG_H
#error  "This should only be included from Op_Msg.h"
#endif

#include "MongoBuffer.h"
#include "ThorsCrypto/crc.h"
#include "ThorSerialize/BsonThor.h"
#include "ThorSerialize/JsonThor.h"

namespace ThorsAnvil::DB::Mongo
{

template<typename Section0, typename... Section1>
template<typename... Views>
Op_Msg<Section0, Section1...>::Op_Msg(OP_MsgFlag flags, Section0 action, Views&&... views)
    : header(OpCode::OP_MSG)
    , flags(flags)
    , section0(std::move(action))
    , sections(std::make_pair(std::forward<Views>(views), std::uint32_t(0))...)
    , checksum(0)
{
    header.prepareToSend(getSize());
}

template<typename SectionInfo>
std::uint32_t getSizeOfKind1(std::uint32_t& sum, SectionInfo const& sectionInfo)
{
    auto const& kind1 = sectionInfo.first;

    std::uint32_t docSize = 0;
    for (auto const& doc: kind1.second)
    {
        docSize += ThorsAnvil::Serialize::bsonGetPrintSize(doc);
    }
    sectionInfo.second = (1 + sizeof(uint32_t) + (kind1.first.size() + 1) + docSize);
    sum += sectionInfo.second;
    return sum;
}

template<typename Section0, typename... Section1>
std::size_t Op_Msg<Section0, Section1...>::getSize() const
{
    std::size_t section0Size = (1 + ThorsAnvil::Serialize::bsonGetPrintSize(section0));
    std::size_t sectionsSize = 0;
    std::apply([&sectionsSize](auto const&... sectionInfor)
    {
        std::make_tuple(getSizeOfKind1(sectionsSize, sectionInfor)...);
    }, sections);

    bool        showCheckSum = (flags & OP_MsgFlag::checksumPresent) != OP_MsgFlag::empty;
    std::size_t dataSize     = sizeof(flags) + section0Size + sectionsSize + (showCheckSum ? sizeof(checksum) : 0);
    return dataSize;
}

template<typename SectionInfo>
std::uint32_t printKind1(std::ostream& stream, SectionInfo const& sectionInfo)
{
    auto const& kind1 = sectionInfo.first;

    stream << std::uint8_t(1)
           << make_LE(sectionInfo.second)
           << kind1.first << '\0';
    for (auto const& doc: kind1.second)
    {
        stream << ThorsAnvil::Serialize::bsonExporter(doc);
    }
    return 0;
}

template<typename Section0, typename... Section1>
inline std::ostream& Op_Msg<Section0, Section1...>::print(std::ostream& stream) const
{
    stream << header
           << make_LE(flags)
           << std::uint8_t(0) << ThorsAnvil::Serialize::bsonExporter(section0);

    // Stream the sections;
    std::apply([&stream](auto const&... sectionInfo)
    {
        std::make_tuple(printKind1(stream, sectionInfo)...);
    }, sections);

    // Output the checksum only if we said it would be there.
    bool showCheckSum = (flags & OP_MsgFlag::checksumPresent) != OP_MsgFlag::empty;
    if (showCheckSum)
    {
        MongoBuffer*    buffer      = dynamic_cast<MongoBuffer*>(stream.rdbuf());
        if (buffer != nullptr)
        {
            Crypto::CRC32C_Checksum     value;
            buffer->apply(value);
            checksum = value.checksum();
        }
        stream << make_LE(checksum);
    }
    return stream;
}

template<typename SectionInfo>
std::uint32_t streamKind1(std::ostream& stream, SectionInfo const& sectionInfo)
{
    auto const& kind1 = sectionInfo.first;

    stream << ThorsAnvil::Serialize::jsonExporter(kind1);
    return 0;
}

template<typename Section0, typename... Section1>
inline std::ostream& Op_Msg<Section0, Section1...>::printHR(std::ostream& stream) const
{
    stream << make_hr(header)
           << "flagBits:    " << flags << "\n"
           << "Section:     [" << ThorsAnvil::Serialize::jsonExporter(section0);

    // Stream the sections;
    std::apply([&stream](auto const&... sectionInfo)
    {
        std::make_tuple(printHRKind1(stream, sectionInfo)...);
    }, sections);

    stream << "\n]\n";

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

template<typename SectionInfo>
std::uint32_t parseKind1(std::istream& stream, SectionInfo const& sectionInfo)
{
    auto const& kind1 = sectionInfo.first;

    std::uint8_t    kindMark = -1;
    stream >> kindMark
           >> make_LE(sectionInfo.second);
    std::getline(stream, kind1.first, '\0');

    if (kindMark != 1)
    {
        // Bad Kind marker
        throw int(8);
    }

    std::uint32_t   size     = 0;
    size -= (sizeof(std::uint32_t) + kind1.first.size() + 1);;
    while (size != 0)
    {
        std::uint32_t oldSize = size;
        std::uint32_t index   = kind1.second.size();
        stream >> ThorsAnvil::Serialize::bsonImporter(kind1.second[index]);
        size -= ThorsAnvil::Serialize::bsonGetPrintSize(kind1.second[index]);

        if (size > oldSize)
        {
            // Wrap around error.
            throw int(5);
        }
    }
    return 0;
}
template<typename Section0, typename... Section1>
std::istream& Op_MsgReply<Section0, Section1...>::parse(std::istream& stream)
{
    std::uint8_t    kindMark = -1;
    stream >> Op_Msg<Section0, Section1...>::header
           >> make_LE(Op_Msg<Section0, Section1...>::flags)
           >> kindMark >> ThorsAnvil::Serialize::bsonImporter(Op_Msg<Section0, Section1...>::section0);

    if (kindMark != 0)
    {
        throw int(9);
    }
    std::apply([&stream](auto&... sectionInfo)
    {
        std::make_tuple(parseKind1(stream, sectionInfo)...);
    }, Op_Msg<Section0, Section1...>::sections);

    bool expectCheckSum = (Op_Msg<Section0, Section1...>::flags & OP_MsgFlag::checksumPresent) != OP_MsgFlag::empty;
    if (expectCheckSum)
    {
        stream >> make_LE(Op_Msg<Section0, Section1...>::checksum);
        // TODO Check the checksum.
        // Need to make the stream understand the concept of CRC32C
        // and append the data to the checksum as it is read.
    }
    if (!stream)
    {
        Op_Msg<Section0, Section1...>::section0.ok  = 0;
        Op_Msg<Section0, Section1...>::section0.codeName += "Error On Input Stream";
    }
    return stream;
}

template<typename Section0, typename... Section1>
Op_MsgReply<Section0, Section1...>::operator bool() const
{
    return this->isOk();
}

template<typename Section0, typename... Section1>
bool Op_MsgReply<Section0, Section1...>::isOk()  const
{
    return Op_Msg<Section0, Section1...>::getAction().ok;
}

template<typename Section0, typename... Section1>
std::string Op_MsgReply<Section0, Section1...>::getHRErrorMessage() const
{
    return Op_Msg<Section0, Section1...>::getAction().getHRErrorMessage();
}

}

#endif
