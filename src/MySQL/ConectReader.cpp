
#include "PackageStream.h"
#include "ConectReader.h"
#include "RespPackageOK.h"
#include "RespPackageERR.h"
#include "RespPackageEOF.h"
#include "RespPackageHandShake.h"
#include "PrepareStatement.h"

using namespace ThorsAnvil::MySQL;

void ConectReader::initFromHandshake(unsigned long newCapabilities, unsigned long newCharset)
{
    capabilities    = newCapabilities;
    charset         = newCharset;
}

void ConectReader::read(char* data, std::size_t len)
{
    stream.read(data, len);
}

bool ConectReader::isEmpty() const
{
    return stream.isEmpty();
}

std::unique_ptr<RespPackage> ConectReader::getNextPackage(ResponceType type)
{
    unsigned char    packageType;
    read(reinterpret_cast<char*>(&packageType), 1);
    switch(packageType)
    {
        case 0x00:
        {
            switch(type)
            {
                case HandshakeOK:   return std::unique_ptr<RespPackage>(new Detail::RespPackageOK(*this));
                case PrepareOK:     return std::unique_ptr<RespPackage>(new Detail::RespPackagePrepare(*this));
                case NotOK: // Fall through
                default:
                    throw std::runtime_error("ConectReader::getNextPackage: Unknown OK Package");
            }
        }
        case 0x0A:  return std::unique_ptr<RespPackage>(new Detail::RespPackageHandShake(*this));
        case 0xFF:  return std::unique_ptr<RespPackage>(new Detail::RespPackageERR(*this));
        case 0xFE:  return std::unique_ptr<RespPackage>(new Detail::RespPackageEOF(*this));
        default:
        {
            throw std::runtime_error(std::string("ConectReader::getNextPackage: Unknown Result Type: ") + std::to_string(packageType));
        }
    }
}

unsigned long ConectReader::lengthEncodedInteger()
{
    unsigned char    type;
    read(reinterpret_cast<char*>(&type), 1);
    return lengthEncodedIntegerUsingSize(type);
}

unsigned long ConectReader::lengthEncodedIntegerUsingSize(unsigned char type)
{
    unsigned long result;
    switch(type)
    {
        case 0xFA:
        case 0xFB:
        case 0xFF:  throw std::runtime_error(std::string("ConectReader::lengthEncodedInteger: Invalid length encoding: ") + std::to_string(type));
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

    for(read(&x, 1); x != '\0'; read(&x, 1))
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
    time.tm_zone    = NULL;
    time.tm_gmtoff  = 0;

    return timegm(&time);
}

MySQLTimeBag ConectReader::readDateIntoTimeBag()
{
    MySQLTimeBag    timeBag;
    long    size    = fixedLengthInteger<1>();
    if (size != 11 && size != 7 && size != 4 && size != 0)
    {   throw std::runtime_error("ConectReader::readDate: Invalid Date Size");
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
    return timeBag.day * (1000LL*60*60*24) + timeBag.hour * (1000LL*60*60) + timeBag.minute * (1000LL*60) + timeBag.second * (1000LL) + timeBag.uSecond;
}
MySQLTimeBag ConectReader::readTimeIntoTimeBag()
{
    MySQLTimeBag    timeBag;
    long    size    = fixedLengthInteger<1>();
    if (size != 12 && size != 8 && size != 0)
    {   throw std::runtime_error("ConectReader::readTime: Invalid Time Size");
    }
    timeBag.type    = MySQLTimeBag::RelativePositive;
    if (size == 12 || size == 8)
    {
        long    negativeTest    = fixedLengthInteger<1>();
        if (negativeTest < 0 || negativeTest > 1)
        {   throw std::runtime_error("ConectReader::readTime: Invalid Negative Test");
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

void ConectReader::reset()
{
    stream.reset();
}
#ifdef COVERAGE_TEST
/*
 * This code is only compiled into the unit tests for code coverage purposes
 * It is not part of the live code.
 */
#include "ConectReader.tpp"

template unsigned long ThorsAnvil::MySQL::ConectReader::fixedLengthInteger<1>();
template unsigned long ThorsAnvil::MySQL::ConectReader::fixedLengthInteger<2>();
template unsigned long ThorsAnvil::MySQL::ConectReader::fixedLengthInteger<3>();
template unsigned long ThorsAnvil::MySQL::ConectReader::fixedLengthInteger<4>();

#endif

