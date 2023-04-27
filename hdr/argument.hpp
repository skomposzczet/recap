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

    bool is_triggered(const std::string& option) {
        return option == name || (option.size() == 1 && option[0] == name[0]);
    }

protected:
    OptionValue value;
    OptionValue default_val;
    std::string name;
};
