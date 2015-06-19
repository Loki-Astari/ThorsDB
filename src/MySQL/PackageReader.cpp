
#include "PackageStream.h"
#include "PackageReader.h"
#include "PackageRespOK.h"
#include "PackageRespERR.h"
#include "PackageRespEOF.h"
#include "PackageRespHandShake.h"

using namespace ThorsAnvil::MySQL;

void PackageReader::read(char* data, std::size_t len)
{
    stream.read(data, len);
}

bool PackageReader::isEmpty() const
{
    return stream.isEmpty();
}

std::unique_ptr<PackageResp> PackageReader::getNextPackage(ResponceType type)
{
    unsigned char    packageType;
    read(reinterpret_cast<char*>(&packageType), 1);
    switch(packageType)
    {
        case 0x00:
        {
            switch(type)
            {
                case HandshakeOK:   return std::unique_ptr<PackageResp>(new Detail::PackageRespOK(*this));
                default:
                    throw std::runtime_error("PackageReader::getNextPackage: Unknown OK Package");
            }
        }
        case 0x0A:  return std::unique_ptr<PackageResp>(new Detail::PackageRespHandShake(*this));
        case 0xFF:  return std::unique_ptr<PackageResp>(new Detail::PackageRespERR(*this));
        case 0xFE:  return std::unique_ptr<PackageResp>(new Detail::PackageRespEOF(*this));
        default:
        {
            throw std::runtime_error(std::string("PackageReader::getNextPackage: Unknown Result Type: ") + std::to_string(packageType));
        }
    }
}

long PackageReader::lengthEncodedInteger()
{
    unsigned char    type;
    read(reinterpret_cast<char*>(&type), 1);
    return lengthEncodedIntegerUsingSize(type);
}

long PackageReader::lengthEncodedIntegerUsingSize(unsigned char type)
{
    long result;
    switch(type)
    {
        case 0xFA:
        case 0xFB:
        case 0xFF:  throw std::runtime_error(std::string("PackageRead::lengthEncodedInteger: Invalid length encoding: ") + std::to_string(type));
        case 0xFC:  result  = fixedLengthInteger<2>(); break;
        case 0xFD:  result  = fixedLengthInteger<3>(); break;
        case 0xFE:  result  = fixedLengthInteger<8>(); break;
        default:    result  = type;
    }
    return result;
}

std::string PackageReader::fixedLengthString(long size)
{
    std::string result(size, ' ');
    read(&result[0], size);
    return result;
}

std::string PackageReader::nulTerminatedString()
{
    std::string result;
    char x;

    for(read(&x, 1); x != '\0'; read(&x, 1))
    {
        result.append(1, x);
    }
    return result;
}

std::string PackageReader::variableLengthString(long size)
{
    return fixedLengthString(size);
}

std::string PackageReader::lengthEncodedString()
{
    long size = lengthEncodedInteger();
    std::string result(size, '\0');
    read(&result[0], size);
    return result;
}

std::string PackageReader::restOfPacketString()
{
    return stream.readRemainingData();
}

#ifdef COVERAGE_TEST
/*
 * This code is only compiled into the unit tests for code coverage purposes
 * It is not part of the live code.
 */
#include "PackageReader.tpp"

template long ThorsAnvil::MySQL::PackageReader::fixedLengthInteger<1>();
template long ThorsAnvil::MySQL::PackageReader::fixedLengthInteger<2>();
template long ThorsAnvil::MySQL::PackageReader::fixedLengthInteger<3>();
template long ThorsAnvil::MySQL::PackageReader::fixedLengthInteger<4>();

#endif

