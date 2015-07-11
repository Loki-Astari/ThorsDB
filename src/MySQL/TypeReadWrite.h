
#ifndef THORSANIVL_MYSQL_TYPE_READ_WRITE_H
#define THORSANIVL_MYSQL_TYPE_READ_WRITE_H

#include "ThorSQL/Statement.h"
#include "ThorSQL/SQLUtil.h"
#include "ConectReader.h"
#include "ConectWriter.h"
#include <sstream>
#include <limits>

namespace ThorsAnvil
{
    namespace MySQL
    {
        namespace Detail
        {



using Buffer = std::vector<char>;
using SQL::UnixTimeStamp;

template<typename T>
inline T standardConverter(std::string const& t, std::size_t* p)
{
    // used for char short
    T   result  = stoi(t, p);
    // Then conversion failed.
    if (result < std::numeric_limits<T>::min() || result > std::numeric_limits<T>::max()) {
        (*p) = 0;
    }
    return result;
}
// Optimized versions
template<> inline int       standardConverter<int>(std::string const& t, std::size_t* p)        {return stoi(t, p);}
template<> inline long      standardConverter<long>(std::string const& t, std::size_t* p)       {return stol(t, p);}
template<> inline long long standardConverter<long long>(std::string const& t, std::size_t* p)  {return stoll(t, p);}

template<typename T>
inline T stringtointeger(std::string const& t)
{
    std::size_t  pos;
    T            result = standardConverter<T>(t, &pos);
    if (pos != t.size()) {
        throw std::runtime_error(
                errorMsg("ThorsAnvil::MySQL::stringtointeger: ",
                         "Failed to convert whole integer: ", t,
                         "\nOnly read the first ", pos, "bytes from this string."
              ));
    }
    return result;
}

inline std::string mapMySQLTypeToString(int mySQLType)
{
    static std::map<int,std::string> names {
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
        {0xff, "MYSQL_TYPE_GEOMETRY"}};

    auto find = names.find(mySQLType);
    if (find == names.end()) {
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
        << "      I am sorry I can't detect this at compile time but once working it should not\n"
        << "      happen again so you should fix this error during testing.\n"
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
    throw std::logic_error(
            errorMsg("ThorsAnvil::MySQL::readParameterValue: ",
                     "Unknown conversion\n",
                     "\n",
                     "This is caused by a `SELECT` clause having different argument types to the C++ lambda parameters\n",
                     getErrorMessage<Tv,T>()
          ));
}

template<typename Src>
unsigned int writeParameterValue(ConectWriter&, Src const&)
{
    // Default action is to throw.
    // The translations we know about are defined below.
    throw std::runtime_error(
            errorMsg("ThorsAnvil::MySQL::writeParameterValue: ",
                     "Unknown conversion"
          ));
}

template<> inline std::string       readParameterValue<MYSQL_TYPE_VAR_STRING,   std::string>(ConectReader& p)        {return p.lengthEncodedString();}
template<> inline std::string       readParameterValue<MYSQL_TYPE_STRING,       std::string>(ConectReader& p)        {return p.lengthEncodedString();}
template<> inline std::string       readParameterValue<MYSQL_TYPE_VARCHAR,      std::string>(ConectReader& p)        {return p.lengthEncodedString();}

template<> inline Buffer            readParameterValue<MYSQL_TYPE_TINY_BLOB,    std::vector<char>>(ConectReader& p)  {return p.lengthEncodedBlob();}
template<> inline Buffer            readParameterValue<MYSQL_TYPE_MEDIUM_BLOB,  std::vector<char>>(ConectReader& p)  {return p.lengthEncodedBlob();}
template<> inline Buffer            readParameterValue<MYSQL_TYPE_BLOB,         std::vector<char>>(ConectReader& p)  {return p.lengthEncodedBlob();}
template<> inline Buffer            readParameterValue<MYSQL_TYPE_LONG_BLOB,    std::vector<char>>(ConectReader& p)  {return p.lengthEncodedBlob();}

template<> inline signed long       readParameterValue<MYSQL_TYPE_DECIMAL,      signed long>(ConectReader& p)        {return stringtointeger<signed long>(p.lengthEncodedString());}
template<> inline signed long       readParameterValue<MYSQL_TYPE_NEWDECIMAL,   signed long>(ConectReader& p)        {return stringtointeger<signed long>(p.lengthEncodedString());}
template<> inline signed long       readParameterValue<MYSQL_TYPE_LONGLONG,     signed long>(ConectReader& p)        {return p.fixedLengthInteger<8>();}
template<> inline signed long       readParameterValue<MYSQL_TYPE_LONG,         signed long>(ConectReader& p)        {return p.fixedLengthInteger<4>();}
template<> inline signed long       readParameterValue<MYSQL_TYPE_INT24,        signed long>(ConectReader& p)        {return p.fixedLengthInteger<4>();}
template<> inline signed long       readParameterValue<MYSQL_TYPE_SHORT,        signed long>(ConectReader& p)        {return p.fixedLengthInteger<2>();}
template<> inline signed long       readParameterValue<MYSQL_TYPE_TINY,         signed long>(ConectReader& p)        {return p.fixedLengthInteger<1>();}
template<> inline unsigned long     readParameterValue<MYSQL_TYPE_DECIMAL,      unsigned long>(ConectReader& p)      {return stringtointeger<unsigned long>(p.lengthEncodedString());}
template<> inline unsigned long     readParameterValue<MYSQL_TYPE_NEWDECIMAL,   unsigned long>(ConectReader& p)      {return stringtointeger<unsigned long>(p.lengthEncodedString());}
template<> inline unsigned long     readParameterValue<MYSQL_TYPE_LONGLONG,     unsigned long>(ConectReader& p)      {return p.fixedLengthInteger<8>();}
template<> inline unsigned long     readParameterValue<MYSQL_TYPE_LONG,         unsigned long>(ConectReader& p)      {return p.fixedLengthInteger<4>();}
template<> inline unsigned long     readParameterValue<MYSQL_TYPE_INT24,        unsigned long>(ConectReader& p)      {return p.fixedLengthInteger<4>();}
template<> inline unsigned long     readParameterValue<MYSQL_TYPE_SHORT,        unsigned long>(ConectReader& p)      {return p.fixedLengthInteger<2>();}
template<> inline unsigned long     readParameterValue<MYSQL_TYPE_TINY,         unsigned long>(ConectReader& p)      {return p.fixedLengthInteger<1>();}

template<> inline signed int        readParameterValue<MYSQL_TYPE_DECIMAL,      signed int>(ConectReader& p)         {return stringtointeger<signed int>(p.lengthEncodedString());}
template<> inline signed int        readParameterValue<MYSQL_TYPE_NEWDECIMAL,   signed int>(ConectReader& p)         {return stringtointeger<signed int>(p.lengthEncodedString());}
template<> inline signed int        readParameterValue<MYSQL_TYPE_LONG,         signed int>(ConectReader& p)         {return p.fixedLengthInteger<4>();}
template<> inline signed int        readParameterValue<MYSQL_TYPE_INT24,        signed int>(ConectReader& p)         {return p.fixedLengthInteger<4>();}
template<> inline signed int        readParameterValue<MYSQL_TYPE_SHORT,        signed int>(ConectReader& p)         {return p.fixedLengthInteger<2>();}
template<> inline signed int        readParameterValue<MYSQL_TYPE_TINY,         signed int>(ConectReader& p)         {return p.fixedLengthInteger<1>();}
template<> inline unsigned int      readParameterValue<MYSQL_TYPE_DECIMAL,      unsigned int>(ConectReader& p)       {return stringtointeger<unsigned int>(p.lengthEncodedString());}
template<> inline unsigned int      readParameterValue<MYSQL_TYPE_NEWDECIMAL,   unsigned int>(ConectReader& p)       {return stringtointeger<unsigned int>(p.lengthEncodedString());}
template<> inline unsigned int      readParameterValue<MYSQL_TYPE_LONG,         unsigned int>(ConectReader& p)       {return p.fixedLengthInteger<4>();}
template<> inline unsigned int      readParameterValue<MYSQL_TYPE_INT24,        unsigned int>(ConectReader& p)       {return p.fixedLengthInteger<4>();}
template<> inline unsigned int      readParameterValue<MYSQL_TYPE_SHORT,        unsigned int>(ConectReader& p)       {return p.fixedLengthInteger<2>();}
template<> inline unsigned int      readParameterValue<MYSQL_TYPE_TINY,         unsigned int>(ConectReader& p)       {return p.fixedLengthInteger<1>();}

template<> inline signed short      readParameterValue<MYSQL_TYPE_DECIMAL,      signed short>(ConectReader& p)       {return stringtointeger<signed short>(p.lengthEncodedString());}
template<> inline signed short      readParameterValue<MYSQL_TYPE_NEWDECIMAL,   signed short>(ConectReader& p)       {return stringtointeger<signed short>(p.lengthEncodedString());}
template<> inline signed short      readParameterValue<MYSQL_TYPE_SHORT,        signed short>(ConectReader& p)       {return p.fixedLengthInteger<2>();}
template<> inline signed short      readParameterValue<MYSQL_TYPE_TINY,         signed short>(ConectReader& p)       {return p.fixedLengthInteger<1>();}
template<> inline unsigned short    readParameterValue<MYSQL_TYPE_DECIMAL,      unsigned short>(ConectReader& p)     {return stringtointeger<unsigned short>(p.lengthEncodedString());}
template<> inline unsigned short    readParameterValue<MYSQL_TYPE_NEWDECIMAL,   unsigned short>(ConectReader& p)     {return stringtointeger<unsigned short>(p.lengthEncodedString());}
template<> inline unsigned short    readParameterValue<MYSQL_TYPE_SHORT,        unsigned short>(ConectReader& p)     {return p.fixedLengthInteger<2>();}
template<> inline unsigned short    readParameterValue<MYSQL_TYPE_TINY,         unsigned short>(ConectReader& p)     {return p.fixedLengthInteger<1>();}

template<> inline char              readParameterValue<MYSQL_TYPE_DECIMAL,      char>(ConectReader& p)               {return stringtointeger<char>(p.lengthEncodedString());}
template<> inline char              readParameterValue<MYSQL_TYPE_NEWDECIMAL,   char>(ConectReader& p)               {return stringtointeger<char>(p.lengthEncodedString());}
template<> inline char              readParameterValue<MYSQL_TYPE_TINY,         char>(ConectReader& p)               {return p.fixedLengthInteger<1>();}
template<> inline signed char       readParameterValue<MYSQL_TYPE_DECIMAL,      signed char>(ConectReader& p)        {return stringtointeger<signed char>(p.lengthEncodedString());}
template<> inline signed char       readParameterValue<MYSQL_TYPE_NEWDECIMAL,   signed char>(ConectReader& p)        {return stringtointeger<signed char>(p.lengthEncodedString());}
template<> inline signed char       readParameterValue<MYSQL_TYPE_TINY,         signed char>(ConectReader& p)        {return p.fixedLengthInteger<1>();}
template<> inline unsigned char     readParameterValue<MYSQL_TYPE_DECIMAL,      unsigned char>(ConectReader& p)      {return stringtointeger<unsigned char>(p.lengthEncodedString());}
template<> inline unsigned char     readParameterValue<MYSQL_TYPE_NEWDECIMAL,   unsigned char>(ConectReader& p)      {return stringtointeger<unsigned char>(p.lengthEncodedString());}
template<> inline unsigned char     readParameterValue<MYSQL_TYPE_TINY,         unsigned char>(ConectReader& p)      {return p.fixedLengthInteger<1>();}

// TODO FIX
template<> inline double            readParameterValue<MYSQL_TYPE_DOUBLE,       double>(ConectReader& p)             {double result;p.read(reinterpret_cast<char*>(&result), 8);return result;}
template<> inline float             readParameterValue<MYSQL_TYPE_DOUBLE,       float>(ConectReader& p)              {double result;p.read(reinterpret_cast<char*>(&result), 8);return result;}
template<> inline float             readParameterValue<MYSQL_TYPE_FLOAT,        float>(ConectReader& p)              {float  result;p.read(reinterpret_cast<char*>(&result), 4);return result;}

// TODO FIX
template<> inline UnixTimeStamp     readParameterValue<MYSQL_TYPE_DATE,         UnixTimeStamp>(ConectReader& p)      {return UnixTimeStamp(p.readDate());}
template<> inline UnixTimeStamp     readParameterValue<MYSQL_TYPE_DATETIME,     UnixTimeStamp>(ConectReader& p)      {return UnixTimeStamp(p.readDate());}
template<> inline UnixTimeStamp     readParameterValue<MYSQL_TYPE_TIMESTAMP,    UnixTimeStamp>(ConectReader& p)      {return UnixTimeStamp(p.readDate());}
template<> inline UnixTimeStamp     readParameterValue<MYSQL_TYPE_TIME,         UnixTimeStamp>(ConectReader& p)      {return UnixTimeStamp(p.readRel());}

template<>
inline unsigned int writeParameterValue<std::string>(ConectWriter& p, std::string const& v)
{   
    p.writeLengthEncodedString(v);
    return MYSQL_TYPE_STRING;
}
template<>
inline unsigned int writeParameterValue<std::vector<char>>(ConectWriter& p, std::vector<char> const& v)
{
    p.writeLengthEncodedBlob(v);
    return MYSQL_TYPE_BLOB;
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
inline unsigned int writeParameterValue<signed long>(ConectWriter& p, signed long const& v)
{
    p.writeFixedLengthInteger<8>(v);
    return MYSQL_TYPE_LONGLONG;
}
template<>
inline unsigned int writeParameterValue<unsigned long>(ConectWriter& p, unsigned long const& v)
{
    p.writeFixedLengthInteger<8>(v);
    return MYSQL_TYPE_LONGLONG;
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
inline unsigned int writeParameterValue<signed char>(ConectWriter& p, signed char const& v)
{
    p.writeFixedLengthInteger<1>(v);
    return MYSQL_TYPE_TINY;
}
template<>
inline unsigned int writeParameterValue<unsigned char>(ConectWriter& p, unsigned char const& v)
{
    p.writeFixedLengthInteger<1>(v);
    return MYSQL_TYPE_TINY;
}
template<>
inline unsigned int writeParameterValue<char>(ConectWriter& p, char const& v)
{
    p.writeFixedLengthInteger<1>(v);
    return MYSQL_TYPE_TINY;
}
// TODO FIX
template<>
inline unsigned int writeParameterValue<long double>(ConectWriter& p, long double const& v)
{
    p.writeRawData(reinterpret_cast<char const*>(&v), 8);
    return MYSQL_TYPE_DOUBLE;
}
// TODO FIX
template<>
inline unsigned int writeParameterValue<double>(ConectWriter& p, double const& v)
{
    p.writeRawData(reinterpret_cast<char const*>(&v), 8);
    return MYSQL_TYPE_DOUBLE;
}
// TODO FIX
template<>
inline unsigned int writeParameterValue<float>(ConectWriter& p, float const& v)
{
    p.writeRawData(reinterpret_cast<char const*>(&v), 4);
    return MYSQL_TYPE_FLOAT;
}
// TODO FIX
template<>
inline unsigned int writeParameterValue<UnixTimeStamp>(ConectWriter& p, UnixTimeStamp const& v)
{
    p.writeRawData(reinterpret_cast<char const*>(&v), 4);
    return MYSQL_TYPE_TIMESTAMP;
}

/*
template<typename Enum>
inline Enum readParameterValue<MYSQL_TYPE_ENUM, Enum>(ConectReader& p)
{
    std::string         value(p.lengthEncodedString());
    std::stringstream   valueStream(value);
    Enum    result;
    valueStream >> enumFromString(result);
    return result;
}*/



/*
Don't know what C++ type is appropriate
#define MYSQL_TYPE_GEOMETRY                     0xff    lengthEncodedString() 
#define MYSQL_TYPE_SET                          0xf8    lengthEncodedString()
#define MYSQL_TYPE_BIT                          0x10    lengthEncodedString()
#define MYSQL_TYPE_YEAR                         0x0d    fixedLengthInteger<2>()
*/


/*
Unknown format on stream
#define MYSQL_TYPE_NULL                         0x06
#define MYSQL_TYPE_NEWDATE                      0x0e
#define MYSQL_TYPE_TIMESTAMP2                   0x11
#define MYSQL_TYPE_DATETIME2                    0x12
#define MYSQL_TYPE_TIME2                        0x13
*/

        }
    }
}

#endif

