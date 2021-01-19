#ifndef THORSANVIL_DB_MONGO_OP_REPLY_TPP
#define THORSANVIL_DB_MONGO_OP_REPLY_TPP

#ifndef THORSANVIL_DB_MONGO_OP_REPLY_H
#error  "This should only be included from Op_Reply.h"
#endif

#include "ThorSerialize/JsonThor.h"
#include "ThorSerialize/BsonThor.h"

namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
#pragma vera-pushoff
std::istream& Op_Reply<Document>::parse(std::istream& stream)
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
            documents.emplace_back();
            stream >> ThorsAnvil::Serialize::bsonImporter(documents.back());
        }
    }

    return stream;
}

template<typename Document>
std::ostream& Op_Reply<Document>::printHR(std::ostream& stream) const
{
    stream << make_hr(header)
           << "responseFlags: " << static_cast<std::int32_t>(responseFlags) << "\n"
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

template<typename Document>
Op_Reply<Document>::operator bool() const
{
    return this->isOk();
}

template<typename Document>
bool Op_Reply<Document>::isOk()  const
{
    return (responseFlags & (OP_ReplyFlag::QueryFailure | OP_ReplyFlag::CursorNotFound)) == OP_ReplyFlag::empty;
}

template<typename Document>
std::string Op_Reply<Document>::getHRErrorMessage() const
{
    std::string result = "Op_Reply: ";
    if (errorInfo.codeName.size() != 0 || errorInfo.$err.size() != 0)
    {
        result += std::to_string(errorInfo.code);
        result += ": ";
        result += errorInfo.codeName;
        result += ": ";
        result += errorInfo.$err;
    }
    else
    {
        result += "OK";
    }
    return result;
}

}
#endif
