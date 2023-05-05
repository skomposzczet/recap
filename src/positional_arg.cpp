#include "positional_arg.hpp"
#include "util.hpp"
#include <format>

namespace rcp {

PositionalArg::order_type PositionalArg::highest_order{0};

Result<> PositionalArg::set(const std::string& new_value) {
    auto res = conditions->is_met(new_value);
    if (res.is_err()) 
        return ResultFactory::err(res.get_err());
        
    value = new_value;
    return ResultFactory::ok();
}

IValueArg::OptionValue PositionalArg::get() const {
    return value;
}

std::string PositionalArg::get_name() const {
    return name;
}

bool PositionalArg::is_named(const std::string& arg_name) const {
    return arg_name == name;
}

bool PositionalArg::is_triggered(const std::string& option) const {
    return is_named(option) || (option.size() == 1u && option[0] == name[0]);
}

bool PositionalArg::is_ambiguous(const IArg& other) const {
    return other.is_triggered(name) || other.is_triggered(std::string{name[0]});
}

std::string PositionalArg::help() const {
    return std::format("-{}  --{}  <{}>  {}", name[0], name, util::upper(name), description);
}

PositionalArg::order_type PositionalArg::get_order() const {
    return order;
}

bool PositionalArg::is_required() const {
    return required;
}

void PositionalArg::update_highest_order(order_type order) {
    highest_order = ( highest_order > order ? highest_order : order + 1 );
}

PositionalArgBuilder::PositionalArgBuilder(const std::string& name) 
    : arg{std::make_shared<PositionalArg>()} 
{
    arg->name = name;
    arg->conditions = std::make_unique<ConditionBunch>(arg->name);
}

std::shared_ptr<PositionalArg> PositionalArgBuilder::get() {
    if (!order_set) {
        arg->order = PositionalArg::highest_order;
    }
    PositionalArg::update_highest_order(arg->order);

    return arg;
}

PositionalArgBuilder& PositionalArgBuilder::order(PositionalArg::order_type order) {
    arg->order = order;
    order_set = true;

    return *this;
}

PositionalArgBuilder& PositionalArgBuilder::with_description(const std::string& description) {
    arg->description = description;
    return *this;
}

PositionalArgBuilder& PositionalArgBuilder::with_condition(const Condition& condition) {
    arg->conditions->add(condition);
    return *this;
}

PositionalArgBuilder& PositionalArgBuilder::with_conditions(const ConditionBunch& bunch) {
    arg->conditions->add_bunch(bunch);
    return *this;
}

PositionalArgBuilder& PositionalArgBuilder::required() {
    arg->required = true;
    return *this;
}

}