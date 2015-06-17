
namespace ThorsAnvil
{
    namespace SQL
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
        }

template<bool ValidateOnly, typename F>
inline void Cursor::activate(F cb)
{
    activate_<ValidateOnly>(cb);
}

template<bool ValidateOnly, typename R, typename... Args>
inline void Cursor::activate_(std::function<R(Args...)> cb)
{
    std::tuple<typename std::decay<Args>::type...>    arguments;
    activateWithArgs<ValidateOnly>(cb, arguments, std::make_index_sequence<sizeof...(Args)>());
}

template<bool ValidateOnly, typename F, typename A, std::size_t... ids>
inline void Cursor::activateWithArgs(F cb, A& arguments, std::index_sequence<ids...> const& id)
{
    try {
        auto list = {retrieve(std::get<ids>(arguments))...};
        [&list](){}();
        Detail::CallWithArgs<ValidateOnly, F, A, ids...>(cb, arguments, id);
    }
    catch(ValidationTmpError const&) {
        // Drop a temporary error.
        // These are only thrown by subsystems if they know they can recover and do
        // better error messaging as a result.
    }
}

template<typename V>
inline int Cursor::retrieve(V& value)
{
    statementProxy.retrieve(value);
    return 1;
}

template<typename... R>
inline void BindArgs<R...>::bindTo(StatementProxy& statementProxy) const
{
    bindArgsTo(statementProxy, std::make_index_sequence<sizeof...(R)>());
}

template<typename... R>
template<std::size_t... ids>
inline void BindArgs<R...>::bindArgsTo(StatementProxy& statementProxy, std::index_sequence<ids...>const&) const
{
    auto list = {bindTheArgument<ids>(statementProxy)...};
    [&list](){}();
}

template<typename... R>
template<std::size_t id>
inline int BindArgs<R...>::bindTheArgument(StatementProxy& statementProxy) const
{
    statementProxy.bind(std::get<id>(arguments).get());
    return id;
}

    }
}

