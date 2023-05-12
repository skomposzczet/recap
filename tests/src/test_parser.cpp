#include "gtest/gtest.h"
#include "parser_builder.hpp"
#include "rcp_error.hpp"

#include <string>
#include <unordered_map>
#include <vector>
#include <list>
#include <random>

#define SIZE(array, type) (sizeof(array) / (sizeof(type)))
#define LOCAL_SIZE() SIZE(args, char*)

using namespace rcp;

constexpr char TEST_APP_NAME[]{"test_app_name"};
constexpr char TEST_ARG_NAME[]{"test arg name"};
constexpr char TEST_COND_NAME[]{"test_cond_name"};

std::vector<std::string> shuffle_args(std::vector<std::vector<std::string>>& rcargs, const std::string& name) {
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(rcargs.begin(), rcargs.end(), g);

    std::vector<std::string> cargs{name};
    while(!rcargs.empty()) {
        auto v = rcargs.back();
        rcargs.pop_back();
        cargs.insert(cargs.end(), v.begin(), v.end());
    }

    return cargs;
}

TEST(ParserTest, enabledHelpVersion_isNotTriggered) {
    Parser parser = ParserBuilder(TEST_APP_NAME)
        .help_enabled().version_enabled()
        .get();
    const char* args[] = {TEST_APP_NAME};
    parser.parse(1, args);
    
    ASSERT_FALSE(parser.help_triggered());
    ASSERT_FALSE(parser.version_triggered());
}

TEST(ParserTest, enabledHelp_parsesHelpOption) {
    Parser parser = ParserBuilder(TEST_APP_NAME).help_enabled().get();
    const char* args[] = {TEST_APP_NAME, "--help"};
    parser.parse(2, args);
    
    ASSERT_TRUE(parser.help_triggered());
}

TEST(ParserTest, disabledHelp_throwsParsingHelpOption) {
    Parser parser = ParserBuilder(TEST_APP_NAME).get();
    const char* args[] = {TEST_APP_NAME, "--help"};
    ASSERT_THROW(parser.parse(2, args), ParseError);
}

TEST(ParserTest, enabledVersion_parsesVersionOption) {
    Parser parser = ParserBuilder(TEST_APP_NAME).version_enabled().get();
    const char* args[] = {TEST_APP_NAME, "--version"};
    parser.parse(2, args);
    
    ASSERT_TRUE(parser.version_triggered());
}

TEST(ParserTest, disabledVersion_throwsParsingVersionOption) {
    Parser parser = ParserBuilder(TEST_APP_NAME).get();
    const char* args[] = {TEST_APP_NAME, "--version"};
    ASSERT_THROW(parser.parse(2, args), ParseError);
}

TEST(ParserTest, addFlag_parsesFlag) {
    Parser parser = ParserBuilder(TEST_APP_NAME)
        .get();

    parser.add_flag(
        FlagBuilder(TEST_ARG_NAME)
            .get()
    );

    const std::string arg = std::string{"-"} + TEST_ARG_NAME[0];
    const char* args[] = {TEST_APP_NAME, arg.data()};

    parser.parse(2, args);
    ASSERT_TRUE(parser.was_called(TEST_ARG_NAME));
}

TEST(ParserTest, addArg_parsesArg) {
    Parser parser = ParserBuilder(TEST_APP_NAME)
        .get();

    parser.add_argument(
        ArgBuilder(TEST_ARG_NAME)
            .get()
    );

    const std::string arg = std::string{"-"} + TEST_ARG_NAME[0];
    const std::string val{"value"};
    const char* args[] = {TEST_APP_NAME, arg.data(), val.data()};

    parser.parse(LOCAL_SIZE(), args);
    ASSERT_EQ(val, parser.get(TEST_ARG_NAME));
}

TEST(ParserTest, addPosArg_parserPosArg) {
    Parser parser = ParserBuilder(TEST_APP_NAME)
        .get();

    parser.add_positional_argument(
        PositionalArgBuilder(TEST_ARG_NAME)
            .get()
    );

    const std::string val{"value"};
    const char* args[] = {TEST_APP_NAME, val.data()};

    parser.parse(LOCAL_SIZE(), args);
    ASSERT_EQ(val, parser.get(TEST_ARG_NAME));
}

TEST(ParserTest, addMultipleTypeArgs_parsesAllInRandomOrder) {
    Parser parser = ParserBuilder(TEST_APP_NAME)
        .help_enabled()
        .version_enabled()
        .get();

    std::unordered_map<std::string, std::string> key_val;
    key_val.emplace("pos_arg1", "pv1");
    key_val.emplace("pos_arg2", "pv2");
    key_val.emplace("arg1", "v1");
    key_val.emplace("rarg2", "v2");

    std::unordered_map<std::string, bool> flag_map;
    flag_map.emplace("help", true);
    flag_map.emplace("version", false);
    flag_map.emplace("flag", true);

    parser.add_positional_argument(PositionalArgBuilder("pos_arg1").get());
    parser.add_positional_argument(PositionalArgBuilder("pos_arg2").get());
    parser.add_argument(ArgBuilder("arg1").get());
    parser.add_argument(ArgBuilder("rarg2").get());
    parser.add_flag(FlagBuilder("flag").get());

    std::vector<std::vector<std::string>> rcargs{
        {key_val["pos_arg1"], key_val["pos_arg2"]},
        {"--arg1", key_val["arg1"]},
        {"-r", key_val["rarg2"]},
        {"--help"},
        {"-f"},
    };

    auto cargs = shuffle_args(rcargs, TEST_APP_NAME);
    auto dbg_str = std::reduce(cargs.begin(), cargs.end(), std::string{""}, [](const auto& s, const auto& ss) {return s + " " + ss;});
    std::vector<const char*> args;
    std::transform(cargs.begin(), cargs.end(), std::back_inserter(args), [](const auto& s ){
        return s.data();
    });

    parser.parse(args.size(), args.data());

    for (const auto& [key, val]: key_val) {
        ASSERT_EQ(val, parser.get(key)) << dbg_str;
    }
    for (const auto& [key, val]: flag_map) {
        ASSERT_EQ(val, parser.was_called(key)) << dbg_str;
    }
}

TEST(ParserTest, addAmbiguousArg_throwsOnAdd) {
    Parser parser = ParserBuilder(TEST_APP_NAME).get();

    parser.add_positional_argument(PositionalArgBuilder(TEST_ARG_NAME).get());
    ASSERT_THROW(parser.add_flag(FlagBuilder(TEST_ARG_NAME).get()), ParseError);
}

TEST(ParserTest, addAmbiguousFlagPosArg_throwsOnAdd) {
    Parser parser = ParserBuilder(TEST_APP_NAME)
        .help_enabled()
        .get();

    ASSERT_THROW(parser.add_positional_argument(PositionalArgBuilder("help").get()), ParseError);
}

TEST(ParserTest, addArgWithCondition_conditionMetParses) {
    Parser parser = ParserBuilder(TEST_APP_NAME)
        .get();
    parser.add_argument(
        ArgBuilder(TEST_ARG_NAME)
            .with_condition(Condition(TEST_COND_NAME, [](const std::string&){return true;}))
            .get()
    );

    const std::string val{"value"};
    const char* args[] = {TEST_APP_NAME, "-t", val.data()};

    parser.parse(LOCAL_SIZE(), args);
    ASSERT_EQ(val, parser.get(TEST_ARG_NAME));
}

TEST(ParserTest, addArgWithCondition_conditionNotMetThrows) {
    Parser parser = ParserBuilder(TEST_APP_NAME)
        .get();
    parser.add_argument(
        ArgBuilder(TEST_ARG_NAME)
            .with_condition(Condition(TEST_COND_NAME, [](const std::string&){return false;}))
            .get()
    );

    const std::string val{"value"};
    const char* args[] = {TEST_APP_NAME, "-t", val.data()};

    ASSERT_THROW(parser.parse(LOCAL_SIZE(), args), ParseError);
}

TEST(ParserTest, addArg_valueWithoudOptionNameThrows) {
    Parser parser = ParserBuilder(TEST_APP_NAME).get();
    parser.add_argument(ArgBuilder(TEST_ARG_NAME).get());

    const char* args[] = {TEST_APP_NAME, "test"};

    ASSERT_THROW(parser.parse(LOCAL_SIZE(), args), ParseError);
}

TEST(ParserTest, noRequiredArgs_parseIsValid) {
    Parser parser = ParserBuilder(TEST_APP_NAME).get();
    parser.add_positional_argument(PositionalArgBuilder(TEST_ARG_NAME).get());

    const char* args[] = {TEST_APP_NAME};

    ASSERT_NO_THROW(parser.parse(LOCAL_SIZE(), args));
}

TEST(ParserTest, requiredArgs_parseNotValidThrows) {
    Parser parser = ParserBuilder(TEST_APP_NAME).get();
    parser.add_positional_argument(PositionalArgBuilder(TEST_ARG_NAME).required().get());

    const char* args[] = {TEST_APP_NAME};

    ASSERT_THROW(parser.parse(LOCAL_SIZE(), args), ParseError);
}