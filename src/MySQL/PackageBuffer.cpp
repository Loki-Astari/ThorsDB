
#include "PackageBuffer.h"

using namespace ThorsAnvil::MySQL;

#ifdef COVERAGE_TEST
/*
 * This code is only compiled into the unit tests for code coverage purposes
 * It is not part of the live code.
 */
#include "PackageBuffer.tpp"
#include "PackageStream.h"


template ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<ThorsAnvil::MySQL::MySQLStream>::PackageBufferMySQLDebugBuffer(ThorsAnvil::MySQL::MySQLStream&,bool);
template void PackageBufferMySQLDebugBuffer<MySQLStream>::read(char*, std::size_t);


#endif

