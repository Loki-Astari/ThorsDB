#ifndef THORSANVIL_DB_MONGO_OP_REPLY_TPP
#define THORSANVIL_DB_MONGO_OP_REPLY_TPP

#ifndef THORSANVIL_DB_MONGO_OP_REPLY_H
#error  "This should only be included from Op_Reply.h"
#endif

#include "ThorSerialize/JsonThor.h"
#include "ThorSerialize/BsonThor.h"

namespace ThorsAnvil::DB::Mongo
{

template<typename View>
Op_Reply<View>::Op_Reply(View&& view)
    : header(OpCode::OP_REPLY)
    , responseFlags(OP_ReplyFlag::empty)
    , cursorID(0)
    , startingFrom(0)
    , numberReturned(0)
    , documents(std::forward<View>(view))
{}

template<typename View>
std::istream& Op_Reply<View>::parse(std::istream& stream)
{
    stream >> header
           >> make_LE(responseFlags)
           >> make_LE(cursorID)
           >> make_LE(startingFrom)
           >> make_LE(numberReturned);
    if ((responseFlags & OP_ReplyFlag::CursorNotFound) == OP_ReplyFlag::CursorNotFound)
    {
        assert(numberReturned == 0);
        errorInfo.ok    = 0.0;
        errorInfo.code  = -1;
        errorInfo.$err  = "CursorNotFound";
    }
    else if ((responseFlags & OP_ReplyFlag::QueryFailure) == OP_ReplyFlag::QueryFailure)
    {
        assert(numberReturned == 1);
        numberReturned = 0;
        stream >> ThorsAnvil::Serialize::bsonImporter(errorInfo);
    }
    else
    {
        for (int loop = 0; loop < numberReturned; ++loop)
        {
            // documents.emplace_back();
            stream >> ThorsAnvil::Serialize::bsonImporter(documents[loop]);//.back());
        }
    }
    return stream;
}

template<typename View>
std::istream& Op_Reply<View>::parseAndThrow(std::istream& stream)
{
    parse(stream);
    if (!this->isOk())
    {
        throw MongoException(this->getHRErrorMessage(), errorInfo);
    }
    return stream;
}

template<typename View>
std::ostream& Op_Reply<View>::print(std::ostream& stream) const
{
    stream << header
           << make_LE(responseFlags)
           << make_LE(cursorID)
           << make_LE(startingFrom)
           << make_LE(numberReturned);
    if ((responseFlags & OP_ReplyFlag::CursorNotFound) == OP_ReplyFlag::CursorNotFound)
    {
        // Nothing
    }
    else if ((responseFlags & OP_ReplyFlag::QueryFailure) == OP_ReplyFlag::QueryFailure)
    {
        stream << ThorsAnvil::Serialize::bsonExporter(errorInfo);
    }
    else
    {
        for (int loop = 0; loop < numberReturned; ++loop)
        {
            stream << ThorsAnvil::Serialize::bsonExporter(documents[loop]);
        }
    }

    return stream;
}

template<typename View>
std::ostream& Op_Reply<View>::printHR(std::ostream& stream) const
{
    stream << make_hr(header)
           << "responseFlags: " << responseFlags << "\n"
           << "cursorID:      " << cursorID      << "\n"
           << "startingFrom:  " << startingFrom  << "\n"
           << "numberReturned:" << numberReturned<< "\n";
    if ((responseFlags & OP_ReplyFlag::CursorNotFound) == OP_ReplyFlag::CursorNotFound)
    {
        stream << "Error:         Cursor not found\n";
    }
    else if ((responseFlags & OP_ReplyFlag::QueryFailure) == OP_ReplyFlag::QueryFailure)
    {
        stream << "Error:         " << ThorsAnvil::Serialize::jsonExporter(errorInfo) << "\n";
    }
    else
    {
        stream << "documents:     " << ThorsAnvil::Serialize::jsonExporter(documents) << "\n";
    }

    return stream;
}

template<typename View>
Op_Reply<View>::operator bool() const
{
    return this->isOk();
}

template<typename View>
bool Op_Reply<View>::isOk()  const
{
    return ((responseFlags & (OP_ReplyFlag::QueryFailure | OP_ReplyFlag::CursorNotFound)) == OP_ReplyFlag::empty)
        && (errorInfo.ok == 1.0);
}

template<typename View>
std::string Op_Reply<View>::getHRErrorMessage() const
{
    std::string result = "Op_Reply: ";
    if (!this->isOk())
    {
        result += errorInfo.getHRErrorMessage();
    }
    else
    {
        result += "OK";
    }
    return result;
}

inline
std::string ErrorInfo::getHRErrorMessage() const
{
    std::string result;

    result += std::to_string(code);
    result += ": ";
    result += codeName;
    result += ": ";
    result += $err;

    return result;
}

template<typename View>
std::ostream& operator<<(std::ostream& stream, Op_Reply<View> const& reply)
{
    return reply.print(stream);
}

template<typename View>
std::istream& operator>>(std::istream& stream, Op_Reply<View>&  reply)
{
    return reply.parse(stream);
}

template<typename View>
std::istream& operator>>(std::istream& stream, Op_Reply<View>&& reply)
{
    return reply.parseAndThrow(stream);
}

template<typename Range>
Op_Reply<ViewType<Range>> get_Op_Reply(Range&& range)
{
    return Op_Reply<ViewType<Range>>(make_XView(std::forward<Range>(range)));
}

#if 0
template<typename Object, ValidSingle<Object>>
Op_Reply<Object> get_Op_Reply(Object& object)
{
    return Op_Reply<Object>(object);
}

template<typename Range, ValidContainer<Range>>
Op_Reply<Range> get_Op_Reply(Range& container)
{
    return Op_Reply<Range>(container);// | FilterBackInserter{});
}
#endif

}

#endif
