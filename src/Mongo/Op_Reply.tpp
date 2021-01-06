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
    std::int32_t    cursorIDLow;
    std::int32_t    cursorIDHig;
    stream >> header
           >> make_LE(responseFlags)
           >> make_LE(cursorIDLow) >> make_LE(cursorIDHig)
           >> make_LE(startingFrom)
           >> make_LE(numberReturned);
    cursorID = (static_cast<std::int64_t>(cursorIDHig) << 32) | (static_cast<std::int64_t>(cursorIDLow) << 0);
    std::size_t size = header.getMessageLength();
    size -= (sizeof(header) + sizeof(responseFlags) + sizeof(cursorID) + sizeof(startingFrom) + sizeof(numberReturned));
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
           << ThorsAnvil::Serialize::jsonExporter(documents);

    return stream;
}

}
#endif
