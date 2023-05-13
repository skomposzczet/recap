#include "arg.hpp"
#include "util.hpp"

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
    bool res = false;
    if (long_allowed)
        res += option == name;
    if (short_allowed)
        res += option.size() == 1 && option[0] == name[0];
    for (const std::string& alias: aliases) 
        res += option == alias;
    return res;
}

bool Arg::is_ambiguous(const IArg& other) const {
    bool res = false;
    res += other.is_named(name);
    if (long_allowed)
        res += other.is_triggered(name);
    if (short_allowed)
        res += other.is_triggered(std::string{name[0]});
    for (const std::string& alias: aliases) 
        res += other.is_triggered(alias);
    return res;
}

ArgInfoVec Arg::get_arg_info() const {
    ArgInfoVec vec{};
    append_base_arg_info(vec);
    append_aliases_arg_info(vec);
    return vec;
}

void Arg::append_base_arg_info(ArgInfoVec& vec) const {
    ArgInfo info(Type::other);
    info.short_version = std::string{name[0]};
    info.long_version = name;
    info.description = description;
    info.value = name;

    vec.push_back(info);
}

void Arg::append_aliases_arg_info(ArgInfoVec& vec) const {
    std::string alias_desc{util::cat("Alias for ", name)};

    for (const std::string& alias: aliases) {
        ArgInfo info(Type::other);
        info.long_version = alias;
        info.value = name;
        info.description = alias_desc;

        vec.push_back(info);
    }
}

ArgBuilder::ArgBuilder(const std::string& name) 
    : arg{std::make_unique<Arg>()} 
{
    arg->name = name;
    arg->conditions = std::make_unique<ConditionBunch>(arg->name);
}

std::shared_ptr<Arg> ArgBuilder::get() {
    if (arg->short_allowed || arg->long_allowed)
        return arg;
    throw BuildError("At least one trigger option is required ; consider allowing short or long");
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

ArgBuilder& ArgBuilder::with_alias(const std::string& alias) {
    arg->aliases.push_back(alias);
    return *this;
}

ArgBuilder& ArgBuilder::with_aliases(const std::vector<std::string>& aliases) {
    std::vector<std::string>& aa = arg->aliases;
    aa.insert(aa.end(), aliases.begin(), aliases.end());
    return *this;
}

ArgBuilder& ArgBuilder::forbid_short() {
    arg->short_allowed = false;
    return *this;
}

ArgBuilder& ArgBuilder::forbid_long() {
    arg->long_allowed = false;
    return *this;
}

}
