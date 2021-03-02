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
Op_Msg<Section0, Section1...>::Op_Msg(OP_MsgFlag flags, Section0 action, KindB<Section1>&&... kind)
    : header(OpCode::OP_MSG)
    , flags(flags)
    , section0(std::move(action))
    , sections(Kind1<Section1>(std::forward<KindB<Section1>>(kind), std::uint32_t{0})...)
    , checksum(0)
{
    header.prepareToSend(getSize());
}

template<typename SectionInfo>
std::uint32_t getSizeOfKind1(std::uint32_t& sum, SectionInfo& sectionInfo)
{
    auto const& kind1 = sectionInfo.first;

    std::uint32_t docSize = 0;
    for (auto const& doc: kind1.second)
    {
        docSize += ThorsAnvil::Serialize::bsonGetPrintSize(doc);
    }
    sectionInfo.second = (1 + sizeof(std::uint32_t) + (kind1.first.size() + 1) + docSize);
    sum += sectionInfo.second;
    return sum;
}

template<typename Section0, typename... Section1>
std::uint32_t Op_Msg<Section0, Section1...>::getSize()
{
    std::uint32_t section0Size = (1 + ThorsAnvil::Serialize::bsonGetPrintSize(section0));
    std::uint32_t sectionsSize = 0;
    std::apply([&sectionsSize](auto&... sectionInfo)
    {
        std::make_tuple(getSizeOfKind1(sectionsSize, sectionInfo)...);
    }, sections);

    bool          showCheckSum = (flags & OP_MsgFlag::checksumPresent) != OP_MsgFlag::empty;
    std::uint32_t dataSize     = sizeof(flags) + section0Size + sectionsSize + (showCheckSum ? sizeof(checksum) : 0);
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
    stream << "\nCount: " << sectionInfo.second << ",\n"
           << ThorsAnvil::Serialize::jsonExporter(sectionInfo.first);
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
        std::make_tuple(streamKind1(stream, sectionInfo)...);
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
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::unordered()
{
    section0.unordered();
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::byPass()
{
    section0.byPass();
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::setComment(std::string val)
{
    section0.setComment(std::move(val));
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::setWrieConcern(int w, bool j, std::time_t wtimeout)
{
    section0.setWrieConcern(w, j, wtimeout);
    return *this;
}

#if 0
template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::addFileds(std::initializer_list<std::string> fieldNames)
{
    section0.addFileds(std::move(fieldNames));
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::addHint(std::string hint)
{
    section0.addHint(std::move(hint));
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::setSkip(std::size_t val)
{
    section0.setSkip(val);
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::setLimit(std::size_t val)
{
    section0.setLimit(val);
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::setBatchSize(std::size_t val)
{
    section0.setBatchSize(val);
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::oneBatch(bool val)
{
    section0.oneBatch(val);
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::setMaxTimeout(std::size_t val)
{
    section0.setMaxTimeout(val);
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::addReadConcern(ReadConcernLevel val)
{
    section0.addReadConcern(std::move(val));
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::addMax(std::string field, int val)
{
    section0.addMax(std::move(field), val);
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::addMin(std::string field, int val)
{
    section0.addMin(std::move(field), val);
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::justKeys(bool val)
{
    section0.justKeys(val);
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::showId(bool val)
{
    section0.showId(val);
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::tailableCursor(bool val)
{
    section0.tailableCursor(val);
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::tailedCursorAwait(bool val)
{
    section0.tailedCursorAwait(val);
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::setNoCursorTimeout(bool val)
{
    section0.setNoCursorTimeout(val);
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::setAllowPartialResults(bool val)
{
    section0.setAllowPartialResults(val);
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::useDisk(bool val)
{
    section0.useDisk(val);
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::setNew(bool val)
{
    section0.setNew(val);
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::setUpsert(bool val)
{
    section0.setUpsert(val);
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::waitFoolDiskFlush(bool val)
{
    section0.waitFoolDiskFlush(val);
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::waitForReplication(std::int32_t count)
{
    section0.waitForReplication(count);
    return *this;
}

template<typename Section0, typename... Section1>
Op_Msg<Section0, Section1...>& Op_Msg<Section0, Section1...>::setWaitTimeout(std::int32_t millisec)
{
    section0.setWaitTimeout(millisec);
    return *this;
}
#endif

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
