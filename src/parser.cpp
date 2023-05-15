#include "parser.hpp"
#include "rcp_error.hpp"
#include "util.hpp"

#include <vector>
#include <algorithm>
#include <stdexcept>

namespace rcp {

void Parser::parse(int argc, const char** argv) {
    if (argc >= 2) {
        input = std::list<std::string>(argv+1, argv+argc);

        while (!input.empty()) {
            auto res = parse_next();
            if (res.is_err())
                throw ParseError(res.get_err());
        }
    }

    auto res = check_valid_parsing();
    if (res.is_err())
        throw ParseError(res.get_err());
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
    auto res = mgr.get_next_after(prev_order);
    if (res.is_err())
        return ResultFactory::err(res.get_err());

    auto pos_arg = res.get_ok();
    prev_order = pos_arg->get_order();
    return pos_arg->set(value);
}

ParseResult Parser::parse_flag(const std::string& option) {
    auto res = std::find_if(flags.begin(), flags.end(), [&option](const auto& flag){
        return flag->is_triggered(option);
    });

    if (res == flags.end()) {
        return ResultFactory::err(util::cat("No such flag registered: ", option));
    }

    (*res)->toggle();
    return ResultFactory::ok();
}

ParseResult Parser::parse_key_arg(const std::string& option) {
    auto res = get_arg_by_option(option);
    if (!res.is_ok())
        return ResultFactory::err(res.get_err());

    if (input.empty())
        return ResultFactory::err(util::cat("Missing value for arg: ", option));

    std::string value = input.front();
    input.pop_front();

    return res.get_ok()->set(value);
}

Result<ArgsVecType::value_type> Parser::get_arg_by_option(const std::string& option) const {
    auto res = std::find_if(args.begin(), args.end(), [&option](const auto& arg){
        return arg->is_triggered(option);
    });

    if (res == args.end())
        return ResultFactory::err<ArgsVecType::value_type>(util::cat("No such arg registered: ", option));

    return ResultFactory::ok(*res);
}

ParseResult Parser::check_valid_parsing() const {
    if (ignore_req_on_help) {
        auto res = get_flag_by_option("help");
        if (res.is_ok() && res.get_ok()->was_toggled())
            return ResultFactory::ok();
    }
    return mgr.check_required_satisfied();
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

bool Parser::help_toggled() const {
    return was_toggled("help");
}

bool Parser::version_toggled() const {
    return was_toggled("version");
}

bool Parser::was_toggled(const std::string& arg_name) const {
    auto res = get_flag_by_option(arg_name);
    if (res.is_ok())
        return res.get_ok()->was_toggled();
    throw ParseError(res.get_err());
}

Result<FlagsVecType::value_type> Parser::get_flag_by_option(const std::string& option) const {
    auto res = std::find_if(flags.begin(), flags.end(), [&option](const auto& arg){
        return arg->is_named(option);
    });

    if (res == flags.end())
        return ResultFactory::err<FlagsVecType::value_type>(util::cat("No such flag registered: ", option));

    return ResultFactory::ok(*res);
}

std::string Parser::version() const {
    return app_info.name + "(" + app_info.version + ")";
}

const AppInfo& Parser::get_app_info() const {
    return app_info;
}

ArgInfoVec Parser::get_arg_info() const {
    ArgInfoVec vec;

    for(const auto& arg: args) {
        auto info_vec = arg->get_arg_info();
        vec.insert(vec.end(), info_vec.begin(), info_vec.end());
    }

    for(const auto& arg: flags) {
        auto info_vec = arg->get_arg_info();
        vec.insert(vec.end(), info_vec.begin(), info_vec.end());
    }

    auto info_vec = mgr.get_arg_info();
    vec.insert(vec.end(), info_vec.begin(), info_vec.end());
    
    return vec;
}

}
