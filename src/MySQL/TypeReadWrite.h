
#ifndef THORSANIVL_MYSQL_TYPE_READ_WRITE_H
#define THORSANIVL_MYSQL_TYPE_READ_WRITE_H

//#include "RespPackage.h"
//#include "RespPackageColumnDefinition.h"
#include "ThorSQL/Statement.h"
#include "ConectReader.h"
#include "ConectWriter.h"
//#include <vector>
//#include <limits>
//#include "ThorMySQL.h"

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
    throw std::runtime_error("ThorsAnvil::MySQL::stringtointeger: Failed to convert whole integer");
    }
    return result;
}

template<int Tv, typename T>
inline T readParameterValue(ConectReader&)
{
    // Default action is to throw.
    // The translations we know about are defined below.
    throw std::runtime_error("ThorsAnvil::MySQL::readParameterValue: Unknown conversion");
}
template<int Dst, typename Src>
void writeParameterValue(ConectWriter&, Src const&)
{
    // Default action is to throw.
    // The translations we know about are defined below.
    throw std::runtime_error("ThorsAnvil::MySQL::writeParameterValue: Unknown conversion");
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

// TO FIX
template<> inline double            readParameterValue<MYSQL_TYPE_DOUBLE,       double>(ConectReader& p)             {double result;p.read(reinterpret_cast<char*>(&result), 8);return result;}
template<> inline float             readParameterValue<MYSQL_TYPE_DOUBLE,       float>(ConectReader& p)              {double result;p.read(reinterpret_cast<char*>(&result), 8);return result;}
template<> inline float             readParameterValue<MYSQL_TYPE_FLOAT,        float>(ConectReader& p)              {float  result;p.read(reinterpret_cast<char*>(&result), 4);return result;}

template<> inline UnixTimeStamp     readParameterValue<MYSQL_TYPE_DATE,         UnixTimeStamp>(ConectReader& p)      {return UnixTimeStamp(p.readDate());}
template<> inline UnixTimeStamp     readParameterValue<MYSQL_TYPE_DATETIME,     UnixTimeStamp>(ConectReader& p)      {return UnixTimeStamp(p.readDate());}
template<> inline UnixTimeStamp     readParameterValue<MYSQL_TYPE_TIMESTAMP,    UnixTimeStamp>(ConectReader& p)      {return UnixTimeStamp(p.readDate());}
template<> inline UnixTimeStamp     readParameterValue<MYSQL_TYPE_TIME,         UnixTimeStamp>(ConectReader& p)      {return UnixTimeStamp(p.readRel());}

template<> inline void writeParameterValue<MYSQL_TYPE_VAR_STRING,   std::string>(ConectWriter& p, std::string const& v)            {p.writeLengthEncodedString(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_STRING,       std::string>(ConectWriter& p, std::string const& v)            {p.writeLengthEncodedString(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_VARCHAR,      std::string>(ConectWriter& p, std::string const& v)            {p.writeLengthEncodedString(v);}

template<> inline void writeParameterValue<MYSQL_TYPE_TINY_BLOB,    std::vector<char>>(ConectWriter& p, std::vector<char> const& v){p.writeLengthEncodedBlob(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_MEDIUM_BLOB,  std::vector<char>>(ConectWriter& p, std::vector<char> const& v){p.writeLengthEncodedBlob(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_BLOB,         std::vector<char>>(ConectWriter& p, std::vector<char> const& v){p.writeLengthEncodedBlob(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_LONG_BLOB,    std::vector<char>>(ConectWriter& p, std::vector<char> const& v){p.writeLengthEncodedBlob(v);}

template<> inline void writeParameterValue<MYSQL_TYPE_DECIMAL,      signed long>(ConectWriter& p, signed long const& v)            {p.writeLengthEncodedString(std::to_string(v));}
template<> inline void writeParameterValue<MYSQL_TYPE_NEWDECIMAL,   signed long>(ConectWriter& p, signed long const& v)            {p.writeLengthEncodedString(std::to_string(v));}
template<> inline void writeParameterValue<MYSQL_TYPE_LONGLONG,     signed long>(ConectWriter& p, signed long const& v)            {p.writeFixedLengthInteger<8>(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_LONG,         signed long>(ConectWriter& p, signed long const& v)            {p.writeFixedLengthInteger<4>(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_INT24,        signed long>(ConectWriter& p, signed long const& v)            {p.writeFixedLengthInteger<4>(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_SHORT,        signed long>(ConectWriter& p, signed long const& v)            {p.writeFixedLengthInteger<2>(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_TINY,         signed long>(ConectWriter& p, signed long const& v)            {p.writeFixedLengthInteger<1>(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_DECIMAL,      unsigned long>(ConectWriter& p, unsigned long const& v)        {p.writeLengthEncodedString(std::to_string(v));}
template<> inline void writeParameterValue<MYSQL_TYPE_NEWDECIMAL,   unsigned long>(ConectWriter& p, unsigned long const& v)        {p.writeLengthEncodedString(std::to_string(v));}
template<> inline void writeParameterValue<MYSQL_TYPE_LONGLONG,     unsigned long>(ConectWriter& p, unsigned long const& v)        {p.writeFixedLengthInteger<8>(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_LONG,         unsigned long>(ConectWriter& p, unsigned long const& v)        {p.writeFixedLengthInteger<4>(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_INT24,        unsigned long>(ConectWriter& p, unsigned long const& v)        {p.writeFixedLengthInteger<4>(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_SHORT,        unsigned long>(ConectWriter& p, unsigned long const& v)        {p.writeFixedLengthInteger<2>(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_TINY,         unsigned long>(ConectWriter& p, unsigned long const& v)        {p.writeFixedLengthInteger<1>(v);}

template<> inline void writeParameterValue<MYSQL_TYPE_DECIMAL,      signed int>(ConectWriter& p, signed int const& v)              {p.writeLengthEncodedString(std::to_string(v));}
template<> inline void writeParameterValue<MYSQL_TYPE_NEWDECIMAL,   signed int>(ConectWriter& p, signed int const& v)              {p.writeLengthEncodedString(std::to_string(v));}
template<> inline void writeParameterValue<MYSQL_TYPE_LONG,         signed int>(ConectWriter& p, signed int const& v)              {p.writeFixedLengthInteger<4>(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_INT24,        signed int>(ConectWriter& p, signed int const& v)              {p.writeFixedLengthInteger<4>(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_SHORT,        signed int>(ConectWriter& p, signed int const& v)              {p.writeFixedLengthInteger<2>(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_TINY,         signed int>(ConectWriter& p, signed int const& v)              {p.writeFixedLengthInteger<1>(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_DECIMAL,      unsigned int>(ConectWriter& p, unsigned int const& v)          {p.writeLengthEncodedString(std::to_string(v));}
template<> inline void writeParameterValue<MYSQL_TYPE_NEWDECIMAL,   unsigned int>(ConectWriter& p, unsigned int const& v)          {p.writeLengthEncodedString(std::to_string(v));}
template<> inline void writeParameterValue<MYSQL_TYPE_LONG,         unsigned int>(ConectWriter& p, unsigned int const& v)          {p.writeFixedLengthInteger<4>(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_INT24,        unsigned int>(ConectWriter& p, unsigned int const& v)          {p.writeFixedLengthInteger<4>(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_SHORT,        unsigned int>(ConectWriter& p, unsigned int const& v)          {p.writeFixedLengthInteger<2>(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_TINY,         unsigned int>(ConectWriter& p, unsigned int const& v)          {p.writeFixedLengthInteger<1>(v);}

template<> inline void writeParameterValue<MYSQL_TYPE_DECIMAL,      signed short>(ConectWriter& p, signed short const& v)          {p.writeLengthEncodedString(std::to_string(v));}
template<> inline void writeParameterValue<MYSQL_TYPE_NEWDECIMAL,   signed short>(ConectWriter& p, signed short const& v)          {p.writeLengthEncodedString(std::to_string(v));}
template<> inline void writeParameterValue<MYSQL_TYPE_SHORT,        signed short>(ConectWriter& p, signed short const& v)          {p.writeFixedLengthInteger<2>(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_TINY,         signed short>(ConectWriter& p, signed short const& v)          {p.writeFixedLengthInteger<1>(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_DECIMAL,      unsigned short>(ConectWriter& p, unsigned short const& v)      {p.writeLengthEncodedString(std::to_string(v));}
template<> inline void writeParameterValue<MYSQL_TYPE_NEWDECIMAL,   unsigned short>(ConectWriter& p, unsigned short const& v)      {p.writeLengthEncodedString(std::to_string(v));}
template<> inline void writeParameterValue<MYSQL_TYPE_SHORT,        unsigned short>(ConectWriter& p, unsigned short const& v)      {p.writeFixedLengthInteger<2>(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_TINY,         unsigned short>(ConectWriter& p, unsigned short const& v)      {p.writeFixedLengthInteger<1>(v);}

template<> inline void writeParameterValue<MYSQL_TYPE_DECIMAL,      char>(ConectWriter& p, char const& v)                          {p.writeLengthEncodedString(std::to_string(v));}
template<> inline void writeParameterValue<MYSQL_TYPE_NEWDECIMAL,   char>(ConectWriter& p, char const& v)                          {p.writeLengthEncodedString(std::to_string(v));}
template<> inline void writeParameterValue<MYSQL_TYPE_LONGLONG,     char>(ConectWriter& p, char const& v)                          {p.writeFixedLengthInteger<8>(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_TINY,         char>(ConectWriter& p, char const& v)                          {p.writeFixedLengthInteger<1>(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_DECIMAL,      signed char>(ConectWriter& p, signed char const& v)            {p.writeLengthEncodedString(std::to_string(v));}
template<> inline void writeParameterValue<MYSQL_TYPE_NEWDECIMAL,   signed char>(ConectWriter& p, signed char const& v)            {p.writeLengthEncodedString(std::to_string(v));}
template<> inline void writeParameterValue<MYSQL_TYPE_LONGLONG,     signed char>(ConectWriter& p, signed char const& v)            {p.writeFixedLengthInteger<8>(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_TINY,         signed char>(ConectWriter& p, signed char const& v)            {p.writeFixedLengthInteger<1>(v);}
template<> inline void writeParameterValue<MYSQL_TYPE_DECIMAL,      unsigned char>(ConectWriter& p, unsigned char const& v)        {p.writeLengthEncodedString(std::to_string(v));}
template<> inline void writeParameterValue<MYSQL_TYPE_NEWDECIMAL,   unsigned char>(ConectWriter& p, unsigned char const& v)        {p.writeLengthEncodedString(std::to_string(v));}
template<> inline void writeParameterValue<MYSQL_TYPE_TINY,         unsigned char>(ConectWriter& p, unsigned char const& v)        {p.writeFixedLengthInteger<1>(v);}

template<> inline void writeParameterValue<MYSQL_TYPE_DOUBLE,       double>(ConectWriter& p, double const& v)                      {p.writeRawData(reinterpret_cast<char const*>(&v), 8);}
template<> inline void writeParameterValue<MYSQL_TYPE_DOUBLE,       float>(ConectWriter& p, float const& v)                        {p.writeRawData(reinterpret_cast<char const*>(&v), 8);}
template<> inline void writeParameterValue<MYSQL_TYPE_FLOAT,        float>(ConectWriter& p, float const& v)                        {p.writeRawData(reinterpret_cast<char const*>(&v), 4);}

template<> inline void writeParameterValue<MYSQL_TYPE_DATE,         UnixTimeStamp>(ConectWriter& p, UnixTimeStamp const& v)        {p.writeRawData(reinterpret_cast<char const*>(&v), 4);}
template<> inline void writeParameterValue<MYSQL_TYPE_DATETIME,     UnixTimeStamp>(ConectWriter& p, UnixTimeStamp const& v)        {p.writeRawData(reinterpret_cast<char const*>(&v), 4);}
template<> inline void writeParameterValue<MYSQL_TYPE_TIMESTAMP,    UnixTimeStamp>(ConectWriter& p, UnixTimeStamp const& v)        {p.writeRawData(reinterpret_cast<char const*>(&v), 4);}
template<> inline void writeParameterValue<MYSQL_TYPE_TIME,         UnixTimeStamp>(ConectWriter& p, UnixTimeStamp const& v)        {p.writeRawData(reinterpret_cast<char const*>(&v), 4);}

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

