#ifndef _POSITIONAL_ARG_H_
#define _POSITIONAL_ARG_H_

#include "iarg.hpp"
#include "arg.hpp"

namespace rcp {

class PositionalArg: public IValueArg
{
friend class PositionalArgBuilder;
public:
    void set(const std::string& new_value) override;
    IValueArg::OptionValue get() const override;

    bool is_named(const std::string& arg_name) const override;
    bool is_triggered(const std::string& option) const override;
    bool is_ambiguous(const IArg& other) const override;

    std::string help() const override;

    unsigned get_order() const;

protected:
    OptionValue value;
    std::string name;
    std::string description;

    unsigned order;

    static unsigned highest_order;
    static void update_highest_order(unsigned order);
};

class PositionalArgBuilder
{
public:
    PositionalArgBuilder(const std::string& name);
    std::shared_ptr<PositionalArg> get();

    PositionalArgBuilder& order(unsigned order);
    PositionalArgBuilder& with_description(const std::string& description);

private:
    std::shared_ptr<PositionalArg> arg;

    bool order_set = false;
};

}

#endif // _POSITIONAL_ARG_H_