#include "gtest/gtest.h"
#include "flag.hpp"
#include "rcp_error.hpp"

using namespace rcp;

constexpr char TEST_FLAG_NAME[]{"test_flag_name"};

TEST(FlagTest, defaultBuild_notCalledAfterBuild) {
    auto flag = FlagBuilder(TEST_FLAG_NAME)
        .get();

    ASSERT_FALSE(flag->was_toggled());
}

TEST(FlagTest, defaultBuild_calledAfterCall) {
    auto flag = FlagBuilder(TEST_FLAG_NAME)
        .get();

    flag->toggle();
    ASSERT_TRUE(flag->was_toggled());
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

    ASSERT_THROW(bldr.get(), BuildError);
}

TEST(FlagTest, ambiguousFlags_returnsTrue) {
    auto flag1 = FlagBuilder(TEST_FLAG_NAME).get();
    auto flag2 = FlagBuilder(TEST_FLAG_NAME).get();
    ASSERT_TRUE(flag1->is_ambiguous(*flag2));
}

TEST(FlagTest, notAmbiguousFlags_returnsFalse) {
    auto flag1 = FlagBuilder(TEST_FLAG_NAME).get();
    auto flag2 = FlagBuilder("other_name").get();
    ASSERT_FALSE(flag1->is_ambiguous(*flag2));
}

TEST(FlagTest, defaultFlag_returnsArgInfoWithShortOnly) {
    auto flag = FlagBuilder(TEST_FLAG_NAME).get();

    auto info = flag->get_arg_info().front();

    ASSERT_EQ(Type::other, info.type);
    ASSERT_TRUE(info.short_version.has_value());
    ASSERT_FALSE(info.long_version.has_value());
}

TEST(FlagTest, flagWithLongVersion_returnsArgInfoWithShortAndLong) {
    auto flag = FlagBuilder(TEST_FLAG_NAME).allow_long().get();

    auto info = flag->get_arg_info().front();

    ASSERT_EQ(Type::other, info.type);
    ASSERT_TRUE(info.short_version.has_value());
    ASSERT_TRUE(info.long_version.has_value());
}