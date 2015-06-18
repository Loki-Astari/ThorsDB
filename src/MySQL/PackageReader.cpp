
#include "PackageReader.h"

#ifdef COVERAGE_TEST
/*
 * This code is only compiled into the unit tests for code coverage purposes
 * It is not part of the live code.
 */
#include "PackageReader.tpp"
#include "Connection.h"
#include <map>
#include <string>

//template ThorsAnvil::MySQL::Connection::Connection(std::string const&, int, std::string const&, std::string const&, std::map<std::string, std::string> const&, ThorsAnvil::MySQL::PackageReaderBase&);
template std::unique_ptr<ThorsAnvil::MySQL::Package> ThorsAnvil::MySQL::PackageReader<ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer>::getNextPackage();
template void                                        ThorsAnvil::MySQL::PackageReader<ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer>::setSocket(int);
//template void                                        ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer::setSocket(int);
#endif

