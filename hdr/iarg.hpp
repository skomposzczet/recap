#ifndef _IARG_H_
#define _IARG_H_

#include <string>

namespace rcp {

class IArg
{
public:
    virtual bool is_named(const std::string&) const = 0;
    virtual bool is_triggered(const std::string&) const = 0;

    virtual bool is_ambiguous(const IArg&) const = 0;

    virtual std::string help() const = 0;
};

}

#endif // _IARG_H_