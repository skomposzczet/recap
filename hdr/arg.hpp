#ifndef _RCP_ARGUMENT_H_
#define _RCP_ARGUMENT_H_

#include "iarg.hpp"
#include "condition.hpp"

#include <string>
#include <vector>
#include <optional>
#include <memory>

namespace rcp {

class Arg: public IValueArg
{
friend class ArgBuilder;
public:
    Result<> set(const std::string& new_value) override;
    IValueArg::OptionValue get() const override;

    std::string get_name() const override;

    bool is_named(const std::string& arg_name) const override;
    bool is_triggered(const std::string& option) const override;
    bool is_ambiguous(const IArg& other) const override;

    std::string help() const override;

private:
    IValueArg::OptionValue value;
    IValueArg::OptionValue default_val;
    std::string name;
    std::string description;

    std::unique_ptr<ConditionBunch> conditions;
};

class ArgBuilder
{
public:
    ArgBuilder(const std::string& name);
    std::shared_ptr<Arg> get();

    ArgBuilder& default_value(const std::string& def);
    ArgBuilder& with_description(const std::string& description);
    ArgBuilder& with_condition(const Condition& condition);
    ArgBuilder& with_conditions(const ConditionBunch& bunch);

private:
    std::shared_ptr<Arg> arg;
};

}

#endif // _RCP_ARGUMENT_H_
