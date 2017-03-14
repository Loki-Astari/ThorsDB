#ifndef THORS_ANVIL_SQL_STATEMENT_H
#define THORS_ANVIL_SQL_STATEMENT_H

/*
 *      ThorsAnvil::SQL::Statement  Represent an SQL statement.
 *                                  It is a simple PIMPLE class that uses a StatementProxy as the implementation.
 *
 *                                  It has one function execute() (in two veriations) which executes the SQL statement.
 *
 *                                  Modification Statements:
 *                                  ========================
 *                                  If your SQL statement is INSERT/UPDATE or DELETE.
 *                                  Then you should use `execute()` or `execute(<BindArguments>)`. Using a version where
 *                                  a lambda is passed as an argument will generate a std::runtime_error exception.
 *
 *                                  After a statement has been executes you can use `rowsAffected()` and `lastInsertID()
 *                                  to get basic information about the query.
 *
 *
 *                                  Query Statements:
 *                                  =================
 *                                  If your statement is SELECT.
 *                                  Then you should use `execute(<lambda>)` or `execute(<BindArguments>, <lambda>)`. Using
 *                                  a version where a lambda is NOT passed will generate a std::runtime_error exception.
 *
 *                                  When the select is execute the lambda function will be executed for each row retrieved
 *                                  from the DB.
 *
 *                                  Argument Binding:
 *                                  =================
 *                                  Each '?' in the SQL statement is a bind point. You MUST provide an argument for each
 *                                  bind point in the statement. Failure to do so will generate a std::logic_error exception.
 *
 *                                  Lambda Arguments:
 *                                  =================
 *                                  For each selected parameter the lambda must have a parameter of the correct type. The
 *                                  order of the lambda parameters is the same as the order of the select parameter.
 *                                  If the number of select parameters does not match the number of parameters in the
 *                                  lambda an exception or the type of the values returned does not match the types of
 *                                  parameters then std::logic_error will be generated.
 *
 *                                  Note: A '*' in the select parameter maps to one parameter for each column.
 *
 *      BindArgs:                   Used as an argument to execute to bind parameters to bind points.
 *                                  See examples
 *
 *      UnixTimeStamp:              Used as a Bind type for Date/Time/DateTime/TimeStamp fields in a DB.
 *                                  Basically to distinguish time objects from integer objects.
 *
 * Example:
 *      Statement       select("SELECT * from People where Age>=?");
 *      select.execute(Bind(25), [](std::string name, int age, char sex, float height) {
 *          std::cout << "Found: " << name << " Details: " << age << " years old << " S:" << sex << " Height: " << height << "\n";
 *      });
 *
 *      Statement       addPeople("INSERT INTO People(NAme, age, sex, height) VALUES(?, ?, ?, ?)");
 *      addPeople.execute(Bind("Loki",  987, 'M', 2.34));
 *      std::cout << "ID: " << addPeople.lastInsertID() << "\n";
 *
 *      addPeople.execute(Bind("Thor", 1003, 'M', 2.43));
 *      std::cout << "ID: " << addPeople.lastInsertID() << "\n";
 *
 *
 * Other Classes:
 * ==============
 *      Detail::ValidationTmpError:     Exception type that may be thrown.
 *
 *      Detail::Cursor:                 Used internally by statement to loop over all the rows retrieved by a select.
 *
 *      Detail::FunctionTraits:         Used to get lambda parameter types.
 *
 *      Lib::StatementProxy:            Used by libraries that implement statement objects.
 *
 */

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
        bool                               modifyDone;
    public:
        Statement(Connection& connect, std::string const& selectStatement, StatementType = ThorsAnvil::SQL::Prepare);
        template<typename F, typename... R>
        void execute(BindArgs<R...> const& binds, F cb);
        template<typename F>
        void execute(F cb);
        template<typename... R>
        void execute(BindArgs<R...> const& binds);
        void execute();

        long rowsAffected() const;
        long lastInsertID() const;
};

struct UnixTimeStamp
{
    std::time_t     time;

        UnixTimeStamp(std::time_t time = 0):
            time(time)
        {}
    bool operator==(UnixTimeStamp const& rhs) const {return time == rhs.time;}
    bool operator!=(UnixTimeStamp const& rhs) const {return ! (this->operator==(rhs));}
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

        // Status Info
        virtual bool isSelect() const                       = 0;
        virtual long rowsAffected() const                   = 0;
        virtual long lastInsertID() const                   = 0;

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
        void retrieve(V& value);
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
        void bindTheArgument(StatementProxy& statementProxy) const;
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

// Thus in the subsequent on there is no call to `binds.bindTo()`
template<typename F, typename... R>
inline void Statement::execute(BindArgs<R...> const& binds, F cb)
{
    if (!statementProxy->isSelect())
    {
        throw std::logic_error("ThrosAnvil::SQL::Statement::execute: Passing callback function to non select");
    }

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
    execute(BindArgs<>(), cb);
}

template<typename... R>
inline void Statement::execute(BindArgs<R...> const& binds)
{
    if (statementProxy->isSelect())
    {
        throw std::logic_error("ThrosAnvil::SQL::Statement::execute: Not passing callback to SELECT statement");
    }

    modifyDone = true;
    binds.bindTo(*statementProxy);
    statementProxy->execute();
    return;
}

inline void Statement::execute()
{
    execute(BindArgs<>());
}

    }
}

#ifndef COVERAGE_SQL
#include "Statement.tpp"
#endif

#endif
