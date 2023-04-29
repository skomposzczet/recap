#ifndef _PARSER_H_
#define _PARSER_H_

#include "arg.hpp"

#include <string>
#include <memory>
#include <vector>

#define NEW_PARSER(app_name) (*(Parser::build(app_name).get()))
namespace rcp {

enum class ExtraOption {
    disabled,
    enabled,
    triggered,
};

using ArgsVecType = std::vector<std::shared_ptr<Arg>>;
class ParserBuilder;

class Parser
{
friend class ParserBuilder;
public:
    static std::unique_ptr<ParserBuilder> build(const std::string& app_name);
    void parse(int argv, char** argc);
    void add_argument(ArgsVecType::value_type arg);

    OptionValue get(const std::string& option) const;
    bool help_triggered() const;
    bool version_triggered() const;

private:
    Parser();

    std::string name;
    std::string description;
    std::string brief;
    std::string epilog;
    double version;
    std::vector<std::string> authors;

    ExtraOption help_arg;
    ExtraOption version_arg;

    ArgsVecType args;

    std::string extract_option(const std::string& str);
    bool parse_extra_args(const std::string& option);
    ArgsVecType::value_type& get_arg_by_option(const std::string& option);
    std::string help();
    std::string authors_help_str() const;
};

class ParserBuilder
{
public:
    ParserBuilder(const std::string& name);

    ParserBuilder& description(const std::string& description);
    ParserBuilder& brief(const std::string& brief);
    ParserBuilder& epilog(const std::string& epilog);
    ParserBuilder& version(double version);
    ParserBuilder& author(const std::string& new_author);
    ParserBuilder& authors(std::initializer_list<std::string> authors);

    ParserBuilder& help_enabled();    
    ParserBuilder& version_enabled();    

    Parser get() const;

private:
    Parser parser;

};

}

#endif // _PARSER_H_