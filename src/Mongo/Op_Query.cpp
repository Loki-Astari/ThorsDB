#include "Op_Query.h"

#ifdef THOR_COVERAGE
#ifdef COVERAGE_Mongo

#include "Op_Query.tpp"
#include "Op_QueryHandShake.h"
#include "test/OpTest.h"
#include <ostream>

using namespace ThorsAnvil::DB::Mongo;

template ThorsAnvil::DB::Mongo::Op_Query<SimpleStringNoConstructor>::Op_Query(std::string const&);
template ThorsAnvil::DB::Mongo::Op_Query<SimpleStringNoConstructor>::Op_Query(std::string const&, std::string&&);
template ThorsAnvil::DB::Mongo::Op_Query<SimpleStringWithConstructor>::Op_Query(std::string const&, std::string&&);
template ThorsAnvil::DB::Mongo::Op_Query<StringAndIntNoConstructor>::Op_Query(std::string const&, std::string&&, int&&);
template ThorsAnvil::DB::Mongo::Op_Query<StringAndIntWithConstructor>::Op_Query(std::string const&, std::string&&, int&&);
template std::ostream& ThorsAnvil::DB::Mongo::Op_Query<StringAndIntNoConstructor>::print(std::ostream&);
template std::ostream& ThorsAnvil::DB::Mongo::Op_Query<StringAndIntNoConstructor>::printHR(std::ostream&);

// Connection Test
template ThorsAnvil::DB::Mongo::Op_Query<HandShake>::Op_Query(std::string const&, std::string&&);
template ThorsAnvil::DB::Mongo::Op_Query<HandShake>::Op_Query(std::string const&, std::string const&);
template std::ostream& ThorsAnvil::DB::Mongo::Op_Query<HandShake>::print(std::ostream&);
template std::ostream& ThorsAnvil::DB::Mongo::Op_Query<HandShake>::printHR(std::ostream&);
#endif
#endif
