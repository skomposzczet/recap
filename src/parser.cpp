#include "parser.hpp"
#include "rcp_error.hpp"

#include <vector>
#include <algorithm>
#include <stdexcept>

namespace rcp {

Parser::Parser() 
    : help_arg{ExtraOption::disabled}, version_arg{ExtraOption::disabled}
{}

void Parser::parse(int argc, const char** argv) {
    for (int i = 1 ; i < argc ; ++i) {
        auto option = extract_option(argv[i]);

        if (parse_extra_args(option))
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

bool Parser::parse_extra_args(const std::string& option) {
    if (help_arg == ExtraOption::enabled && option == "help") {
        help_arg = ExtraOption::triggered;
        return true;
    }
    if (version_arg == ExtraOption::enabled && option == "version") {
        version_arg = ExtraOption::triggered;
        return true;
    }
    return false;
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

OptionValue Parser::get(const std::string& option) const {
    auto res = std::find_if(args.begin(), args.end(), [&option](const auto& arg){
        return arg->is_triggered(option);
    });

    if (res == args.end())
        throw ParseError(std::string{"Unregistered option: "} + option);

    return (*res)->get();
}

bool Parser::help_triggered() const {
    return help_arg == ExtraOption::triggered;
}

bool Parser::version_triggered() const {
    return version_arg == ExtraOption::triggered;
}

std::string Parser::help() const { 
    std::string help_text = std::string{"\t"} + name + " - " + brief
        + "\n\n\t" + description + "\n\n";

    const std::string arg_pre{"\t\t"}; 
    for (auto& arg: args) {
        help_text += arg_pre + arg->help() + "\n";
    }
    if (help_arg == ExtraOption::enabled)
        help_text += arg_pre + "--help\n";
    if (version_arg == ExtraOption::enabled)
        help_text += arg_pre + "--version\n";

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
