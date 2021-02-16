#ifndef THORS_ANVIL_DB_MONGO_MONGO_CONNECTION_TPP
#define THORS_ANVIL_DB_MONGO_MONGO_CONNECTION_TPP

#ifndef THORS_ANVIL_DB_MONGO_MONGO_CONNECTION_H
#error  "This should only be included from MongoConnection.h"
#endif

#include "Op_GetMore.h"
#include "Op_KillCursors.h"
#include "CmdDB_GetMore.h"
#include <algorithm>

namespace ThorsAnvil::DB::Mongo
{

template<typename T>
class CursorExtractor
{
    public:
        CursorExtractor(T& /*value*/, MongoConnection& /*connection*/)
        {
            std::cerr << "\tDo Nothing Cursor Extractor\n";
        }
};

template<>
class CursorExtractor<Op_GetMore>
{
    Op_GetMore&     getMore;
    bool            gotLastCursor;
    public:
        CursorExtractor(Op_GetMore& value, MongoConnection& connection)
            : getMore(value)
            , gotLastCursor(false)
        {
            std::cerr << "\tOp_GetMore Cursor Extractor\n";
            if (getMore.cursorID == -1)
            {
                gotLastCursor = true;
                getMore.cursorID = connection.getLastOpenCursor();
                getMore.header.prepareToSend(getMore.getSize());
            }
        }
        ~CursorExtractor()
        {
            if (gotLastCursor)
            {
                getMore.cursorID = -1;
            }
        }
};
template<>
class CursorExtractor<CmdDB_GetMore>
{
    CmdDB_GetMore&     getMore;
    bool            gotLastCursor;
    public:
        CursorExtractor(CmdDB_GetMore& value, MongoConnection& connection)
            : getMore(value)
            , gotLastCursor(false)
        {
            std::cerr << "\tCmdDB_GetMore Cursor Extractor\n";
            std::cerr << "\tCurrent: " << getMore.getQuery().getMore << "\n";
            if (getMore.getQuery().getMore == -1)
            {
                std::cerr << "\t\tWe need to extract\n"
                          << "\t\t" << connection.getLastOpenCursor() << "\n";

                gotLastCursor = true;
                getMore.getQuery().getMore = connection.getLastOpenCursor();
            }
        }
        ~CursorExtractor()
        {
            if (gotLastCursor)
            {
                getMore.getQuery().getMore = -1;
            }
        }
};
template<>
class CursorExtractor<Op_KillCursors>
{
    Op_KillCursors& killCursor;
    int             gotLastCursor;
    public:
        CursorExtractor(Op_KillCursors& value, MongoConnection& connection)
            : killCursor(value)
            , gotLastCursor(0)
        {
            std::cerr << "\tOp_KillCursor Cursor Extractor\n";
            if (killCursor.numberOfCursorIDs == -1)
            {
                gotLastCursor = -1;
                killCursor.cursorIDs.push_back(connection.getLastOpenCursor(true));
            }
            if (killCursor.numberOfCursorIDs == -2)
            {
                gotLastCursor = -2;
                killCursor.cursorIDs = connection.getAllOpenCursors(true);
            }
            killCursor.header.prepareToSend(killCursor.getSize());
        }
        ~CursorExtractor()
        {
            if (gotLastCursor)
            {
                killCursor.numberOfCursorIDs = gotLastCursor;
            }
        }
};

template<typename T>
MongoConnection& MongoConnection::operator<<(T&& value)
{
    std::cerr << "Pulling last cursor ID\n";
    CursorExtractor<T>  setCursor(value, *this);
    std::cerr << "Sending Data to Connection\n";
    stream << std::forward<T>(value) << std::flush;
    std::cerr << "All Send\n";
    return *this;
}

template<typename Reply>
struct CursorRetriever
{
    static CursorId getCursorId(Reply const& reply)
    {
        std::cerr << "Op Reply Get Cursor\n";
        return reply.cursorID;
    }
};
template<typename Cursor>
struct CursorRetriever<CmdDB_FindReplyBase<Cursor>>
{
    static CursorId getCursorId(CmdDB_FindReplyBase<Cursor> const& reply)
    {
        std::cerr << "CmdDB Reply Get Cursor\n";
        return reply.reply.cursor.id;
    }
};

template<typename T>
MongoConnection& MongoConnection::operator>>(T&& value)
{
    stream >> std::forward<T>(value);

    CursorId cursor = CursorRetriever<T>::getCursorId(value);

    if (cursor != 0)
    {
        lastCursor = cursor;
        auto& find = openCursors[lastCursor];

        find.first     = std::min(find.first, value.startingFrom);
        find.second    = std::max(find.second, (value.startingFrom + value.numberReturned));
    }
    return *this;
}

}

#endif
