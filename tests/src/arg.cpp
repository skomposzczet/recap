#include "gtest/gtest.h"
#include "arg.hpp"

using namespace rcp;

constexpr char TEST_ARG_NAME[]{"test_name"};

TEST(ArgTest, setDefaultValue_returnDefaultValue) {
    std::string defval{"some_value"};
    auto arg = ArgBuilder(TEST_ARG_NAME).default_value(defval).get();
    ASSERT_EQ(defval, arg->get().value());
}

TEST(ArgTest, noDefaultValue_returnNone) {
    auto arg = ArgBuilder(TEST_ARG_NAME).get();
    ASSERT_FALSE(arg->get().has_value());
}