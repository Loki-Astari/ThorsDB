
#include <gtest/gtest.h>
#include "CmdDB_Find.h"

TEST(CmdDB_FindTest, DBCollectionQuery)
{
    // Support all thes creations.
#if 0
    auto insert = make_CmdDB_Find("database", "collection", ThorsAnvil::DB::Mongo::QueryOptions{});
    auto insert = make_CmdDB_Find("database", "collection", ThorsAnvil::DB::Mongo::QueryOptions{}, 56);
    auto insert = make_CmdDB_Find("database", "collection", ThorsAnvil::DB::Mongo::QueryOptions{}, 56, 10);
    auto insert = make_CmdDB_Find("database", "collection", ThorsAnvil::DB::Mongo::QueryOptions{}, 56, 10, 101);

    auto insert = make_CmdDB_Find("database", "collection", ThorsAnvil::DB::Mongo::QueryOptions{}, {.name ="Martin"});
    auto insert = make_CmdDB_Find("database", "collection", ThorsAnvil::DB::Mongo::QueryOptions{}, {.name ="Martin"}, 56);
    auto insert = make_CmdDB_Find("database", "collection", ThorsAnvil::DB::Mongo::QueryOptions{}, {.name ="Martin"}, 56, 10);
    auto insert = make_CmdDB_Find("database", "collection", ThorsAnvil::DB::Mongo::QueryOptions{}, {.name ="Martin"}, 56, 10, 101);

    auto insert = make_CmdDB_Find("database", "collection", ThorsAnvil::DB::Mongo::QueryOptions{}, {.name ="Martin"}, {.sort="x"});
    auto insert = make_CmdDB_Find("database", "collection", ThorsAnvil::DB::Mongo::QueryOptions{}, {.name ="Martin"}, {.sort="x"}, 56);
    auto insert = make_CmdDB_Find("database", "collection", ThorsAnvil::DB::Mongo::QueryOptions{}, {.name ="Martin"}, {.sort="x"}, 56, 10);
    auto insert = make_CmdDB_Find("database", "collection", ThorsAnvil::DB::Mongo::QueryOptions{}, {.name ="Martin"}, {.sort="x"}, 56, 10, 101);

    auto insert = make_CmdDB_Find("database", "collection", ThorsAnvil::DB::Mongo::QueryOptions{}, {.sort="x"});
    auto insert = make_CmdDB_Find("database", "collection", ThorsAnvil::DB::Mongo::QueryOptions{}, {.sort="x"}, 56);
    auto insert = make_CmdDB_Find("database", "collection", ThorsAnvil::DB::Mongo::QueryOptions{}, {.sort="x"}, 56, 10);
    auto insert = make_CmdDB_Find("database", "collection", ThorsAnvil::DB::Mongo::QueryOptions{}, {.sort="x"}, 56, 10, 101);
#endif
}

