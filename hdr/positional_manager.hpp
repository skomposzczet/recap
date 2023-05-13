#ifndef _RCP_POSITIONAL_ARG_MANAGER_H_
#define _RCP_POSITIONAL_ARG_MANAGER_H_

#include "positional_arg.hpp"
#include "result.hpp"
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

    std::optional<IValueArg::OptionValue> get(const std::string& arg_name) const;
    
    void add(value_type arg);

    Result<> check_required_satisfied() const;

    ArgInfoVec get_arg_info() const;

    Result<value_type> get_next_after(long long prev_order) const;

private:
    container args;
};

}

#endif // _RCP_POSITIONAL_ARG_MANAGER_H_
