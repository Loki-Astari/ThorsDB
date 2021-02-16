#ifndef THORSANVIL_DB_MONGO_CMD_DB_FIND_TPP
#define THORSANVIL_DB_MONGO_CMD_DB_FIND_TPP

#ifndef THORSANVIL_DB_MONGO_CMD_DB_FIND_H
#error  "This should only be included from CmdDB_Find.h"
#endif

namespace ThorsAnvil::DB::Mongo
{

template<typename Filter, typename Sort>
Find<Filter, Sort>::Find(FindOptions const& options, std::string collection, Filter&& filter, Sort&& sort)
    : FindOptional(options)
    , find(std::move(collection))
    , filter(std::forward<Filter>(filter))
    , sort(std::forward<Sort>(sort))
{
    updateFilter();
}

template<typename Filter, typename Sort>
void Find<Filter, Sort>::updateFilter()
{
    if constexpr (!std::is_same_v<Filter, FindAll>)
    {
        findFilter["filter"]      = true;
    }
    if constexpr (!std::is_same_v<Sort, DefaultSort>)
    {
        findFilter["sort"]        = true;
    }
}

template<typename Filter, typename Sort>
CmdDB_Find<Filter, DefaultSort> send_CmdDB_Find(std::string db, std::string collection, Filter&& filter, Sort&& sort)
{
    return CmdDB_Find<Filter, DefaultSort>(std::move(db), std::move(collection), {}, {}, std::forward<Filter>(filter), std::forward<Sort>(sort));
}

template<typename Filter, typename Sort>
CmdDB_Find<Filter, DefaultSort> send_CmdDB_Find(std::string db, std::string collection, FindOptions const& findOpt, Filter&& filter, Sort&& sort)
{
    return CmdDB_Find<Filter, DefaultSort>(std::move(db), std::move(collection), {}, findOpt, std::forward<Filter>(filter), std::forward<Sort>(sort));
}

template<typename Filter, typename Sort>
CmdDB_Find<Filter, DefaultSort> send_CmdDB_Find(std::string db, std::string collection, Op_QueryOptions const& options, Filter&& filter, Sort&& sort)
{
    return CmdDB_Find<Filter, DefaultSort>(std::move(db), std::move(collection), options, {}, std::forward<Filter>(filter), std::forward<Sort>(sort));
}

template<typename Filter, typename Sort>
CmdDB_Find<Filter, DefaultSort> send_CmdDB_Find(std::string db, std::string collection, Op_QueryOptions const& options, FindOptions const& findOpt, Filter&& filter, Sort&& sort)
{
    return CmdDB_Find<Filter, DefaultSort>(std::move(db), std::move(collection), options, findOpt, std::forward<Filter>(filter), std::forward<Sort>(sort));
}

template<typename Sort>
CmdDB_Find<FindAll, Sort> send_CmdDB_FindAllSort(std::string db, std::string collection, Sort&& sort)
{
    return CmdDB_Find<FindAll, Sort>(std::move(db), std::move(collection), {}, {}, FindAll{}, std::forward<Sort>(sort));
}

template<typename Sort>
CmdDB_Find<FindAll, Sort> send_CmdDB_FindAllSort(std::string db, std::string collection, FindOptions const& findOpt, Sort&& sort)
{
    return CmdDB_Find<FindAll, Sort>(std::move(db), std::move(collection), {}, findOpt, FindAll{}, std::forward<Sort>(sort));
}

template<typename Sort>
CmdDB_Find<FindAll, Sort> send_CmdDB_FindAllSort(std::string db, std::string collection, Op_QueryOptions const& options, Sort&& sort)
{
    return CmdDB_Find<FindAll, Sort>(std::move(db), std::move(collection), options, {}, FindAll{}, std::forward<Sort>(sort));
}

template<typename Sort>
CmdDB_Find<FindAll, Sort> send_CmdDB_FindAllSort(std::string db, std::string collection, Op_QueryOptions const& options, FindOptions const&findOpt, Sort&& sort)
{
    return CmdDB_Find<FindAll, Sort>(std::move(db), std::move(collection), options, findOpt, FindAll{}, std::forward<Sort>(sort));
}

}

#endif
