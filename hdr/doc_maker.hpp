#ifndef _RCP_DOC_MAKER_H_
#define _RCP_DOC_MAKER_H_

#include <string>
#include <optional>
#include <vector>

namespace rcp {

using OptStr = std::optional<std::string>;

struct AppInfo {
    std::string name;
    std::string version;
    std::vector<std::string> authors;
    struct {
        std::string full;
        std::string brief;
        std::string epilog;
    } description;
};

enum class Type {
    positional,
    other,
};

struct ArgInfo {
    ArgInfo(Type type) : type{type} {}
    Type type;
    unsigned order = -1;
    bool required = false;
    OptStr short_version;
    OptStr long_version;
    OptStr value;
    std::string description;
};

template <typename T>
concept CanGenerate = requires(T t) {
    { T::generate(AppInfo{}, std::vector<ArgInfo>{}) } -> std::same_as<T>;
    { t.to_screen() } -> std::same_as<void>;
};

}

#endif // _RCP_DOC_MAKER_H_