
#include <gtest/gtest.h>
#include "Op_Msg.h"
#include "test/Op_MsgTest.h"

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

TEST(Op_MsgTest, CreateOp_Msg)
{
    OP_Msg<Kind0<Kind0SimpleStringNoConstructor>>      message1("Data"s);
}

