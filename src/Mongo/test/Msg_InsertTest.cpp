
#include <gtest/gtest.h>
#include "Msg_Insert.h"
#include "test/CommonInsertTest.h"

using namespace ThorsAnvil::DB::Mongo;


// Op_Msg<ViewTypeFinder<User&>::View> send_Msg_Insert<User&>(std::string, std::string, Op_QueryOptions const&, User&);


TEST(Msg_InsertTest, BaseSingleItem)
{
    User   user{"M1", "P1", 25};
    auto insert = send_Msg_Insert("database", "collection", {.ret = 10}, user);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(23, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, Base)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", {.ret = 10}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(28, std::count(std::begin(result), std::end(result), '\n'));
}

#if 0
TEST(Msg_InsertTest, BaseUnordered)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", users).unordered();

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(24, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseBypass)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", {.ret = 10}, users).byPass();

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(24, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseUnordredBypass)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", users).unordered().byPass();

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(25, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseSetWrieConcern)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", {.ret = 10}, users).setWrieConcern(2, false, 400);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(29, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseUnorderedWrieConcern)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", users).unordered().setWrieConcern(2, false, 400);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(30, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseBypassWriteConcenr)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", {.ret = 10}, users).byPass().setWrieConcern(2, false, 400);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(30, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseUnorderedBypassWriteConcenr)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", users).unordered().byPass().setWrieConcern(2, false, 400);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(31, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", {.ret = 10}, users).setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(24, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseUnorderedSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", users).unordered().setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(25, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseBypassSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", {.ret = 10}, users).byPass().setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(25, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseUnordredBypassSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", users).unordered().byPass().setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(26, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseSetWrieConcernSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", {.ret = 10}, users).setWrieConcern(2, false, 400).setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(30, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseUnorderedWrieConcernSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", users).unordered().setWrieConcern(2, false, 400).setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(31, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseBypassWriteConcenrSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", {.ret = 10}, users).byPass().setWrieConcern(2, false, 400).setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(31, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseUnorderedBypassWriteConcenrSetComment)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", users).unordered().byPass().setWrieConcern(2, false, 400).setComment("Testing comment");

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(32, std::count(std::begin(result), std::end(result), '\n'));
}
#endif

TEST(Msg_InsertTest, BaseUnorderedOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", {.ret = 10}, {.ordered = false}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(29, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseBypassOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", {.bypassDocumentValidation = true}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(29, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseUnordredBypassOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", {.ret = 10}, {.ordered = false, .bypassDocumentValidation = true}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(30, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseSetWrieConcernOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", {.writeConcern = {2, false, 400}}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(34, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseUnorderedWrieConcernOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", {}, {.ordered = false, .writeConcern = {2, false, 400}}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(35, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseBypassWriteConcenrOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", {.writeConcern = {2, false, 400}, .bypassDocumentValidation = true}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(35, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseUnorderedBypassWriteConcenrOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", {.ret = 10}, {.ordered = false, .writeConcern = {2, false, 400}, .bypassDocumentValidation = true}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(36, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseSetCommentOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", {.comment = "Testing comment"}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(29, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseUnorderedSetCommentOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", {}, {.ordered = false, .comment = "Testing comment"}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(30, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseBypassSetCommentOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", {.bypassDocumentValidation = true, .comment = "Testing comment"}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(30, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseUnordredBypassSetCommentOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", {.ret = 10}, {.ordered = false, .bypassDocumentValidation = true, .comment = "Testing comment"}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(31, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseSetWrieConcernSetCommentOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", {.writeConcern = {2, false, 400}, .comment = "Testing comment"}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(35, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseUnorderedWrieConcernSetCommentOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", {}, {.ordered = false, .writeConcern = {2, false, 400}, .comment = "Testing comment"}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(36, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseBypassWriteConcenrSetCommentOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", {.writeConcern = {2, false, 400}, .bypassDocumentValidation = true, .comment = "Testing comment"}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(36, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(Msg_InsertTest, BaseUnorderedBypassWriteConcenrSetCommentOption)
{
    std::vector<User>   users{User{"M1", "P1", 25}, {"M2", "P2", 56}};
    auto insert = send_Msg_Insert("database", "collection", {.ret = 10}, {.ordered = false, .writeConcern = {2, false, 400}, .bypassDocumentValidation = true, .comment = "Testing comment"}, users);

    std::stringstream stream;
    stream << make_hr(insert);

    std::string result = stream.str();
    EXPECT_EQ(37, std::count(std::begin(result), std::end(result), '\n'));
}

