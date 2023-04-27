#include "argument.hpp"

namespace rcp {

OptionValue Argument::get() const {
    return value.has_value() ? value : default_val;
}

void Argument::set(const std::string& new_value) {
    value = new_value;
}

bool Argument::is_triggered(const std::string& option) {
    return option == name || (option.size() == 1 && option[0] == name[0]);
}

ArgumentBuilder::ArgumentBuilder(const std::string& name) 
    : arg{std::make_unique<Argument>()} 
{
    arg->name = name;
}

std::shared_ptr<Argument> ArgumentBuilder::get() {
    return arg;
}

ArgumentBuilder& ArgumentBuilder::default_value(const std::string& def) {
    arg->default_val = def;
    return *this;
}

ArgumentBuilder& ArgumentBuilder::with_description(const std::string& description) {
    arg->description = description;
    return *this;
}

}
