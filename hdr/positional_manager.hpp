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
    PosArgManager();
    PosArgManager(const PosArgManager& other);

    std::optional<IValueArg::OptionValue> get(const std::string& arg_name) const;
    
    void add(value_type arg);

    Result<> check_required_satisfied() const;

    container::iterator next();
    container::iterator end();

private:
    container args;
    container::iterator it;
};

}

#endif // _RCP_POSITIONAL_ARG_MANAGER_H_
