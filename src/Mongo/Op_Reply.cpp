#include "Op_Reply.h"

#ifdef THOR_COVERAGE
#ifdef COVERAGE_Mongo

#include "Op_Reply.tpp"
#include "Op_ReplyHandShake.h"
#include "ThorSerialize/Serialize.tpp"

using namespace ThorsAnvil::DB::Mongo;

template std::istream& ThorsAnvil::DB::Mongo::Op_Reply<HandShakeReplyDoc>::parse(std::istream&);
template std::ostream& ThorsAnvil::DB::Mongo::Op_Reply<HandShakeReplyDoc>::printHR(std::ostream&);

template void ThorsAnvil::Serialize::Serializer::print<std::vector<HandShakeReplyDoc>>(std::vector<HandShakeReplyDoc> const&);
template void ThorsAnvil::Serialize::DeSerializer::parse<HandShakeReplyDoc>(HandShakeReplyDoc&);


#endif
#endif
