#ifndef THORS_ANVIL_DB_POSTGRES_CONECT_READER_TPP
#define THORS_ANVIL_DB_POSTGRES_CONECT_READER_TPP

#include <boost/endian/conversion.hpp>
#include <type_traits>

namespace ThorsAnvil::DB::Postgres
{

template<typename T>
T ConectReader::readInt()
{
    using UT = typename std::make_unsigned<T>::type;

    T   result;
    stream.read(reinterpret_cast<char*>(&result), sizeof(T));
    return boost::endian::big_to_native(static_cast<UT>(result));
}

template<typename T>
std::vector<T> ConectReader::readIntVector(int len)
{
    using UT = typename std::make_unsigned<T>::type;

    std::vector<T>  result(len);
    stream.read(reinterpret_cast<char*>(&result[0]), sizeof(T) * len);
    std::transform(std::begin(result), std::end(result), std::begin(result), [](T const& value){return boost::endian::big_to_native(static_cast<UT>(value));});

    return result;
}

}

#endif
