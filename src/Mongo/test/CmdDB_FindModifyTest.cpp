
#include <gtest/gtest.h>
#include "CmdDB_FindModify.h"

using namespace ThorsAnvil::DB::Mongo;

TEST(CmdDB_FindModifyTest, Base)
{
    auto find = make_CmdDB_FindDelete("database", "collection");
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetNew)
{
    auto find = make_CmdDB_FindDelete("database", "collection", {.returnModified = true});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"new\":"));
    EXPECT_EQ(14, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetNewChain)
{
    auto find = make_CmdDB_FindDelete("database", "collection").setNew();
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"new\":"));
    EXPECT_EQ(14, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetFields)
{
    auto find = make_CmdDB_FindDelete("database", "collection", {.fields = {{"John",1},{"Mark",1}}});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"fields\":"));
    EXPECT_EQ(18, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetFieldsChain)
{
    auto find = make_CmdDB_FindDelete("database", "collection").addFileds({"John", "Mark"});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"fields\":"));
    EXPECT_EQ(18, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetUpsert)
{
    auto find = make_CmdDB_FindDelete("database", "collection", {.upsert = true});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"upsert\":"));
    EXPECT_EQ(14, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetUpsertChain)
{
    auto find = make_CmdDB_FindDelete("database", "collection").setUpsert();
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"upsert\":"));
    EXPECT_EQ(14, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetByPass)
{
    auto find = make_CmdDB_FindDelete("database", "collection", {.bypassDocumentValidation = true});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"bypassDocumentValidation\":"));
    EXPECT_EQ(14, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetByPassChain)
{
    auto find = make_CmdDB_FindDelete("database", "collection").byPass();
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"bypassDocumentValidation\":"));
    EXPECT_EQ(14, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetHint)
{
    auto find = make_CmdDB_FindDelete("database", "collection", {.hint = "Cool"});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"hint\":"));
    EXPECT_EQ(14, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetHintChain)
{
    auto find = make_CmdDB_FindDelete("database", "collection").addHint("Cool");
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"hint\":"));
    EXPECT_EQ(14, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetComment)
{
    auto find = make_CmdDB_FindDelete("database", "collection", {.comment = "Long comment"});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"comment\":"));
    EXPECT_EQ(14, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetCommentChain)
{
    auto find = make_CmdDB_FindDelete("database", "collection").setComment("A Comment for the ages");
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"comment\":"));
    EXPECT_EQ(14, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetAll)
{
    auto find = make_CmdDB_FindDelete("database", "collection", {.returnModified = true, .fields = {{"John",1},{"Mark",1}}, .upsert = true, .bypassDocumentValidation = true, .hint = "Cool", .comment = "Long comment"});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"comment\":"));
    EXPECT_EQ(23, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetAllChain)
{
    auto find = make_CmdDB_FindDelete("database", "collection").setNew().addFileds({"John", "Mark"}).setUpsert().byPass().addHint("Cool").setComment("A Comment for the ages");
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"comment\":"));
    EXPECT_EQ(23, std::count(std::begin(result), std::end(result), '\n'));
}

