#ifndef THORSANVIL_DB_MONGO_OP_MSG_TPP
#define THORSANVIL_DB_MONGO_OP_MSG_TPP

namespace ThorsAnvil::DB::Mongo
{

// ---- Kind0

template<typename Data>
template<typename... Args>
inline Kind0<Data>::Kind0(Args&&... arg)
    : data{std::forward<Args>(arg)...}
{}


template<typename Data>
inline std::size_t Kind0<Data>::getSize(std::ostream& stream)
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

    std::size_t sectionSize = getSectionSize(stream, std::make_index_sequence<sizeof...(Kind)>());
    std::size_t dataSize    = sizeof(flagBits) + sectionSize + (showCheckSum ? sizeof(checksum) : 0);
    header.prepareToSend(dataSize);

    stream << header
           << make_LE(flagBits);

    // Stream the sections;
    printSection(stream, std::make_index_sequence<sizeof...(Kind)>());

    // Output the checksum only if we said it would be there.
    if (showCheckSum)
    {
        stream << make_LE(checksum);
    }
    return stream;
}

template<typename... Kind>
template<std::size_t... I>
std::size_t OP_Msg<Kind...>::getSectionSize(std::ostream& stream, std::index_sequence<I...>&&)
{
    std::size_t size = 0;
    ((size += std::get<I>(sections).getSize(stream)), ...);
    return size;
}

template<typename... Kind>
template<std::size_t... I>
void OP_Msg<Kind...>::printSection(std::ostream& stream, std::index_sequence<I...>&&)
{
    ((stream << std::get<I>(sections)), ...);
}

}

#endif
