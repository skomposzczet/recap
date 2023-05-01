#ifndef _ARGUMENT_H_
#define _ARGUMENT_H_

#include "iarg.hpp"

#include <string>
#include <vector>
#include <optional>
#include <memory>

namespace rcp {

using OptionValue = std::optional<std::string>;

class Arg: public IArg
{
friend class ArgBuilder;
public:
    void set(const std::string& new_value);
    OptionValue get() const;

    bool is_named(const std::string& arg_name) const override;
    bool is_triggered(const std::string& option) const override;
    bool is_ambiguous(const IArg& other) const override;

    std::string help() const override;

protected:
    OptionValue value;
    OptionValue default_val;
    std::string name;
    std::string description;
};

class ArgBuilder
{
public:
    ArgBuilder(const std::string& name);
    std::shared_ptr<Arg> get();

    ArgBuilder& default_value(const std::string& def);
    ArgBuilder& with_description(const std::string& description);

private:
    std::shared_ptr<Arg> arg;
};

}

#endif // _ARGUMENT_H_
