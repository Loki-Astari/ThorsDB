#ifndef THORS_ANVIL_DB_ACCESS_STATEMENT_H
#define THORS_ANVIL_DB_ACCESS_STATEMENT_H

/*
 *      ThorsAnvil::DB::Access::Statement      Represent an Access statement.
 *      ThorsAnvil::DB::Access::BindArgs:      Used to bind arguments to a statement.
 *      ThorsAnvil::DB::Access::UnixTimeStamp: Used as a Bind type for Date/Time/DateTime/TimeStamp fields in a DB.
 *
 *                See: doc/usage.md for usage details
 *                See: doc/internal.md for implementation details
 *
 * Other Classes:
 * ==============
 *      Detail::Cursor:                 Used internally by statement to loop over all the rows retrieved by a select.
 *
 *      Detail::FunctionTraits:         Used to get lambda parameter types.
 *
 *      Lib::ValidationTmpError:        Exception type that may be thrown.
 *
 *      Lib::StatementProxy:            Used by libraries that implement statement objects.
 *
 */

#include <memory>
#include <vector>
#include <string>
#include <ctime>
#include <tuple>
#include <functional>
#include <stdexcept>

namespace ThorsAnvil::DB::Access
{
        namespace Detail
        {
            class Cursor;
        }
        namespace Lib
        {
            class StatementProxy;
        }

class Connection;
template<typename... Args>
class BindArgs;

class Statement
{
    private:
        std::unique_ptr<Lib::StatementProxy>    statementProxy;
        bool                                    modifyDone;
    public:
        Statement(Connection& connect, std::string const& selectStatement);
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


template<typename... Args>
class BindArgs
{
    using DataTuple = std::tuple<std::reference_wrapper<typename std::add_const<Args>::type>...>;

    DataTuple  arguments;
    public:
        explicit BindArgs(Args const&... args)
            : arguments(args...)
        {}

        void bindTo(Lib::StatementProxy& statementProxy) const;
    private:
        template<std::size_t... ids>
        void bindArgsTo(Lib::StatementProxy& statementProxy, std::index_sequence<ids...>const&) const;
        template<std::size_t id>
        void bindTheArgument(Lib::StatementProxy& statementProxy) const;
};
template<typename... Args>
BindArgs<Args...> Bind(Args const&... args)
{
    return BindArgs<Args...>(args...);
}


namespace Detail
{

    class Cursor
    {
        Lib::StatementProxy&     statementProxy;
        public:
            explicit operator bool();
            Cursor(Lib::StatementProxy& statementProxy);

            template<bool ValidateOnly, typename F>
            void activate(F cb);

            template<bool ValidateOnly, typename R, typename... Args>
            void activate_(std::function<R(Args...)> cb);

            template<bool ValidateOnly, typename F, typename A, std::size_t... ids>
            void activateWithArgs(F func, A& arguments, std::index_sequence<ids...> const&);

            template<typename V>
            void retrieve(V& value);
    };

}

namespace Lib
{

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
            // abort called if an exception is thrown by the lambda provided to SELECT
            // After abort returns the exception is re-thrown. So if an exception is
            // generated during abort() the application will terminate.
            virtual void   abort()                              = 0;

            // bind() is called for each parameter provided by ThorsAnvil::DB::Access::Bind()
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

            Detail::Cursor execute();
            // After all the bind parameters have been bound with the above calls.
            // This method is called to initiate the communication with the DB
            virtual void doExecute()                            = 0;

            // Called from the Detail::Cursor object to see if there are more rows
            // to be retrieved from the DB
            virtual bool more()                                 = 0;

            // For each parameter in the lambda one of these functions is called
            // to retrieve a value returned by the DB. They are called in the order
            // of the parameters in the lambda
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


            // Status Info
            // These functions can be called to get information about
            // the last call to execute() if this was a modification
            // statement.
            virtual bool isSelect() const                       = 0;
            virtual long rowsAffected() const                   = 0;
            virtual long lastInsertID() const                   = 0;
    };

}

}

#include "Statement.tpp"

#endif
