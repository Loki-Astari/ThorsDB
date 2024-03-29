#include "ThorsDBCommon/StreamInterface.h"
#include "ConectReader.h"
#include "RespPackageOK.h"
#include "RespPackageEOF.h"
#include "RespPackageERR.h"
#include "ThorsIOUtil/Utility.h"
#include "ThorsLogging/ThorsLogging.h"

using namespace ThorsAnvil::DB::MySQL;

void ConectReader::initFromHandshake(unsigned long newCapabilities, unsigned long newCharset)
{
    capbil      = newCapabilities;
    charset     = newCharset;
}

void ConectReader::read(char* data, std::size_t len)
{
    stream.read(data, len);
}

bool ConectReader::isEmpty() const
{
    return stream.isEmpty();
}

std::unique_ptr<RespPackageEOF> ConectReader::recvMessageEOF()
{
    std::unique_ptr<RespPackage> result = recvMessage({{0xFE, [](int firstByte, ConectReader& reader){return new RespPackageEOF(firstByte, reader);}}});
    return downcastUniquePtr<RespPackageEOF>(std::move(result));
}

std::unique_ptr<RespPackage> ConectReader::recvMessage(OKMap const& actions /*= {}*/)
{
    int    packageType  = fixedLengthInteger<1>();;
    auto find = actions.find(packageType);
    if (find != actions.end())
    {
        return std::unique_ptr<RespPackage>(find->second(packageType, *this));
    }
    else if (packageType == 0x00)
    {
        return std::unique_ptr<RespPackage>(new RespPackageOK(packageType, *this));
    }
    else if (packageType == 0xFE)
    {
        // EOF default action: => read and ignore.
        RespPackageEOF  eofPackage(packageType, *this);
        return nullptr;
    }
    else if (packageType == 0xFF)
    {
        // Error default action: => read and throw
        RespPackageERR  errorPackage(packageType, *this);
        ThorsLogAndThrow("ThorsAnvil::DB::MySQL::ConectReader",
                         "recvMessage",
                         "Error Message from Server: ", errorPackage.message());
    }
    else
    {
        find = actions.find(-1);
        if (find != actions.end())
        {
            return std::unique_ptr<RespPackage>(find->second(packageType, *this));
        }
        ThorsLogAndThrowCritical("ThorsAnvil::DB::MySQL::ConectReader",
                                 "recvMessage",
                                 "Unknown Result Type: ", packageType, ": >", restOfPacketString(), "<");
    }
}

unsigned long long ConectReader::lengthEncodedInteger()
{
    unsigned char    type;
    read(reinterpret_cast<char*>(&type), 1);
    return lengthEncodedIntegerUsingSize(type);
}

unsigned long long ConectReader::lengthEncodedIntegerUsingSize(unsigned char type)
{
    unsigned long result;
    switch (type)
    {
        case 0xFA:
        case 0xFB:
        case 0xFF:
            ThorsLogAndThrowCritical("ThorsAnvil::DB::MySQL::ConectReader",
                                     "lengthEncodedIntegerUsingSize",
                                     "Invalid length encoding: ", type);
        case 0xFC:  result  = fixedLengthInteger<2>(); break;
        case 0xFD:  result  = fixedLengthInteger<3>(); break;
        case 0xFE:  result  = fixedLengthInteger<8>(); break;
        default:    result  = type;
    }
    return result;
}

std::string ConectReader::fixedLengthString(std::size_t size)
{
    std::string result(size, ' ');
    read(&result[0], size);
    return result;
}

std::string ConectReader::nulTerminatedString()
{
    std::string result;
    char x;

    for (read(&x, 1); x != '\0'; read(&x, 1))
    {
        result.append(1, x);
    }
    return result;
}

std::string ConectReader::variableLengthString(std::size_t size)
{
    return fixedLengthString(size);
}

std::string ConectReader::lengthEncodedString()
{
    unsigned long size = lengthEncodedInteger();
    std::string result(size, '\0');
    read(&result[0], size);
    return result;
}

std::string ConectReader::restOfPacketString()
{
    return stream.readRemainingData();
}

std::vector<char> ConectReader::lengthEncodedBlob()
{
    long size = lengthEncodedInteger();
    std::vector<char> result(size, '\0');
    read(&result[0], size);
    return result;
}
std::time_t ConectReader::readDate()
{
    MySQLTimeBag    timeBag = readDateIntoTimeBag();
    tm              time;

    time.tm_sec     = timeBag.second;
    time.tm_min     = timeBag.minute;
    time.tm_hour    = timeBag.hour;
    time.tm_mday    = timeBag.day;
    time.tm_mon     = timeBag.month - 1;
    time.tm_year    = timeBag.year - 1900;
    time.tm_isdst   = false;
#ifdef __WINNT__
    return _mkgmtime(&time);
#else
    time.tm_zone    = NULL;
    time.tm_gmtoff  = 0;

    return timegm(&time);
#endif

}

MySQLTimeBag ConectReader::readDateIntoTimeBag()
{
    MySQLTimeBag    timeBag;
    long    size    = fixedLengthInteger<1>();
    if (size != 11 && size != 7 && size != 4 && size != 0)
    {
        ThorsLogAndThrowCritical("ThorsAnvil::DB::MySQL::ConectReader",
                                 "readDateIntoTimeBag",
                                 "Invalid Date Size", size, "\nExpecting: 11/7/4/0");
    }
    if (size == 11 || size == 7 || size == 4)
    {
        timeBag.year    = fixedLengthInteger<2>();
        timeBag.month   = fixedLengthInteger<1>();
        timeBag.day     = fixedLengthInteger<1>();
    }
    if (size == 11 || size == 7)
    {
        timeBag.hour    = fixedLengthInteger<1>();
        timeBag.minute  = fixedLengthInteger<1>();
        timeBag.second  = fixedLengthInteger<1>();
    }
    if (size == 11)
    {
        timeBag.uSecond = fixedLengthInteger<4>();
    }
    return timeBag;
}

std::time_t ConectReader::readRel()
{
    MySQLTimeBag    timeBag = readTimeIntoTimeBag();
    return timeBag.day * (60LL*60*24) + timeBag.hour * (60LL*60) + timeBag.minute * (60LL) + timeBag.second;
}
unsigned long long ConectReader::readRelMicro()
{
    MySQLTimeBag    timeBag = readTimeIntoTimeBag();
    return timeBag.day * (1000LL*60*60*24)
            + timeBag.hour * (1000LL*60*60)
            + timeBag.minute * (1000LL*60)
            + timeBag.second * (1000LL)
            + timeBag.uSecond;
}
MySQLTimeBag ConectReader::readTimeIntoTimeBag()
{
    MySQLTimeBag    timeBag;
    long    size    = fixedLengthInteger<1>();
    if (size != 12 && size != 8 && size != 0)
    {
        ThorsLogAndThrowCritical("ThorsAnvil::DB::MySQL::ConectReader",
                                 "readTimeIntoTimeBag",
                                 "Invalid Time Size: ", size, "\nExpecting 12/8/0");
    }
    timeBag.type    = MySQLTimeBag::RelativePositive;
    if (size == 12 || size == 8)
    {
        long    negativeTest    = fixedLengthInteger<1>();
        if (negativeTest < 0 || negativeTest > 1)
        {
            ThorsLogAndThrowCritical("ThorsAnvil::DB::MySQL::ConectReader",
                                     "readTimeIntoTimeBag",
                                     "Invalid Negative Test");
        }
        if (negativeTest == 1)
        {
            timeBag.type    = MySQLTimeBag::RelativeNegative;
        }
        timeBag.day     = fixedLengthInteger<4>();
        timeBag.hour    = fixedLengthInteger<1>();
        timeBag.minute  = fixedLengthInteger<1>();
        timeBag.second  = fixedLengthInteger<1>();
    }
    if (size == 12)
    {
        timeBag.uSecond = fixedLengthInteger<4>();
    }
    return timeBag;
}

void ConectReader::drop()
{
    stream.drop();
}

void ConectReader::reset()
{
    stream.reset();
}
