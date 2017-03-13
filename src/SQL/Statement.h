#ifndef THORS_ANVIL_SQL_STATEMENT_H
#define THORS_ANVIL_SQL_STATEMENT_H

#include "SQLUtil.h"
#include <memory>
#include <vector>
#include <string>
#include <ctime>
#include <tuple>

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

struct UnixTimeStamp
{
    std::time_t     time;

        explicit UnixTimeStamp(std::time_t time):
            time(time)
        {}
};

class ValidationTmpError: public std::runtime_error
{
    public:
        ValidationTmpError(std::string const& msg)
            : std::runtime_error(msg)
        {}
};

class StatementProxy
{
    public:
        virtual ~StatementProxy()
        {}
        virtual void   abort()                              = 0;

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
        virtual void   bind(std::vector<char> const&)       = 0;

        virtual void   bind(UnixTimeStamp const&)           = 0;

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
        virtual void   retrieve(std::vector<char>&)         = 0;

        virtual void   retrieve(UnixTimeStamp&)             = 0;
};

class Cursor
{
    StatementProxy&     statementProxy;
    public:
        explicit operator bool();
        Cursor(StatementProxy& statementProxy);

        template<bool ValidateOnly, typename F>
        void activate(F cb);

        template<bool ValidateOnly, typename R, typename... Args>
        void activate_(std::function<R(Args...)> cb);

        template<bool ValidateOnly, typename F, typename A, std::size_t... ids>
        void activateWithArgs(F func, A& arguments, std::index_sequence<ids...> const&);

        template<typename V>
        int retrieve(V& value);
};

template<typename... Args>
class BindArgs
{
    using DataTuple = std::tuple<std::reference_wrapper<typename std::add_const<Args>::type>...>;

    DataTuple  arguments;
    public:
        explicit BindArgs(Args const&... args)
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
BindArgs<Args...> Bind(Args const&... args)
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

// This version of execute takes bind parameters.
// The next one is identical but takes no parameters.
// Thus in the subsequent on there is no call to `binds.bindTo()`
template<typename F, typename... R>
inline void Statement::execute(BindArgs<R...> const& binds, F cb)
{
    typedef typename Detail::FunctionTraits<decltype(cb)>::FunctionType   CBTraits;
    // Call before calling execute.
    // This gets the proxy to validate the conversion of the values returned
    // by a select into the parameters used by the CB function. If it fails now
    // we can do less tidy up work at the DB level.
    Cursor validator(*statementProxy);
    validator.activate<true, CBTraits>(cb);

    binds.bindTo(*statementProxy);
    Cursor cursor = statementProxy->execute();
    try
    {
        while (cursor)
        {
            cursor.activate<false, CBTraits>(cb);
        }
    }
    catch (...)
    {
        statementProxy->abort();
        throw;
    }
}
template<typename F>
inline void Statement::execute(F cb)
{
    typedef typename Detail::FunctionTraits<decltype(cb)>::FunctionType   CBTraits;
    // Call before calling execute.
    // This gets the proxy to validate the conversion of the values returned
    // by a select into the parameters used by the CB function. If it fails now
    // we can do less tidy up work at the DB level.
    Cursor validator(*statementProxy);
    validator.activate<true, CBTraits>(cb);

    Cursor cursor = statementProxy->execute();
    try
    {
        while (cursor)
        {
            cursor.activate<false, CBTraits>(cb);
        }
    }
    catch (...)
    {
        statementProxy->abort();
        throw;
    }
}

    }
}

#ifndef COVERAGE_SQL
#include "Statement.tpp"
#endif

#endif
