#include "gtest/gtest.h"
#include "arg.hpp"
#include "util.hpp"
#include <format>

using namespace rcp;

constexpr char TEST_ARG_NAME[]{"test_name"};

TEST(ArgTest, setDefaultValue_returnDefaultValue) {
    const std::string defval{"some_value"};
    auto arg = ArgBuilder(TEST_ARG_NAME).default_value(defval).get();
    ASSERT_EQ(defval, arg->get().value());
}

TEST(ArgTest, noDefaultValue_returnNone) {
    auto arg = ArgBuilder(TEST_ARG_NAME).get();
    ASSERT_FALSE(arg->get().has_value());
}

TEST(ArgTest, valueSet_returnsValue) {
    auto arg = ArgBuilder(TEST_ARG_NAME).get();
    const std::string expected{"some_value"};
    arg->set(expected);
    ASSERT_EQ(expected, arg->get().value());
}

TEST(ArgTest, defaultValValueSet_returnsNewValue) {
    auto arg = ArgBuilder(TEST_ARG_NAME).default_value("any").get();
    const std::string expected{"some_value"};
    arg->set(expected);
    ASSERT_EQ(expected, arg->get().value());
}

TEST(ArgTest, descriptionSet_returnsHelpWithDescription) {
    const std::string test_desc{"test description"};
    auto arg = ArgBuilder(TEST_ARG_NAME).with_description(test_desc).get();
    const std::string expected = std::format("-{}  --{}  <{}>  {}", TEST_ARG_NAME[0], TEST_ARG_NAME, util::upper(TEST_ARG_NAME), test_desc);
    ASSERT_EQ(expected, arg->help());
}

TEST(ArgTest, isTriggered_triggeredByShortAndLongVersion) {
    auto arg = ArgBuilder(TEST_ARG_NAME).get();
    ASSERT_TRUE(arg->is_triggered(TEST_ARG_NAME));
    ASSERT_TRUE(arg->is_triggered(std::string{TEST_ARG_NAME[0]}));
}