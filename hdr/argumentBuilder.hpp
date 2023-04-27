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

private:
    std::unique_ptr<Argument> arg;
};