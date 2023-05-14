#include "positional_manager.hpp"
#include "util.hpp"
#include <algorithm>

namespace rcp {

std::optional<IValueArg::OptionValue> PosArgManager::get(const std::string& arg_name) const {
    auto res = std::find_if(args.begin(), args.end(), [&arg_name](const auto& arg) {
        return arg.second->is_named(arg_name);
    });

    if (res != args.end())
        return (*res).second->get();

    return {};
}

void PosArgManager::add(PosArgManager::value_type arg) {
    const auto order = arg->get_order();
    if (args.contains(order))
        throw BuildError(util::cat("Positional arg with order ", order, " already exists."));
    args.emplace(order, arg);
}

ArgInfoVec PosArgManager::get_arg_info() const {
    ArgInfoVec vec;
    for (const auto& [order, arg]: args)
        vec.push_back(arg->get_arg_info().front());
    return vec;
}

Result<PosArgManager::value_type> PosArgManager::get_next_after(long long prev_order) const {
    if (args.empty())
        return ResultFactory::err<PosArgManager::value_type>("No positional args.");

    if (prev_order < 0)
        return ResultFactory::ok((*args.begin()).second);
    
    PositionalArg::order_type prev = static_cast<PositionalArg::order_type>(prev_order);
    for(const auto& [order, arg]: args) {
        if (order > prev)
            return ResultFactory::ok(arg);
    }

    std::string err_str = util::cat("No positional argument with order higher than ", prev_order);
    return ResultFactory::err<PosArgManager::value_type>(err_str);
}

Result<> PosArgManager::check_required_satisfied() const {
    for(const auto& [order, arg]: args) {
        if (arg->is_required() && !arg->get().has_value())
            return ResultFactory::err(util::cat("Requires value but not set: ", arg->get_name()));
    }

    return ResultFactory::ok();
}

}
