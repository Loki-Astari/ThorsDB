/*
 * These tests are weak.
 * WEAK: I threw them together eyeballed the results to make sure they were correct.
 *       The automation part was to make sure they don't change the line count of output
 *       as we modify the code. So it will work a lot of the time but will not work
 *       all of the time.
 *
 *      Yes I was lazy and threw them together
 */


#include <gtest/gtest.h>
#include "CmdDB_Insert.h"
#include "test/CommonInsertTest.h"


using namespace ThorsAnvil::DB::Mongo;

TEST(CmdDB_InsertTest, BaseSingleItem)
{
    User   user{"M1", "P1", 25};
    auto insert = send_CmdDB_Insert("database", "collection", {.ret = 10}, user);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(18, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_InsertTest, Base)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", {.ret = 10}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(23, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseUnordered)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", users).unordered();

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(24, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseBypass)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", {.ret = 10}, users).byPass();

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(24, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseUnordredBypass)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", users).unordered().byPass();

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(25, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseSetWrieConcern)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", {.ret = 10}, users).setWrieConcern(2, false, 400);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(29, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseUnorderedWrieConcern)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", users).unordered().setWrieConcern(2, false, 400);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(30, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseBypassWriteConcenr)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", {.ret = 10}, users).byPass().setWrieConcern(2, false, 400);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(30, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseUnorderedBypassWriteConcenr)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", users).unordered().byPass().setWrieConcern(2, false, 400);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(31, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", {.ret = 10}, users).setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(24, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseUnorderedSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", users).unordered().setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(25, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseBypassSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", {.ret = 10}, users).byPass().setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(25, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseUnordredBypassSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", users).unordered().byPass().setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(26, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseSetWrieConcernSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", {.ret = 10}, users).setWrieConcern(2, false, 400).setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(30, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseUnorderedWrieConcernSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", users).unordered().setWrieConcern(2, false, 400).setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(31, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseBypassWriteConcenrSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", {.ret = 10}, users).byPass().setWrieConcern(2, false, 400).setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(31, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseUnorderedBypassWriteConcenrSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", users).unordered().byPass().setWrieConcern(2, false, 400).setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(32, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseUnorderedOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", {.ret = 10}, {.ordered = false}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(24, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseBypassOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", {.bypassDocumentValidation = true}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(24, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseUnordredBypassOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", {.ret = 10}, {.ordered = false, .bypassDocumentValidation = true}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(25, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseSetWrieConcernOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", {.writeConcern = {2, false, 400}}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(29, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseUnorderedWrieConcernOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", {}, {.ordered = false, .writeConcern = {2, false, 400}}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(30, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseBypassWriteConcenrOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", {.writeConcern = {2, false, 400}, .bypassDocumentValidation = true}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(30, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseUnorderedBypassWriteConcenrOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", {.ret = 10}, {.ordered = false, .writeConcern = {2, false, 400}, .bypassDocumentValidation = true}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(31, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseSetCommentOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", {.comment = "Testing comment"}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(24, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseUnorderedSetCommentOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", {}, {.ordered = false, .comment = "Testing comment"}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(25, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseBypassSetCommentOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", {.bypassDocumentValidation = true, .comment = "Testing comment"}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(25, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseUnordredBypassSetCommentOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", {.ret = 10}, {.ordered = false, .bypassDocumentValidation = true, .comment = "Testing comment"}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(26, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseSetWrieConcernSetCommentOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", {.writeConcern = {2, false, 400}, .comment = "Testing comment"}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(30, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseUnorderedWrieConcernSetCommentOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", {}, {.ordered = false, .writeConcern = {2, false, 400}, .comment = "Testing comment"}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(31, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseBypassWriteConcenrSetCommentOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", {.writeConcern = {2, false, 400}, .bypassDocumentValidation = true, .comment = "Testing comment"}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(31, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_InsertTest, BaseUnorderedBypassWriteConcenrSetCommentOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_CmdDB_Insert("database", "collection", {.ret = 10}, {.ordered = false, .writeConcern = {2, false, 400}, .bypassDocumentValidation = true, .comment = "Testing comment"}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(32, std::count(std::begin(result), std::end(result), '\n'));
}

