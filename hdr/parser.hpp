#ifndef _PARSER_H_
#define _PARSER_H_

#include "arg.hpp"

#include <string>
#include <memory>
#include <vector>

namespace rcp {

enum class ExtraOption {
    disabled,
    enabled,
    triggered,
};

using ArgsVecType = std::vector<std::shared_ptr<Arg>>;

class Parser
{
friend class ParserBuilder;
public:
    void parse(int argv, const char** argc);
    void add_argument(ArgsVecType::value_type arg);

    OptionValue get(const std::string& option) const;
    bool help_triggered() const;
    bool version_triggered() const;

    std::string help() const;
    std::string version() const;

    const std::vector<std::string>& get_authors() const;

private:
    Parser();

    std::string name;
    std::string description;
    std::string brief;
    std::string epilog;
    std::string _version;
    std::vector<std::string> authors;

    ExtraOption help_arg;
    ExtraOption version_arg;

    ArgsVecType args;

    std::string extract_option(const std::string& str);
    bool parse_extra_args(const std::string& option);
    ArgsVecType::value_type& get_arg_by_option(const std::string& option);
    std::string authors_help_str() const;
};

}

#endif // _PARSER_H_