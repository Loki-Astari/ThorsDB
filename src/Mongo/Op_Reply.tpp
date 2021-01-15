#ifndef THORSANVIL_DB_MONGO_OP_REPLY_TPP
#define THORSANVIL_DB_MONGO_OP_REPLY_TPP

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
    for (int loop = 0; loop < numberReturned; ++loop)
    {
        documents.emplace_back();
        stream >> ThorsAnvil::Serialize::bsonImporter(documents.back());
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
           << "numberReturned:" << numberReturned<< "\n"
           << ThorsAnvil::Serialize::jsonExporter(documents) << "\n";

    return stream;
}

template<typename Document>
Op_Reply<Document>::operator bool() const
{
    return (responseFlags & OP_ReplyFlag::QueryFailure) == OP_ReplyFlag::empty;
}

template<typename Document>
std::string const& Op_Reply<Document>::getFailureMessage()  const
{
    return getDocument(0).$err;
}

}
#endif
