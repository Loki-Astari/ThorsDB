#ifndef THORS_ANVIL_DB_MONGO_MSG_INSERT_H
#define THORS_ANVIL_DB_MONGO_MSG_INSERT_H

#include "Op_Msg.h"
#include "CmdDB_Insert.h"

namespace ThorsAnvil::DB::Mongo
{


class DBInsert: public InsertOptional
{
    public:
        // using Options = InsertOptions;
        DBInsert(std::string db, InsertOptions const& options, std::string collection);

    private:
        friend class ThorsAnvil::Serialize::Traits<DBInsert>;

        std::string                 db;
        std::string                 insert;
};

DBInsert::DBInsert(std::string db, InsertOptions const& options, std::string collection)
    : InsertOptional(options)
    , db(std::move(db))
    , insert(std::move(collection))
{}

template<typename Range>
KindB<Range> createKindBRange(Range&& r)
{
    return KindB<Range>{"documents", ViewType<Range>{std::forward<Range>(r)}};
}

template<typename Range>
Op_Msg<DBInsert, Range> send_Msg_Insert(std::string db, std::string collection, Range&& r)
{
    return send_Op_Msg<DBInsert, Range>(OP_MsgFlag::empty, DBInsert{std::move(db), InsertOptions{}, std::move(collection)}, createKindBRange(std::forward<Range>(r)));
}

template<typename Range>
Op_Msg<DBInsert, Range> send_Msg_Insert(std::string db, std::string collection, InsertOptions const& options, Range&& r)
{
    return send_Op_Msg<DBInsert, Range>(OP_MsgFlag::empty, DBInsert{std::move(db), options, std::move(collection)}, createKindBRange(std::forward<Range>(r)));
}

template<typename Range>
Op_Msg<DBInsert, Range> send_Msg_Insert(std::string db, std::string collection, Op_QueryOptions const& options, Range&& r)
{
    return send_Op_Msg<DBInsert, Range>(OP_MsgFlag::empty, DBInsert{std::move(db), InsertOptions{}, std::move(collection)}, createKindBRange(std::forward<Range>(r)));
}

template<typename Range>
Op_Msg<DBInsert, Range> send_Msg_Insert(std::string db, std::string collection, Op_QueryOptions const& options, InsertOptions const& insertOpt, Range&& r)
{
    return send_Op_Msg<DBInsert, Range>(OP_MsgFlag::empty, DBInsert{std::move(db), insertOpt, std::move(collection)}, createKindBRange(std::forward<Range>(r)));
}

}


ThorsAnvil_ExpandTrait(ThorsAnvil::DB::Mongo::InsertOptional,  ThorsAnvil::DB::Mongo::DBInsert,        db, insert);

// #include "MsgInsert.tpp"

#endif
