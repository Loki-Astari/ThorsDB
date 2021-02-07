#ifndef THORSANVIL_DB_MONGO_CMD_DB_FIND_TPP
#define THORSANVIL_DB_MONGO_CMD_DB_FIND_TPP

#ifndef THORSANVIL_DB_MONGO_CMD_DB_FIND_H
#error  "This should only be included from CmdDB_Find.h"
#endif

namespace ThorsAnvil::DB::Mongo
{

template<typename Filter, typename Sort>
Find<Filter, Sort>::Find(FindOptions options, std::string collection, Filter&& filter, Sort&& sort)
    : FindOptional(std::move(options))
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

template<typename Document>
bool CmdDB_FindReply<Document>::isOk() const
{
    return Op_Reply<FindReply<Document>>::isOk() && findData.ok == 1.0;
}

}

#endif
