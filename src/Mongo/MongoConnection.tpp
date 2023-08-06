#ifndef THORSANVIL_DB_MONGO_MONGO_CONNECTION_TPP
#define THORSANVIL_DB_MONGO_MONGO_CONNECTION_TPP

#ifndef THORSANVIL_DB_MONGO_MONGO_CONNECTION_H
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
        {}
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
            if (getMore.getQuery().getMore == -1)
            {
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
    CursorExtractor<T>  setCursor(value, *this);

    // Make sure there is enough room in the stream buffer
    // so that we don't flush before compression or crc calculations.
    MongoBuffer& buffer = dynamic_cast<MongoBuffer&>(*stream.rdbuf());
    buffer.resizeOutputBuffer(value.getMessageLength() + 5);

    value.setCompression(compression);

    stream << std::forward<T>(value) << std::flush;
    return *this;
}

template<typename T>
struct CursorRetriever
{
    static CursorId getCursorId(T const& /*reply*/)                         {return 0;}
    static std::uint32_t begin(T const&)                                    {return 0;}
    static std::uint32_t end(T const&)                                      {return 0;}
};
template<typename Reply>
struct CursorRetriever<Op_Reply<Reply>>
{
    static CursorId getCursorId(Op_Reply<Reply> const& reply)               {return reply.cursorID;}
    static std::uint32_t begin(Op_Reply<Reply> const& reply)                {return reply.startingFrom;}
    static std::uint32_t end(Op_Reply<Reply> const& reply)                  {return reply.startingFrom + reply.numberReturned;}
};
template<typename Cursor>
struct CursorRetriever<CmdDB_FindReplyBase<Cursor>>
{
    static CursorId getCursorId(CmdDB_FindReplyBase<Cursor> const& reply)   {return reply.reply.cursor.id;}
    static std::uint32_t begin(CmdDB_FindReplyBase<Cursor> const&)          {return 0;}
    static std::uint32_t end(CmdDB_FindReplyBase<Cursor> const&)            {return 0;}
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

        find.first     = std::min(find.first, CursorRetriever<T>::begin(value));
        find.second    = std::max(find.second, CursorRetriever<T>::end(value));
    }
    return *this;
}

}

#endif
