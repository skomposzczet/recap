#ifndef _ARGUMENT_BUILDER_H_
#define _ARGUMENT_BUILDER_H_

#include "argument.hpp"

#include <string>
#include <memory>

class ArgumentBuilder
{
public:
    ArgumentBuilder(const std::string& name) 
        : arg{std::make_unique<Argument>()}
    {
        arg->name = name;
    }

    std::shared_ptr<Argument> get() 
    {
        return arg;
    }

    ArgumentBuilder& default_value(const std::string& def)
    {
        arg->default_val = def;
        return *this;
    }

    ArgumentBuilder& with_description(const std::string& description)
    {
        arg->description = description;
        return *this;
    }

private:
    std::shared_ptr<Argument> arg;
};

#endif // _ARGUMENT_BUILDER_H_