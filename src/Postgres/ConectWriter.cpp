#include "ConectWriter.h"

using namespace ThorsAnvil::DB::Postgres;

void ConectWriter::writeString(std::string const& data)
{
    stream.write(&data[0], data.size() + 1);
}
