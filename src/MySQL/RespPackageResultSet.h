
#ifndef THORSANIVL_MYSQL_DETAIL_RESP_PACKAGE_RESULT_SET_H
#define THORSANIVL_MYSQL_DETAIL_RESP_PACKAGE_RESULT_SET_H

#include "RespPackage.h"
#include "RespPackageColumnDefinition.h"
#include "ThorSQL/Statement.h"
#include <vector>
#include <limits>
#include "ThorMySQL.h"

namespace ThorsAnvil
{
    namespace MySQL
    {
        namespace Detail
        {


using Buffer = std::vector<char>;

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
    throw std::runtime_error("Failed to convert whole integer");
    }
    return result;
}

template<int Tv, typename T>
inline T readParameterValue(ConectReader&)
{
    // Default action is to throw.
    // The translations we know about are defined below.
    throw std::runtime_error("Unknown conversion");
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

using SQL::UnixTimeStamp;
template<> inline UnixTimeStamp     readParameterValue<MYSQL_TYPE_DATE,         UnixTimeStamp>(ConectReader& p)      {return UnixTimeStamp(p.readDate());}
template<> inline UnixTimeStamp     readParameterValue<MYSQL_TYPE_DATETIME,     UnixTimeStamp>(ConectReader& p)      {return UnixTimeStamp(p.readDate());}
template<> inline UnixTimeStamp     readParameterValue<MYSQL_TYPE_TIMESTAMP,    UnixTimeStamp>(ConectReader& p)      {return UnixTimeStamp(p.readDate());}
template<> inline UnixTimeStamp     readParameterValue<MYSQL_TYPE_TIME,         UnixTimeStamp>(ConectReader& p)      {return UnixTimeStamp(p.readRel());}

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


template<typename T> inline T readNullParameter()                                              {throw std::runtime_error(std::string("readNullParameter: Undefined for this type ") + typeid(T).name());}

template<> inline std::string readNullParameter<std::string>()                                 {return "";}
template<> inline int         readNullParameter<int>()                                         {return 0;}

class RespPackageResultSet: public RespPackage
{
    ConectReader&   reader;
    std::vector<RespPackageColumnDefinition> const& columns;
    std::string nullMap;
    int         nextColumn;

    template<typename T>
    T readNextValue()
    {
        int currentColumn   = nextColumn;
        ++nextColumn;
        switch(columns[currentColumn].type)
        {
            case MYSQL_TYPE_STRING:         return readParameterValue<MYSQL_TYPE_STRING, T>(reader);
            case MYSQL_TYPE_VARCHAR:        return readParameterValue<MYSQL_TYPE_VARCHAR, T>(reader);
            case MYSQL_TYPE_VAR_STRING:     return readParameterValue<MYSQL_TYPE_VAR_STRING, T>(reader);
            case MYSQL_TYPE_ENUM:           return readParameterValue<MYSQL_TYPE_ENUM, T>(reader);
            case MYSQL_TYPE_SET:            return readParameterValue<MYSQL_TYPE_SET, T>(reader);
            case MYSQL_TYPE_LONG_BLOB:      return readParameterValue<MYSQL_TYPE_LONG_BLOB, T>(reader);
            case MYSQL_TYPE_MEDIUM_BLOB:    return readParameterValue<MYSQL_TYPE_MEDIUM_BLOB, T>(reader);
            case MYSQL_TYPE_BLOB:           return readParameterValue<MYSQL_TYPE_BLOB, T>(reader);
            case MYSQL_TYPE_TINY_BLOB:      return readParameterValue<MYSQL_TYPE_TINY_BLOB, T>(reader);
            case MYSQL_TYPE_GEOMETRY:       return readParameterValue<MYSQL_TYPE_GEOMETRY, T>(reader);
            case MYSQL_TYPE_BIT:            return readParameterValue<MYSQL_TYPE_BIT, T>(reader);
            case MYSQL_TYPE_DECIMAL:        return readParameterValue<MYSQL_TYPE_DECIMAL, T>(reader);
            case MYSQL_TYPE_NEWDECIMAL:     return readParameterValue<MYSQL_TYPE_NEWDECIMAL, T>(reader);
            case MYSQL_TYPE_LONGLONG:       return readParameterValue<MYSQL_TYPE_LONGLONG, T>(reader);
            case MYSQL_TYPE_LONG:           return readParameterValue<MYSQL_TYPE_LONG, T>(reader);
            case MYSQL_TYPE_INT24:          return readParameterValue<MYSQL_TYPE_INT24, T>(reader);
            case MYSQL_TYPE_SHORT:          return readParameterValue<MYSQL_TYPE_SHORT, T>(reader);
            case MYSQL_TYPE_YEAR:           return readParameterValue<MYSQL_TYPE_YEAR, T>(reader);
            case MYSQL_TYPE_TINY:           return readParameterValue<MYSQL_TYPE_TINY, T>(reader);
            case MYSQL_TYPE_DOUBLE:         return readParameterValue<MYSQL_TYPE_DOUBLE, T>(reader);
            case MYSQL_TYPE_FLOAT:          return readParameterValue<MYSQL_TYPE_FLOAT, T>(reader);
            case MYSQL_TYPE_DATE:           return readParameterValue<MYSQL_TYPE_DATE, T>(reader);
            case MYSQL_TYPE_DATETIME:       return readParameterValue<MYSQL_TYPE_DATETIME, T>(reader);
            case MYSQL_TYPE_TIMESTAMP:      return readParameterValue<MYSQL_TYPE_TIMESTAMP, T>(reader);
            case MYSQL_TYPE_TIME:           return readParameterValue<MYSQL_TYPE_TIME, T>(reader);
            case MYSQL_TYPE_NEWDATE:        return readParameterValue<MYSQL_TYPE_NEWDATE, T>(reader);
            case MYSQL_TYPE_TIMESTAMP2:     return readParameterValue<MYSQL_TYPE_TIMESTAMP2, T>(reader);
            case MYSQL_TYPE_DATETIME2:      return readParameterValue<MYSQL_TYPE_DATETIME2, T>(reader);
            case MYSQL_TYPE_TIME2:          return readParameterValue<MYSQL_TYPE_TIME2, T>(reader);
            default:
                throw std::runtime_error("Unimplemented Column Type");
        }
    }

    template<typename T>
    T getValue(T const& defaultValue)
    {
        int     index   = nextColumn / 8;
        int     bit     = nextColumn % 8;
        if (nullMap[index] & (1 << bit)) {
            return defaultValue;
        }
        return readNextValue<T>();
    }
    public:
        RespPackageResultSet(int firstByte, ConectReader& reader, std::vector<RespPackageColumnDefinition> const& columns);
        virtual  std::ostream& print(std::ostream& s) const {return s;}

        void   retrieve(char& value)                        {value = getValue<char>('\0');}
        void   retrieve(signed char& value)                 {value = getValue<signed char>('\0');}
        void   retrieve(signed short& value)                {value = getValue<signed short>(0);}
        void   retrieve(signed int& value)                  {value = getValue<signed int>(0);}
        void   retrieve(signed long& value)                 {value = getValue<signed long>(0);}
        void   retrieve(signed long long& value)            {value = getValue<signed long long>(0);}
        void   retrieve(unsigned char& value)               {value = getValue<unsigned char>(0);}
        void   retrieve(unsigned short& value)              {value = getValue<unsigned short>(0);}
        void   retrieve(unsigned int& value)                {value = getValue<unsigned int>(0);}
        void   retrieve(unsigned long& value)               {value = getValue<unsigned long>(0);}
        void   retrieve(unsigned long long& value)          {value = getValue<unsigned long long>(0);}

        void   retrieve(float& value)                       {value = getValue<float>(0);}
        void   retrieve(double& value)                      {value = getValue<double>(0);}
        void   retrieve(long double& value)                 {value = getValue<long double>(0);}

        void   retrieve(std::string& value)                 {value = getValue<std::string>("");}

        void   retrieve(std::vector<char>& value)           {value = getValue<std::vector<char>>(std::vector<char>());};
        void   retrieve(SQL::UnixTimeStamp& value)          {value = getValue<SQL::UnixTimeStamp>(SQL::UnixTimeStamp(static_cast<std::time_t>(0)));}
};
        }
    }
}

#endif

