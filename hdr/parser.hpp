#ifndef _RCP_PARSER_H_
#define _RCP_PARSER_H_

#include "flag.hpp"
#include "arg.hpp"
#include "positional_arg.hpp"
#include "positional_manager.hpp"
#include "result.hpp"
#include "doc_maker.hpp"

#include <string>
#include <memory>
#include <vector>
#include <optional>
#include <list>

namespace rcp {

using FlagsVecType = std::vector<std::shared_ptr<Flag>>;
using ArgsVecType = std::vector<std::shared_ptr<Arg>>;

using ParseResult = Result<>; 

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

    std::string version() const;
    template<typename T = NixDocMaker> requires CanGenerate<T>
    T help() const;

private:
    Parser() = default;

    AppInfo app_info;

    FlagsVecType flags;
    ArgsVecType args;
    PosArgManager mgr;

    long long prev_order = -1;
    bool ignore_req_on_help = false;

    std::list<std::string> input;

    ParseResult check_ambiguity(const IArg& arg) const;
    ParseResult check_valid_parsing() const;

    ParseResult parse_next();
    ParseResult parse_positional(const std::string& value);
    ParseResult parse_flag(const std::string& option);
    ParseResult parse_key_arg(const std::string& option);

    static std::optional<std::string> extract_option(const std::string& str);
    Result<ArgsVecType::value_type> get_arg_by_option(const std::string& option) const;
    Result<FlagsVecType::value_type> get_flag_by_option(const std::string& option) const;

    const AppInfo& get_app_info() const;
    ArgInfoVec get_arg_info() const;

};

template<typename T> requires CanGenerate<T>
T Parser::help() const {
    return T::generate(get_app_info(), get_arg_info());
}

}

#endif // _RCP_PARSER_H_
