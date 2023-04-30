#include "gtest/gtest.h"
#include "flag.hpp"
#include "rcp_error.hpp"

using namespace rcp;

constexpr char TEST_FLAG_NAME[]{"test_flag_name"};

TEST(FlagTest, defaultBuild_notCalledAfterBuild) {
    auto flag = FlagBuilder(TEST_FLAG_NAME)
        .get();

    ASSERT_FALSE(flag->was_called());
}

TEST(FlagTest, defaultBuild_calledAfterCall) {
    auto flag = FlagBuilder(TEST_FLAG_NAME)
        .get();

    flag->call();
    ASSERT_TRUE(flag->was_called());
}

TEST(FlagTest, defaultBuild_triggeredOnlyByShortVersion) {
    auto flag = FlagBuilder(TEST_FLAG_NAME)
        .get();

    ASSERT_TRUE(flag->is_triggered(std::string{TEST_FLAG_NAME[0]}));    
    ASSERT_FALSE(flag->is_triggered(TEST_FLAG_NAME));    
}

TEST(FlagTest, allowLongBuild_triggeredByBoth) {
    auto flag = FlagBuilder(TEST_FLAG_NAME)
        .allow_long()
        .get();

    ASSERT_TRUE(flag->is_triggered(std::string{TEST_FLAG_NAME[0]}));    
    ASSERT_TRUE(flag->is_triggered(TEST_FLAG_NAME));    
}

TEST(FlagTest, allowLongForbidShortBuild_triggeredOnlyByShortVersion) {
    auto flag = FlagBuilder(TEST_FLAG_NAME)
        .allow_long()
        .forbid_short()
        .get();

    ASSERT_FALSE(flag->is_triggered(std::string{TEST_FLAG_NAME[0]}));    
    ASSERT_TRUE(flag->is_triggered(TEST_FLAG_NAME));    
}

TEST(FlagTest, forbidShortBuild_throwsOnBuild) {
    auto bldr = FlagBuilder(TEST_FLAG_NAME)
        .forbid_short();

    ASSERT_THROW(bldr.get(), RcpError);
}

TEST(FlagTest, descriptionSet_descriptionContainedInHelpString) {
    const std::string desc{"test description"};
    auto flag = FlagBuilder(TEST_FLAG_NAME)
        .with_description(desc)
        .get();

    ASSERT_TRUE(flag->help().find(desc) != std::string::npos);
}