
#include "ConectWriter.h"
#include "PackageStream.h"
#include <stdexcept>
#include <typeinfo>

using namespace ThorsAnvil::MySQL;

void ConectWriter::initFromHandshake(unsigned long newCapabilities, unsigned long newCharset)
{
    capabilities    = newCapabilities;
    charset         = newCharset;
}

void ConectWriter::writeLengthEncodedInteger(unsigned long long value)
{
    /*
     * Length encoded integers.
     *  Val <  251              => 1bytes
     *  Val >= 251  < 2^16      => 0Xfc + 2byte Value
     *  Val >= 2^16 < 2^24      => 0Xfd + 3byte Value
     *  Val >= 2^24 < 2^64      => 0Xfe + 8byte Value
     */
    static char const Mark2Byte = '\xFC';
    static char const Mark3Byte = '\xFD';
    static char const Mark8Byte = '\xFE';

    if (value < 251)            {                                                               writeFixedLengthInteger<1>(value);}
    else if (value <= 0xFFFF)   {   stream.write(reinterpret_cast<char const*>(&Mark2Byte), 1); writeFixedLengthInteger<2>(value);}
    else if (value <= 0xFFFFFF) {   stream.write(reinterpret_cast<char const*>(&Mark3Byte), 1); writeFixedLengthInteger<3>(value);}
    else                        {   stream.write(reinterpret_cast<char const*>(&Mark8Byte), 1); writeFixedLengthInteger<8>(value);}
}

void ConectWriter::writeFixedLengthString(std::string const& value, std::size_t size)
{
    std::string  output(value);
    output.resize(size);
    writeVariableLengthString(output);
}

void ConectWriter::writeNullTerminatedString(std::string const& value)
{
    stream.write(value.c_str(), value.size() + 1);
}

void ConectWriter::writeVariableLengthString(std::string const& value)
{
    stream.write(&value[0], value.size());
}

void ConectWriter::writeLengthEncodedString(std::string const& value)
{
    writeLengthEncodedInteger(value.size());
    writeVariableLengthString(value);
}

void ConectWriter::flush()
{
    stream.flush();
}
void ConectWriter::reset()
{
    stream.startNewConversation();
}

#ifdef COVERAGE_MySQL
/*
 * This code is only compiled into the unit tests for code coverage purposes
 * It is not part of the live code.
 */
#include "ConectWriter.tpp"

template void ThorsAnvil::MySQL::ConectWriter::writeFixedLengthInteger<1>(unsigned long long);
template void ThorsAnvil::MySQL::ConectWriter::writeFixedLengthInteger<2>(unsigned long long);
template void ThorsAnvil::MySQL::ConectWriter::writeFixedLengthInteger<3>(unsigned long long);
template void ThorsAnvil::MySQL::ConectWriter::writeFixedLengthInteger<4>(unsigned long long);
template void ThorsAnvil::MySQL::ConectWriter::writeFixedLengthInteger<8>(unsigned long long);

#endif

