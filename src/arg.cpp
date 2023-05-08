#include "arg.hpp"
#include "util.hpp"
#include <format>

namespace rcp {

IValueArg::OptionValue Arg::get() const {
    return value.has_value() ? value : default_val;
}

Result<> Arg::set(const std::string& new_value) {
    auto res = conditions->is_met(new_value);
    if (res.is_err()) 
        return ResultFactory::err(res.get_err());
        
    value = new_value;
    return ResultFactory::ok();
}

std::string Arg::get_name() const {
    return name;
}

bool Arg::is_named(const std::string& arg_name) const {
    return arg_name == name;
}

bool Arg::is_triggered(const std::string& option) const {
    return option == name || (option.size() == 1 && option[0] == name[0]);
}

bool Arg::is_ambiguous(const IArg& other) const {
    bool res = false;
    res += other.is_named(name);
    res += other.is_triggered(name);
    res += other.is_triggered(std::string{name[0]});
    return res; 
}

std::string Arg::help() const {
    return std::format("-{}  --{}  <{}>  {}", name[0], name, util::upper(name), description);
}

ArgBuilder::ArgBuilder(const std::string& name) 
    : arg{std::make_unique<Arg>()} 
{
    arg->name = name;
    arg->conditions = std::make_unique<ConditionBunch>(arg->name);
}

std::shared_ptr<Arg> ArgBuilder::get() {
    return arg;
}

ArgBuilder& ArgBuilder::default_value(const std::string& def) {
    arg->default_val = def;
    return *this;
}

ArgBuilder& ArgBuilder::with_description(const std::string& description) {
    arg->description = description;
    return *this;
}

ArgBuilder& ArgBuilder::with_condition(const Condition& condition) {
    arg->conditions->add(condition);
    return *this;
}

ArgBuilder& ArgBuilder::with_conditions(const ConditionBunch& bunch) {
    arg->conditions->add_bunch(bunch);
    return *this;
}

}
