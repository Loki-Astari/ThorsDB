#include "BindBuffer.h"

using namespace ThorsAnvil::DB::MySQL;

BindBuffer::BindBuffer(std::vector<RespPackageColumnDefinition> const& col)
    : columns(col)
    , currentCol(0)
    , bindStream(valueBuffer)
    , bindWriter(bindStream)
{}

void BindBuffer::bindToMySQL(ConectWriter& writer) const
{
    // https://dev.mysql.com/doc/internals/en/com-stmt-execute.html#com-stmt-execute
    //  Called once to write information about all the bound parameters.
    if (columns.size() == 0)
    {
        return;
    }
    std::vector<char>  null((columns.size() + 7) / 8);
    writer.writeRawData(&null[0], null.size());

    writer.writeFixedLengthInteger<1>(1);
    writer.writeRawData(&typeBuffer[0], typeBuffer.size());
    writer.writeRawData(&valueBuffer[0], valueBuffer.size());
}

void BindBuffer::reset()
{
    currentCol  = 0;
    typeBuffer.clear();
    valueBuffer.clear();
}

std::size_t BindBuffer::countBoundParameters() const
{
    return currentCol;
}
