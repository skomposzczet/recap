#ifndef _PARSER_H_
#define _PARSER_H_

#include "argument.hpp"
#include <string>
#include <memory>
#include <vector>

using ArgumentsVecType = std::vector<std::shared_ptr<Argument>>;

class Parser
{
friend class ParserBuilder;
public:
    static ParserBuilder build(const std::string& app_name) {
        return ParserBuilder(app_name);
    }

    void parse(int argv, char** argc) {
        // todo
    }

    void add_argument(ArgumentsVecType::value_type arg) {
        args.push_back(arg);
    }

private:
    Parser() = default;

    std::string name;
    std::string description;
    std::string brief;
    std::string epilog;
    double version;

    ArgumentsVecType args;
};

class ParserBuilder
{
public:
    ParserBuilder(const std::string& name) {
        parser.name = name;
    }

    ParserBuilder& description(const std::string& description) {
        parser.description = description;
        return *this;
    }

    ParserBuilder& brief(const std::string& brief) {
        parser.brief = brief;
        return *this;
    }

    ParserBuilder& epilog(const std::string& epilog) {
        parser.epilog = epilog;
        return *this;
    }

    ParserBuilder& version(double version) {
        parser.version = version;
        return *this;
    }

    Parser get() const {
        return parser;
    }

private:
    Parser parser;

};

#endif // _PARSER_H_