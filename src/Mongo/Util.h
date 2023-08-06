#ifndef THORSANVIL_DB_MONGO_UTIL_H
#define THORSANVIL_DB_MONGO_UTIL_H

#include <boost/endian/conversion.hpp>

#include <ostream>
#include <istream>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <type_traits>

namespace ThorsAnvil::DB::Mongo
{

/*
 * Check if a type T has a member type called Options
 *  hasOptions:     bool that detects if Options is there.
 *  HasOptions:     Type is bool if Actual has Options otherwise invalid type.
 *  NoOptions:      Type is bool is Actual does not have Options otherwise invalid type.
 *
 * This is used to special the constructor of a templated class where:
 *  1: The template parameter is used to define a member of the class.
 *  2: The template parameter has the potentially for an Options attribute.
 *  3: The constructor for the tempalted class must thus either pass the "Options" attribute
 *     from its constructor to the constructor of the member.
 *
 *      template<typename X>
 *      class Y
 *      {
 *              X       member;
 *          public:
 *              // This constructor is valid of X does NOT have an Options type.
 *              // Thus we will not pass an Options object through to the constructor of member.
 *              template<NoOptions<X> = true>
 *              Y()
 *                  : member{}
 *              {}
 *              // This constructor is valid of X DOES have an Options type.
 *              // Thus we will pass an Options object through to the constructor of member.
 *              template<HasOptions<X> = true, typename... Args>
 *              Y(typename X::Options&& options)
 *                  : member{std::forward<typename X::Options>(options)}
 *              {}
 *              // Note: the "= true" above is simply for aesthetic purposes to make it look nice
 *              //       a   "= false" is just as valid.
 *      };
 */
template<typename Actual, typename = void>
constexpr bool hasOptions = false;
template<typename Actual>
constexpr bool hasOptions<Actual, std::void_t<decltype(sizeof(typename Actual::Options))>> = true;

template<typename Actual>
using HasOptions            = std::enable_if_t<hasOptions<Actual>, bool>;
template<typename Actual>
using NoOptions             = std::enable_if_t<!hasOptions<Actual>, bool>;

template<typename Actual>
using HasUserData         = std::enable_if_t<!std::is_same_v<typename Actual::UserData, void>, bool>;
template<typename Actual>
using NoUserData          = std::enable_if_t<std::is_same_v<typename Actual::UserData, void>, bool>;

/*
 * Remove all const/volatile/reference from the types Actual
 * This probably needs some deeper review. I am sure somebody else has this problem before
 * and there is a more standard way of achieving the same effect.
 */
template<typename Actual>
using Base                  = std::remove_cv_t<std::remove_reference_t<std::remove_cv_t<Actual>>>;

/*
 * Stream an object in a "Human Readable" format.
 * Very useful for de-bugging.
 *
 * It simply wraps the object and when passed to a stream calls the method `printHR()`.
 */
template<typename Streamable>
struct HumanReadable
{
    Streamable const&     object;
    public:
        HumanReadable(Streamable const& object)
            : object(object)
        {}
        std::ostream& print(std::ostream& stream) const
        {
            return object.printHR(stream);
        }
};

template<typename Streamable>
std::ostream& operator<<(std::ostream& stream, HumanReadable<Streamable> const& reply)
{
    return reply.print(stream);
}

template<typename Stremable>
HumanReadable<Stremable> make_hr(Stremable const& object)
{
    return HumanReadable<Stremable>(object);
}


/*
 * Stream an object in a "Human Readable" format but dumping the binary version of the object.
 * Very useful for de-bugging.
 *
 * It streams the object as it normally would but to a temporary stream so we can get the string.
 * It then prints the string in groups of 4 bytes in hex and ascii.
 * Very much like octal dump.
 */
template<typename Streamable>
struct BinaryDump
{
    Streamable const&       object;
    public:
        BinaryDump(Streamable const& object)
            : object(object)
        {}
        std::ostream& print(std::ostream& stream) const
        {
            std::stringstream tmp;
            tmp << object;
            std::string dump = tmp.str();
            std::string line;
            int loop = 0;
            for (unsigned char val: dump)
            {
                stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(val) << std::dec << " ";
                line += ((val >= 32 && val <= 127) ? val : '-');
                line += " ";
                ++loop;
                if (loop % 4 == 0)
                {
                    stream << line << "\n";
                    line = "";
                    loop = 0;
                }
            }
            stream << std::string((4 - loop) * 2, ' ') << line << "\n";
            return stream;
        }
};

template<typename Streamable>
std::ostream& operator<<(std::ostream& stream, BinaryDump<Streamable> const& reply)
{
    return reply.print(stream);
}

template<typename Stremable>
BinaryDump<Stremable> make_bd(Stremable const& object)
{
    return BinaryDump<Stremable>(object);
}

/*
 * Send integer to/from stream in little endian form
 * Handles   16/32/64 bit values.
 *
 * Used like:
 *
 *      std::int32_t    value = 5;
 *
 *      // Write a little endian version of value to the file stream.
 *      file << make_LE(value);
 *      // Outputs  05 00 00 00
 *
 *      // Read a little endian version of value from the file stream.
 *      file >> make_LE(value);
 *      // Input   05 00 00 00  will be read into value and be int{5}
 */
template<typename T>
struct LittleEndian
{
    using UT = typename std::make_unsigned<T>::type;
    using ST = typename std::make_signed<UT>::type;
    T&   value;
    LittleEndian(T& value)
        : value(value)
    {}
    std::ostream& print(std::ostream& stream) const
    {
        ST   output = boost::endian::native_to_little(static_cast<ST>(static_cast<UT>(value)));
        stream.write(reinterpret_cast<char const*>(&output), sizeof(output));
        return stream;
    }
    std::istream& scan(std::istream& stream) const
    {
        ST   input;
        stream.read(reinterpret_cast<char*>(&input), sizeof(input));
        value = static_cast<T>(boost::endian::little_to_native(input));
        return stream;
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& stream, LittleEndian<T> const& data)
{
    return data.print(stream);
}

template<typename T>
std::istream& operator>>(std::istream& stream, LittleEndian<T> const& data)
{
    return data.scan(stream);
}

template<typename T>
struct LittleEndianConst
{
    using UT = typename std::make_unsigned<T>::type;
    using ST = typename std::make_signed<UT>::type;
    T const&   value;
    LittleEndianConst(T const& value)
        : value(value)
    {}
    std::ostream& print(std::ostream& stream) const
    {
        ST   output = boost::endian::native_to_little(static_cast<ST>(static_cast<UT>(value)));
        stream.write(reinterpret_cast<char const*>(&output), sizeof(output));
        return stream;
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& stream, LittleEndianConst<T> const& data)
{
    return data.print(stream);
}

template<typename T>
LittleEndian<T> make_LE(T& value)               {return LittleEndian<T>(value);}
template<typename T>
LittleEndianConst<T> make_LE(T const& value)    {return LittleEndianConst<T>(value);}

}

#endif
