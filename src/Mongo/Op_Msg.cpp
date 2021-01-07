#include "Op_Msg.h"

#ifdef THOR_COVERAGE
#ifdef COVERAGE_Mongo

#include "Op_Msg.tpp"
#include "HandShake.h"
#include "test/OpTest.h"
#include "ThorSerialize/CustomSerialization.h"
#include "ThorSerialize/CustomSerialization.tpp"
using namespace ThorsAnvil::DB::Mongo;

// Constructor
template ThorsAnvil::DB::Mongo::Op_Msg<Kind0<SimpleStringNoConstructor>>::Op_Msg(std::string&&);
template ThorsAnvil::DB::Mongo::Op_Msg<Kind0<SimpleStringWithConstructor>>::Op_Msg(std::string&&);
template ThorsAnvil::DB::Mongo::Op_Msg<Kind0<StringAndIntNoConstructor>>::Op_Msg(StringAndIntNoConstructor&&);
template ThorsAnvil::DB::Mongo::Op_Msg<Kind0<StringAndIntWithConstructor>>::Op_Msg(StringAndIntWithConstructor&&);

// Stream Binary
template std::ostream& ThorsAnvil::DB::Mongo::Op_Msg<Kind0<SimpleStringNoConstructor>>::print(std::ostream&);
template std::ostream& ThorsAnvil::DB::Mongo::Kind0<SimpleStringNoConstructor>::print(std::ostream&) const;
template void ThorsAnvil::Serialize::Serializer::print<SimpleStringNoConstructor>(SimpleStringNoConstructor const&);

// Stream HR
template std::ostream& ThorsAnvil::DB::Mongo::Op_Msg<Kind0<SimpleStringNoConstructor>>::printHR(std::ostream&);

// Op_MsgHandShake
template void ThorsAnvil::Serialize::Serializer::print<Driver>(Driver const&);
template void ThorsAnvil::Serialize::Serializer::print<OS>(OS const&);
template void ThorsAnvil::Serialize::Serializer::print<Application>(Application const&);
template void ThorsAnvil::Serialize::Serializer::print<Client>(Client const&);
template void ThorsAnvil::Serialize::Serializer::print<HandShake>(HandShake const&);

// Auth
#pragma vera-pushoff
using namespace ThorsAnvil::Serialize;
#pragma vera-pop

template std::ostream& ThorsAnvil::DB::Mongo::Op_Msg<Kind0<AuthInit>>::print(std::ostream&);
template ThorsAnvil::DB::Mongo::Op_Msg<Kind0<AuthInit>>::Op_Msg(AuthInit&&);

template std::size_t DefaultCustomSerializer<Binary>::getPrintSizeBson(BsonPrinter&, Binary const&) const;
template void DefaultCustomSerializer<Binary>::writeCustom(PrinterInterface&, Binary const&) const;
template void DefaultCustomSerializer<Binary>::readBson(BsonParser&, char, Binary&) const;
template void DefaultCustomSerializer<Binary>::readJson(JsonParser&, Binary&) const;
template void DefaultCustomSerializer<Binary>::readYaml(YamlParser&, Binary&) const;
template void DefaultCustomSerializer<Binary>::writeBson(BsonPrinter&, Binary const&) const;
template void DefaultCustomSerializer<Binary>::writeJson(JsonPrinter&, Binary const&) const;
template void DefaultCustomSerializer<Binary>::writeYaml(YamlPrinter&, Binary const&) const;

template ThorsAnvil::DB::Mongo::Op_Msg<Kind0<AuthInitReply>>::Op_Msg();
template std::istream& ThorsAnvil::DB::Mongo::Op_Msg<Kind0<AuthInitReply>>::parse(std::istream&);
template std::ostream& ThorsAnvil::DB::Mongo::Op_Msg<Kind0<AuthInitReply>>::printHR(std::ostream&);

template ThorsAnvil::DB::Mongo::Op_Msg<Kind0<AuthContinue>>::Op_Msg(AuthContinue&&);
template std::ostream& ThorsAnvil::DB::Mongo::Op_Msg<Kind0<AuthContinue>>::print(std::ostream&);

#endif
#endif
