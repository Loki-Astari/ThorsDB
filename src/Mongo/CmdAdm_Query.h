#ifndef THORS_ANVIL_DB_MONGO_CMD_ADM_QUERY_H
#define THORS_ANVIL_DB_MONGO_CMD_ADM_QUERY_H

// https://docs.mongodb.com/manual/reference/command/nav-crud/

/*
 * $    Usage:  CmdAdm_Query
 *
 * >        This is not designed to be used directly. See CmdAdm_ListDatabases.h for example.
 * >        It is expected for a CmdAdm that you create an appropriate typedef.
 */
#include "BaseCmd.h"
#include "Op_Query.h"

#include <string>
#include <iostream>

namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
class CmdAdm_Query: public Op_Query<Document>
{
    public:
        template<typename Doc = Document, NoOptions<Doc> = true, typename... Args>
        CmdAdm_Query(Args&&... args);
        template<typename Doc = Document, NoOptions<Doc> = true, typename... Args>
        CmdAdm_Query(Op_QueryOptions const& option, Args&&... args);
        template<typename Doc = Document, HasOptions<Doc> = true, typename... Args>
        CmdAdm_Query(Op_QueryOptions const& option, typename Doc::Options const& docOpt, Args&&... args);

        std::ostream& printHR(std::ostream& stream) const {return stream << make_hr(static_cast<Op_Query<Document> const&>(*this));}
};

template<typename Document>
std::ostream& operator<<(std::ostream& stream, CmdAdm_Query<Document> const& data);

}

#include "CmdAdm_Query.tpp"

#endif
