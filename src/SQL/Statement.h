
#ifndef THORS_ANVIL_SQL_STATEMENT_H
#define THORS_ANVIL_SQL_STATEMENT_H

#include "SQLUtil.h"
#include <memory>
#include <string>

namespace ThorsAnvil
{
    namespace SQL
    {

class Connection;
class StatementProxy;
class Cursor;
template<typename... Args>
class BindArgs;

class Statement
{
    private:
        std::unique_ptr<StatementProxy>    statementProxy;
    public:
        Statement(Connection& connect, std::string const& selectStatement, StatementType = ThorsAnvil::SQL::Prepare);
        template<typename F, typename... R>
        void execute(BindArgs<R...> const& binds, F cb);
        template<typename F>
        void execute(F cb);
};

class StatementProxy
{
    public:
        virtual ~StatementProxy()
        {}

        virtual void   bind(char)                           = 0;
        virtual void   bind(signed char)                    = 0;
        virtual void   bind(signed short)                   = 0;
        virtual void   bind(signed int)                     = 0;
        virtual void   bind(signed long)                    = 0;
        virtual void   bind(signed long long)               = 0;
        virtual void   bind(unsigned char)                  = 0;
        virtual void   bind(unsigned short)                 = 0;
        virtual void   bind(unsigned int)                   = 0;
        virtual void   bind(unsigned long)                  = 0;
        virtual void   bind(unsigned long long)             = 0;

        virtual void   bind(float)                          = 0;
        virtual void   bind(double)                         = 0;
        virtual void   bind(long double)                    = 0;

        virtual void   bind(std::string const&)             = 0;

        // -----

        Cursor execute();
        virtual void doExecute()                            = 0;
        virtual bool more()                                 = 0;

        virtual void   retrieve(char&)                      = 0;
        virtual void   retrieve(signed char&)               = 0;
        virtual void   retrieve(signed short&)              = 0;
        virtual void   retrieve(signed int&)                = 0;
        virtual void   retrieve(signed long&)               = 0;
        virtual void   retrieve(signed long long&)          = 0;
        virtual void   retrieve(unsigned char&)             = 0;
        virtual void   retrieve(unsigned short&)            = 0;
        virtual void   retrieve(unsigned int&)              = 0;
        virtual void   retrieve(unsigned long&)             = 0;
        virtual void   retrieve(unsigned long long&)        = 0;

        virtual void   retrieve(float&)                     = 0;
        virtual void   retrieve(double&)                    = 0;
        virtual void   retrieve(long double&)               = 0;

        virtual void   retrieve(std::string&)               = 0;
};

class Cursor
{
    StatementProxy&     statementProxy;
    public:
        explicit operator bool();
        Cursor(StatementProxy& statementProxy);

        template<typename F>
        void activate(F cb);

        template<typename R, typename... Args>
        void activate_(std::function<R(Args...)> cb);

        template<typename F, typename A, std::size_t... ids>
        void activateWithArgs(F func, A& arguments, std::index_sequence<ids...>);

        template<typename V>
        int retrieve(V& value);
};

template<typename... Args>
class BindArgs
{
    std::tuple<std::reference_wrapper<Args>...>     arguments;
    public:
        BindArgs(Args... args)
            : arguments(args...)
        {}

        void bindTo(StatementProxy& statementProxy) const;
    private:
        template<std::size_t... ids>
        void bindArgsTo(StatementProxy& statementProxy, std::index_sequence<ids...>const&) const;
        template<std::size_t id>
        int bindTheArgument(StatementProxy& statementProxy) const;
};

// -- Bindings
template<typename... Args>
BindArgs<Args...> Bind(Args... args)
{
    return BindArgs<Args...>(args...);
}

// -- Statement

// Classes need to get the type of a lambda to
// Coerce the correct function in Cursor to be
// called.
namespace Detail
{
    template<typename T>
    struct FunctionTraits
        : public FunctionTraits<decltype(&T::operator())>
    {};

    template <typename ClassType, typename ReturnType, typename... Args>
    struct FunctionTraits<ReturnType(ClassType::*)(Args...) const>
    {
        typedef std::function<ReturnType(Args...)>  FunctionType;
    };
}

template<typename F, typename... R>
inline void Statement::execute(BindArgs<R...> const& binds, F cb)
{
    binds.bindTo(*statementProxy);
    Cursor cursor = statementProxy->execute();
    while(cursor) {
        typedef typename Detail::FunctionTraits<decltype(cb)>::FunctionType   CBTraits;
        cursor.activate<CBTraits>(cb);
    }
}
template<typename F>
inline void Statement::execute(F cb)
{
    Cursor cursor = statementProxy->execute();
    while(cursor) {
        typedef typename Detail::FunctionTraits<decltype(cb)>::FunctionType   CBTraits;
        cursor.activate<CBTraits>(cb);
    }
}

    }
}

#ifndef COVERAGE_SQL
#include "Statement.tpp"
#endif

#endif


