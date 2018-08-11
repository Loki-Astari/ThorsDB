#include "ConectWriter.h"

#ifdef COVERAGE_Postgres
/*
 * This code is only compiled into the unit tests for code coverage purposes
 * It is not part of the live code.
 */
#include "ConectWriter.tpp"
#endif

using namespace ThorsAnvil::Postgres;

void ConectWriter::writeString(std::string const& data)
{
    stream.write(&data[0], data.size() + 1);
}

template void ConectWriter::writeInt(char);
template void ConectWriter::writeInt(std::int16_t);
template void ConectWriter::writeInt(std::int32_t);
template void ConectWriter::writeInt(std::int64_t);

template void ConectWriter::writeIntVector(std::vector<char> const&);
template void ConectWriter::writeIntVector(std::vector<std::int16_t> const&);
template void ConectWriter::writeIntVector(std::vector<std::int32_t> const&);
template void ConectWriter::writeIntVector(std::vector<std::int64_t> const&);
