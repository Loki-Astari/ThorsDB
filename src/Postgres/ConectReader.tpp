#include "ThorSQL/Endian.h"
#include <type_traits>
#include <algorithm>

namespace ThorsAnvil
{
    namespace Postgres
    {

template<typename T>
T ConectReader::readInt()
{
    using UT = typename std::make_unsigned<T>::type;

    T   result;
    stream.read(reinterpret_cast<char*>(&result), sizeof(T));
    return Util::convertToHost(static_cast<UT>(result));
}

template<typename T>
std::vector<T> ConectReader::readIntVector(int len)
{
    using UT = typename std::make_unsigned<T>::type;

    std::vector<T>  result(len);
    stream.read(reinterpret_cast<char*>(&result[0]), sizeof(T) * len);
    std::transform(std::begin(result), std::end(result), std::begin(result), [](T const& value){return Util::convertToHost(static_cast<UT>(value));});

    return result;
}

    }
}
