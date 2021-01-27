
#include <gtest/gtest.h>
#include "CmdDB_Find.h"

using namespace ThorsAnvil::DB::Mongo;

TEST(CmdDB_FindTest, Base)
{
    auto find = make_CmdDB_Find("database", "collection", {});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(12, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, Projection)
{
    auto find = make_CmdDB_Find("database", "collection", {}).addFileds({"Ace", "Cool", "Three"});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(18, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, ProjectionOptions)
{
    auto find = make_CmdDB_Find("database", "collection", {{}, {.projection = {{"Ace",1}, {"Cool",1}, {"Three",1}}}});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(18, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, Hint)
{
    auto find = make_CmdDB_Find("database", "collection", {}).addHint("Do Stuff");
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, HintOptions)
{
    auto find = make_CmdDB_Find("database", "collection", {{}, {.hint = "Do Stuff"}});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, Skip)
{
    auto find = make_CmdDB_Find("database", "collection", {}).setSkip(12);
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, SkipOptions)
{
    auto find = make_CmdDB_Find("database", "collection", {{}, {.skip = 12}});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, Limit)
{
    auto find = make_CmdDB_Find("database", "collection", {}).setLimit(12);
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, LimitOptions)
{
    auto find = make_CmdDB_Find("database", "collection", {{}, {.limit = 12}});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, Batch)
{
    auto find = make_CmdDB_Find("database", "collection", {}).setBatchSize(12);
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, BatchOptions)
{
    auto find = make_CmdDB_Find("database", "collection", {{}, {.batchSize = 12}});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, SingleBatch)
{
    auto find = make_CmdDB_Find("database", "collection", {}).oneBatch(true);
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, SingleBatchOptions)
{
    auto find = make_CmdDB_Find("database", "collection", {{}, {.singleBatch = false}});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, Comment)
{
    auto find = make_CmdDB_Find("database", "collection", {}).setComment("Comment Here");
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, CommentOption)
{
    auto find = make_CmdDB_Find("database", "collection", {{}, {.comment = "Comment Here"}});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, TimeOut)
{
    auto find = make_CmdDB_Find("database", "collection", {}).setMaxTimeout(6000);
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, TimeOutOptions)
{
    auto find = make_CmdDB_Find("database", "collection", {{}, {.maxTimeMS = 6000}});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, ReadConcern)
{
    auto find = make_CmdDB_Find("database", "collection", {}).addReadConcern(ReadConcernLevel::majority);
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(16, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, ReadConcernOption)
{
    auto find = make_CmdDB_Find("database", "collection", {{}, {.readConcern = ReadConcernLevel::majority}});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(16, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, Max)
{
    auto find = make_CmdDB_Find("database", "collection", {}).addMax("Age", 12);
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(16, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, MaxOption)
{
    auto find = make_CmdDB_Find("database", "collection", {{}, {.max = {{"Age",12}}}});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(16, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, Min)
{
    auto find = make_CmdDB_Find("database", "collection", {}).addMin("Age", 12);
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(16, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, MinOption)
{
    auto find = make_CmdDB_Find("database", "collection", {{}, {.min = {{"Age",12}}}});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(16, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, Key)
{
    auto find = make_CmdDB_Find("database", "collection", {}).justKeys(false);
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, KeyOption)
{
    auto find = make_CmdDB_Find("database", "collection", {{}, {.returnKey = true}});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, ID)
{
    auto find = make_CmdDB_Find("database", "collection", {}).showId(true);
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, IDOption)
{
    auto find = make_CmdDB_Find("database", "collection", {{}, {.showRecordId = true}});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, Tailable)
{
    auto find = make_CmdDB_Find("database", "collection", {}).tailableCursor(true);
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, TailableOption)
{
    auto find = make_CmdDB_Find("database", "collection", {{}, {.tailable = true}});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, Await)
{
    auto find = make_CmdDB_Find("database", "collection", {}).tailedCursorAwait(true);
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(14, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, AwaitOptions)
{
    auto find = make_CmdDB_Find("database", "collection", {{}, {.awaitData = true}});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, CursorTimeOut)
{
    auto find = make_CmdDB_Find("database", "collection", {}).setNoCursorTimeout(true);
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, CursorTimeOutOptions)
{
    auto find = make_CmdDB_Find("database", "collection", {{}, {.noCursorTimeout = true}});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, PartialResults)
{
    auto find = make_CmdDB_Find("database", "collection", {}).setAllowPartialResults(true);
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, PartialResultsOptions)
{
    auto find = make_CmdDB_Find("database", "collection", {{}, {.allowPartialResults = true}});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, CollationOptions)
{
    auto find = make_CmdDB_Find("database", "collection", {{}, {.collation = {.locale = "GB", .caseLevel = true, .caseFirst = "P", .strength = 10, .numericOrdering = true, .alternate = "X", .maxVariable = "Plop", .backwards = true} }});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(22, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, UseDisk)
{
    auto find = make_CmdDB_Find("database", "collection", {}).useDisk(true);
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}
TEST(CmdDB_FindTest, UseDiskOptions)
{
    auto find = make_CmdDB_Find("database", "collection", {{}, {.allowDiskUse = true}});
    std::stringstream stream;
    stream << make_hr(find);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}

