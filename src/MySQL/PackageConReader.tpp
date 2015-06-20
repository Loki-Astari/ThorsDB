
#include "ThorMySQL.h"
#include "PackageStream.h"
#include <boost/lexical_cast.hpp>

namespace ThorsAnvil
{
    namespace MySQL
    {

#if     1
#define THOR_MYSQL_READ_INT(into, len)      stream.read(reinterpret_cast<char*>(&into), len)
#define THOR_MYSQL_WRITE_INT(from,len)      stream.sendBuffer.insert(sendBuffer.end(), reinterpret_cast<char*>(&from), reinterpret_cast<char*>(&from) + len)

#elif   BOOST_BIG_ENDIAN
#error  Not Tested
#elif   BOOST_PDP_ENDIAN
#error  Not Tested
#else
#error  Unknow Endianess
#endif

template<int len>
inline long PackageConReader::fixedLengthInteger()
{
    long    result = 0;
    THOR_MYSQL_READ_INT(result, len);

    return result;
}


template<int Tv, typename T>
inline T readParameterValue(PackageConReader&)
{
    // Default action is to throw.
    // The translations we know about are defined below.
    static_assert(true, "Undefined parameter read.");
}

using Buffer = std::vector<char>;

template<> inline std::string   readParameterValue<MYSQL_TYPE_VAR_STRING,   std::string>(PackageConReader& p)        {return p.lengthEncodedString();}
template<> inline std::string   readParameterValue<MYSQL_TYPE_STRING,       std::string>(PackageConReader& p)        {return p.lengthEncodedString();}
template<> inline std::string   readParameterValue<MYSQL_TYPE_VARCHAR,      std::string>(PackageConReader& p)        {return p.lengthEncodedString();}

template<> inline Buffer        readParameterValue<MYSQL_TYPE_TINY_BLOB,    std::vector<char>>(PackageConReader& p)  {return p.lengthEncodedBlob();}
template<> inline Buffer        readParameterValue<MYSQL_TYPE_MEDIUM_BLOB,  std::vector<char>>(PackageConReader& p)  {return p.lengthEncodedBlob();}
template<> inline Buffer        readParameterValue<MYSQL_TYPE_BLOB,         std::vector<char>>(PackageConReader& p)  {return p.lengthEncodedBlob();}
template<> inline Buffer        readParameterValue<MYSQL_TYPE_LONG_BLOB,    std::vector<char>>(PackageConReader& p)  {return p.lengthEncodedBlob();}

template<> inline long          readParameterValue<MYSQL_TYPE_DECIMAL,      long>(PackageConReader& p)               {return boost::lexical_cast<long>(p.lengthEncodedString());}
template<> inline long          readParameterValue<MYSQL_TYPE_NEWDECIMAL,   long>(PackageConReader& p)               {return boost::lexical_cast<long>(p.lengthEncodedString());}
template<> inline long          readParameterValue<MYSQL_TYPE_LONGLONG,     long>(PackageConReader& p)               {return p.fixedLengthInteger<8>();}
template<> inline long          readParameterValue<MYSQL_TYPE_LONG,         long>(PackageConReader& p)               {return p.fixedLengthInteger<4>();}
template<> inline long          readParameterValue<MYSQL_TYPE_INT24,        long>(PackageConReader& p)               {return p.fixedLengthInteger<4>();}
template<> inline long          readParameterValue<MYSQL_TYPE_SHORT,        long>(PackageConReader& p)               {return p.fixedLengthInteger<2>();}
template<> inline long          readParameterValue<MYSQL_TYPE_TINY,         long>(PackageConReader& p)               {return p.fixedLengthInteger<1>();}

template<> inline int           readParameterValue<MYSQL_TYPE_DECIMAL,      int>(PackageConReader& p)                {return boost::lexical_cast<int>(p.lengthEncodedString());}
template<> inline int           readParameterValue<MYSQL_TYPE_NEWDECIMAL,   int>(PackageConReader& p)                {return boost::lexical_cast<int>(p.lengthEncodedString());}
template<> inline int           readParameterValue<MYSQL_TYPE_LONG,         int>(PackageConReader& p)                {return p.fixedLengthInteger<4>();}
template<> inline int           readParameterValue<MYSQL_TYPE_INT24,        int>(PackageConReader& p)                {return p.fixedLengthInteger<4>();}
template<> inline int           readParameterValue<MYSQL_TYPE_SHORT,        int>(PackageConReader& p)                {return p.fixedLengthInteger<2>();}
template<> inline int           readParameterValue<MYSQL_TYPE_TINY,         int>(PackageConReader& p)                {return p.fixedLengthInteger<1>();}

template<> inline short         readParameterValue<MYSQL_TYPE_DECIMAL,      short>(PackageConReader& p)              {return boost::lexical_cast<short>(p.lengthEncodedString());}
template<> inline short         readParameterValue<MYSQL_TYPE_NEWDECIMAL,   short>(PackageConReader& p)              {return boost::lexical_cast<short>(p.lengthEncodedString());}
template<> inline short         readParameterValue<MYSQL_TYPE_SHORT,        short>(PackageConReader& p)              {return p.fixedLengthInteger<2>();}
template<> inline short         readParameterValue<MYSQL_TYPE_TINY,         short>(PackageConReader& p)              {return p.fixedLengthInteger<1>();}

template<> inline char          readParameterValue<MYSQL_TYPE_DECIMAL,      char>(PackageConReader& p)               {return boost::lexical_cast<char>(p.lengthEncodedString());}
template<> inline char          readParameterValue<MYSQL_TYPE_NEWDECIMAL,   char>(PackageConReader& p)               {return boost::lexical_cast<char>(p.lengthEncodedString());}
template<> inline char          readParameterValue<MYSQL_TYPE_TINY,         char>(PackageConReader& p)               {return p.fixedLengthInteger<1>();}

template<> inline double        readParameterValue<MYSQL_TYPE_DOUBLE,       double>(PackageConReader& p)             {double result;p.read(reinterpret_cast<char*>(&result), 8);return result;}
template<> inline float         readParameterValue<MYSQL_TYPE_DOUBLE,       float>(PackageConReader& p)              {double result;p.read(reinterpret_cast<char*>(&result), 8);return result;}
template<> inline float         readParameterValue<MYSQL_TYPE_FLOAT,        float>(PackageConReader& p)              {float  result;p.read(reinterpret_cast<char*>(&result), 4);return result;}

template<> inline time_t        readParameterValue<MYSQL_TYPE_DATE,         time_t>(PackageConReader& p)             {return p.readDate();}
template<> inline time_t        readParameterValue<MYSQL_TYPE_DATETIME,     time_t>(PackageConReader& p)             {return p.readDate();}
template<> inline time_t        readParameterValue<MYSQL_TYPE_TIMESTAMP,    time_t>(PackageConReader& p)             {return p.readDate();}

template<> inline long          readParameterValue<MYSQL_TYPE_TIME,         long>(PackageConReader& p)               {return p.readRel();}

/*
template<typename Enum>
inline Enum readParameterValue<MYSQL_TYPE_ENUM, Enum>(PackageConReader& p)
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
    }
}

