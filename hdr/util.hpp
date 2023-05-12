#ifndef _RCP_UTIL_H_
#define _RCP_UTIL_H_

#include <string>
#include <sstream>

namespace rcp {

namespace util {

std::string upper(std::string str);

template<typename... Args>
std::string cat(Args&&... args)
{
    std::ostringstream oss;
    (oss << ... << std::forward<Args>(args));
    return oss.str();
}

}

}

#endif // _RCP_UTIL_H_