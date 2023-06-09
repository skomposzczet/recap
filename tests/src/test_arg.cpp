#include "gtest/gtest.h"
#include "arg.hpp"
#include "util.hpp"
#include <format>

using namespace rcp;

constexpr char TEST_ARG_NAME[]{"test_name"};
constexpr char TEST_COND_NAME[]{"test_cond_name"};

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

TEST(ArgTest, isTriggered_triggeredByShortAndLongVersion) {
    auto arg = ArgBuilder(TEST_ARG_NAME).get();
    ASSERT_TRUE(arg->is_triggered(TEST_ARG_NAME));
    ASSERT_TRUE(arg->is_triggered(std::string{TEST_ARG_NAME[0]}));
}

TEST(ArgTest, onlyShortAllowed_triggeredOnlyByShort) {
    auto arg = ArgBuilder(TEST_ARG_NAME)
        .forbid_long()
        .get();

    ASSERT_FALSE(arg->is_triggered(TEST_ARG_NAME));
    ASSERT_TRUE(arg->is_triggered(std::string{TEST_ARG_NAME[0]}));
}

TEST(ArgTest, onlyLongAllowed_triggeredOnlyByLong) {
    auto arg = ArgBuilder(TEST_ARG_NAME)
        .forbid_short()
        .get();

    ASSERT_TRUE(arg->is_triggered(TEST_ARG_NAME));
    ASSERT_FALSE(arg->is_triggered(std::string{TEST_ARG_NAME[0]}));
}

TEST(ArgTest, shortAndLongNotForbid_getThrows) {
    auto arg = ArgBuilder(TEST_ARG_NAME)
        .forbid_short()
        .forbid_long();

    ASSERT_THROW(arg.get(), BuildError);
}

TEST(ArgTest, ambiguousArgs_returnsTrue) {
    auto arg1 = ArgBuilder(TEST_ARG_NAME).get();
    auto arg2 = ArgBuilder(TEST_ARG_NAME).get();
    ASSERT_TRUE(arg1->is_ambiguous(*arg2));
}

TEST(ArgTest, notAmbiguousArgs_returnsFalse) {
    auto arg1 = ArgBuilder(TEST_ARG_NAME).get();
    auto arg2 = ArgBuilder(TEST_ARG_NAME).get();
    ASSERT_TRUE(arg1->is_ambiguous(*arg2));
}

TEST(ArgTest, argWithCondition_meetsPasses) {
    auto arg = ArgBuilder(TEST_ARG_NAME)
        .with_condition(Condition(TEST_COND_NAME, [](const std::string&){return true;}))
        .get();

    ASSERT_TRUE(arg->set("").is_ok());
}

TEST(ArgTest, argWithConditionBunch_doenstMeetFails) {
    ConditionBunch bunch(TEST_COND_NAME);
    bunch.add(Condition("cond", [](const std::string&){return false;}));

    auto arg = ArgBuilder(TEST_ARG_NAME)
        .with_conditions(bunch)
        .get();

    std::string expected = std::string{TEST_ARG_NAME} + "::" + TEST_COND_NAME + "::cond";
    ASSERT_EQ(expected, arg->set("").get_err());
}

TEST(ArgTest, createArg_returnsCorrectArgInfo) {
    auto arg = ArgBuilder(TEST_ARG_NAME)
        .get();

    auto info = arg->get_arg_info().front();
    ASSERT_EQ(Type::other, info.type);
    ASSERT_EQ(TEST_ARG_NAME, info.value);
}

TEST(ArgTest, createArgWithAliases_argInfoContainsDataAboutAliases) {
    std::string alias{"first_alias"};
    std::vector<std::string> aliases{"alias", "other_alias"};

    auto arg = ArgBuilder(TEST_ARG_NAME)
        .with_aliases(aliases)
        .with_alias(alias)
        .get();

    aliases.push_back(alias);

    auto info_vec = arg->get_arg_info();

    ASSERT_EQ(aliases.size()+1, info_vec.size());
    info_vec.erase(info_vec.begin());
    for (size_t i{0} ; i < info_vec.size() ; ++i) {
        ASSERT_EQ(Type::other, info_vec.at(i).type);
        ASSERT_EQ(TEST_ARG_NAME, info_vec.at(i).value);
        ASSERT_EQ(aliases.at(i), info_vec.at(i).long_version);
    }
}

TEST(ArgTest, createArgWithAlias_isTriggeredByAlias) {
    std::string alias{"alias"};
    auto arg = ArgBuilder(TEST_ARG_NAME)
        .with_alias(alias)
        .get();

    ASSERT_TRUE(arg->is_triggered(alias));
}

TEST(ArgTest, createArgWithAliases_isTriggeredByAliases) {
    std::vector<std::string> aliases{"alias", "other_alias"};
    auto arg = ArgBuilder(TEST_ARG_NAME)
        .with_aliases(aliases)
        .get();

    ASSERT_TRUE(arg->is_triggered(aliases.at(0)));
    ASSERT_TRUE(arg->is_triggered(aliases.at(1)));
}
