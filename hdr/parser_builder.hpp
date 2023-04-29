#ifndef _PARSER_BUILDER_H_
#define _PARSER_BUILDER_H_

#include "parser.hpp"
#include <string>

namespace rcp {

class ParserBuilder
{
public:
    ParserBuilder(const std::string& name);

    ParserBuilder& description(const std::string& description);
    ParserBuilder& brief(const std::string& brief);
    ParserBuilder& epilog(const std::string& epilog);
    ParserBuilder& version(const std::string& version);
    ParserBuilder& author(const std::string& new_author);
    ParserBuilder& authors(std::initializer_list<std::string> authors);

    ParserBuilder& help_enabled();    
    ParserBuilder& version_enabled();    

    Parser get() const;

private:
    Parser parser;

};

}

#endif // _PARSER_BUILDER_H_
