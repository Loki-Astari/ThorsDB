
#include "PackageConWriter.h"

using namespace ThorsAnvil::MySQL;

void PackageConWriter::writeLengthEncodedInteger(long value)
{
    /*
     * Length encoded integers.
     *  Val <  251              => 1bytes
     *  Val >= 251  < 2^16      => 0Xfc + 2byte Value
     *  Val >= 2^16 < 2^24      => 0Xfd + 3byte Value
     *  Val >= 2^24 < 2^64      => 0Xfe + 8byte Value
     */
    static char const Mark2Byte = 0xFC;
    static char const Mark3Byte = 0xFD;
    static char const Mark8Byte = 0xFE;

    if (value < 251)            {                                                               writeFixedLengthInteger<1>(value);}
    else if (value <= 0xFFFF)   {   stream.write(reinterpret_cast<char const*>(&Mark2Byte), 1); writeFixedLengthInteger<2>(value);}
    else if (value <= 0xFFFFFF) {   stream.write(reinterpret_cast<char const*>(&Mark3Byte), 1); writeFixedLengthInteger<3>(value);}
    else                        {   stream.write(reinterpret_cast<char const*>(&Mark8Byte), 1); writeFixedLengthInteger<8>(value);}
}

void PackageConWriter::writeFixedLengthString(std::string const& value, long size)
{
    std::string  output(value);
    output.resize(size);
    writeVariableLengthString(output);
}

void PackageConWriter::writeNullTerminatedString(std::string const& value)
{
    stream.write(value.c_str(), value.size() + 1);
}

void PackageConWriter::writeVariableLengthString(std::string const& value)
{
    stream.write(&value[0], value.size());
}

void PackageConWriter::writeLengthEncodedString(std::string const& value)
{
    writeLengthEncodedInteger(value.size());
    writeVariableLengthString(value);
}

