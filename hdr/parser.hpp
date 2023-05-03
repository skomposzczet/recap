#ifndef _PARSER_H_
#define _PARSER_H_

#include "flag.hpp"
#include "arg.hpp"
#include "positional_arg.hpp"
#include "positional_manager.hpp"

#include <string>
#include <memory>
#include <vector>
#include <optional>
#include <list>

namespace rcp {

using FlagsVecType = std::vector<std::shared_ptr<Flag>>;
using ArgsVecType = std::vector<std::shared_ptr<Arg>>;

class Parser
{
friend class ParserBuilder;
public:
    void parse(int argv, const char** argc);
    void add_flag(FlagsVecType::value_type arg);
    void add_argument(ArgsVecType::value_type arg);
    void add_positional_argument(PosArgManager::value_type arg);

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
    PosArgManager mgr;

    std::list<std::string> input;

    std::optional<std::string> extract_option(const std::string& str);
    std::optional<ArgsVecType::value_type> get_arg_by_option(const std::string& option);

    bool parse_positional(const std::string& value);
    bool parse_flag(const std::string& option);
    bool parse_key_arg(const std::string& option);
    
    std::string authors_help_str() const;
};

}

#endif // _PARSER_H_