#ifndef THORSANVIL_DB_MONGO_OP_H
#define THORSANVIL_DB_MONGO_OP_H

#include <cstdint>

namespace ThorsAnvil::DB::Mongo
{

enum class OpCode : std::int32_t
{
    OP_REPLY        = 1,                    // Reply to a client request. responseTo is set.
    OP_UPDATE       = 2001,                 // Update document.
    OP_INSERT       = 2002,                 // Insert new document.
    // RESERVED        = 2003,                 // Formerly used for OP_GET_BY_OID.
    OP_QUERY        = 2004,                 // Query a collection.
    OP_GET_MORE     = 2005,                 // Get more data from a query. See Cursors.
    OP_DELETE       = 2006,                 // Delete documents.
    OP_KILL_CURSORS = 2007,                 // Notify database that the client has finished with the cursor.
    OP_COMPRESSED   = 2012,                 // Handled specially (see Op_MSgHeader.cpp)
    OP_MSG          = 2013,                 // Send a message using the format introduced in MongoDB 3.6.
};

using CursorId = std::int64_t;

}

#endif
