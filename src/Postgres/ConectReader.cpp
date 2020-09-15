#include "ConectReader.h"

using namespace ThorsAnvil::DB::Postgres;

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
