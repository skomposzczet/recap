#ifndef _RCP_DOC_MAKER_H_
#define _RCP_DOC_MAKER_H_

#include <string>
#include <optional>
#include <vector>

namespace rcp {

struct ArgInfo;
using ArgInfoVec = std::vector<ArgInfo>;
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
    { T::generate(AppInfo{}, ArgInfoVec{}) } -> std::same_as<T>;
    { t.to_screen() } -> std::same_as<void>;
};

class NixDocMaker
{
public:
    static NixDocMaker generate(const AppInfo& info, const ArgInfoVec& infos);
    void to_screen() const;

private:
    std::string res;
    
    NixDocMaker() = default;

    void create_intro(const AppInfo& info);
    void create_usage(const ArgInfoVec& infos);
    void create_arg_section(const ArgInfoVec& infos);
    void create_end(const AppInfo& info);

    static bool has_options(const ArgInfoVec& infos);
    static std::string arg_info_vec_to_str(ArgInfoVec& info_vec);
    static std::string arg_info_to_str(const ArgInfo& info);
};

std::string arg_info_vec_to_str(ArgInfoVec& info_vec);
std::string arg_info_to_str(const ArgInfo& info);

}

#endif // _RCP_DOC_MAKER_H_