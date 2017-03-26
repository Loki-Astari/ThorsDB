#include "ThorSQL/Endian.h"
#include <type_traits>

namespace ThorsAnvil
{
    namespace Postgres
    {

template<typename T>
void ConectWriter::writeInt(T value)
{
    using UT = typename std::make_unsigned<T>::type;

    T output = Util::convertToNet(static_cast<UT>(value));
    stream.write(reinterpret_cast<char*>(&output), sizeof(T));
}

template<typename T>
void ConectWriter::writeIntVector(std::vector<T>const& data)
{
    using UT = typename std::make_unsigned<T>::type;

    std::vector<T> output(data.size());
    std::transform(std::begin(data), std::end(data), std::begin(output), [](T const& value){return Util::convertToNet(static_cast<UT>(value));});
    stream.write(reinterpret_cast<char*>(&output[0]), sizeof(T) * output.size());
}

    }
}
