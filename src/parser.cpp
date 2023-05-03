#include "parser.hpp"
#include "rcp_error.hpp"

#include <vector>
#include <algorithm>
#include <stdexcept>

namespace rcp {

void Parser::parse(int argc, const char** argv) {
    if (argc < 2)
        return;

    input = std::list<std::string>(argv+1, argv+argc);

    while (!input.empty()) {
        std::string current = input.front();
        input.pop_front();

        auto option = extract_option(current);
        if (!option.has_value()) {
            if (parse_positional(current))
                continue;
            else
                throw ParseError(std::string{"Unexpected item: "} + current + "; option should start with hyphen");
        } else {
            if (parse_flag(option.value()))
                continue;

            if (parse_key_arg(option.value()))
                continue;
        }

        throw ParseError(std::string{"Failed to parse \""} + current + "\"");
    }
}

std::optional<std::string> Parser::extract_option(const std::string& str) {
    std::vector<std::string> prefixes{"--", "-"};

    for (const auto& prefix: prefixes) {
        if (str.rfind(prefix) != std::string::npos) {
            std::string option = std::string(str.begin()+prefix.size(), str.end());
            if (!option.empty())
                return option;
        }
    }

    return {};
}

bool Parser::parse_positional(const std::string& value) {
    auto it = mgr.next();
    if (it == mgr.end())
        return false;

    (*it).second->set(value);
    return true;
}

bool Parser::parse_flag(const std::string& option) {
    auto res = std::find_if(flags.begin(), flags.end(), [&option](const auto& flag){
        return flag->is_triggered(option);
    });

    if (res == flags.end()) {
        return false;
    }

    (*res)->call();
    return true;
}

bool Parser::parse_key_arg(const std::string& option) {
    if (input.empty())
        return false;

    auto res = get_arg_by_option(option);
    if (!res.has_value())
        return false;

    std::string value = input.front();
    input.pop_front();
    (*res)->set(value);
    return true;
}

std::optional<ArgsVecType::value_type> Parser::get_arg_by_option(const std::string& option) {
    auto res = std::find_if(args.begin(), args.end(), [&option](const auto& arg){
        return arg->is_triggered(option);
    });

    if (res == args.end())
        return {};

    return *res;
}

void Parser::add_flag(FlagsVecType::value_type flag) {
    flags.push_back(flag);
}

void Parser::add_argument(ArgsVecType::value_type arg) {
    args.push_back(arg);
}

void Parser::add_positional_argument(PosArgManager::value_type arg) {
    mgr.add(arg);
}

IValueArg::OptionValue Parser::get(const std::string& arg_name) const {
    auto pos_res = mgr.get(arg_name);
    if (pos_res.has_value())
        return *pos_res;

    auto res = std::find_if(args.begin(), args.end(), [&arg_name](const auto& arg){
        return arg->is_named(arg_name);
    });
    if (res != args.end())
        return (*res)->get();

    throw ParseError(std::string{"Unregistered arg: "} + arg_name);
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
