
#ifndef THORS_ANVIL_MYSQL_PACKAGE_READER_H
#define THORS_ANVIL_MYSQL_PACKAGE_READER_H


#include <memory>
#include <functional>
#include <vector>
#include <string>
#include <ctime>

#include "MySQLConfig.h"
#if     defined(THOR_ENDIAN_SML)
#define THOR_MYSQL_READ_INT(into, len)      stream.read(reinterpret_cast<char*>(&into), len)
#elif   defined(THOR_ENDIAN_BIG)
#error  Have not defined this for large endian systems.
#else
#error  Unknow Endianess
#endif

namespace ThorsAnvil
{
    namespace MySQL
    {

struct MySQLTimeBag
{
    enum TimeType {Absolute, RelativeNegative, RelativePositive};
 
    TimeType             type;
    unsigned long        year;
    unsigned long        month;
    unsigned long        day;
    unsigned long        hour;
    unsigned long        minute;
    unsigned long        second;
    unsigned long        uSecond;
    MySQLTimeBag()
        : type(Absolute), year(0), month(0), day(0), hour(0), minute(0), second(0), uSecond(0)
    {}
};

class Package
{
    public:
        virtual ~Package()  = 0;
};
class PackageStream;
class RespPackage;
class ConectReader
{
    PackageStream&   stream;
    unsigned long    capabilities;
    unsigned long    charset;

    unsigned long   lengthEncodedIntegerUsingSize(unsigned char size);
    public:
        using OKAction = std::function<std::unique_ptr<RespPackage>(ConectReader&)>;
        ConectReader(PackageStream& stream)
            : stream(stream)
            , capabilities(0)
            , charset(0)
        {}

        void initFromHandshake(unsigned long capabilities, unsigned long charset);
        std::unique_ptr<RespPackage>    getNextPackage(int expectedResult, OKAction expectedResultAction);

        void        read(char* data, std::size_t len);
        bool        isEmpty() const;

        unsigned long   getCapabilities()                                                   {return capabilities;}
        template<int len>
        unsigned long   fixedLengthInteger(unsigned long requiredCap)                       {return (requiredCap & capabilities) ? fixedLengthInteger<len>()    : 0;}
        unsigned long   lengthEncodedInteger(unsigned long requiredCap)                     {return (requiredCap & capabilities) ? lengthEncodedInteger()       : 0;}
        std::string     fixedLengthString(std::size_t size, unsigned long requiredCap)      {return (requiredCap & capabilities) ? fixedLengthString(size)      : "";}
        std::string     nulTerminatedString(unsigned long requiredCap)                      {return (requiredCap & capabilities) ? nulTerminatedString()        : "";}
        std::string     variableLengthString(std::size_t size, unsigned long requiredCap)   {return (requiredCap & capabilities) ? variableLengthString(size)   : "";}
        std::string     lengthEncodedString(unsigned long requiredCap)                      {return (requiredCap & capabilities) ? lengthEncodedString()        : "";}
        std::string     restOfPacketString(unsigned long requiredCap)                       {return (requiredCap & capabilities) ? restOfPacketString()         : "";}

        template<int len>
        unsigned long   fixedLengthInteger();
        unsigned long   lengthEncodedInteger();
        std::string     fixedLengthString(std::size_t size);
        std::string     nulTerminatedString();
        std::string     variableLengthString(std::size_t size);
        std::string     lengthEncodedString();
        std::string     restOfPacketString();

        std::vector<char> lengthEncodedBlob();
        std::time_t       readDate();
        std::time_t       readRel();
        unsigned long long readRelMicro();
        MySQLTimeBag      readDateIntoTimeBag();
        MySQLTimeBag      readTimeIntoTimeBag();

        void            reset();
};


    }
}

#ifndef COVERAGE_MySQL
#include "ConectReader.tpp"
#endif

#endif

