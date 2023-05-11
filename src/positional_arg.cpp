#include "positional_arg.hpp"
#include "util.hpp"

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

bool PositionalArg::is_triggered(const std::string&) const {
    return false;
}

bool PositionalArg::is_ambiguous(const IArg& other) const {
    return other.is_named(name);
}

ArgInfoVec PositionalArg::get_arg_info() const {
    ArgInfo info(Type::positional);
    info.order = order;
    info.required = required;
    info.description = description;
    info.value = name;

    return {info};
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