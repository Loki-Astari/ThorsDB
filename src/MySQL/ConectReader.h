#ifndef THORS_ANVIL_MYSQL_PACKAGE_READER_H
#define THORS_ANVIL_MYSQL_PACKAGE_READER_H

#include "MySQLTimeBag.h"
#include "ThorSQL/SQLUtil.h"
#include <memory>
#include <functional>
#include <vector>
#include <map>
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

namespace ThorsAnvil::DB::SQL
{
    class StreamInterface;
}

namespace ThorsAnvil::DB::MySQL
{

class RespPackage;
class RespPackageEOF;

class ConectReader
{
    SQL::StreamInterface&   stream;
    unsigned long           capbil;
    unsigned long           charset;

    unsigned long long lengthEncodedIntegerUsingSize(unsigned char size);
    public:
        using OKAction = std::function<RespPackage*(int byte, ConectReader&)>;
        using OKMap    = std::map<int, OKAction>;
        ConectReader(SQL::StreamInterface& stream)
            : stream(stream)
            , capbil(0)
            , charset(0)
        {}

        void initFromHandshake(unsigned long capabilities, unsigned long charset);
        std::unique_ptr<RespPackageEOF> recvMessageEOF();
        std::unique_ptr<RespPackage>    recvMessage(OKMap const& actions = {});
    public:


        void        read(char* data, std::size_t len);
        bool        isEmpty() const;

        unsigned long   getCapabilities()                                       {return capbil;}
        template<int len>
        unsigned long long fixedLengthInteger(unsigned long rCap)               {return (rCap & capbil) ? fixedLengthInteger<len>() : 0;}
        unsigned long long lengthEncodedInteger(unsigned long rCap)             {return (rCap & capbil) ? lengthEncodedInteger()    : 0;}
        std::string     fixedLengthString(std::size_t s, unsigned long rCap)    {return (rCap & capbil) ? fixedLengthString(s)      : "";}
        std::string     nulTerminatedString(unsigned long rCap)                 {return (rCap & capbil) ? nulTerminatedString()     : "";}
        std::string     variableLengthString(std::size_t s, unsigned long rCap) {return (rCap & capbil) ? variableLengthString(s)   : "";}
        std::string     lengthEncodedString(unsigned long rCap)                 {return (rCap & capbil) ? lengthEncodedString()     : "";}
        std::string     restOfPacketString(unsigned long rCap)                  {return (rCap & capbil) ? restOfPacketString()      : "";}

        template<int len>
        unsigned long long fixedLengthInteger();
        unsigned long long lengthEncodedInteger();
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

        void            drop();
        void            reset();
};


}

#ifndef COVERAGE_MySQL
#include "ConectReader.tpp"
#endif

#endif
