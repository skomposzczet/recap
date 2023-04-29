#include "parser_builder.hpp"

namespace rcp {

ParserBuilder::ParserBuilder(const std::string& name) {
    parser.name = name;
}

ParserBuilder& ParserBuilder::description(const std::string& description) {
    parser.description = description;
    return *this;
}

ParserBuilder& ParserBuilder::brief(const std::string& brief) {
    parser.brief = brief;
    return *this;
}

ParserBuilder& ParserBuilder::epilog(const std::string& epilog) {
    parser.epilog = epilog;
    return *this;
}

ParserBuilder& ParserBuilder::version(double version) {
    parser.version = version;
    return *this;
}

ParserBuilder& ParserBuilder::author(const std::string& new_author) {
    parser.authors.push_back(new_author);
    return *this;
}

ParserBuilder& ParserBuilder::authors(std::initializer_list<std::string> authors) {
    parser.authors.insert(parser.authors.end(), authors.begin(), authors.end());
    return *this;
}

ParserBuilder& ParserBuilder::help_enabled() {
    parser.help_arg = ExtraOption::enabled;
    return *this;
}

ParserBuilder& ParserBuilder::version_enabled() {
    parser.version_arg = ExtraOption::enabled;
    return *this;
}

Parser ParserBuilder::get() const {
    return parser;
}

}