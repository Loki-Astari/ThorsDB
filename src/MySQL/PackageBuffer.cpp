#include "PackageBuffer.h"

using namespace ThorsAnvil::MySQL;

#ifdef COVERAGE_MySQL
/*
 * This code is only compiled into the unit tests for code coverage purposes
 * It is not part of the live code.
 */
#include "ThorSQL/StreamSimple.h"
#include "PackageBuffer.tpp"
#include "test/MockStream.h"


template
ThorsAnvil::MySQL::PackageBuffer<ThorsAnvil::SQL::StreamSimple>::PackageBuffer
(ThorsAnvil::SQL::StreamSimple&, bool);

template void PackageBuffer<ThorsAnvil::SQL::StreamSimple>::read(char*, std::size_t);

template std::string ThorsAnvil::MySQL::PackageBuffer<MockStream>::readRemainingData();
template void ThorsAnvil::MySQL::PackageBuffer<MockStream>::read(char*, std::size_t);
template void ThorsAnvil::MySQL::PackageBuffer<MockStream>::flush();
template void ThorsAnvil::MySQL::PackageBuffer<MockStream>::reset();
template void ThorsAnvil::MySQL::PackageBuffer<MockStream>::write(char const*, std::size_t);
template bool ThorsAnvil::MySQL::PackageBuffer<MockStream>::isEmpty();
template ThorsAnvil::MySQL::PackageBuffer<MockStream>::PackageBuffer(MockStream&, bool);

#endif
