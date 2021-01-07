#include "Op_Msg.h"

#ifdef THOR_COVERAGE
#ifdef COVERAGE_Mongo

#include "Op_Msg.tpp"
#include "test/Op_MsgTest.h"
using namespace ThorsAnvil::DB::Mongo;

// Constructor
template ThorsAnvil::DB::Mongo::OP_Msg<Kind0<Kind0SimpleStringNoConstructor>>::OP_Msg(std::string&&);
template ThorsAnvil::DB::Mongo::OP_Msg<Kind0<Kind0SimpleStringWithConstructor>>::OP_Msg(std::string&&);
template ThorsAnvil::DB::Mongo::OP_Msg<Kind0<Kind0StringAndIntNoConstructor>>::OP_Msg(Kind0StringAndIntNoConstructor&&);
template ThorsAnvil::DB::Mongo::OP_Msg<Kind0<Kind0StringAndIntWithConstructor>>::OP_Msg(Kind0StringAndIntWithConstructor&&);

// Stream Binary
template std::ostream& ThorsAnvil::DB::Mongo::OP_Msg<Kind0<Kind0SimpleStringNoConstructor>>::print(std::ostream&);
template std::ostream& ThorsAnvil::DB::Mongo::Kind0<Kind0SimpleStringNoConstructor>::print(std::ostream&) const;
template void ThorsAnvil::Serialize::Serializer::print<Kind0SimpleStringNoConstructor>(Kind0SimpleStringNoConstructor const&);

// Stream HR
template std::ostream& ThorsAnvil::DB::Mongo::OP_Msg<Kind0<Kind0SimpleStringNoConstructor>>::printHR(std::ostream&);

#endif
#endif
