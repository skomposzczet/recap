#ifndef _POSITIONAL_ARG_MANAGER_H_
#define _POSITIONAL_ARG_MANAGER_H_

#include "positional_arg.hpp"
#include <map>
#include <memory>
#include <optional>

namespace rcp {

class PosArgManager
{
public:
    using container = std::map<PositionalArg::order_type, std::shared_ptr<PositionalArg>>;
    using value_type = container::mapped_type;
    PosArgManager() = default;

    std::optional<IValueArg::OptionValue> get(const std::string& arg_name);
    
    void add(value_type arg);

    container::iterator next();
    container::iterator end();

private:
    container args;
    container::iterator it = args.begin();
};

}

#endif // _POSITIONAL_ARG_MANAGER_H_