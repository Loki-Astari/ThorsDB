
#include "PackageBuffer.h"

using namespace ThorsAnvil::MySQL;

void PackageBufferMySQLDebugBuffer::readData(char* /*buffer*/, std::size_t /*len*/)
{
    stream.readData(nullptr, 0);
}


