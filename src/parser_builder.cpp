#include "parser_builder.hpp"

namespace rcp {

ParserBuilder::ParserBuilder(const std::string& name) {
    parser.app_info.name = name;
}

ParserBuilder& ParserBuilder::description(const std::string& description) {
    parser.app_info.description.full = description;
    return *this;
}

ParserBuilder& ParserBuilder::brief(const std::string& brief) {
    parser.app_info.description.brief = brief;
    return *this;
}

ParserBuilder& ParserBuilder::epilog(const std::string& epilog) {
    parser.app_info.description.epilog = epilog;
    return *this;
}

ParserBuilder& ParserBuilder::version(const std::string& version) {
    parser.app_info.version = version;
    return *this;
}

ParserBuilder& ParserBuilder::author(const std::string& new_author) {
    parser.app_info.authors.push_back(new_author);
    return *this;
}

ParserBuilder& ParserBuilder::authors(std::initializer_list<std::string> authors) {
    parser.app_info.authors.insert(parser.app_info.authors.end(), authors.begin(), authors.end());
    return *this;
}

ParserBuilder& ParserBuilder::help_enabled() {
    parser.add_flag(
        FlagBuilder("help")
            .allow_long()
            .forbid_short()
            .with_description("Display information about application.")
            .get()
    );
    return *this;
}

ParserBuilder& ParserBuilder::version_enabled() {
    parser.add_flag(
        FlagBuilder("version")
            .allow_long()
            .forbid_short()
            .with_description("Display information about version.")
            .get()
    );
    return *this;
}

Parser ParserBuilder::get() const {
    return parser;
}

}