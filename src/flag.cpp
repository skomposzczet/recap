#include "flag.hpp"
#include "rcp_error.hpp"

namespace rcp {

bool Flag::was_called() const {
    return called;
}

bool Flag::is_named(const std::string& arg_name) const {
    return arg_name == name;
}

std::string Flag::get_name() const {
    return name;
}

bool Flag::is_triggered(const std::string& option) const {
    if (allow_short) {
        if (option == std::string{name[0]})
            return true;
    }
    if (allow_long) {
        if (option == name)
            return true;
    }
    return false;
}

bool Flag::is_ambiguous(const IArg& other) const {
    int res = false;
    if (allow_short) {
        res += other.is_triggered(std::string{name[0]});
    }
    if (allow_long) {
        res += other.is_triggered(name);
    }
    return res;
}

void Flag::call() {
    called = true;
}

ArgInfoVec Flag::get_arg_info() const {
    ArgInfo info{Type::other};
    if (allow_short)
        info.short_version = std::string{name[0]};
    if (allow_long)
        info.long_version = name;
    info.description = description;
    
    return {info};
}

FlagBuilder::FlagBuilder(const std::string& name) 
    : flag{std::make_shared<Flag>()}
{
    flag->name = name;
}

FlagBuilder& FlagBuilder::allow_long() {
    flag->allow_long = true;
    return *this;
}

FlagBuilder& FlagBuilder::forbid_short() {
    flag->allow_short = false;
    return *this;
}

FlagBuilder& FlagBuilder::with_description(const std::string& description) {
    flag->description = description;
    return *this;
}

std::shared_ptr<Flag> FlagBuilder::get() {
    if (!flag->allow_long && !flag->allow_short)
        throw BuildError("At least one trigger option is required ; consider allowing short or not forbidding long");
    return flag;
}

}