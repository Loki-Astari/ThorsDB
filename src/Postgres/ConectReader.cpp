#include "ConectReader.h"

#ifdef COVERAGE_Postgres
/*
 * This code is only compiled into the unit tests for code coverage purposes
 * It is not part of the live code.
 */
#include "ConectReader.tpp"
#endif

using namespace ThorsAnvil::Postgres;

std::string ConectReader::readString()
{
    // TODO make PackageBuffer more efficient for this operation.
    std::string result;
    for (char nextChar = readChar(); nextChar; nextChar = readChar())
    {
        result.append(1, nextChar);
    }
    return result;
}

template std::int16_t ConectReader::readInt<std::int16_t>();
template std::int32_t ConectReader::readInt<std::int32_t>();
template std::int64_t ConectReader::readInt<std::int64_t>();
template char         ConectReader::readInt<char>();

template std::vector<char>         ConectReader::readIntVector<char>(int len);
template std::vector<std::int16_t> ConectReader::readIntVector<std::int16_t>(int len);
template std::vector<std::int32_t> ConectReader::readIntVector<std::int32_t>(int len);
template std::vector<std::int64_t> ConectReader::readIntVector<std::int64_t>(int len);
