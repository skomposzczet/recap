#ifndef _PARSER_H_
#define _PARSER_H_

#include "flag.hpp"
#include "arg.hpp"
#include "positional_arg.hpp"

#include <string>
#include <memory>
#include <vector>

namespace rcp {

using FlagsVecType = std::vector<std::shared_ptr<Flag>>;
using ArgsVecType = std::vector<std::shared_ptr<Arg>>;
using PosArgsVecType = std::vector<std::shared_ptr<PositionalArg>>;

class Parser
{
friend class ParserBuilder;
public:
    void parse(int argv, const char** argc);
    void add_flag(FlagsVecType::value_type arg);
    void add_argument(ArgsVecType::value_type arg);
    void add_positional_argument(PosArgsVecType::value_type arg);

    IValueArg::OptionValue get(const std::string& arg_name) const;
    bool was_called(const std::string& arg_name) const;
    
    bool help_triggered() const;
    bool version_triggered() const;

    std::string help() const;
    std::string version() const;

    const std::vector<std::string>& get_authors() const;

private:
    Parser() = default;

    std::string name;
    std::string description;
    std::string brief;
    std::string epilog;
    std::string _version;
    std::vector<std::string> authors;

    FlagsVecType flags;
    ArgsVecType args;
    PosArgsVecType pos_args;

    std::string extract_option(const std::string& str);
    bool parse_flags(const std::string& option);
    ArgsVecType::value_type& get_arg_by_option(const std::string& option);
    std::string authors_help_str() const;
};

}

#endif // _PARSER_H_