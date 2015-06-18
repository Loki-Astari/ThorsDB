
namespace ThorsAnvil
{
    namespace MySQL
    {

template<typename T>
std::unique_ptr<Package> PackageReader<T>::getNextPackage()
{
    std::unique_ptr<Package>    result;
    return result;
}

template<typename T>
void PackageReader<T>::setSocket(int socket)
{
    buffer.setSocket(socket);
}

    }
}

