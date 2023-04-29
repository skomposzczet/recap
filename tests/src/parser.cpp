#include "gtest/gtest.h"
#include "parser_builder.hpp"
#include "rcp_error.hpp"

#include <string>

using namespace rcp;

constexpr char TEST_APP_NAME[]{"test app name"};

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