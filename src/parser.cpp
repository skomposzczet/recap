#include "parser.hpp"
#include "rcp_error.hpp"
#include "util.hpp"

#include <vector>
#include <algorithm>
#include <stdexcept>

namespace rcp {

void Parser::parse(int argc, const char** argv) {
    if (argc < 2)
        return;

    input = std::list<std::string>(argv+1, argv+argc);

    while (!input.empty()) {
        auto res = parse_next();
        if (res.is_err())
            throw ParseError(res.get_err());
    }
}

ParseResult Parser::parse_next() {
    std::string current = input.front();
    input.pop_front();

    auto option = extract_option(current);
    if (!option.has_value())
        return parse_positional(current);

    auto res = parse_flag(option.value());
    if (res.is_ok())
        return res;

    return parse_key_arg(option.value());
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

ParseResult Parser::parse_positional(const std::string& value) {
    auto it = mgr.next();
    if (it == mgr.end())
        return ResultFactory::err(util::cat("Unexpected item: ", value));

    return (*it).second->set(value);
}

ParseResult Parser::parse_flag(const std::string& option) {
    auto res = std::find_if(flags.begin(), flags.end(), [&option](const auto& flag){
        return flag->is_triggered(option);
    });

    if (res == flags.end()) {
        return ResultFactory::err(util::cat("No such flag registered: ", option));
    }

    (*res)->call();
    return ResultFactory::ok();
}

ParseResult Parser::parse_key_arg(const std::string& option) {
    auto res = get_arg_by_option(option);
    if (!res.has_value())
        return ResultFactory::err(util::cat("No such arg registered: ", option));

    if (input.empty())
        return ResultFactory::err(util::cat("Missing value for arg: ", option));

    std::string value = input.front();
    input.pop_front();

    return (*res)->set(value);
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
    auto res = check_ambiguity(*flag);
    if (res.is_err())
        throw ParseError(res.get_err());

    flags.push_back(flag);
}

void Parser::add_argument(ArgsVecType::value_type arg) {
    auto res = check_ambiguity(*arg);
    if (res.is_err())
        throw ParseError(res.get_err());

    args.push_back(arg);
}

void Parser::add_positional_argument(PosArgManager::value_type arg) {
    auto res = check_ambiguity(*arg);
    if (res.is_err())
        throw ParseError(res.get_err());

    mgr.add(arg);
}

ParseResult Parser::check_ambiguity(const IArg& arg) const {
    const std::string& err_str = util::cat("Ambiguity :", arg.get_name(), " but ");

    for (const auto& earg: flags) {
        if (arg.is_ambiguous(*earg))
            return ResultFactory::err(util::cat(err_str, "flag \"", earg->get_name(), "\" already exists"));
    }
    for (const auto& earg: args) {
        if (arg.is_ambiguous(*earg))
            return ResultFactory::err(util::cat(err_str, "arg \"", earg->get_name(), "\" already exists"));
    }
    auto res = mgr.get(arg.get_name());
    if (res.has_value())
            return ResultFactory::err(util::cat(err_str, "positional arg \"", arg.get_name(), "\" already exists"));

    return ResultFactory::ok();
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
