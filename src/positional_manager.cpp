#include "positional_manager.hpp"
#include <algorithm>

namespace rcp {

std::optional<IValueArg::OptionValue> PosArgManager::get(const std::string& arg_name) {
    auto res = std::find_if(args.begin(), args.end(), [&arg_name](const auto& arg) {
        return arg.second->is_named(arg_name);
    });

    if (res != args.end())
        return (*res).second->get();

    return {};
}

void PosArgManager::add(value_type arg) {
    args.emplace(arg->get_order(), arg);
    it = args.begin();
}

PosArgManager::container::iterator PosArgManager::next() {
    return it++;
}

PosArgManager::container::iterator PosArgManager::end() {
    return args.end();
}

}
