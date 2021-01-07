#include "Op_Query.h"

#ifdef THOR_COVERAGE
#ifdef COVERAGE_Mongo

#include "Op_Query.tpp"
#include "HandShake.h"
#include "ThorSerialize/CustomSerialization.h"
#include "ThorSerialize/CustomSerialization.tpp"
#include "test/OpTest.h"
#include <ostream>

using namespace ThorsAnvil::DB::Mongo;

template ThorsAnvil::DB::Mongo::Op_Query<StringAndIntNoConstructor>::Op_Query(std::string const&, StringAndIntNoConstructor&);
template ThorsAnvil::DB::Mongo::Op_Query<HandShake>::Op_Query(std::string const&, HandShake&);
template std::ostream& ThorsAnvil::DB::Mongo::Op_Query<StringAndIntNoConstructor>::print(std::ostream&);
template std::ostream& ThorsAnvil::DB::Mongo::Op_Query<StringAndIntNoConstructor>::printHR(std::ostream&);

// Connection Test
template std::ostream& ThorsAnvil::DB::Mongo::Op_Query<HandShake>::print(std::ostream&);
template std::ostream& ThorsAnvil::DB::Mongo::Op_Query<HandShake>::printHR(std::ostream&);

// HAndShake
#pragma vera-pushoff
using namespace ThorsAnvil::Serialize;
#pragma vera-pop

int MongoUtility::ObjectID::classCounter;
template void DefaultCustomSerializer<MongoUtility::UTCDateTime>::readCustom(ParserInterface&, MongoUtility::UTCDateTime&) const;
template void DefaultCustomSerializer<MongoUtility::UTCDateTime>::writeCustom(PrinterInterface&, MongoUtility::UTCDateTime const&) const;
template std::size_t DefaultCustomSerializer<MongoUtility::UTCDateTime>::getPrintSizeBson(BsonPrinter&, MongoUtility::UTCDateTime const&) const;
template void DefaultCustomSerializer<MongoUtility::UTCDateTime>::readBson(BsonParser&, char, MongoUtility::UTCDateTime&) const;
template void DefaultCustomSerializer<MongoUtility::UTCDateTime>::readJson(JsonParser&, MongoUtility::UTCDateTime&) const;
template void DefaultCustomSerializer<MongoUtility::UTCDateTime>::readYaml(YamlParser&, MongoUtility::UTCDateTime&) const;
template void DefaultCustomSerializer<MongoUtility::UTCDateTime>::writeBson(BsonPrinter&, MongoUtility::UTCDateTime const&) const;
template void DefaultCustomSerializer<MongoUtility::UTCDateTime>::writeJson(JsonPrinter&, MongoUtility::UTCDateTime const&) const;
template void DefaultCustomSerializer<MongoUtility::UTCDateTime>::writeYaml(YamlPrinter&, MongoUtility::UTCDateTime const&) const;
template void DefaultCustomSerializer<MongoUtility::ObjectID>::readCustom(ParserInterface&, MongoUtility::ObjectID&) const;
template void DefaultCustomSerializer<MongoUtility::ObjectID>::writeCustom(PrinterInterface&, MongoUtility::ObjectID const&) const;
template std::size_t DefaultCustomSerializer<MongoUtility::ObjectID>::getPrintSizeBson(BsonPrinter&, MongoUtility::ObjectID const&) const;
template void DefaultCustomSerializer<MongoUtility::ObjectID>::readBson(BsonParser&, char, MongoUtility::ObjectID&) const;
template void DefaultCustomSerializer<MongoUtility::ObjectID>::readJson(JsonParser&, MongoUtility::ObjectID&) const;
template void DefaultCustomSerializer<MongoUtility::ObjectID>::readYaml(YamlParser&, MongoUtility::ObjectID&) const;
template void DefaultCustomSerializer<MongoUtility::ObjectID>::writeBson(BsonPrinter&, MongoUtility::ObjectID const&) const;
template void DefaultCustomSerializer<MongoUtility::ObjectID>::writeJson(JsonPrinter&, MongoUtility::ObjectID const&) const;
template void DefaultCustomSerializer<MongoUtility::ObjectID>::writeYaml(YamlPrinter&, MongoUtility::ObjectID const&) const;


#endif
#endif
