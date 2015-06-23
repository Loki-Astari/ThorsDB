
#include "PackageStream.h"
#include "ConectReader.h"
#include "RespPackageOK.h"
#include "RespPackageERR.h"
#include "RespPackageEOF.h"
#include "RespPackageHandShake.h"

using namespace ThorsAnvil::MySQL;

void ConectReader::initFromHandshake(long newCapabilities, long newCharset)
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

long ConectReader::lengthEncodedInteger()
{
    unsigned char    type;
    read(reinterpret_cast<char*>(&type), 1);
    return lengthEncodedIntegerUsingSize(type);
}

long ConectReader::lengthEncodedIntegerUsingSize(unsigned char type)
{
    long result;
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
    long size = lengthEncodedInteger();
    std::string result(size, '\0');
    read(&result[0], size);
    return result;
}

std::string ConectReader::restOfPacketString()
{
    return stream.readRemainingData();
}

#ifdef COVERAGE_TEST
/*
 * This code is only compiled into the unit tests for code coverage purposes
 * It is not part of the live code.
 */
#include "ConectReader.tpp"

template long ThorsAnvil::MySQL::ConectReader::fixedLengthInteger<1>();
template long ThorsAnvil::MySQL::ConectReader::fixedLengthInteger<2>();
template long ThorsAnvil::MySQL::ConectReader::fixedLengthInteger<3>();
template long ThorsAnvil::MySQL::ConectReader::fixedLengthInteger<4>();

#endif

