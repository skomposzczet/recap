#ifndef _ARGUMENT_H_
#define _ARGUMENT_H_

#include <string>
#include <vector>
#include <optional>
#include <memory>

namespace rcp {

using OptionValue = std::optional<std::string>;

class Arg
{
friend class ArgBuilder;
public:
    OptionValue get() const;

    bool is_triggered(const std::string& option) const;
    void set(const std::string& new_value);

    std::string help() const;

protected:
    OptionValue value;
    OptionValue default_val;
    std::string name;
    std::string description;
};

class ArgBuilder
{
public:
    ArgBuilder(const std::string& name);
    std::shared_ptr<Arg> get();

    ArgBuilder& default_value(const std::string& def);
    ArgBuilder& with_description(const std::string& description);

private:
    std::shared_ptr<Arg> arg;
};

}

#endif // _ARGUMENT_H_
