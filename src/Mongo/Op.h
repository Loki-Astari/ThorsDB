#ifndef THORSANVIL_DB_MONGO_OP_H
#define THORSANVIL_DB_MONGO_OP_H

#include <boost/endian/conversion.hpp>

#include <ostream>
#include <istream>
#include <cstdint>
#include <type_traits>

namespace ThorsAnvil::DB::Mongo
{

enum class OpCode : std::int32_t
{
    OP_REPLY        = 1,                    // Reply to a client request. responseTo is set.
    OP_UPDATE       = 2001,                 // Update document.
    OP_INSERT       = 2002,                 // Insert new document.
    // RESERVED        = 2003,                 // Formerly used for OP_GET_BY_OID.
    OP_QUERY        = 2004,                 // Query a collection.
    OP_GET_MORE     = 2005,                 // Get more data from a query. See Cursors.
    OP_DELETE       = 2006,                 // Delete documents.
    OP_KILL_CURSORS = 2007,                 // Notify database that the client has finished with the cursor.
    OP_MSG          = 2013,                 // Send a message using the format introduced in MongoDB 3.6.
};

template<typename Streamable>
struct HumanReadable
{
    Streamable&     object;
    public:
        HumanReadable(Streamable& object)
            : object(object)
        {}
        friend std::ostream& operator<<(std::ostream& stream, HumanReadable const& reply) {return reply.object.printHR(stream);}
};
template<typename Stremable>
HumanReadable<Stremable> make_hr(Stremable& object) {return HumanReadable<Stremable>(object);}

template<typename T>
struct LittleEndian
{
    using UT = typename std::make_unsigned<T>::type;
    T&   value;
    LittleEndian(T& value)
        : value(value)
    {}
    friend std::ostream& operator<<(std::ostream& stream, LittleEndian const& data)
    {
        std::int32_t    output = boost::endian::native_to_little(static_cast<std::int32_t>(static_cast<UT>(data.value)));
        stream.write(reinterpret_cast<char const*>(&output), sizeof(output));
        return stream;
    }
    friend std::istream& operator>>(std::istream& stream, LittleEndian const& data)
    {
        std::int32_t    input;
        stream.read(reinterpret_cast<char*>(&input), sizeof(input));
        data.value = static_cast<T>(boost::endian::little_to_native(input));
        return stream;
    }
};
template<typename T>
LittleEndian<T> make_LE(T& value)   {return LittleEndian<T>(value);}

}

#endif
