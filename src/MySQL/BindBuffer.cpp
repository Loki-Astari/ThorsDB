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


#ifdef COVERAGE_MySQL
/*
 * This code is only compiled into the unit tests for code coverage purposes
 * It is not part of the live code.
 */
#include "BindBuffer.tpp"

template void ThorsAnvil::DB::MySQL::BindBuffer::bindValue<ThorsAnvil::DB::SQL::UnixTimeStamp>(ThorsAnvil::DB::SQL::UnixTimeStamp const&);
template void ThorsAnvil::DB::MySQL::BindBuffer::bindValue<std::string>(std::string const&);
template void ThorsAnvil::DB::MySQL::BindBuffer::bindValue<std::vector<char>>(std::vector<char> const&);
template void ThorsAnvil::DB::MySQL::BindBuffer::bindValue<signed char>(signed char const&);
template void ThorsAnvil::DB::MySQL::BindBuffer::bindValue<char>(char const&);
template void ThorsAnvil::DB::MySQL::BindBuffer::bindValue<double>(double const&);
template void ThorsAnvil::DB::MySQL::BindBuffer::bindValue<long double>(long double const&);
template void ThorsAnvil::DB::MySQL::BindBuffer::bindValue<float>(float const&);
template void ThorsAnvil::DB::MySQL::BindBuffer::bindValue<unsigned char>(unsigned char const&);
template void ThorsAnvil::DB::MySQL::BindBuffer::bindValue<int>(int const&);
template void ThorsAnvil::DB::MySQL::BindBuffer::bindValue<unsigned int>(unsigned int const&);
template void ThorsAnvil::DB::MySQL::BindBuffer::bindValue<long>(long const&);
template void ThorsAnvil::DB::MySQL::BindBuffer::bindValue<unsigned long>(unsigned long const&);
template void ThorsAnvil::DB::MySQL::BindBuffer::bindValue<short>(short const&);
template void ThorsAnvil::DB::MySQL::BindBuffer::bindValue<unsigned short>(unsigned short const&);
template void ThorsAnvil::DB::MySQL::BindBuffer::bindValue<long long>(long long const&);
template void ThorsAnvil::DB::MySQL::BindBuffer::bindValue<unsigned long long>(unsigned long long const&);

#endif
