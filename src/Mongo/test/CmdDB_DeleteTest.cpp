
#include <gtest/gtest.h>
#include "CmdDB_Delete.h"

struct User
{
    std::string     user;
    std::string     password;
    int             age;
};
ThorsAnvil_MakeTrait(User, user, password, age);


TEST(CmdDB_DeleteTest, Base)
{
    User   user{"M1", "P1", 25};
    auto insert = make_CmdDB_Delete("database", "collection", ThorsAnvil::DB::Mongo::OP_QueryFlag::empty, user);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(22, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_DeleteTest, BaseUnordered)
{
    User   user{"M1", "P1", 25};
    auto insert = make_CmdDB_Delete("database", "collection", ThorsAnvil::DB::Mongo::OP_QueryFlag::empty, user).unordered();

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(23, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_DeleteTest, BaseSetWrieConcern)
{
    User   user{"M1", "P1", 25};
    auto insert = make_CmdDB_Delete("database", "collection", ThorsAnvil::DB::Mongo::OP_QueryFlag::empty, user).setWrieConcern(2, false, 400);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(28, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_DeleteTest, BaseUnorderedWrieConcern)
{
    User   user{"M1", "P1", 25};
    auto insert = make_CmdDB_Delete("database", "collection", ThorsAnvil::DB::Mongo::OP_QueryFlag::empty, user).unordered().setWrieConcern(2, false, 400);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(29, std::count(std::begin(result), std::end(result), '\n'));
}

