
#include <gtest/gtest.h>
#include "CmdDB_FindModify.h"

using namespace ThorsAnvil::DB::Mongo;

struct FindValueToMod
{
    int value;
};
ThorsAnvil_MakeTrait(FindValueToMod, value);

TEST(CmdDB_FindModifyTest, Base)
{
    auto find = send_CmdDB_FindDelete("database", "collection", FindValueToMod{48});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(17, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetNew)
{
    auto find = send_CmdDB_FindDelete("database", "collection", {.returnModified = true}, FindValueToMod{48});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"new\":"));
    EXPECT_EQ(18, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetNewChain)
{
    auto find = send_CmdDB_FindDelete("database", "collection", FindValueToMod{48}).setNew();
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"new\":"));
    EXPECT_EQ(18, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetFields)
{
    auto find = send_CmdDB_FindDelete("database", "collection", {.fields = {{"John",1},{"Mark",1}}}, FindValueToMod{48});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"fields\":"));
    EXPECT_EQ(22, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetFieldsChain)
{
    auto find = send_CmdDB_FindDelete("database", "collection", FindValueToMod{48}).addFileds({"John", "Mark"});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"fields\":"));
    EXPECT_EQ(22, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetUpsert)
{
    auto find = send_CmdDB_FindDelete("database", "collection", {.upsert = true}, FindValueToMod{48});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"upsert\":"));
    EXPECT_EQ(18, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetUpsertChain)
{
    auto find = send_CmdDB_FindDelete("database", "collection", FindValueToMod{48}).setUpsert();
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"upsert\":"));
    EXPECT_EQ(18, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetByPass)
{
    auto find = send_CmdDB_FindDelete("database", "collection", {.bypassDocumentValidation = true}, FindValueToMod{48});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"bypassDocumentValidation\":"));
    EXPECT_EQ(18, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetByPassChain)
{
    auto find = send_CmdDB_FindDelete("database", "collection", FindValueToMod{48}).byPass();
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"bypassDocumentValidation\":"));
    EXPECT_EQ(18, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetHint)
{
    auto find = send_CmdDB_FindDelete("database", "collection", {.hint = "Cool"}, FindValueToMod{48});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"hint\":"));
    EXPECT_EQ(18, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetHintChain)
{
    auto find = send_CmdDB_FindDelete("database", "collection", FindValueToMod{48}).addHint("Cool");
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"hint\":"));
    EXPECT_EQ(18, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetComment)
{
    auto find = send_CmdDB_FindDelete("database", "collection", {.comment = "Long comment"}, FindValueToMod{48});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"comment\":"));
    EXPECT_EQ(18, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetCommentChain)
{
    auto find = send_CmdDB_FindDelete("database", "collection", FindValueToMod{48}).setComment("A Comment for the ages");
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"comment\":"));
    EXPECT_EQ(18, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetAll)
{
    auto find = send_CmdDB_FindDelete("database", "collection", {.returnModified = true, .fields = {{"John",1},{"Mark",1}}, .upsert = true, .bypassDocumentValidation = true, .hint = "Cool", .comment = "Long comment"}, FindValueToMod{48});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"comment\":"));
    EXPECT_EQ(27, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_FindModifyTest, PassQueryFMFlagsFlagsSetAllChain)
{
    auto find = send_CmdDB_FindDelete("database", "collection", FindValueToMod{48}).setNew().addFileds({"John", "Mark"}).setUpsert().byPass().addHint("Cool").setComment("A Comment for the ages");
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_NE(std::string::npos, result.find("\"comment\":"));
    EXPECT_EQ(27, std::count(std::begin(result), std::end(result), '\n'));
}

