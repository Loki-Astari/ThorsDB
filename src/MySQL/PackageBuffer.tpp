
template<typename T>
inline void ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<T>::read(char* /*buffer*/, std::size_t /*len*/)
{
    stream.read(nullptr, 0);
}


