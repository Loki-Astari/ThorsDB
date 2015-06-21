#ifndef THORSANVIL_MYSQL_DETAILS_PACKAGE_REQU_H
#define THORSANVIL_MYSQL_DETAILS_PACKAGE_REQU_H

#include "PackageConWriter.h"
#include <ostream>
#include <iomanip>

namespace ThorsAnvil
{
    namespace MySQL
    {

class PackageRequ
{
    std::string     humanMessage;
    public:
        PackageRequ(std::string const& humanMessage)
            : humanMessage(humanMessage)
        {}
        virtual ~PackageRequ()  {}
        virtual  std::ostream& print(std::ostream& s)   const = 0;
        virtual  void build(PackageConWriter& writer)   const = 0;
        void send(PackageConWriter& writer) const
        {
            this->build(writer);
            writer.flush();
        }

        friend std::ostream& operator<<(std::ostream& s, PackageRequ const& data)
        {
            return data.print(s)
                    << "humanMessage(" << data.humanMessage << ") ";
        }
};

    }
}

#endif
