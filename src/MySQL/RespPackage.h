#ifndef THORSANVIL_MYSQL_DETAILS_PACKAGE_RESP_H
#define THORSANVIL_MYSQL_DETAILS_PACKAGE_RESP_H

#include <ostream>

namespace ThorsAnvil
{
    namespace MySQL
    {

class ConectReader;
class RespPackage
{
    protected:
        ConectReader&   reader;
        bool                ok;
        bool                error;
        bool                eof;
        std::string         humanMessage;
    public:
        RespPackage(ConectReader& reader)
            : reader(reader)
            , ok(false)
            , error(false)
            , eof(false)
        {}
        virtual ~RespPackage() {}
        virtual  std::ostream& print(std::ostream& s) const = 0;

        bool                isOK()              const                   {return ok;}
        bool                isError()           const                   {return error;}
        bool                isEOF()             const                   {return eof;}
        std::string const&  message()           const                   {return humanMessage;}

        friend std::ostream& operator<<(std::ostream& s, RespPackage const& data)
        {
            return data.print(s)
                    << "humanMessage(" << data.humanMessage << ") ";
        }
};

    }
}

#endif
