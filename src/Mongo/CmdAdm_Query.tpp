#ifndef THORSANVIL_DB_MONGO_CMD_ADM_QUERY_TPP
#define THORSANVIL_DB_MONGO_CMD_ADM_QUERY_TPP

#ifndef THORSANVIL_DB_MONGO_CMD_ADM_QUERY_H
#error  "This should only be included from CmdAdm_Query.h"
#endif

namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
template<typename Doc, NoOptions<Doc>, typename... Args>
CmdAdm_Query<Document>::CmdAdm_Query(Op_QueryOptions const& options, Args&&... args)
    : Op_Query<Document>("admin.$cmd", options, std::forward<Args>(args)...)
{}

template<typename Document>
template<typename Doc, HasOptions<Doc>, typename... Args>
CmdAdm_Query<Document>::CmdAdm_Query(Op_QueryOptions const& options, typename Doc::Options const& docOpt, Args&&... args)
    : Op_Query<Document>("admin.$cmd", options, docOpt, std::forward<Args>(args)...)
{}

template<typename Document>
std::ostream& operator<<(std::ostream& stream, CmdAdm_Query<Document> const& data)
{
    return stream << static_cast<Op_Query<Document> const&>(data);
}

}

#endif
