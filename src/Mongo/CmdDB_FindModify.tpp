#ifndef THORS_ANVIL_DB_MONGO_CMD_DB_FIND_MODIFY_TPP
#define THORS_ANVIL_DB_MONGO_CMD_DB_FIND_MODIFY_TPP

#ifndef THORS_ANVIL_DB_MONGO_CMD_DB_FIND_MODIFY_H
#error  "This should only be included from CmdDB_FindModify.h"
#endif

namespace ThorsAnvil::DB::Mongo
{

template<typename Find, typename Sort, typename Update>
FindModify<Find, Sort, Update>::FindModify(FindModifyOptions const& options, std::string collection, bool remove, Find&& find, Sort&& sort)
    : FindModifyOptional(options)
    , findAndModify(std::move(collection))
    , query(std::forward<Find>(find))
    , sort(std::forward<Sort>(sort))
    , remove(remove)
{
    filter["query"]     = !std::is_same_v<FindAll, Base<Find>>;
    filter["sort"]      = !std::is_same_v<DefaultSort, Base<Sort>>;
    filter["remove"]    = true;
}

template<typename Find, typename Sort, typename Update>
FindModify<Find, Sort, Update>::FindModify(FindModifyOptions const& options, std::string collection, Update&& update, Find&& find, Sort&& sort)
    : FindModifyOptional(options)
    , findAndModify(std::move(collection))
    , query(std::forward<Find>(find))
    , sort(std::forward<Sort>(sort))
    , remove(false)
    , update(std::forward<Update>(update))
{
    filter["query"]     = !std::is_same_v<FindAll, Base<Find>>;
    filter["sort"]      = !std::is_same_v<DefaultSort, Base<Sort>>;
    filter["update"]    = true;
}

template<typename Find, typename Sort>
CmdDB_FindModify<Find, Sort, NoUpdate> send_CmdDB_FindDelete(std::string db, std::string collection, Find&& find, Sort&& sort)
{
    return CmdDB_FindModify<Find, Sort, NoUpdate>(std::move(db), std::move(collection), {}, {}, true, std::forward<Find>(find), std::forward<Sort>(sort));
}

template<typename Find, typename Sort>
CmdDB_FindModify<Find, Sort, NoUpdate> send_CmdDB_FindDelete(std::string db, std::string collection, FindModifyOptions const& findModOpt, Find&& find, Sort&& sort)
{
    return CmdDB_FindModify<Find, Sort, NoUpdate>(std::move(db), std::move(collection), {}, findModOpt, true, std::forward<Find>(find), std::forward<Sort>(sort));
}

template<typename Find, typename Sort>
CmdDB_FindModify<Find, Sort, NoUpdate> send_CmdDB_FindDelete(std::string db, std::string collection, Op_QueryOptions const& options, Find&& find, Sort&& sort)
{
    return CmdDB_FindModify<Find, Sort, NoUpdate>(std::move(db), std::move(collection), options, {}, true, std::forward<Find>(find), std::forward<Sort>(sort));
}

template<typename Find, typename Sort>
CmdDB_FindModify<Find, Sort, NoUpdate> send_CmdDB_FindDelete(std::string db, std::string collection, Op_QueryOptions const& options, FindModifyOptions const& findModOpt, Find&& find, Sort&& sort)
{
    return CmdDB_FindModify<Find, Sort, NoUpdate>(std::move(db), std::move(collection), options, findModOpt, true, std::forward<Find>(find), std::forward<Sort>(sort));
}


template<typename Update, typename Find, typename Sort>
CmdDB_FindModify<Find, Sort, Update> send_CmdDB_FindUpdate(std::string db, std::string collection, Update&& update, Find&& find, Sort&& sort)
{
    return CmdDB_FindModify<Find, Sort, Update>(std::move(db), std::move(collection), {}, {}, std::forward<Update>(update), std::forward<Find>(find), std::forward<Sort>(sort));
}

template<typename Update, typename Find, typename Sort>
CmdDB_FindModify<Find, Sort, Update> send_CmdDB_FindUpdate(std::string db, std::string collection, FindModifyOptions const& findModOpt, Update&& update, Find&& find, Sort&& sort)
{
    return CmdDB_FindModify<Find, Sort, Update>(std::move(db), std::move(collection), {}, findModOpt, std::forward<Update>(update), std::forward<Find>(find), std::forward<Sort>(sort));
}

template<typename Update, typename Find, typename Sort>
CmdDB_FindModify<Find, Sort, Update> send_CmdDB_FindUpdate(std::string db, std::string collection, Op_QueryOptions const& options, Update&& update, Find&& find, Sort&& sort)
{
    return CmdDB_FindModify<Find, Sort, Update>(std::move(db), std::move(collection), options, {}, std::forward<Update>(update), std::forward<Find>(find), std::forward<Sort>(sort));
}

template<typename Update, typename Find, typename Sort>
CmdDB_FindModify<Find, Sort, Update> send_CmdDB_FindUpdate(std::string db, std::string collection, Op_QueryOptions const& options, FindModifyOptions const& findModOpt, Update&& update, Find&& find, Sort&& sort)
{
    return CmdDB_FindModify<Find, Sort, Update>(std::move(db), std::move(collection), options, findModOpt, std::forward<Update>(update), std::forward<Find>(find), std::forward<Sort>(sort));
}

}

#endif
