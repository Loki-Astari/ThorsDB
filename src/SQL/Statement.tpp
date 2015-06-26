
namespace ThorsAnvil
{
    namespace SQL
    {

// -- StatementProxy
inline StatementProxy::~StatementProxy() {}

// -- Cursor
inline Cursor::Cursor(StatementProxy& statementProxy)
    : statementProxy(statementProxy)
{}

inline Cursor::operator bool()
{
    return statementProxy.more();
}

template<typename F>
inline void Cursor::activate(F cb)
{
    activate_(cb);
}

template<typename R, typename... Args>
inline void Cursor::activate_(std::function<R(Args...)> cb)
{
    std::tuple<Args...>    arguments;
    activateWithArgs(cb, arguments, std::make_index_sequence<sizeof...(Args)>());
}

template<typename F, typename A, std::size_t... ids>
inline void Cursor::activateWithArgs(F cb, A& arguments, std::index_sequence<ids...>)
{
    auto list = {retrieve(std::get<ids>(arguments))...};
    [&list](){}();
    cb(std::get<ids>(arguments)...);
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
    statementProxy.bind(std::get<id>(arguments));
    return id;
}

    }
}

