
#include <gtest/gtest.h>
#include "CmdDB_Delete.h"

struct User
{
    std::string     user;
    std::string     password;
    int             age;
};
ThorsAnvil_MakeTrait(User, user, password, age);

using ThorsAnvil::DB::Mongo::make_CmdDB_Delete;

TEST(CmdDB_DeleteTest, Base)
{
    User   user{"M1", "P1", 25};
    auto insert = make_CmdDB_Delete("database", "collection", {}, user);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(22, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_DeleteTest, BaseUnordered)
{
    User   user{"M1", "P1", 25};
    auto insert = make_CmdDB_Delete("database", "collection", {}, user).unordered();

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(23, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_DeleteTest, BaseSetWrieConcern)
{
    User   user{"M1", "P1", 25};
    auto insert = make_CmdDB_Delete("database", "collection", {}, user).setWrieConcern(2, false, 400);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(28, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_DeleteTest, BaseUnorderedWrieConcern)
{
    User   user{"M1", "P1", 25};
    auto insert = make_CmdDB_Delete("database", "collection", {}, user).unordered().setWrieConcern(2, false, 400);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(29, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_DeleteTest, BaseUnorderedOptions)
{
    User   user{"M1", "P1", 25};
    auto insert = make_CmdDB_Delete("database", "collection", {{}, {.ordered = false}}, user);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(23, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_DeleteTest, BaseSetWrieConcernOptions)
{
    User   user{"M1", "P1", 25};
    auto insert = make_CmdDB_Delete("database", "collection", {{}, {.writeConcern{2, false, 400}}}, user);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(28, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_DeleteTest, BaseUnorderedWrieConcernOptions)
{
    User   user{"M1", "P1", 25};
    auto insert = make_CmdDB_Delete("database", "collection", {{}, {.ordered = false, .writeConcern{2, false, 400}}}, user);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(29, std::count(std::begin(result), std::end(result), '\n'));
}

