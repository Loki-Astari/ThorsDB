#ifndef THORSANVIL_DB_MONGO_OP_KILLCURSORS_TPP
#define THORSANVIL_DB_MONGO_OP_KILLCURSORS_TPP

#include "ThorSerialize/Traits.h"
#include "ThorSerialize/BsonThor.h"
#include "ThorSerialize/JsonThor.h"

namespace ThorsAnvil::DB::Mongo
{

inline
Op_KillCursors::Op_KillCursors(std::initializer_list<std::int64_t> const& cursors)
    : header(OpCode::OP_KILL_CURSORS)
    , numberOfCursorIDs(cursors.size())
    , cursorIDs(cursors)
{
    header.prepareToSend(getSize());
}

inline
std::size_t Op_KillCursors::getSize() const
{
    std::size_t objectSize = sizeof(std::int32_t)
                           + sizeof(numberOfCursorIDs)
                           + (cursorIDs.size() * sizeof(std::int64_t));
    return objectSize;
}

inline
std::ostream& Op_KillCursors::print(std::ostream& stream) const
{
    stream << header
           << make_LE(0)
           << make_LE(numberOfCursorIDs);
    for (auto const& val: cursorIDs)
    {
           stream << make_LE(val);
    }
    return stream;
}

inline
std::ostream& Op_KillCursors::printHR(std::ostream& stream) const
{
    stream << make_hr(header)
           << "ZERO:                " << 0 << " (reserved)\n"
           << "numberOfCursorIDs:   " << numberOfCursorIDs << "\n"
           << "cursorIDs:           [";
    for (auto const& val: cursorIDs)
    {
           stream << val;
    }
    stream << "]\n";
    return stream;
}

}

#endif
