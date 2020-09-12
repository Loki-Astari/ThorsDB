#include "Op_Msg.h"

#ifdef THOR_COVERAGE
#ifdef COVERAGE_Mongo

#include "Op_Msg.tpp"
#include "HandShake.h"
#include "test/OpTest.h"
#include "test/OpTest.h"
using namespace ThorsAnvil::DB::Mongo;

// Constructor
template ThorsAnvil::DB::Mongo::OP_Msg<Kind0<SimpleStringNoConstructor>>::OP_Msg(std::string&&);
template ThorsAnvil::DB::Mongo::OP_Msg<Kind0<SimpleStringWithConstructor>>::OP_Msg(std::string&&);
template ThorsAnvil::DB::Mongo::OP_Msg<Kind0<StringAndIntNoConstructor>>::OP_Msg(StringAndIntNoConstructor&&);
template ThorsAnvil::DB::Mongo::OP_Msg<Kind0<StringAndIntWithConstructor>>::OP_Msg(StringAndIntWithConstructor&&);

// Stream Binary
template std::ostream& ThorsAnvil::DB::Mongo::OP_Msg<Kind0<SimpleStringNoConstructor>>::print(std::ostream&);
template std::ostream& ThorsAnvil::DB::Mongo::Kind0<SimpleStringNoConstructor>::print(std::ostream&) const;
template void ThorsAnvil::Serialize::Serializer::print<SimpleStringNoConstructor>(SimpleStringNoConstructor const&);

// Stream HR
template std::ostream& ThorsAnvil::DB::Mongo::OP_Msg<Kind0<SimpleStringNoConstructor>>::printHR(std::ostream&);

// Op_MsgHandShake
template void ThorsAnvil::Serialize::Serializer::print<Driver>(Driver const&);
template void ThorsAnvil::Serialize::Serializer::print<OS>(OS const&);
template void ThorsAnvil::Serialize::Serializer::print<Application>(Application const&);
template void ThorsAnvil::Serialize::Serializer::print<Client>(Client const&);
template void ThorsAnvil::Serialize::Serializer::print<HandShake>(HandShake const&);
#endif
#endif
