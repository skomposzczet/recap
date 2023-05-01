#include "arg.hpp"
#include "util.hpp"
#include <format>

namespace rcp {

OptionValue Arg::get() const {
    return value.has_value() ? value : default_val;
}

void Arg::set(const std::string& new_value) {
    value = new_value;
}

bool Arg::is_named(const std::string& arg_name) const {
    return arg_name == name;
}

bool Arg::is_triggered(const std::string& option) const {
    return option == name || (option.size() == 1 && option[0] == name[0]);
}

bool Arg::is_ambiguous(const IArg& other) const {
    return other.is_triggered(name) + other.is_triggered(std::string{name[0]});
}

std::string Arg::help() const {
    return std::format("-{}  --{}  <{}>  {}", name[0], name, util::upper(name), description);
}

ArgBuilder::ArgBuilder(const std::string& name) 
    : arg{std::make_unique<Arg>()} 
{
    arg->name = name;
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

}
