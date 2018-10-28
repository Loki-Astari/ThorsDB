#ifndef THORS_ANVIL_MYSQL_PACKAGE_REQU_STRING_H
#define THORS_ANVIL_MYSQL_PACKAGE_REQU_STRING_H

#include "RequPackage.h"
#include <string>
#include <ostream>

namespace ThorsAnvil
{
    namespace MySQL
    {

class ConectWriter;

class RequPackageString: public RequPackage
{
    std::string     message;

    public:
        RequPackageString(std::string const& message);

        virtual  std::ostream& print(std::ostream& s)   const override;
        virtual  void build(ConectWriter& writer)       const override;
};

    }
}

#endif
