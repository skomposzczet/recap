#include "gtest/gtest.h"
#include "positional_arg.hpp"
#include "util.hpp"
#include <format>

using namespace rcp;

constexpr char TEST_ARG_NAME[]{"test_name"};

TEST(PositionalArgTest, noDefaultValue_returnNone) {
    auto arg = PositionalArgBuilder(TEST_ARG_NAME).get();
    ASSERT_FALSE(arg->get().has_value());
}

TEST(PositionalArgTest, valueSet_returnsValue) {
    auto arg = PositionalArgBuilder(TEST_ARG_NAME).get();
    const std::string expected{"some_value"};
    arg->set(expected);
    ASSERT_EQ(expected, arg->get().value());
}

TEST(PositionalArgTest, descriptionSet_returnsHelpWithDescription) {
    const std::string test_desc{"test description"};
    auto arg = PositionalArgBuilder(TEST_ARG_NAME)
        .with_description(test_desc)
        .get();

    const std::string expected = std::format("-{}  --{}  <{}>  {}", 
        TEST_ARG_NAME[0], TEST_ARG_NAME, util::upper(TEST_ARG_NAME), test_desc);
    ASSERT_EQ(expected, arg->help());
}

TEST(PositionalArgTest, orderNotSet_defaultOrderGiven) {
    auto arg1 = PositionalArgBuilder(TEST_ARG_NAME).get();
    auto arg2 = PositionalArgBuilder(TEST_ARG_NAME).get();

    ASSERT_EQ(arg1->get_order() + 1, arg2->get_order());
}

TEST(PositionalArgTest, orderSet_returnSetOrder) {
    unsigned ord{999};
    auto arg = PositionalArgBuilder(TEST_ARG_NAME)
        .order(ord)
        .get();

    ASSERT_EQ(ord, arg->get_order());
}

TEST(PositionalArgTest, orderSet_highestOrderUpdated) {
    auto hi = PositionalArgBuilder("").get()->get_order();

    auto arg1 = PositionalArgBuilder(TEST_ARG_NAME)
        .order(hi)
        .get();
    auto arg2 = PositionalArgBuilder(TEST_ARG_NAME)
        .get();

    ASSERT_EQ(arg1->get_order() + 1, arg2->get_order());
}

TEST(PositionalArgTest, isTriggered_triggeredByShortAndLongVersion) {
    auto arg = PositionalArgBuilder(TEST_ARG_NAME).get();
    ASSERT_TRUE(arg->is_triggered(TEST_ARG_NAME));
    ASSERT_TRUE(arg->is_triggered(std::string{TEST_ARG_NAME[0]}));
}

TEST(PositionalArgTest, ambiguousArgs_returnsTrue) {
    auto arg1 = PositionalArgBuilder(TEST_ARG_NAME).get();
    auto arg2 = PositionalArgBuilder(TEST_ARG_NAME).get();
    ASSERT_TRUE(arg1->is_ambiguous(*arg2));
}

TEST(PositionalArgTest, notAmbiguousArgs_returnsFalse) {
    auto arg1 = PositionalArgBuilder(TEST_ARG_NAME).get();
    auto arg2 = PositionalArgBuilder(TEST_ARG_NAME).get();
    ASSERT_TRUE(arg1->is_ambiguous(*arg2));
}