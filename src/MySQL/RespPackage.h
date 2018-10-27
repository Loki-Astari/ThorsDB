#ifndef THORS_ANVIL_MYSQL_DETAILS_PACKAGE_RESP_H
#define THORS_ANVIL_MYSQL_DETAILS_PACKAGE_RESP_H

#include "ThorSQL/SQLUtil.h"
#include "ConectReader.h"
#include <ostream>
#include <string>
#include <memory>

namespace ThorsAnvil
{
    namespace MySQL
    {

enum class RespType {Ok, Err, Eof, HandShake, AuthSwitchRequest, Prepare, PrepareExecute, ResultSet, Authentication};

class RespPackage
{
    protected:
        ConectReader&   reader;
        RespType        type;
        bool            ok;
        bool            error;
        bool            eof;
        std::string     humanMessage;
    public:
        RespPackage(RespType type, ConectReader& reader)
            : reader(reader)
            , type(type)
            , ok(false)
            , error(false)
            , eof(false)
        {}
        virtual ~RespPackage() {}
        virtual  std::ostream& print(std::ostream& s) const = 0;

        RespType            is()                const                   {return type;}
        bool                isOK()              const                   {return ok;}
        bool                isError()           const                   {return error;}
        bool                isEOF()             const                   {return eof;}
        std::string const&  message()           const                   {return humanMessage;}
        bool                hasCap(int c)       const                   {return (reader.getCapabilities() & c) != 0;}

        friend std::ostream& operator<<(std::ostream& s, RespPackage const& data)
        {
            return data.print(s)
                    << "humanMessage(" << data.humanMessage << ") ";
        }
};

template<typename To>
std::unique_ptr<To> downcastUniquePtr(std::unique_ptr<RespPackage>&& item);

    }
}

#ifndef COVERAGE_MySQL
#include "RespPackage.tpp"
#endif

#endif
