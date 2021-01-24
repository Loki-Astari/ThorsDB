
#include <gtest/gtest.h>
#include "CmdDB_Insert.h"

struct User
{
    std::string     user;
    std::string     password;
    int             age;
};
ThorsAnvil_MakeTrait(User, user, password, age);

using ThorsAnvil::DB::Mongo::make_CmdDB_Insert;

TEST(CmdDB_InsertTest, Base)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = make_CmdDB_Insert("database", "collection", {}, std::begin(users), std::end(users));

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(23, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_InsertTest, BaseUnordered)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = make_CmdDB_Insert("database", "collection", {}, std::begin(users), std::end(users)).unordered();

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(24, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_InsertTest, BaseBypass)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = make_CmdDB_Insert("database", "collection", {}, std::begin(users), std::end(users)).byPass();

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(24, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_InsertTest, BaseUnordredBypass)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = make_CmdDB_Insert("database", "collection", {}, std::begin(users), std::end(users)).unordered().byPass();

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(25, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_InsertTest, BaseSetWrieConcern)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = make_CmdDB_Insert("database", "collection", {}, std::begin(users), std::end(users)).setWrieConcern(2, false, 400);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(29, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_InsertTest, BaseUnorderedWrieConcern)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = make_CmdDB_Insert("database", "collection", {}, std::begin(users), std::end(users)).unordered().setWrieConcern(2, false, 400);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(30, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_InsertTest, BaseBypassWriteConcenr)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = make_CmdDB_Insert("database", "collection", {}, std::begin(users), std::end(users)).byPass().setWrieConcern(2, false, 400);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(30, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_InsertTest, BaseUnorderedBypassWriteConcenr)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = make_CmdDB_Insert("database", "collection", {}, std::begin(users), std::end(users)).unordered().byPass().setWrieConcern(2, false, 400);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(31, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = make_CmdDB_Insert("database", "collection", {}, std::begin(users), std::end(users)).setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(24, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_InsertTest, BaseUnorderedSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = make_CmdDB_Insert("database", "collection", {}, std::begin(users), std::end(users)).unordered().setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(25, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_InsertTest, BaseBypassSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = make_CmdDB_Insert("database", "collection", {}, std::begin(users), std::end(users)).byPass().setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(25, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_InsertTest, BaseUnordredBypassSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = make_CmdDB_Insert("database", "collection", {}, std::begin(users), std::end(users)).unordered().byPass().setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(26, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_InsertTest, BaseSetWrieConcernSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = make_CmdDB_Insert("database", "collection", {}, std::begin(users), std::end(users)).setWrieConcern(2, false, 400).setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(30, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_InsertTest, BaseUnorderedWrieConcernSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = make_CmdDB_Insert("database", "collection", {}, std::begin(users), std::end(users)).unordered().setWrieConcern(2, false, 400).setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(31, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_InsertTest, BaseBypassWriteConcenrSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = make_CmdDB_Insert("database", "collection", {}, std::begin(users), std::end(users)).byPass().setWrieConcern(2, false, 400).setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(31, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_InsertTest, BaseUnorderedBypassWriteConcenrSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = make_CmdDB_Insert("database", "collection", {}, std::begin(users), std::end(users)).unordered().byPass().setWrieConcern(2, false, 400).setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(32, std::count(std::begin(result), std::end(result), '\n'));
}

