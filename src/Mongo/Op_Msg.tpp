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
inline std::ostream& operator<<(std::ostream& stream, Kind0<Data> const& msg)
{
    stream.write("\x00", 1);
    stream << ThorsAnvil::Serialize::bsonExporter(msg.data);
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
inline std::ostream& operator<<(std::ostream& stream, OP_Msg<Kind...>& msg)
{
    std::size_t dataSize = sizeof(msg.flagBits) + sizeof(msg.checksum);
    // Add Size of all sections;

    msg.header.prepareToSend(dataSize);

    stream << msg.header
           << make_LE(msg.flagBits);
    // Stream the sections;
           //<< msg.sections;
    stream << make_LE(msg.checksum);
    return stream;
}

}

#endif
