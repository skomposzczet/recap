#ifndef _FLAG_H_
#define _FLAG_H_

#include <string>
#include <memory>

namespace rcp {

class FlagBuilder;
class Flag
{
friend class FlagBuilder;
public:
    bool was_called() const;

    bool is_triggered(const std::string& option) const;
    void call();

    std::string help() const;

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