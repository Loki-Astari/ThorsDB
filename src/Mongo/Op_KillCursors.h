#ifndef THORSANVIL_DB_MONGO_OP_KILLCURSORS_H
#define THORSANVIL_DB_MONGO_OP_KILLCURSORS_H

#include "Op.h"
#include "MsgHeader.h"
#include "ThorSerialize/Traits.h"
#include <ostream>
#include <initializer_list>

// https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/#op-kill-cursors
namespace ThorsAnvil::DB::Mongo
{

class Op_KillCursors
{
    MsgHeader                   header;
    // std::int32_t                zero;               // 0 reserved for future use
    std::int32_t                numberOfCursorIDs;
    std::vector<std::int64_t>   cursorIDs;
    public:
        Op_KillCursors(std::initializer_list<std::int64_t> const& cursors);

        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<Op_KillCursors> const& data);
        friend std::ostream& operator<<(std::ostream& stream, Op_KillCursors const& data) {return data.print(stream);}
    private:
        std::size_t   getSize()                     const;
    protected:
        std::ostream& print(std::ostream& stream) const;
        std::ostream& printHR(std::ostream& stream) const;
};

}

#include "Op_KillCursors.tpp"

#endif
