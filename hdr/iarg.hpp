#ifndef _RCP_IARG_H_
#define _RCP_IARG_H_

#include "result.hpp"
#include "doc_maker.hpp"

#include <string>
#include <optional>

namespace rcp {

using ArgInfoVec = std::vector<ArgInfo>;

class IArg
{
public:
    virtual std::string get_name() const = 0;

    virtual bool is_named(const std::string&) const = 0;
    virtual bool is_triggered(const std::string&) const = 0;

    virtual bool is_ambiguous(const IArg&) const = 0;

    virtual ArgInfoVec get_arg_info() const = 0;
};

class IValueArg: public IArg
{
public:
    using OptionValue = std::optional<std::string>;

    virtual Result<> set(const std::string& new_value) = 0;
    virtual IValueArg::OptionValue get() const = 0;
};

}

#endif // _RCP_IARG_H_