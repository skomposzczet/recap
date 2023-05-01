#ifndef _ARGUMENT_H_
#define _ARGUMENT_H_

#include "iarg.hpp"

#include <string>
#include <vector>
#include <optional>
#include <memory>

namespace rcp {

class Arg: public IValueArg
{
friend class ArgBuilder;
public:
    void set(const std::string& new_value) override;
    IValueArg::OptionValue get() const override;

    bool is_named(const std::string& arg_name) const override;
    bool is_triggered(const std::string& option) const override;
    bool is_ambiguous(const IArg& other) const override;

    std::string help() const override;

protected:
    IValueArg::OptionValue value;
    IValueArg::OptionValue default_val;
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
