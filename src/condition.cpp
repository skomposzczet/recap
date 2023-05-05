#include "condition.hpp"
#include "util.hpp"

namespace rcp {

bool is_int(Condition::input_type input) {
    if(input.empty() || ((!isdigit(input.at(0))) && (input.at(0) != '-') && (input.at(0) != '+'))) 
        return false;

    char* p;
    strtol(input.data(), &p, 10);

    return *p == 0;
}

bool is_float(Condition::input_type input) {
    std::string::const_iterator it = input.begin();
    bool decimalPoint = false;
    size_t minSize = 0u;
    if(input.size() > 0 && (input.at(0) == '-' || input.at(0) == '+')) {
        it++;
        minSize++;
    }
    while(it != input.end()) {
        if(*it == '.') {
            if(!decimalPoint)
                decimalPoint = true;
            else 
                break;
        }
        else if(!std::isdigit(*it) && ((*it!='f') || it+1 != input.end() || !decimalPoint)) {
            break;
        }

        ++it;
    }
    return input.size() > minSize && it == input.end();
}

Condition Condition::int_parsable() {
    return Condition("IntParsable", is_int);
}

Condition Condition::float_parsable() {
    return Condition("FloatParsable", is_float);
}

Condition::Condition(const std::string& name, const ICondition::CheckingFunc& func) 
    : name{name}, cond{func} {}

ConditionResult Condition::is_met(ICondition::input_type input) const {
    if (cond(input))
        return ResultFactory::ok();
    return ResultFactory::err(name);
}

std::string Condition::get_name() const {
    return name;
}

ConditionBunch::ConditionBunch(const std::string& name) 
    : name{name} {}

ConditionResult ConditionBunch::is_met(input_type input) const {
    for (const auto& condition: conds) {
        auto res = condition->is_met(input);
        if (res.is_err())
            return ResultFactory::err(util::cat(name, "::", res.get_err()));
    }
    return ResultFactory::ok();
}

ConditionBunch& ConditionBunch::add(const Condition& condition) {
    conds.emplace_back(std::make_shared<Condition>(condition));
    return *this;
}

ConditionBunch& ConditionBunch::add_bunch(const ConditionBunch& condition) {
    conds.emplace_back(std::make_shared<ConditionBunch>(condition));
    return *this;
}

}