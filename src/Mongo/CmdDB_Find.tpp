#ifndef THORSANVIL_DB_MONGO_CMD_DB_FIND_TPP
#define THORSANVIL_DB_MONGO_CMD_DB_FIND_TPP

#ifndef THORSANVIL_DB_MONGO_CMD_DB_FIND_H
#error  "This should only be included from CmdDB_Find.h"
#endif

namespace ThorsAnvil::DB::Mongo
{

template<typename Filter, typename Sort>
template<typename Opt, ValidCmdFindOption<Opt>>
Find<Filter, Sort>::Find(Opt&& options, std::string const& collection, Filter&& filter, Sort&& sort)
    : FindOptional(std::forward<Opt>(options))
    , find(collection)
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

}

#endif
