#ifndef _RCP_POSITIONAL_ARG_H_
#define _RCP_POSITIONAL_ARG_H_

#include "iarg.hpp"
#include "arg.hpp"

namespace rcp {

class PositionalArg: public IValueArg
{
friend class PositionalArgBuilder;
public:
    Result<> set(const std::string& new_value) override;
    IValueArg::OptionValue get() const override;

    std::string get_name() const override;

    bool is_named(const std::string& arg_name) const override;
    bool is_triggered(const std::string& option) const override;
    bool is_ambiguous(const IArg& other) const override;

    ArgInfoVec get_arg_info() const override;

    using order_type = unsigned;
    order_type get_order() const;
    bool is_required() const;

private:
    OptionValue value;
    std::string name;
    std::string description;

    order_type order;
    bool required = false;

    std::unique_ptr<ConditionBunch> conditions;

    static order_type highest_order;
    static void update_highest_order(order_type order);
};

class PositionalArgBuilder
{
public:
    PositionalArgBuilder(const std::string& name);
    std::shared_ptr<PositionalArg> get();

    PositionalArgBuilder& order(PositionalArg::order_type order);
    PositionalArgBuilder& with_description(const std::string& description);
    PositionalArgBuilder& with_condition(const Condition& condition);
    PositionalArgBuilder& with_conditions(const ConditionBunch& bunch);
    PositionalArgBuilder& required();

private:
    std::shared_ptr<PositionalArg> arg;

    bool order_set = false;
};

}

#endif // _RCP_POSITIONAL_ARG_H_