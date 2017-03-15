#ifndef THORSANVIL_MYSQL_DETAILS_PACKAGE_RESP_H
#define THORSANVIL_MYSQL_DETAILS_PACKAGE_RESP_H

#include "ThorSQL/SQLUtil.h"
#include <ostream>
#include <iostream>

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
        RespPackage(ConectReader& reader, std::string const& respName)
            : reader(reader)
            , ok(false)
            , error(false)
            , eof(false)
        {
            thorUnused(respName);
            // std::cerr << "\n<----- Resp: " << respName << "\n\n";
        }
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

template<typename To>
std::unique_ptr<To> downcastUniquePtr(std::unique_ptr<RespPackage>&& item)
{
    if (item.get() == nullptr)
    {
        return nullptr;
    }

    std::unique_ptr<To> result;
    // Note the dynamic_cast may fail and throw.
    // If this happens item retains ownership.
    try
    {
        result.reset(&dynamic_cast<To&>(*item));
    }
    catch (std::bad_cast const&)
    {
        throw std::domain_error("ThorsAnvil::SQL::downcastUniquePtr: Casting reponse to expected type failed");
    }
    // Now that ownership has been transferred.
    // Release the original pointer.
    item.release();

    return result;
}

    }
}

#endif
