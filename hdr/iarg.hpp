#ifndef _RCP_IARG_H_
#define _RCP_IARG_H_

#include <string>
#include <optional>

namespace rcp {

class IArg
{
public:
    virtual std::string get_name() const = 0;

    virtual bool is_named(const std::string&) const = 0;
    virtual bool is_triggered(const std::string&) const = 0;

    virtual bool is_ambiguous(const IArg&) const = 0;

    virtual std::string help() const = 0;
};

class IValueArg: public IArg
{
public:
    using OptionValue = std::optional<std::string>;

    virtual void set(const std::string& new_value) = 0;
    virtual IValueArg::OptionValue get() const = 0;
};

}

#endif // _RCP_IARG_H_