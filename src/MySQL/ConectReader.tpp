
#include "ThorMySQL.h"
#include "PackageStream.h"
#include "RespPackage.h"
#include <limits>

namespace ThorsAnvil
{
    namespace MySQL
    {

template<typename Resp>
std::unique_ptr<Resp> ConectReader::recvMessage(int expectedResult, OKAction expectedResultAction)
{
    std::unique_ptr<RespPackage>    resp = getNextPackage(expectedResult, expectedResultAction);
    if (resp->isError()) {
        throw std::runtime_error(resp->message());;
    }

    if (expectedResult != 0xFE && resp->isEOF()) {
        // EOF is special case.
        // If we are getting a set of results then the end of the set is marked
        // by an EOF package. This is not an error. We release it here and
        // make the resp empty. Anybody that is looking for an end of sequence
        // will need to validate that the pointer returned is not nullptr
        resp    = nullptr;
    }
    else {
        // Throws exception if cast fails.
        Resp&                   resultRef = dynamic_cast<Resp&>(*resp);
        [&resultRef](){}();
    }

    // Now we know the dynamic_cast will work and not return a nullptr.
    // release it do the dynamic cast and store it in a new unique_ptr
    // for return.
    std::unique_ptr<Resp>   result(dynamic_cast<Resp*>(resp.release()));
    return result;
}

template<int len>
inline unsigned long ConectReader::fixedLengthInteger()
{
    unsigned long result = 0;
    THOR_MYSQL_READ_INT(result, len);

    return result;
}

template<int len>
inline unsigned long ConectReader::fixedLengthInteger()
{
    unsigned long result = 0;
    THOR_MYSQL_READ_INT(result, len);

    return result;
}


template<int Tv, typename T>
inline T readParameterValue(ConectReader&)
{
    // Default action is to throw.
    // The translations we know about are defined below.
    static_assert(true, "Undefined parameter read.");
}

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

template<> inline std::string   readParameterValue<MYSQL_TYPE_VAR_STRING,   std::string>(ConectReader& p)        {return p.lengthEncodedString();}
template<> inline std::string   readParameterValue<MYSQL_TYPE_STRING,       std::string>(ConectReader& p)        {return p.lengthEncodedString();}
template<> inline std::string   readParameterValue<MYSQL_TYPE_VARCHAR,      std::string>(ConectReader& p)        {return p.lengthEncodedString();}

template<> inline Buffer        readParameterValue<MYSQL_TYPE_TINY_BLOB,    std::vector<char>>(ConectReader& p)  {return p.lengthEncodedBlob();}
template<> inline Buffer        readParameterValue<MYSQL_TYPE_MEDIUM_BLOB,  std::vector<char>>(ConectReader& p)  {return p.lengthEncodedBlob();}
template<> inline Buffer        readParameterValue<MYSQL_TYPE_BLOB,         std::vector<char>>(ConectReader& p)  {return p.lengthEncodedBlob();}
template<> inline Buffer        readParameterValue<MYSQL_TYPE_LONG_BLOB,    std::vector<char>>(ConectReader& p)  {return p.lengthEncodedBlob();}

template<> inline long          readParameterValue<MYSQL_TYPE_DECIMAL,      long>(ConectReader& p)               {return stringtointeger<long>(p.lengthEncodedString());}
template<> inline long          readParameterValue<MYSQL_TYPE_NEWDECIMAL,   long>(ConectReader& p)               {return stringtointeger<long>(p.lengthEncodedString());}
template<> inline long          readParameterValue<MYSQL_TYPE_LONGLONG,     long>(ConectReader& p)               {return p.fixedLengthInteger<8>();}
template<> inline long          readParameterValue<MYSQL_TYPE_LONG,         long>(ConectReader& p)               {return p.fixedLengthInteger<4>();}
template<> inline long          readParameterValue<MYSQL_TYPE_INT24,        long>(ConectReader& p)               {return p.fixedLengthInteger<4>();}
template<> inline long          readParameterValue<MYSQL_TYPE_SHORT,        long>(ConectReader& p)               {return p.fixedLengthInteger<2>();}
template<> inline long          readParameterValue<MYSQL_TYPE_TINY,         long>(ConectReader& p)               {return p.fixedLengthInteger<1>();}

template<> inline int           readParameterValue<MYSQL_TYPE_DECIMAL,      int>(ConectReader& p)                {return stringtointeger<int>(p.lengthEncodedString());}
template<> inline int           readParameterValue<MYSQL_TYPE_NEWDECIMAL,   int>(ConectReader& p)                {return stringtointeger<int>(p.lengthEncodedString());}
template<> inline int           readParameterValue<MYSQL_TYPE_LONG,         int>(ConectReader& p)                {return p.fixedLengthInteger<4>();}
template<> inline int           readParameterValue<MYSQL_TYPE_INT24,        int>(ConectReader& p)                {return p.fixedLengthInteger<4>();}
template<> inline int           readParameterValue<MYSQL_TYPE_SHORT,        int>(ConectReader& p)                {return p.fixedLengthInteger<2>();}
template<> inline int           readParameterValue<MYSQL_TYPE_TINY,         int>(ConectReader& p)                {return p.fixedLengthInteger<1>();}

template<> inline short         readParameterValue<MYSQL_TYPE_DECIMAL,      short>(ConectReader& p)              {return stringtointeger<short>(p.lengthEncodedString());}
template<> inline short         readParameterValue<MYSQL_TYPE_NEWDECIMAL,   short>(ConectReader& p)              {return stringtointeger<short>(p.lengthEncodedString());}
template<> inline short         readParameterValue<MYSQL_TYPE_SHORT,        short>(ConectReader& p)              {return p.fixedLengthInteger<2>();}
template<> inline short         readParameterValue<MYSQL_TYPE_TINY,         short>(ConectReader& p)              {return p.fixedLengthInteger<1>();}

template<> inline char          readParameterValue<MYSQL_TYPE_DECIMAL,      char>(ConectReader& p)               {return stringtointeger<char>(p.lengthEncodedString());}
template<> inline char          readParameterValue<MYSQL_TYPE_NEWDECIMAL,   char>(ConectReader& p)               {return stringtointeger<char>(p.lengthEncodedString());}
template<> inline char          readParameterValue<MYSQL_TYPE_TINY,         char>(ConectReader& p)               {return p.fixedLengthInteger<1>();}

template<> inline double        readParameterValue<MYSQL_TYPE_DOUBLE,       double>(ConectReader& p)             {double result;p.read(reinterpret_cast<char*>(&result), 8);return result;}
template<> inline float         readParameterValue<MYSQL_TYPE_DOUBLE,       float>(ConectReader& p)              {double result;p.read(reinterpret_cast<char*>(&result), 8);return result;}
template<> inline float         readParameterValue<MYSQL_TYPE_FLOAT,        float>(ConectReader& p)              {float  result;p.read(reinterpret_cast<char*>(&result), 4);return result;}

template<> inline time_t        readParameterValue<MYSQL_TYPE_DATE,         time_t>(ConectReader& p)             {return p.readDate();}
template<> inline time_t        readParameterValue<MYSQL_TYPE_DATETIME,     time_t>(ConectReader& p)             {return p.readDate();}
template<> inline time_t        readParameterValue<MYSQL_TYPE_TIMESTAMP,    time_t>(ConectReader& p)             {return p.readDate();}

template<> inline long          readParameterValue<MYSQL_TYPE_TIME,         long>(ConectReader& p)               {return p.readRel();}

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
    }
}

