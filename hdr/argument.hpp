#ifndef _ARGUMENT_H_
#define _ARGUMENT_H_

#include <string>
#include <vector>
#include <optional>
#include <memory>

namespace rcp {

using OptionValue = std::optional<std::string>;

class Argument
{
friend class ArgumentBuilder;
public:
    OptionValue get() const;
    void set(const std::string& new_value);

    bool is_triggered(const std::string& option);

protected:
    OptionValue value;
    OptionValue default_val;
    std::string name;
    std::string description;
};

class ArgumentBuilder
{
public:
    ArgumentBuilder(const std::string& name);
    std::shared_ptr<Argument> get();

    ArgumentBuilder& default_value(const std::string& def);
    ArgumentBuilder& with_description(const std::string& description);

private:
    std::shared_ptr<Argument> arg;
};

}

#endif // _ARGUMENT_H_
