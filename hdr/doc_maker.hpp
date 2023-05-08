#ifndef _RCP_DOC_MAKER_H_
#define _RCP_DOC_MAKER_H_

#include <string>
#include <optional>
#include <vector>

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

#endif // _RCP_DOC_MAKER_H_