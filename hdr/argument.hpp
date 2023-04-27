#include <string>
#include <vector>
#include <optional>

using OptionValue = std::optional<std::string>;

template<typename T>
class Argument
{
public:
    OptionValue get() const {
        return value.has_value() ? value : default_val;
    };

    bool is_triggered(const std::string& option) {
        return option == name || option == std::string(name[0]);
    }

protected:
    OptionValue value;
    OptionValue default_val;
    std::string name;
};
