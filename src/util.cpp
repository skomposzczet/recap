#include "util.hpp"
#include <string>
#include <algorithm>

namespace rcp {

namespace util {

std::string upper(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

}

}