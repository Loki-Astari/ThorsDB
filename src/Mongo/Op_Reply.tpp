#ifndef THORSANVIL_DB_MONGO_OP_REPLY_TPP
#define THORSANVIL_DB_MONGO_OP_REPLY_TPP

#ifndef THORSANVIL_DB_MONGO_OP_REPLY_H
#error  "This should only be included from Op_Reply.h"
#endif

#include "ThorSerialize/JsonThor.h"
#include "ThorSerialize/BsonThor.h"

namespace ThorsAnvil::DB::Mongo
{

template<typename Document, typename View>
Op_Reply<Document, View>::Op_Reply(Document& data)
    : header(OpCode::OP_REPLY)
    , responseFlags(OP_ReplyFlag::empty)
    , cursorID(0)
    , startingFrom(0)
    , numberReturned(0)
    , documents(make_XView(data))
{}

template<typename Document, typename View>
#pragma vera-pushoff
std::istream& Op_Reply<Document, View>::parse(std::istream& stream)
#pragma vera-pop
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

template<typename Document, typename View>
std::istream& Op_Reply<Document, View>::parseAndThrow(std::istream& stream)
{
    parse(stream);
    if (!this->isOk())
    {
        throw MongoException(this->getHRErrorMessage(), errorInfo);
    }
    return stream;
}

template<typename Document, typename View>
std::ostream& Op_Reply<Document, View>::print(std::ostream& stream) const
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

template<typename Document, typename View>
std::ostream& Op_Reply<Document, View>::printHR(std::ostream& stream) const
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

template<typename Document, typename View>
Op_Reply<Document, View>::operator bool() const
{
    return this->isOk();
}

template<typename Document, typename View>
bool Op_Reply<Document, View>::isOk()  const
{
    return ((responseFlags & (OP_ReplyFlag::QueryFailure | OP_ReplyFlag::CursorNotFound)) == OP_ReplyFlag::empty)
        && (errorInfo.ok == 1.0);
}

template<typename Document, typename View>
std::string Op_Reply<Document, View>::getHRErrorMessage() const
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

}

#endif
