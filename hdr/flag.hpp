#ifndef _FLAG_H_
#define _FLAG_H_

#include "iarg.hpp"

#include <string>
#include <memory>

namespace rcp {

class FlagBuilder;
class Flag: public IArg
{
friend class FlagBuilder;
public:
    void call();
    bool was_called() const;

    bool is_named(const std::string& arg_name) const override;
    bool is_triggered(const std::string& option) const override;
    bool is_ambiguous(const IArg& other) const override;

    std::string help() const override;

private:
    std::string name;
    std::string description;
    
    bool allow_long = false;
    bool allow_short = true;

    bool called = false;
};

class FlagBuilder
{
public:
    FlagBuilder(const std::string& name);

    FlagBuilder& allow_long();
    FlagBuilder& forbid_short();
    FlagBuilder& with_description(const std::string& description);

    std::shared_ptr<Flag> get();

private:
    std::shared_ptr<Flag> flag;

};

}

#endif // _FLAG_H_