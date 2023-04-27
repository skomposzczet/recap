#ifndef _PARSER_H_
#define _PARSER_H_

#include "argument.hpp"

#include <string>
#include <memory>
#include <vector>

#define NEW_PARSER(app_name) (*(Parser::build(app_name).get()))

namespace rcp {

using ArgumentsVecType = std::vector<std::shared_ptr<Argument>>;
class ParserBuilder;

class Parser
{
friend class ParserBuilder;
public:
    static std::unique_ptr<ParserBuilder> build(const std::string& app_name);
    void parse(int argv, char** argc);
    void add_argument(ArgumentsVecType::value_type arg);

private:
    Parser() = default;

    std::string name;
    std::string description;
    std::string brief;
    std::string epilog;
    double version;

    ArgumentsVecType args;

    void display_help();
};

class ParserBuilder
{
public:
    ParserBuilder(const std::string& name);

    ParserBuilder& description(const std::string& description) ;
    ParserBuilder& brief(const std::string& brief);
    ParserBuilder& epilog(const std::string& epilog);
    ParserBuilder& version(double version);

    Parser get() const;

private:
    Parser parser;

};

}

#endif // _PARSER_H_