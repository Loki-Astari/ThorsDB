
#include "PackageBuffer.h"

using namespace ThorsAnvil::MySQL;

#ifdef COVERAGE_MySQL
/*
 * This code is only compiled into the unit tests for code coverage purposes
 * It is not part of the live code.
 */
#include "PackageBuffer.tpp"
#include "PackageStream.h"
#include "test/MockStream.h"


template ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<ThorsAnvil::MySQL::MySQLStream>::PackageBufferMySQLDebugBuffer(ThorsAnvil::MySQL::MySQLStream&,bool);
template void PackageBufferMySQLDebugBuffer<MySQLStream>::read(char*, std::size_t);

template std::string ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<MockStream>::readRemainingData();
template void ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<MockStream>::read(char*, std::size_t);
template void ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<MockStream>::flush();
template void ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<MockStream>::reset();
template void ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<MockStream>::write(char const*, std::size_t);
template bool ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<MockStream>::isEmpty();
template ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<MockStream>::PackageBufferMySQLDebugBuffer(MockStream&,bool);

#endif

