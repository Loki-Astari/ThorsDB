#include "Op_Msg.h"

#ifdef THOR_COVERAGE
#ifdef COVERAGE_Mongo

#include "Op_Msg.tpp"
#include "test/Op_MsgTest.h"
using namespace ThorsAnvil::DB::Mongo;

template ThorsAnvil::DB::Mongo::OP_Msg<Kind0<Kind0SimpleStringNoConstructor>>::OP_Msg(std::string&&);


#endif
#endif
