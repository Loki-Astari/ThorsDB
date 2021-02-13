#ifndef THORSANVIL_DB_MONGO_CMD_DB_FIND_COMMON_H
#define THORSANVIL_DB_MONGO_CMD_DB_FIND_COMMON_H

namespace ThorsAnvil::DB::Mongo
{

// Template classes for Find/FindModify
struct FindAll      {};     // Used as the default Find   template parameter. Finds All.
struct DefaultSort  {};     // Used as the default Sort   template parameter. No sort applied.
struct NoUpdate     {};     // Used as the default Update template parameter. No update applied.

}

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::FindAll);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::DefaultSort);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::NoUpdate);

#endif
