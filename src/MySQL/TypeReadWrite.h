#ifndef THORSANVIL_DB_MYSQL_TYPE_READ_WRITE_H
#define THORSANVIL_DB_MYSQL_TYPE_READ_WRITE_H

#include "ThorMySQL.h"
#include "ConectReader.h"
#include "ConectWriter.h"
#include "ThorsDB/Statement.h"
#include "ThorsIOUtil/Utility.h"
#include "ThorsLogging/ThorsLogging.h"
#include <map>
#include <vector>
#include <string>
//#include <cstddef>   // for size_t (removed because it crashes clang 3.5 on travis
#include <sstream>
#include <limits>
#include <stdexcept>
#include <time.h>

namespace ThorsAnvil::DB::MySQL
{

using Buffer = std::vector<char>;
using Access::UnixTimeStamp;

template<typename T>
inline T standardConverter(std::string const& t, std::size_t* p)
{
    // used for char short
    T   result  = stoi(t, p);
    // Then conversion failed.
    if (result < std::numeric_limits<T>::min() || result > std::numeric_limits<T>::max())
    {
        (*p) = 0;
    }
    return result;
}
// Optimized versions
template<> inline int       standardConverter<int>(std::string const& t, std::size_t* p)        {return stoi(t, p);}
template<> inline long      standardConverter<long>(std::string const& t, std::size_t* p)       {return stol(t, p);}
template<> inline long long standardConverter<long long>(std::string const& t, std::size_t* p)  {return stoll(t, p);}

template<typename T>
inline T stringtointeger(ConectReader& p)
{
    std::size_t  pos;
    std::string const& t = p.lengthEncodedString();
    T            result = standardConverter<T>(t, &pos);
    if (pos != t.size())
    {
        ThorsLogAndThrow("ThorsAnvil::DB::MySQL",
                         "stringtointeger",
                         "Failed to convert whole integer: ", t,
                         "\nOnly read the first ", pos, "bytes from this string.");
    }
    return result;
}

inline std::string mapMySQLTypeToString(int mySQLType)
{
    static std::map<int, std::string> names
    {
        {-1,    "===="},
        {0x00,  "MYSQL_TYPE_DECIMAL"},
        {0x01,  "MYSQL_TYPE_TINY"},
        {0x02,  "MYSQL_TYPE_SHORT"},
        {0x03,  "MYSQL_TYPE_LONG"},
        {0x04,  "MYSQL_TYPE_FLOAT"},
        {0x05,  "MYSQL_TYPE_DOUBLE"},
        {0x06,  "MYSQL_TYPE_NULL"},
        {0x07,  "MYSQL_TYPE_TIMESTAMP"},
        {0x08,  "MYSQL_TYPE_LONGLONG"},
        {0x09,  "MYSQL_TYPE_INT24"},
        {0x0a,  "MYSQL_TYPE_DATE"},
        {0x0b,  "MYSQL_TYPE_TIME"},
        {0x0c,  "MYSQL_TYPE_DATETIME"},
        {0x0d,  "MYSQL_TYPE_YEAR"},
        {0x0e,  "MYSQL_TYPE_NEWDATE"},
        {0x0f,  "MYSQL_TYPE_VARCHAR"},
        {0x10,  "MYSQL_TYPE_BIT"},
        {0x11,  "MYSQL_TYPE_TIMESTAMP2"},
        {0x12,  "MYSQL_TYPE_DATETIME2"},
        {0x13,  "MYSQL_TYPE_TIME2"},
        {0xf6,  "MYSQL_TYPE_NEWDECIMAL"},
        {0xf7,  "MYSQL_TYPE_ENUM"},
        {0xf8,  "MYSQL_TYPE_SET"},
        {0xf9,  "MYSQL_TYPE_TINY_BLOB"},
        {0xfa,  "MYSQL_TYPE_MEDIUM_BLOB"},
        {0xfb,  "MYSQL_TYPE_LONG_BLOB"},
        {0xfc,  "MYSQL_TYPE_BLOB"},
        {0xfd,  "MYSQL_TYPE_VAR_STRING"},
        {0xfe,  "MYSQL_TYPE_STRING"},
        {0xff,  "MYSQL_TYPE_GEOMETRY"},
        {0x101, "MYSQL_TYPE_TINY_UNSIGNED"},
        {0x102, "MYSQL_TYPE_SHORT_UNSIGNED"},
        {0x103, "MYSQL_TYPE_LONG_UNSIGNED"},
        {0x108, "MYSQL_TYPE_LONGLONG_UNSIGNED"},
        {0x109, "MYSQL_TYPE_INT24_UNSIGNED"}
    };

    auto find = names.find(mySQLType);
    if (find == names.end())
    {
        return "Unknown???";
    }
    return find->second;
}


template<int Tv, typename T>
inline std::string getErrorMessage()
{
    std::stringstream msg;
    msg << "In this case:\n"
        << "\tC++ Type:   " << typeid(T).name() << "\n"
        << "\tMySQL Type: " << mapMySQLTypeToString(Tv) << "\n"
        << "\n"
        << "Note: This type of error should not happen.\n"
        << "      I am sorry I can't detect this at compile time because the types returned by MySQL\n"
        << "      are generated from run-time values. But this is the type of error you should discover\n"
        << "      while testing and once corrected it should not happen again.\n"
        << "\n"
        << "      It is done so you are forced to externally decide what data loss is going to\n"
        << "      happen and make an explicit choice (and maybe decide if that needs to be validated)\n"
        << "\n"
        << "      If your code was previously working and this happens it means the type of a\n"
        << "      column in the DB has changed to an incompatible type. You may need to talk\n"
        << "      to your DB admin.";
    return msg.str();
}

template<int Tv, typename T>
inline T readParameterValue(ConectReader&)
{
    // Default action is to throw.
    // The translations we know about are defined below.
    ThorsLogAndThrowLogical("ThorsAnvil::DB::MySQL", "readParameterValue",
                            "Unknown conversion\n\n",
                            "This is caused by a `SELECT` clause having different argument types to the C++ lambda parameters\n",
                            getErrorMessage<Tv, T>());
}

template<typename Src>
unsigned int writeParameterValue(ConectWriter&, Src const&)
{
    // Default action is to throw.
    // The translations we know about are defined below.
    ThorsLogAndThrowLogical("ThorsAnvil::DB::MySQL",
                            "writeParameterValue",
                            "Unknown conversion", getErrorMessage<-1, Src>());
}

template<typename T>
T getBitField(ConectReader& p)
{
    std::string bitField    = p.lengthEncodedString();
    if (bitField.size() > sizeof(T))
    {
        ThorsLogAndThrowLogical("ThorsAnvil::DB::MySQL",
                                "getBitField",
                                "Bitfield to large for destination\n",
                                "   From DB:     ", bitField.size(), " bytes\n",
                                "   Output Type: ", typeid(T).name(), " Size: ", sizeof(T), "\n");
    }

    char const* valuePtr    = bitField.data();
    T           result      = 0;
    char*       resultPtr   = reinterpret_cast<char*>(&result);
    for (std::size_t loop=0;loop < sizeof(T) && loop < bitField.size(); ++loop)
    {
        resultPtr[loop] = valuePtr[loop];
    }
    // TODO take into account endianess
    return result;
}

/*
 * STRINGS
 */
using Intsll = signed long long;
using Intsl  = signed long;
using Intsi  = signed int;
using Intss  = signed short;
using Intsc  = signed char;
using Intull = unsigned long long;
using Intul  = unsigned long;
using Intui  = unsigned int;
using Intus  = unsigned short;
using Intuc  = unsigned char;
using String = std::string;
using Vector = std::vector<char>;

inline unsigned long long maskBuild(int length)
{
    static constexpr unsigned long long mask = -1;
    unsigned long long shield = (1ULL << (length * 8)) -1;

    return mask ^ shield;
}

inline unsigned long long signExtend(unsigned long long value, int length)
{
    return ((length != sizeof(unsigned long long)) && ((value >> (length * 8 - 1)) & 0x1))
        ? maskBuild(length) | value     // negative value
        : value;                        // positive value
}
// VARCHAR(32)
template<> inline String readParameterValue<MYSQL_TYPE_VAR_STRING,    String>(ConectReader& p) {return p.lengthEncodedString();}
// VARCHAR(64)
template<> inline String readParameterValue<MYSQL_TYPE_STRING,        String>(ConectReader& p) {return p.lengthEncodedString();}
//TINYTEXT TEXT MEDIUMTEXT LONGTEXT
template<> inline String readParameterValue<MYSQL_TYPE_BLOB,          String>(ConectReader& p) {return p.lengthEncodedString();}

/*
 * BLOBS
 */
template<> inline Vector readParameterValue<MYSQL_TYPE_VAR_STRING,      Vector>(ConectReader& p) {return p.lengthEncodedBlob();}
template<> inline Vector readParameterValue<MYSQL_TYPE_STRING,          Vector>(ConectReader& p) {return p.lengthEncodedBlob();}
template<> inline Vector readParameterValue<MYSQL_TYPE_BLOB,            Vector>(ConectReader& p) {return p.lengthEncodedBlob();}

/*
 * Integers
 */
 // signed long long
template<> inline Intsll readParameterValue<MYSQL_TYPE_LONGLONG,      Intsll>(ConectReader& p) {return signExtend(p.fixedLengthInteger<8>(), 8);}
template<> inline Intsll readParameterValue<MYSQL_TYPE_LONG,          Intsll>(ConectReader& p) {return signExtend(p.fixedLengthInteger<4>(), 4);}
template<> inline Intsll readParameterValue<MYSQL_TYPE_INT24,         Intsll>(ConectReader& p) {return signExtend(p.fixedLengthInteger<4>(), 4);}
template<> inline Intsll readParameterValue<MYSQL_TYPE_SHORT,         Intsll>(ConectReader& p) {return signExtend(p.fixedLengthInteger<2>(), 2);}
template<> inline Intsll readParameterValue<MYSQL_TYPE_TINY,          Intsll>(ConectReader& p) {return signExtend(p.fixedLengthInteger<1>(), 1);}
// No MYSQL_TYPE_BIT => Signed integer (only unsigned is supported on purpose)

 // unsigned long long
template<> inline Intull readParameterValue<MYSQL_TYPE_LONGLONG_UNSIGNED,Intull>(ConectReader& p) {return p.fixedLengthInteger<8>();}
template<> inline Intull readParameterValue<MYSQL_TYPE_LONG_UNSIGNED, Intull>(ConectReader& p) {return p.fixedLengthInteger<4>();}
template<> inline Intull readParameterValue<MYSQL_TYPE_INT24_UNSIGNED,Intull>(ConectReader& p) {return p.fixedLengthInteger<4>();}
template<> inline Intull readParameterValue<MYSQL_TYPE_SHORT_UNSIGNED,Intull>(ConectReader& p) {return p.fixedLengthInteger<2>();}
template<> inline Intull readParameterValue<MYSQL_TYPE_TINY_UNSIGNED, Intull>(ConectReader& p) {return p.fixedLengthInteger<1>();}
template<> inline Intull readParameterValue<MYSQL_TYPE_BIT,           Intull>(ConectReader& p) {return getBitField<Intull>(p);}

/*
 * Integers
 */
 // signed long
template<> inline Intsl  readParameterValue<MYSQL_TYPE_LONGLONG,      Intsl>(ConectReader& p)  {return signExtend(p.fixedLengthInteger<8>(), 8);}
template<> inline Intsl  readParameterValue<MYSQL_TYPE_LONG,          Intsl>(ConectReader& p)  {return signExtend(p.fixedLengthInteger<4>(), 4);}
template<> inline Intsl  readParameterValue<MYSQL_TYPE_INT24,         Intsl>(ConectReader& p)  {return signExtend(p.fixedLengthInteger<4>(), 4);}
template<> inline Intsl  readParameterValue<MYSQL_TYPE_SHORT,         Intsl>(ConectReader& p)  {return signExtend(p.fixedLengthInteger<2>(), 2);}
template<> inline Intsl  readParameterValue<MYSQL_TYPE_TINY,          Intsl>(ConectReader& p)  {return signExtend(p.fixedLengthInteger<1>(), 1);}
// No MYSQL_TYPE_BIT => Signed integer (only unsigned is supported on purpose)

 // unsigned long
template<> inline Intul  readParameterValue<MYSQL_TYPE_LONGLONG_UNSIGNED,Intul>(ConectReader& p)  {return p.fixedLengthInteger<8>();}
template<> inline Intul  readParameterValue<MYSQL_TYPE_LONG_UNSIGNED, Intul>(ConectReader& p)  {return p.fixedLengthInteger<4>();}
template<> inline Intul  readParameterValue<MYSQL_TYPE_INT24_UNSIGNED,Intul>(ConectReader& p)  {return p.fixedLengthInteger<4>();}
template<> inline Intul  readParameterValue<MYSQL_TYPE_SHORT_UNSIGNED,Intul>(ConectReader& p)  {return p.fixedLengthInteger<2>();}
template<> inline Intul  readParameterValue<MYSQL_TYPE_TINY_UNSIGNED, Intul>(ConectReader& p)  {return p.fixedLengthInteger<1>();}
template<> inline Intul  readParameterValue<MYSQL_TYPE_BIT,           Intul>(ConectReader& p)  {return getBitField<Intul>(p);}

 // signed int
template<> inline Intsi  readParameterValue<MYSQL_TYPE_LONG,          Intsi>(ConectReader& p)  {return signExtend(p.fixedLengthInteger<4>(), 4);}
template<> inline Intsi  readParameterValue<MYSQL_TYPE_INT24,         Intsi>(ConectReader& p)  {return signExtend(p.fixedLengthInteger<4>(), 4);}
template<> inline Intsi  readParameterValue<MYSQL_TYPE_SHORT,         Intsi>(ConectReader& p)  {return signExtend(p.fixedLengthInteger<2>(), 2);}
template<> inline Intsi  readParameterValue<MYSQL_TYPE_TINY,          Intsi>(ConectReader& p)  {return signExtend(p.fixedLengthInteger<1>(), 1);}
// No MYSQL_TYPE_BIT => Signed integer (only unsigned is supported on purpose)

 // unsigned int
template<> inline Intui  readParameterValue<MYSQL_TYPE_LONG_UNSIGNED, Intui>(ConectReader& p)  {return p.fixedLengthInteger<4>();}
template<> inline Intui  readParameterValue<MYSQL_TYPE_INT24_UNSIGNED,Intui>(ConectReader& p)  {return p.fixedLengthInteger<4>();}
template<> inline Intui  readParameterValue<MYSQL_TYPE_SHORT_UNSIGNED,Intui>(ConectReader& p)  {return p.fixedLengthInteger<2>();}
template<> inline Intui  readParameterValue<MYSQL_TYPE_TINY_UNSIGNED, Intui>(ConectReader& p)  {return p.fixedLengthInteger<1>();}
template<> inline Intui  readParameterValue<MYSQL_TYPE_BIT,           Intui>(ConectReader& p)  {return getBitField<Intui>(p);}

 // signed short
template<> inline Intss  readParameterValue<MYSQL_TYPE_SHORT,         Intss>(ConectReader& p)  {return signExtend(p.fixedLengthInteger<2>(), 2);}
template<> inline Intss  readParameterValue<MYSQL_TYPE_TINY,          Intss>(ConectReader& p)  {return signExtend(p.fixedLengthInteger<1>(), 1);}
// No MYSQL_TYPE_BIT => Signed integer (only unsigned is supported on purpose)

 // unsigned short
template<> inline Intus  readParameterValue<MYSQL_TYPE_SHORT_UNSIGNED,Intus>(ConectReader& p)  {return p.fixedLengthInteger<2>();}
template<> inline Intus  readParameterValue<MYSQL_TYPE_TINY_UNSIGNED, Intus>(ConectReader& p)  {return p.fixedLengthInteger<1>();}
template<> inline Intus  readParameterValue<MYSQL_TYPE_BIT,           Intus>(ConectReader& p)  {return getBitField<Intus>(p);}

 // char
template<> inline char   readParameterValue<MYSQL_TYPE_TINY,          char>(ConectReader& p)   {return signExtend(p.fixedLengthInteger<1>(), 1);}
template<> inline char   readParameterValue<MYSQL_TYPE_TINY_UNSIGNED, char>(ConectReader& p)   {return signExtend(p.fixedLengthInteger<1>(), 1);}
// No MYSQL_TYPE_BIT => Signed integer (only unsigned is supported on purpose)


 // signed char
template<> inline Intsc  readParameterValue<MYSQL_TYPE_TINY,          Intsc>(ConectReader& p)  {return signExtend(p.fixedLengthInteger<1>(), 1);}
// No MYSQL_TYPE_BIT => Signed integer (only unsigned is supported on purpose)

 // unsigned char
template<> inline Intuc  readParameterValue<MYSQL_TYPE_TINY_UNSIGNED, Intuc>(ConectReader& p)  {return p.fixedLengthInteger<1>();}
template<> inline Intuc  readParameterValue<MYSQL_TYPE_BIT,           Intuc>(ConectReader& p)  {return getBitField<Intuc>(p);}

/*
 * Floating point
 */
template<> inline long double readParameterValue<MYSQL_TYPE_DOUBLE,   long double>(ConectReader& p)
{
    double result;
    p.read(reinterpret_cast<char*>(&result), 8);
    return result;
}
template<> inline double readParameterValue<MYSQL_TYPE_DOUBLE,        double>(ConectReader& p)
{
    double result;
    p.read(reinterpret_cast<char*>(&result), 8);
    return result;
}
template<> inline long double readParameterValue<MYSQL_TYPE_FLOAT,    long double>(ConectReader& p)
{
    float result;
    p.read(reinterpret_cast<char*>(&result), 4);
    return result;
}
template<> inline double  readParameterValue<MYSQL_TYPE_FLOAT,         double>(ConectReader& p)
{
    float result;
    p.read(reinterpret_cast<char*>(&result), 4);
    return result;
}
template<> inline float  readParameterValue<MYSQL_TYPE_FLOAT,         float>(ConectReader& p)
{
    float  result;
    p.read(reinterpret_cast<char*>(&result), 4);
    return result;
}
// https://dev.mysql.com/doc/internals/en/binary-protocol-value.html#packet-ProtocolBinary::MYSQL_TYPE_DECIMAL
template<> inline long double  readParameterValue<MYSQL_TYPE_DECIMAL, long double>(ConectReader& p)
{
    std::string value = p.lengthEncodedString();
    long double result = std::stold(value);
    return result;
}
template<> inline double  readParameterValue<MYSQL_TYPE_DECIMAL,      double>(ConectReader& p)
{
    std::string value = p.lengthEncodedString();
    double result = std::stod(value);
    return result;
}
template<> inline float  readParameterValue<MYSQL_TYPE_DECIMAL,       float>(ConectReader& p)
{
    std::string value = p.lengthEncodedString();
    float result = std::stof(value);
    return result;
}
//https://dev.mysql.com/doc/internals/en/binary-protocol-value.html#packet-ProtocolBinary::MYSQL_TYPE_NEWDECIMAL
template<> inline long double  readParameterValue<MYSQL_TYPE_NEWDECIMAL, long double>(ConectReader& p)
{
    return readParameterValue<MYSQL_TYPE_DECIMAL, long double>(p);
}
template<> inline double  readParameterValue<MYSQL_TYPE_NEWDECIMAL,      double>(ConectReader& p)
{
    return readParameterValue<MYSQL_TYPE_DECIMAL, double>(p);
}
template<> inline float  readParameterValue<MYSQL_TYPE_NEWDECIMAL,       float>(ConectReader& p)
{
    return readParameterValue<MYSQL_TYPE_DECIMAL, float>(p);
}

/*
 * Time/Date
 */
// https://dev.mysql.com/doc/internals/en/binary-protocol-value.html#packet-ProtocolBinary::MYSQL_TYPE_DATE
// https://dev.mysql.com/doc/internals/en/binary-protocol-value.html#packet-ProtocolBinary::MYSQL_TYPE_TIME
// https://dev.mysql.com/doc/internals/en/binary-protocol-value.html#packet-ProtocolBinary::MYSQL_TYPE_TIMESTAMP
template<> inline UnixTimeStamp readParameterValue<MYSQL_TYPE_DATE,      UnixTimeStamp>(ConectReader& p){return UnixTimeStamp(p.readDate());}
template<> inline UnixTimeStamp readParameterValue<MYSQL_TYPE_TIME,      UnixTimeStamp>(ConectReader& p){return UnixTimeStamp(p.readRel());}
template<> inline UnixTimeStamp readParameterValue<MYSQL_TYPE_TIMESTAMP, UnixTimeStamp>(ConectReader& p){return UnixTimeStamp(p.readDate());}
template<> inline UnixTimeStamp readParameterValue<MYSQL_TYPE_DATETIME,  UnixTimeStamp>(ConectReader& p){return UnixTimeStamp(p.readDate());}
template<> inline int           readParameterValue<MYSQL_TYPE_YEAR,      int>          (ConectReader& p){return p.fixedLengthInteger<2>();}

// https://dev.mysql.com/doc/internals/en/com-query-response.html#packet-Protocol::ColumnType
template<>
inline unsigned int writeParameterValue<std::vector<char>>(ConectWriter& p, std::vector<char> const& v)
{
    p.writeLengthEncodedBlob(v);
    return MYSQL_TYPE_BLOB;
}

template<>
inline unsigned int writeParameterValue<std::string>(ConectWriter& p, std::string const& v)
{
    p.writeLengthEncodedString(v);
    return MYSQL_TYPE_STRING;
}

template<>
inline unsigned int writeParameterValue<char>(ConectWriter& p, char const& v)
{
    // If you bind as a string it matches both number and string.
    // Note the char '8' will match the number 8 (a bit wierd) but this is the
    // same behavior as you get from mysql console.
    std::string value(1, v);
    p.writeLengthEncodedString(value);
    return MYSQL_TYPE_STRING;

    // If you bind as a number it does not match text.
    //p.writeFixedLengthInteger<1>(v);
    //return MYSQL_TYPE_TINY;
}

template<>
inline unsigned int writeParameterValue<signed char>(ConectWriter& p, signed char const& v)
{
    return writeParameterValue<char>(p, static_cast<char>(v));
}

template<>
inline unsigned int writeParameterValue<unsigned char>(ConectWriter& p, unsigned char const& v)
{
    return writeParameterValue<char>(p, static_cast<char>(v));
}

template<>
inline unsigned int writeParameterValue<signed short>(ConectWriter& p, signed short const& v)
{
    p.writeFixedLengthInteger<2>(v);
    return MYSQL_TYPE_SHORT;
}

template<>
inline unsigned int writeParameterValue<unsigned short>(ConectWriter& p, unsigned short const& v)
{
    p.writeFixedLengthInteger<2>(v);
    return MYSQL_TYPE_SHORT;
}

template<>
inline unsigned int writeParameterValue<signed int>(ConectWriter& p, signed int const& v)
{
    p.writeFixedLengthInteger<4>(v);
    return MYSQL_TYPE_LONG;
}

template<>
inline unsigned int writeParameterValue<unsigned int>(ConectWriter& p, unsigned int const& v)
{
    p.writeFixedLengthInteger<4>(v);
    return MYSQL_TYPE_LONG;
}

template<>
inline unsigned int writeParameterValue<signed long>(ConectWriter& p, signed long const& v)
{
    p.writeFixedLengthInteger<4>(v);
    return MYSQL_TYPE_LONG;
}

template<>
inline unsigned int writeParameterValue<unsigned long>(ConectWriter& p, unsigned long const& v)
{
    p.writeFixedLengthInteger<4>(v);
    return MYSQL_TYPE_LONG;
}

template<>
inline unsigned int writeParameterValue<signed long long>(ConectWriter& p, signed long long const& v)
{
    p.writeFixedLengthInteger<8>(v);
    return MYSQL_TYPE_LONGLONG;
}

template<>
inline unsigned int writeParameterValue<unsigned long long>(ConectWriter& p, unsigned long long const& v)
{
    p.writeFixedLengthInteger<8>(v);
    return MYSQL_TYPE_LONGLONG;
}

template<>
inline unsigned int writeParameterValue<float>(ConectWriter& p, float const& v)
{
    p.writeRawData(reinterpret_cast<char const*>(&v), 4);
    return MYSQL_TYPE_FLOAT;
}

template<>
inline unsigned int writeParameterValue<double>(ConectWriter& p, double const& v)
{
    p.writeRawData(reinterpret_cast<char const*>(&v), 8);
    return MYSQL_TYPE_DOUBLE;
}

template<>
inline unsigned int writeParameterValue<ThorsAnvil::DB::Access::UnixTimeStamp>(ConectWriter& p, ThorsAnvil::DB::Access::UnixTimeStamp const& v)
{
    p.writeDate(v.time);
    return MYSQL_TYPE_TIMESTAMP;
}

}

#endif
