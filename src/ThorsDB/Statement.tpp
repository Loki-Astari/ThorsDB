#ifndef THORSANVIL_DB_THORSDB_STATEMENT_TPP
#define THORSANVIL_DB_THORSDB_STATEMENT_TPP

#include "ThorsLogging/ThorsLogging.h"
#include <iostream>

namespace ThorsAnvil::DB::Access
{
    namespace Detail
    {

        template<bool ValidateOnly, typename F, typename A, std::size_t... ids>
        class CallWithArgs
        {
            public:
                CallWithArgs(F, A, std::index_sequence<ids...> const&)
                {}
        };
        template<typename F, typename A, std::size_t... ids>
        class CallWithArgs<false, F, A, ids...>
        {
            public:
                CallWithArgs(F cb, A args, std::index_sequence<ids...> const&)
                {
                    cb(std::get<ids>(args)...);
                }
        };

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

// -- Detail::Cursor
//      Used internally to loop over each row as it is read from the DB
template<bool ValidateOnly, typename F>
inline void Detail::Cursor::activate(F cb)
{
    activate_<ValidateOnly>(cb);
}

template<bool ValidateOnly, typename R, typename... Args>
inline void Detail::Cursor::activate_(std::function<R(Args...)> cb)
{
    std::tuple<typename std::decay<Args>::type...>    arguments;
    activateWithArgs<ValidateOnly>(cb, arguments, std::make_index_sequence<sizeof...(Args)>());
}

template<bool ValidateOnly, typename F, typename A, std::size_t... ids>
inline void Detail::Cursor::activateWithArgs(F cb, A& arguments, std::index_sequence<ids...> const& id)
{
    try
    {
        auto list = {0, (retrieve(std::get<ids>(arguments)), 0)...};
        (void)list;
        Detail::CallWithArgs<ValidateOnly, F, A, ids...>(cb, arguments, id);
    }
    catch (Lib::ValidationTmpError const& e)
    {
        ThorsCatchMessage("ThorsAnvil::DB::Access::Detail::Cursor", "activateWithArg", e.what());
        // Drop a temporary error.
        // These are only thrown by subsystems if they know they can recover and do
        // better error messaging as a result.
    }
}

template<typename V>
inline void Detail::Cursor::retrieve(V& value)
{
    statementProxy.retrieve(value);
}

// -- Bindings
//    Used to group arguments together to bind to the Access bind points
template<typename... R>
inline void BindArgs<R...>::bindTo(Lib::StatementProxy& statementProxy) const
{
    bindArgsTo(statementProxy, std::make_index_sequence<sizeof...(R)>());
}

template<typename... R>
template<std::size_t... ids>
inline void BindArgs<R...>::bindArgsTo(Lib::StatementProxy& statementProxy, std::index_sequence<ids...>const&) const
{
    auto list = {0, (bindTheArgument<ids>(statementProxy), 0)...};
    (void)list;
}

template<typename... R>
template<std::size_t id>
inline void BindArgs<R...>::bindTheArgument(Lib::StatementProxy& statementProxy) const
{
    statementProxy.bind(std::get<id>(arguments).get());
}

// -- Statement
//      The four versions of the execute command.
template<typename F, typename... R>
inline void Statement::execute(BindArgs<R...> const& binds, F cb)
{
    if (!statementProxy->isSelect())
    {
        ThorsLogAndThrowLogical("ThrosAnvil::Access::Statement", "execute", "Passing callback function to non select");
    }

    typedef typename Detail::FunctionTraits<decltype(cb)>::FunctionType   CBTraits;
    // Call before calling execute.
    // This gets the proxy to validate the conversion of the values returned
    // by a select into the parameters used by the CB function. If it fails now
    // we can do less tidy up work at the DB level.
    Detail::Cursor validator(*statementProxy);
    validator.activate<true, CBTraits>(cb);

    binds.bindTo(*statementProxy);
    Detail::Cursor cursor = statementProxy->execute();
    try
    {
        while (cursor)
        {
            cursor.activate<false, CBTraits>(cb);
        }
    }
    catch (std::exception const& e)
    {
        ThorsCatchMessage("ThorsAnvil::DB::Access::Statement", "execute", e.what());
        ThorsRethrowMessage("ThorsAnvil::DB::Access::Statement", "execute", e.what());
        statementProxy->abort();
        throw;
    }
    catch (...)
    {
        ThorsCatchMessage("ThorsAnvil::DB::Access::Statement", "execute", "UNKNOWN");
        ThorsRethrowMessage("ThorsAnvil::DB::Access::Statement", "execute", "UNKNOWN");
        statementProxy->abort();
        throw;
    }
}

template<typename F>
inline void Statement::execute(F cb)
{
    execute(Bind(), cb);
}

template<typename... R>
inline void Statement::execute(BindArgs<R...> const& binds)
{
    if (statementProxy->isSelect())
    {
        ThorsLogAndThrowLogical("ThrosAnvil::Access::Statement", ":execute", "Not passing callback to SELECT statement");
    }

    modifyDone = true;
    binds.bindTo(*statementProxy);
    statementProxy->execute();
    return;
}

}

#endif
