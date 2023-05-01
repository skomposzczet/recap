#include "gtest/gtest.h"
#include "parser_builder.hpp"
#include "rcp_error.hpp"

#include <string>

#define SIZE(array, type) (sizeof(array) / (sizeof(type)))
#define LOCAL_SIZE() SIZE(args, char*)

using namespace rcp;

constexpr char TEST_APP_NAME[]{"test app name"};
constexpr char TEST_ARG_NAME[]{"test arg name"};

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

TEST(ParserTest, addAuthors_returnsAuthors) {
    std::vector<std::string> as{"a0", "a1", "a2", "a3"};
    Parser parser = ParserBuilder(TEST_APP_NAME)
        .author(as.at(0))
        .author(as.at(1))
        .authors({as.at(2), as.at(3)})
        .get();
    
    ASSERT_EQ(as, parser.get_authors());
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