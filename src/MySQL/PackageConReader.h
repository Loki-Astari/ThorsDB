
#ifndef THORS_ANVIL_MYSQL_PACKAGE_READER_H
#define THORS_ANVIL_MYSQL_PACKAGE_READER_H

#include <memory>
#include <vector>
#include <string>

namespace ThorsAnvil
{
    namespace MySQL
    {

struct MySQLTimeBag
{
    enum TimeType {Absolute, RelativeNegative, RelativePositive};
 
    TimeType    type;
    long        year;
    long        month;
    long        day;
    long        hour;
    long        minute;
    long        second;
    long        uSecond;
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
class PackageResp;
class PackageConReader
{
    PackageStream&   stream;
    long             capabilities;
    long             charset;
    public:
        PackageConReader(PackageStream& stream)
            : stream(stream)
            , capabilities(0)
            , charset(0)
        {}
        enum ResponceType       { HandshakeOK };
        enum PacketCompletion   { OK, EOF_OK };

        void initFromHandshake(long capabilities, long charset);
        std::unique_ptr<PackageResp>    getNextPackage(ResponceType type);

        void        read(char* data, std::size_t len);
        bool        isEmpty() const;

        template<int len>
        long        fixedLengthInteger(long requiredCap)                {return (requiredCap & capabilities) ? fixedLengthInteger<len>()    : 0;}
        long        lengthEncodedInteger(long requiredCap)              {return (requiredCap & capabilities) ? lengthEncodedInteger()       : 0;}
        std::string fixedLengthString(long size, long requiredCap)      {return (requiredCap & capabilities) ? fixedLengthString(size)      : "";}
        std::string nulTerminatedString(long requiredCap)               {return (requiredCap & capabilities) ? nulTerminatedString()        : "";}
        std::string variableLengthString(long size, long requiredCap)   {return (requiredCap & capabilities) ? variableLengthString(size)   : "";}
        std::string lengthEncodedString(long requiredCap)               {return (requiredCap & capabilities) ? lengthEncodedString()        : "";}
        std::string restOfPacketString(long requiredCap)                {return (requiredCap & capabilities) ? restOfPacketString()         : "";}

        template<int len>
        long        fixedLengthInteger();
        long        lengthEncodedInteger();
        long        lengthEncodedIntegerUsingSize(unsigned char size);
        std::string fixedLengthString(long size);
        std::string nulTerminatedString();
        std::string variableLengthString(long size);
        std::string lengthEncodedString();
        std::string restOfPacketString();

        std::vector<char> lengthEncodedBlob();
        time_t            readDate();
        long              readRel();
        long              readRelMicro();
        MySQLTimeBag      readDateIntoTimeBag();
        MySQLTimeBag      readTimeIntoTimeBag();
};


    }
}

#ifndef COVERAGE_TEST
#include "PackageConReader.tpp"
#endif

#endif

