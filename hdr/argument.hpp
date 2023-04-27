#ifndef _ARGUMENT_H_
#define _ARGUMENT_H_

#include <string>
#include <vector>
#include <optional>

using OptionValue = std::optional<std::string>;

class Argument
{
friend class ArgumentBuilder;
public:
    OptionValue get() const {
        return value.has_value() ? value : default_val;
    };

    void set(const std::string& new_value) {
        value = new_value;
    }

    bool is_triggered(const std::string& option) {
        return option == name || (option.size() == 1 && option[0] == name[0]);
    }

protected:
    OptionValue value;
    OptionValue default_val;
    std::string name;
    std::string description;
};

#endif // _ARGUMENT_H_