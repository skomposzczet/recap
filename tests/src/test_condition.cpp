#include "gtest/gtest.h"
#include "condition.hpp"

using namespace rcp;

constexpr char TEST_COND_NAME[]{"test_cond_name"};
constexpr char TEST_BUNCH_NAME[]{"test_bunch_name"};

TEST(ConditionTest, create_passes) {
    Condition cond(TEST_COND_NAME, [](const std::string&){return true;});
    ASSERT_TRUE(cond.is_met("").is_ok());
}

TEST(ConditionTest, create_fails) {
    Condition cond(TEST_COND_NAME, [](const std::string&){return false;});
    ASSERT_EQ(TEST_COND_NAME, cond.is_met("").get_err());
}

TEST(ConditionTest, createBunch_failsReturnsFullScopeName) {
    ConditionBunch bunch(TEST_BUNCH_NAME);
    bunch.add(Condition(TEST_COND_NAME, [](const std::string&){return false;}));

    std::string expected = std::string{TEST_BUNCH_NAME} + "::" + TEST_COND_NAME;
    ASSERT_EQ(expected, bunch.is_met("").get_err());
}

TEST(ConditionTest, createBunchOnePasses_failsReturnsFullScopeName) {
    ConditionBunch bunch(TEST_BUNCH_NAME);
    bunch.add(Condition("any", [](const std::string&){return true;}));
    bunch.add(Condition(TEST_COND_NAME, [](const std::string&){return false;}));

    std::string expected = std::string{TEST_BUNCH_NAME} + "::" + TEST_COND_NAME;
    ASSERT_EQ(expected, bunch.is_met("").get_err());
}

TEST(ConditionTest, createBunchBunch_failsReturnsFullScopeName) {
    ConditionBunch bunch("outer");
    ConditionBunch middle("middle");
    middle.add(Condition("inner", [](const std::string&){return false;}));
    bunch.add_bunch(middle);

    std::string expected = "outer::middle::inner";
    ASSERT_EQ(expected, bunch.is_met("").get_err());
}

TEST(ConditionTest, createIntParsable_passesInt) {
    Condition condition = Condition::int_parsable();
    ASSERT_TRUE(condition.is_met(std::to_string(99)).is_ok());
}

TEST(ConditionTest, createIntParsable_failsNotInt) {
    Condition condition = Condition::int_parsable();
    ASSERT_FALSE(condition.is_met("NaN").is_ok());
}

TEST(ConditionTest, createFloatParsable_passesFloat) {
    Condition condition = Condition::float_parsable();
    ASSERT_TRUE(condition.is_met(".99f").is_ok());
}

TEST(ConditionTest, createFloatParsable_failsNotFloat) {
    Condition condition = Condition::float_parsable();
    ASSERT_FALSE(condition.is_met("NaN").is_ok());
}
