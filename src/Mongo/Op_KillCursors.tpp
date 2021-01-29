#ifndef THORSANVIL_DB_MONGO_OP_KILLCURSORS_TPP
#define THORSANVIL_DB_MONGO_OP_KILLCURSORS_TPP

#ifndef THORSANVIL_DB_MONGO_OP_KILLCURSORS_H
#error  "This should only be included from Op_KillCursors.h"
#endif

namespace ThorsAnvil::DB::Mongo
{

template<typename... Docs>
Op_KillCursors::Op_KillCursors(Op_Reply<Docs> const&... replys)
    : header(OpCode::OP_KILL_CURSORS)
    , numberOfCursorIDs(sizeof...(replys))
{
    cursorIDs.reserve(sizeof...(replys));
    (cursorIDs.emplace_back(replys.cursorID),...);
    header.prepareToSend(getSize());
}

inline
Op_KillCursors::Op_KillCursors(bool all)
    : header(OpCode::OP_KILL_CURSORS)
    , numberOfCursorIDs(all ? -2 : -1)
{
    // Update CursorExtractor<Op_KillCursors>
    // As this will change the size of the object.
}

inline
std::size_t Op_KillCursors::getSize() const
{
    std::size_t objectSize = sizeof(std::int32_t)
                           + sizeof(numberOfCursorIDs)
                           + (cursorIDs.size() * sizeof(CursorId));
    return objectSize;
}

inline
std::ostream& Op_KillCursors::print(std::ostream& stream) const
{
    stream << header
           << make_LE(std::int32_t{0})
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
