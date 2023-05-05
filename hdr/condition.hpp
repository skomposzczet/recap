#ifndef _RCP_CONDITION_H_
#define _RCP_CONDITION_H_

#include "result.hpp"

#include <functional>
#include <string>
#include <vector>
#include <memory>

namespace rcp {

using ConditionResult = Result<>;

class ICondition
{
public:
    using input_type = const std::string&;
    using CheckingFunc = std::function<bool(input_type)>;

    virtual ConditionResult is_met(input_type) const = 0;
};

class Condition: public ICondition
{
public:
    static Condition int_parsable();
    static Condition float_parsable();

    Condition(const std::string& name, const CheckingFunc& func);

    ConditionResult is_met(input_type input) const override;
    std::string get_name() const;

private:
    std::string name;
    CheckingFunc cond;

};

class ConditionBunch: public ICondition
{
public:
    ConditionBunch(const std::string& name);

    ConditionResult is_met(input_type input) const override;

    ConditionBunch& add(const Condition& condition);
    ConditionBunch& add_bunch(const ConditionBunch& condition);

private:
    std::string name;
    std::vector<std::shared_ptr<ICondition>> conds;
};

}

#endif // _RCP_CONDITION_H_