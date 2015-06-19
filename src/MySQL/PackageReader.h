
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
class PackageReader
{
    PackageStream&   buffer;
    public:
        PackageReader(PackageStream& buffer)
            : buffer(buffer)
        {}
        std::unique_ptr<Package>    getNextPackage();

        void        readData(char* data, std::size_t len);

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
#include "PackageReader.tpp"
#endif

#endif

