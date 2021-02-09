
#include <gtest/gtest.h>
#include "CmdDB_GetMore.h"

using namespace ThorsAnvil::DB::Mongo;

TEST(CmdDB_GetMoreTest, Base)
{
    auto getMore = make_CmdDB_GetMore("database", "collection");
    std::stringstream stream;
    stream << make_hr(getMore);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_GetMoreTest, SetBatchSize)
{
    auto getMore = make_CmdDB_GetMore("database", "collection").setBatchSize(3);
    std::stringstream stream;
    stream << make_hr(getMore);

    std::string result = stream.str();
    EXPECT_EQ(14, std::count(std::begin(result), std::end(result), '\n'));
    EXPECT_NE(std::string::npos, result.find("\"batchSize\":"));
}

TEST(CmdDB_GetMoreTest, SetBatchSizeOptions)
{
    auto getMore = make_CmdDB_GetMore("database", "collection", {.batchSize = 3});
    std::stringstream stream;
    stream << make_hr(getMore);

    std::string result = stream.str();
    EXPECT_EQ(14, std::count(std::begin(result), std::end(result), '\n'));
    EXPECT_NE(std::string::npos, result.find("\"batchSize\":"));
}

TEST(CmdDB_GetMoreTest, SetMaxTimeout)
{
    auto getMore = make_CmdDB_GetMore("database", "collection").setMaxTimeout(6);
    std::stringstream stream;
    stream << make_hr(getMore);

    std::string result = stream.str();
    EXPECT_EQ(14, std::count(std::begin(result), std::end(result), '\n'));
    EXPECT_NE(std::string::npos, result.find("\"maxTimeMS\":"));
}

TEST(CmdDB_GetMoreTest, SetMaxTimeoutOptions)
{
    auto getMore = make_CmdDB_GetMore("database", "collection", {.maxTimeMS = 3});
    std::stringstream stream;
    stream << make_hr(getMore);

    std::string result = stream.str();
    EXPECT_EQ(14, std::count(std::begin(result), std::end(result), '\n'));
    EXPECT_NE(std::string::npos, result.find("\"maxTimeMS\":"));
}

TEST(CmdDB_GetMoreTest, SetComment)
{
    auto getMore = make_CmdDB_GetMore("database", "collection").setComment("Hi There");
    std::stringstream stream;
    stream << make_hr(getMore);

    std::string result = stream.str();
    EXPECT_EQ(14, std::count(std::begin(result), std::end(result), '\n'));
    EXPECT_NE(std::string::npos, result.find("\"comment\":"));
}

TEST(CmdDB_GetMoreTest, SetCommentOptions)
{
    auto getMore = make_CmdDB_GetMore("database", "collection", {.comment = "Hi There Again"});
    std::stringstream stream;
    stream << make_hr(getMore);

    std::string result = stream.str();
    EXPECT_EQ(14, std::count(std::begin(result), std::end(result), '\n'));
    EXPECT_NE(std::string::npos, result.find("\"comment\":"));
}

TEST(CmdDB_GetMoreTest, SetAll)
{
    auto getMore = make_CmdDB_GetMore("database", "collection").setComment("Hi There").setMaxTimeout(89).setBatchSize(102);
    std::stringstream stream;
    stream << make_hr(getMore);

    std::string result = stream.str();
    EXPECT_EQ(16, std::count(std::begin(result), std::end(result), '\n'));
    EXPECT_NE(std::string::npos, result.find("\"batchSize\":"));
    EXPECT_NE(std::string::npos, result.find("\"maxTimeMS\":"));
    EXPECT_NE(std::string::npos, result.find("\"comment\":"));
}

TEST(CmdDB_GetMoreTest, SetAllOptions)
{
    auto getMore = make_CmdDB_GetMore("database", "collection", {.batchSize = 12, .maxTimeMS = 104, .comment = "Hi There Again"});
    std::stringstream stream;
    stream << make_hr(getMore);

    std::string result = stream.str();
    EXPECT_EQ(16, std::count(std::begin(result), std::end(result), '\n'));
    EXPECT_NE(std::string::npos, result.find("\"batchSize\":"));
    EXPECT_NE(std::string::npos, result.find("\"maxTimeMS\":"));
    EXPECT_NE(std::string::npos, result.find("\"comment\":"));
}
