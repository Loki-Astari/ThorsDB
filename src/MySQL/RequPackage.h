#ifndef THORSANVIL_DB_MYSQL_DETAILS_PACKAGE_REQU_H
#define THORSANVIL_DB_MYSQL_DETAILS_PACKAGE_REQU_H

#include "ConectWriter.h"
#include "RequPackage.h"
#include <ostream>
#include <string>

namespace ThorsAnvil::DB::MySQL
{

class RequPackage
{
    std::string     humanMessage;
    public:
        RequPackage(std::string const& humanMessage, std::string const& requName)
            : humanMessage(humanMessage)
        {
            (void)requName;
        }
        virtual ~RequPackage()  {}
        virtual  std::ostream& print(std::ostream& s)   const = 0;
        virtual  void build(ConectWriter& writer)       const = 0;
        void send(ConectWriter& writer) const
        {
            this->build(writer);
            writer.flush();
        }

        friend std::ostream& operator<<(std::ostream& s, RequPackage const& data)
        {
            return data.print(s)
                    << "humanMessage(" << data.humanMessage << ") ";
        }
};

}

#endif
