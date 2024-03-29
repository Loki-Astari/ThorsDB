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
#include "CmdDB_GetLastError.h"

using namespace ThorsAnvil::DB::Mongo;

TEST(CmdDB_GetLastErrorTest, Base)
{
    auto getError = send_CmdDB_GetLastError("database");
    std::stringstream stream;
    stream << make_hr(getError);

    std::string result = stream.str();
    EXPECT_EQ(12, std::count(std::begin(result), std::end(result), '\n'));
}

TEST(CmdDB_GetLastErrorTest, WaitForFlushToDisk)
{
    auto getError = send_CmdDB_GetLastError("database").waitFoolDiskFlush();
    std::stringstream stream;
    stream << make_hr(getError);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
    EXPECT_NE(std::string::npos, result.find("\"j\":"));
}

TEST(CmdDB_GetLastErrorTest, WaitForFlushToDiskOptions)
{
    auto getError = send_CmdDB_GetLastError("database", {.j = false});
    std::stringstream stream;
    stream << make_hr(getError);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
    EXPECT_NE(std::string::npos, result.find("\"j\":"));
}

TEST(CmdDB_GetLastErrorTest, waitForReplication)
{
    auto getError = send_CmdDB_GetLastError("database").waitForReplication(12);
    std::stringstream stream;
    stream << make_hr(getError);

    std::string result = stream.str();
    EXPECT_EQ(14, std::count(std::begin(result), std::end(result), '\n'));
    EXPECT_NE(std::string::npos, result.find("\"w\":"));
}

TEST(CmdDB_GetLastErrorTest, waitForReplicationOptions)
{
    auto getError = send_CmdDB_GetLastError("database", {.w = 12});
    std::stringstream stream;
    stream << make_hr(getError);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
    EXPECT_NE(std::string::npos, result.find("\"w\":"));
}

TEST(CmdDB_GetLastErrorTest, setWaitTimeout)
{
    auto getError = send_CmdDB_GetLastError("database").setWaitTimeout(14);
    std::stringstream stream;
    stream << make_hr(getError);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
    EXPECT_NE(std::string::npos, result.find("\"wtimeout\":"));
}

TEST(CmdDB_GetLastErrorTest, setWaitTimeoutOptions)
{
    auto getError = send_CmdDB_GetLastError("database", {.wtimeout = 14});
    std::stringstream stream;
    stream << make_hr(getError);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
    EXPECT_NE(std::string::npos, result.find("\"wtimeout\":"));
}

TEST(CmdDB_GetLastErrorTest, setComment)
{
    auto getError = send_CmdDB_GetLastError("database").setComment("Comment is Short");
    std::stringstream stream;
    stream << make_hr(getError);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
    EXPECT_NE(std::string::npos, result.find("\"comment\":"));
}

TEST(CmdDB_GetLastErrorTest, setCommentOptions)
{
    auto getError = send_CmdDB_GetLastError("database", {.comment = "Comment is Short"});
    std::stringstream stream;
    stream << make_hr(getError);

    std::string result = stream.str();
    EXPECT_EQ(13, std::count(std::begin(result), std::end(result), '\n'));
    EXPECT_NE(std::string::npos, result.find("\"comment\":"));
}

TEST(CmdDB_GetLastErrorTest, setAll)
{
    auto getError = send_CmdDB_GetLastError("database").setComment("Comment is Short").setWaitTimeout(45).waitFoolDiskFlush().waitForReplication(23);
    std::stringstream stream;
    stream << make_hr(getError);

    std::string result = stream.str();
    EXPECT_EQ(16, std::count(std::begin(result), std::end(result), '\n'));
    EXPECT_NE(std::string::npos, result.find("\"j\":"));
    EXPECT_NE(std::string::npos, result.find("\"w\":"));
    EXPECT_NE(std::string::npos, result.find("\"wtimeout\":"));
    EXPECT_NE(std::string::npos, result.find("\"comment\":"));
}

TEST(CmdDB_GetLastErrorTest, setAllOptions)
{
    auto getError = send_CmdDB_GetLastError("database", {.j = false, .w = 23, .wtimeout = 45, .comment = "Comment is Short"});
    std::stringstream stream;
    stream << make_hr(getError);

    std::string result = stream.str();
    EXPECT_EQ(16, std::count(std::begin(result), std::end(result), '\n'));
    EXPECT_NE(std::string::npos, result.find("\"j\":"));
    EXPECT_NE(std::string::npos, result.find("\"w\":"));
    EXPECT_NE(std::string::npos, result.find("\"wtimeout\":"));
    EXPECT_NE(std::string::npos, result.find("\"comment\":"));
}
