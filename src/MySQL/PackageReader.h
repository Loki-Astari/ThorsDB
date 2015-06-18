
#ifndef THORS_ANVIL_MYSQL_PACKAGE_READER_H
#define THORS_ANVIL_MYSQL_PACKAGE_READER_H

#include <memory>

namespace ThorsAnvil
{
    namespace MySQL
    {

class Package
{
    public:
        virtual ~Package()  = 0;
};
class PackageReaderBase
{
    public:
        virtual std::unique_ptr<Package>    getNextPackage()    = 0;
        virtual void                        setSocket(int)      = 0;
};
template<typename T>
class PackageReader: public PackageReaderBase
{
    T   buffer;
    public:
        virtual std::unique_ptr<Package>    getNextPackage()    override;
        virtual void                        setSocket(int)      override;
};

    }
}

#ifndef COVERAGE_TEST
#include "PackageReader.tpp"
#endif

#endif

