#include "parser.hpp"
#include "rcp_error.hpp"

#include <vector>
#include <algorithm>
#include <stdexcept>

namespace rcp {

void Parser::parse(int argc, const char** argv) {
    for (int i = 1 ; i < argc ; ++i) {
        auto option = extract_option(argv[i]);

        if (parse_flags(option))
            continue;

        auto& arg = get_arg_by_option(option);
        if (++i >= argc)
            throw ParseError("Missing value for option: " + option);
        arg->set(argv[i]);
    }
}

std::string Parser::extract_option(const std::string& str) {
    std::vector<std::string> prefixes{"--", "-"};

    for (const auto& prefix: prefixes) {
        if (str.rfind(prefix) != std::string::npos) {
            std::string option = std::string(str.begin()+prefix.size(), str.end());
            if (!option.empty())
                return option;
        }
    }

    throw ParseError(std::string{"Unexpected item: "} + str + "; option should start with hyphen");
}

bool Parser::parse_flags(const std::string& option) {
    auto res = std::find_if(flags.begin(), flags.end(), [&option](const auto& flag){
        return flag->is_triggered(option);
    });

    if (res == flags.end()) {
        return false;
    }

    (*res)->call();
    return true;
}

ArgsVecType::value_type& Parser::get_arg_by_option(const std::string& option) {
    auto res = std::find_if(args.begin(), args.end(), [&option](const auto& arg){
        return arg->is_triggered(option);
    });

    if (res == args.end())
        throw ParseError(std::string{"Unregistered option: "} + option);

    return *res;
}

void Parser::add_argument(ArgsVecType::value_type arg) {
    args.push_back(arg);
}

void Parser::add_flag(FlagsVecType::value_type flag) {
    flags.push_back(flag);
}

OptionValue Parser::get(const std::string& arg_name) const {
    auto res = std::find_if(args.begin(), args.end(), [&arg_name](const auto& arg){
        return arg->is_named(arg_name);
    });

    if (res == args.end())
        throw ParseError(std::string{"Unregistered arg: "} + arg_name);

    return (*res)->get();
}

bool Parser::help_triggered() const {
    return was_called("help");
}

bool Parser::version_triggered() const {
    return was_called("version");
}

bool Parser::was_called(const std::string& arg_name) const {
    auto res = std::find_if(flags.begin(), flags.end(), [&arg_name](const auto& flag){
        return flag->is_named(arg_name);
    });

    if (res == flags.end())
        throw ParseError(std::string{"Unregistered flag: "} + arg_name);

    return (*res)->was_called();
}

std::string Parser::help() const { 
    std::string help_text = std::string{"\t"} + name + " - " + brief
        + "\n\n\t" + description + "\n\n";

    const std::string arg_pre{"\t\t"};
    for (auto& arg: args) {
        help_text += arg_pre + arg->help() + "\n";
    }
    for (auto& flag: flags) {
        help_text += arg_pre + flag->help() + "\n";
    }

    help_text += authors_help_str();

    return help_text;
}

std::string Parser::authors_help_str() const {
    if (authors.empty())
        return std::string{""};

    std::string begin{"\n\n\t"};
    if (authors.size() == 1u) 
        return begin + "author: " + authors.at(0) + "\n";
    
    begin += "authors: ";
    for (const std::string& author: authors)
        begin += author + ", ";
    return begin + "\b\b \n";
}

std::string Parser::version() const {
    return name + "(" + _version + ")";
}

const std::vector<std::string>& Parser::get_authors() const {
    return authors;
}

}
