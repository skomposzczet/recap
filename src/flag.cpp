#include "flag.hpp"
#include "rcp_error.hpp"
#include <format>

namespace rcp {

bool Flag::was_called() const {
    return called;
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

void Flag::call() {
    called = true;
}

std::string Flag::help() const {
    std::string res;
    if (allow_short) {
        res += std::format("-{}  ", name[0]);
    }
    if (allow_long) {
        res += std::format("--{}  ", name);
    }
    return res + description;
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
        throw RcpError("At least one trigger option is required ; consider allowing short or not forbidding long");
    return flag;
}

}