#ifndef THORSANVIL_DB_POSTGRES_STATEMENT_H
#define THORSANVIL_DB_POSTGRES_STATEMENT_H

#include "PostgresConfig.h"
#include "ThorsDB/Statement.h"
#include <string>
#include <vector>
//#include <cstddef>   // for size_t (removed because it crashes clang 3.5 on travis

namespace ThorsAnvil::DB::Postgres
{

class Statement: public Access::Lib::StatementProxy
{
    public:
        Statement(std::string const& statement);

    private:
        virtual void   abort()                              override    {}

        virtual void   bind(char)                           override    {}
        virtual void   bind(signed char)                    override    {}
        virtual void   bind(signed short)                   override    {}
        virtual void   bind(signed int)                     override    {}
        virtual void   bind(signed long)                    override    {}
        virtual void   bind(signed long long)               override    {}
        virtual void   bind(unsigned char)                  override    {}
        virtual void   bind(unsigned short)                 override    {}
        virtual void   bind(unsigned int)                   override    {}
        virtual void   bind(unsigned long)                  override    {}
        virtual void   bind(unsigned long long)             override    {}

        virtual void   bind(float)                          override    {}
        virtual void   bind(double)                         override    {}
        virtual void   bind(long double)                    override    {}

        virtual void   bind(std::string const&)             override    {}
        virtual void   bind(std::vector<char> const&)       override    {}

        virtual void   bind(Access::UnixTimeStamp const&)      override    {}

            // -----

        virtual void doExecute()                            override    {}
        virtual bool more()                                 override    {return false;}

        virtual bool isSelect() const                       override    {return true;}
        virtual long rowsAffected() const                   override    {return 0;}
        virtual long lastInsertID() const                   override    {return 0;}

        virtual void   retrieve(char& value)                override    {value=0;}
        virtual void   retrieve(signed char& value)         override    {value=0;}
        virtual void   retrieve(signed short& value)        override    {value=0;}
        virtual void   retrieve(signed int& value)          override    {value=0;}
        virtual void   retrieve(signed long& value)         override    {value=0;}
        virtual void   retrieve(signed long long& value)    override    {value=0;}
        virtual void   retrieve(unsigned char& value)       override    {value=0;}
        virtual void   retrieve(unsigned short& value)      override    {value=0;}
        virtual void   retrieve(unsigned int& value)        override    {value=0;}
        virtual void   retrieve(unsigned long& value)       override    {value=0;}
        virtual void   retrieve(unsigned long long& value)  override    {value=0;}

        virtual void   retrieve(float& value)               override    {value=0;}
        virtual void   retrieve(double& value)              override    {value=0;}
        virtual void   retrieve(long double& value)         override    {value=0;}

        virtual void   retrieve(std::string& value)         override    {value="";}
        virtual void   retrieve(std::vector<char>& value)   override    {value=std::vector<char>();}

        virtual void   retrieve(Access::UnixTimeStamp& value)  override    {value=Access::UnixTimeStamp(static_cast<std::time_t>(0));}
    private:
};

}

#endif
