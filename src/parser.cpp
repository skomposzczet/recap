#include "parser.hpp"

namespace rcp {

std::unique_ptr<ParserBuilder> Parser::build(const std::string& app_name) {
    return std::make_unique<ParserBuilder>(app_name);
}

void Parser::parse(int argv, char** argc) {
    // todo
}

void Parser::add_argument(ArgumentsVecType::value_type arg) {
    args.push_back(arg);
}

void Parser::display_help() { 
    // todo
}

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

Parser ParserBuilder::get() const {
    return parser;
}

}
